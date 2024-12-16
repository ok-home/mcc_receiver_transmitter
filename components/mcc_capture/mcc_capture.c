/* logic analyzer hal example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "mcc_capture.h"
#include "mcc_capture_ll.h"
#include "soc/soc.h"
#include "soc/dport_access.h"
#include "esp_log.h"
#include "string.h"
#include "rom/cache.h"
#include "esp_cache.h" //esp_mm
#include "esp_psram.h"

#define MCC_TASK_STACK 2048
#define DMA_FRAME 4032

// frame buff & dma descripter
static mcc_frame_t mcc_frame = {
    .fb.buf = NULL,
    .fb.len = 0, // bytes *** one sample = 2 byte
    .dma = NULL};

static TaskHandle_t mcc_capture_task_handle = 0; // main task handle
static int mcc_capture_started = 0;              // flag start dma

#ifdef CONFIG_IDF_TARGET_ESP32
//
// esp32 only
// sample sequence in 32 word - adr0=sample1, adr1=sample0
// swap sample sequence
//
static inline void swap_buf(uint16_t *buf, int cnt)
{
    uint16_t tmp;
    for (int i = 0; i < cnt; i += 2)
    {
        tmp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = tmp;
    }
}
#endif

/**
 * @brief allocate dma descriptor
 *
 * @param uint32_t size - size of sample frame buffer (bytes)
 * @param uint8_t *buffer - pointer of sample frame buffer
 *
 * @return
 *     - dma descriptor ( NULL if no mem )
 */
 // ONLY 2 dma_frame
static lldesc_t *allocate_dma_descriptors(uint32_t size, uint8_t *buffer)
{
    uint32_t count = size / DMA_FRAME;     //  dma frames count
    if(count != 2) {return 0;} // only 2 dma frame
    lldesc_t *dma = (lldesc_t *)heap_caps_malloc((count + 1) * sizeof(lldesc_t), MALLOC_CAP_DMA);
    if (dma == NULL)
    {
        return dma;
    }
    int x = 0;
    for (; x < count; x++)
    {
        dma[x].size = DMA_FRAME;
        dma[x].length = DMA_FRAME;
        dma[x].sosf = 0;
        dma[x].eof = 0;
        dma[x].owner = 1;
        dma[x].buf = buffer + DMA_FRAME * x;
        if(x==0)
            {dma[x].empty = (uint32_t)&dma[(x + 1)];} // goto frame 1
        else
            {dma[x].empty = (uint32_t)&dma[(0)];} // non stop, goto frame0
    }
    return dma;
}
/**
 *  @brief  full stop & free all
 */
void mcc_capture_stop(void)
{
    // stop dma transfer
    mcc_capture_ll_stop();
    // deinit dma isr
    mcc_capture_ll_deinit_dma_eof_isr();
    // free allocated mem
    if (mcc_frame.dma)
    {
        free(mcc_frame.dma);
        mcc_frame.dma = NULL;
    }
    if (mcc_frame.fb.buf)
    {
        free(mcc_frame.fb.buf);
        mcc_frame.fb.buf = NULL;
        mcc_frame.fb.len = 0;
    }
    // la status - stopped -> ready to new start
    mcc_capture_started = 0;
}
/**
 *  @brief main logic analyzer task
 *       call callback function after dma transfer
 *       callback param = 0 if timeout or reset
 *  @param *arg - pointer to callback function from cfg
 */
static void mcc_capture_task(void *arg)
{
    int noTimeout;
    mcc_capture_config_t *cfg = (mcc_capture_config_t *)arg;
    static int cnt = 0;
    while (1)
    {
        noTimeout = ulTaskNotifyTake(pdFALSE, cfg->meashure_timeout); // portMAX_DELAY
        if (noTimeout == 1)                                                // dma data ready
        {
            // dma data ready
#ifdef CONFIG_IDF_TARGET_ESP32
            // esp32 -> sample sequence in 32 word - adr0=sample1, adr1=sample0
            // swap sample sequence on esp32.
            swap_buf((uint16_t *)mcc_frame.fb.buf, mcc_frame.fb.len / (cfg->number_channels / 8)); // 16 channels only on esp32
#endif
            cfg->mcc_capture_cb((uint8_t *)mcc_frame.fb.buf+(DMA_FRAME*(cnt&1)), 2016, mcc_capture_ll_get_sample_rate(cfg->sample_rate), cfg->number_channels);
            //            mcc_capture_stop();
            //            vTaskDelete(mcc_capture_task_handle);
       }
        else // timeout detected
        {
            printf("err notimeout %d",noTimeout);
            //            cfg->mcc_capture_cb(NULL, 0, 0, 0); // timeout
            //            mcc_capture_stop();
            //            vTaskDelete(mcc_capture_task_handle);
        }
        //printf("notmo=%d cnt %d\n",noTimeout,cnt);
        // debug - stop after 100 cb
        cnt++;
        if(cnt==100) {mcc_capture_ll_stop(); 
        cnt=0;
        vTaskDelete(mcc_capture_task_handle);}
    }
}
/**
 * @brief Start logic analyzer
 *
 * @param config Configurations - see mcc_capture_config_t struct
 *
 * @return
 *     - ESP_OK                 Success
 *     - ESP_ERR_INVALID_ARG    Parameter error
 *     - ESP_ERR_NO_MEM         No memory to initialize mcc_capture
 *     - ESP_ERR_INVALID_STATE  mcc_capture already working
 *     - ESP_FAIL               Initialize fail
 */
