/* logic analyzer hal example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "mcc_encoder.h"
#include "soc/soc.h"
#include "soc/dport_access.h"
#include "esp_log.h"
#include "string.h"

#include "esp_timer.h"

#define MCC_TASK_STACK 2048

// frame buff & dma descripter
static mcc_frame_t mcc_frame ;
static lldesc_t *mcc_frame_dma;
static TaskHandle_t mcc_capture_task_handle = 0; // main task handle
static int mcc_capture_started = 0;              // flag start dma

/**
 * @brief allocate dma descriptor
 *
 * @return
 *     - dma descriptor ( NULL if no mem )
 */
 // ONLY 2 dma_frame buf0 & buf1 with DMA_FRAME size
static lldesc_t *allocate_dma_descriptors(uint8_t *buf0, uint8_t *buf1)
{
    lldesc_t *dma = (lldesc_t *)heap_caps_malloc((2) * sizeof(lldesc_t), MALLOC_CAP_DMA); // only 2 descriptor
    if (dma == NULL)
    {
        return dma;
    }
        dma[0].size = DMA_FRAME;
        dma[0].length = DMA_FRAME;
        dma[0].sosf = 0;
        dma[0].eof = 0;
        dma[0].owner = 1;
        dma[0].buf = buf0;
        dma[0].empty = (uint32_t)&dma[1]; // goto frame 1

        dma[1].size = DMA_FRAME;
        dma[1].length = DMA_FRAME;
        dma[1].sosf = 0;
        dma[1].eof = 0;
        dma[1].owner = 1;
        dma[1].buf = buf1;
        dma[1].empty = (uint32_t)&dma[0]; // goto frame 0

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
    // la status - stopped -> ready to new start
    mcc_capture_started = 0;
}
/**
 *  @brief main logic analyzer task
 *       call callback function after dma transfer
 *       callback param = 0 if timeout or reset
 *  @param *arg - pointer to callback function from cfg
 */
 void IRAM_ATTR mcc_capture_task(void *arg)
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
            //int64_t t0 = esp_timer_get_time();
            cfg->mcc_capture_cb(cnt&1);
            //int64_t t1 = esp_timer_get_time();
            //printf("time %lld\n",t1-t0);
            //            mcc_capture_stop();
            //            vTaskDelete(mcc_capture_task_handle);
       }
        else // timeout detected
        {
            printf("err notimeout %d ------------------------\n",noTimeout);
            //            cfg->mcc_capture_cb(-1); // timeout
            //            mcc_capture_stop();
            //            vTaskDelete(mcc_capture_task_handle);
        }
        //printf("notmo=%d cnt %d\n",noTimeout,cnt);
        // debug - stop after 100 cb
        cnt++;
        //if(cnt==10) {mcc_capture_ll_stop(); 
        //cnt=0;
        //printf("stop task\n");
        //vTaskDelete(mcc_capture_task_handle);}
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
    // check GPIO num - 0-MAX_GPIO or num < 0 // todo use macros to legal pin definition // now it controlled from WS headers
    for (int i = 0; i < MCC_HW_DEFAULT_CHANNELS; i++)
    {
        if (config->pin[i] > MCC_HW_MAX_GPIO || config->pin[i] < MCC_HW_MIN_GPIO)
        {
            goto _ret;
        }
    }
    if (config->buf0 == NULL || config->buf1 == NULL)
    {
        goto _ret;
    }
    //  allocate dma descriptor buffer
    mcc_frame.dma = allocate_dma_descriptors(config->buf0, config->buf1);
    if (mcc_frame.dma == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto _ret;
    }
    // configure   - pin definition, pin trigger, sample frame & dma frame, clock divider
    mcc_capture_ll_config(config->pin, MCC_HW_DEFAULT_SAMPLE_RATE, MCC_HW_DEFAULT_CHANNELS, &mcc_frame);
    // start main task - check logic analyzer get data & call cb // todo -> test priority change
    if (pdPASS != xTaskCreatePinnedToCore(mcc_capture_task, "mcc_task", MCC_TASK_STACK * 4, config, configMAX_PRIORITIES-1 , &mcc_capture_task_handle,1))
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

    mcc_capture_started = 0;
    return ret;
_ret:
    mcc_capture_started = 0;
    return ESP_ERR_INVALID_ARG;
}