esp_err_t start_mcc_capture(mcc_capture_config_t *config)
{
    esp_err_t ret = 0;

    // check la status
    if (mcc_capture_started)
    {
        return ESP_ERR_INVALID_STATE;
    }
    mcc_capture_started = 1;
    // check cb pointer
    if (config->mcc_capture_cb == NULL)
    {
        goto _ret;
    }
    if (config->number_channels != MCC_HW_MAX_CHANNELS)
    {
        goto _ret;
    }
    // check GPIO num - 0-MAX_GPIO or num < 0 // todo use macros to legal pin definition // now it controlled from WS headers
    for (int i = 0; i < config->number_channels; i++)
    {
        if (config->pin[i] > MCC_HW_MAX_GPIO || config->pin[i] < MCC_HW_MIN_GPIO)
        {
            goto _ret;
        }
    }
    if (config->pin_trigger != MCC_HW_MIN_GPIO) // no trigger already
    {
        goto _ret;
    }
    // check intr edge
    // check sample rate
    if (config->sample_rate != MCC_HW_DEFAULT_SAMPLE_RATE)
    {
        goto _ret;
    }
    // check number of samples
    if (config->number_of_samples != 2016 )
    {
        goto _ret;
    }
    // allocate frame buffer
    uint32_t bytes_to_alloc = config->number_of_samples * (config->number_channels / 8)*2;//2*DMA_FRAME -> 
    // alloc on RAM
    uint32_t largest_free_block = heap_caps_get_largest_free_block(MALLOC_CAP_DMA); // byte
    if (largest_free_block < bytes_to_alloc + ((bytes_to_alloc / DMA_FRAME) + 1) * sizeof(lldesc_t))
    {
        bytes_to_alloc = largest_free_block - ((bytes_to_alloc / DMA_FRAME) + 2) * sizeof(lldesc_t); // free space with dma lldesc size
    }
    ESP_LOGD("DMA HEAP Before", "All_dma_heap=%d Largest_dma_heap_block=%d", heap_caps_get_free_size(MALLOC_CAP_DMA), heap_caps_get_largest_free_block(MALLOC_CAP_DMA));
    mcc_frame.fb.len = bytes_to_alloc & ~0x3; // burst transfer word align
    mcc_frame.fb.buf = heap_caps_calloc(mcc_frame.fb.len, 1, MALLOC_CAP_DMA);
    //    mcc_frame.fb.buf = heap_caps_malloc(mcc_frame.fb.len, MALLOC_CAP_DMA);
    ESP_LOGD("DMA HEAP After", "All_dma_heap=%d Largest_dma_heap_block=%d", heap_caps_get_free_size(MALLOC_CAP_DMA), heap_caps_get_largest_free_block(MALLOC_CAP_DMA));
    if (mcc_frame.fb.buf == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto _retcode;
    }
    ESP_LOGD("DMA HEAP", "Allocated %d bytes", mcc_frame.fb.len);
    //  allocate dma descriptor buffer
    mcc_frame.dma = allocate_dma_descriptors(mcc_frame.fb.len, mcc_frame.fb.buf);
    if (mcc_frame.dma == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto _freebuf_ret;
    }
    // configure   - pin definition, pin trigger, sample frame & dma frame, clock divider
    mcc_capture_ll_config(config->pin, config->sample_rate, config->number_channels, &mcc_frame);
    // start main task - check logic analyzer get data & call cb // todo -> test priority change
    if (pdPASS != xTaskCreate(mcc_capture_task, "mcc_task", MCC_TASK_STACK * 4, config, uxTaskPriorityGet(NULL) /*configMAX_PRIORITIES - 5*/, &mcc_capture_task_handle))
    {
        ret = ESP_ERR_NO_MEM;
        goto _freedma_ret;
    }
    // init dma eof isr
    ret = mcc_capture_ll_init_dma_eof_isr(mcc_capture_task_handle);
    if (ret != ESP_OK)
    {
        goto _freetask_ret;
    }
    // start meashure
    mcc_capture_ll_start();

    return ESP_OK;

_freetask_ret:
    vTaskDelete(mcc_capture_task_handle);
_freedma_ret:
    free(mcc_frame.dma);
_freebuf_ret:
    free(mcc_frame.fb.buf);
_retcode:
    mcc_capture_started = 0;
    return ret;
_ret:
    mcc_capture_started = 0;
    return ESP_ERR_INVALID_ARG;
}
