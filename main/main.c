#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "driver/rmt_rx.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"

#include "mcc_encoder.h"
#include "mcc_capture.h"

static const char *TAG = "MCC TEST";

static rmt_channel_handle_t tx_chan_handle = NULL;
static rmt_encoder_handle_t tx_encoder = NULL;


void rmt_mcc_tx_init(void){
 rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_TX_PIN,
        .mem_block_symbols = TX_BLOCK_SYMBOL, //for esp32c3 48 !!!!!!! esp32s -> 64 !!!!!!! 
        .flags.invert_out = false,
        .resolution_hz = RMT_TX_CLK,
        .trans_queue_depth = 5, // set the maximum number of transactions that can pend in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &tx_chan_handle));

    rmt_copy_encoder_config_t tx_encoder_config = {};
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&tx_encoder_config, &tx_encoder));
    ESP_ERROR_CHECK(rmt_enable(tx_chan_handle));
}

void rmt_mcc_tx_task(void*p)
{
    rmt_mcc_word_t rmt_word = {0};
    mcc_code_word_t tst_word = {
        .miles = 12,
        .spid = 211,
        .yz_mod = 0x1A,
    };
    rmt_mcc_tx_init();
    rmt_transmit_config_t rmt_tx_config = {
        .loop_count = 8,
    };
    rmt_mcc_word_encode(&tst_word,&rmt_word);
    while(1)
    {
            gpio_set_level(5, 1);
            ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &rmt_word, sizeof(rmt_word), &rmt_tx_config));
            rmt_tx_wait_all_done(tx_chan_handle, portMAX_DELAY);
            gpio_set_level(5, 0);            
            vTaskDelay(1);
    }

}

static mcc_capture_buf_t sample_buf[2] = {0};


void mcc_decode_cb(int frame)
{
   if (frame >= 0) // 2016
    frame &= 1;
    uint16_t *samle_buf16 = samle_buf[frame].buff;
    memcpy((uint8_t)samle_buf[(~frame)&1].rollback_buf,(uint8_t)samle_buf[frame].to_rollback_buf,(TIME_SLOT_SIZE*10)*2);
   {
      for (int i = TIME_SLOT_SIZE*10; i < DMA_FRAME/2+TIME_SLOT_SIZE*10; i++)
      {
         int ret = mcc_word_decode(0, samle_buf16[i] & 1);
         if(ret<0)
            i += ret*TIME_SLOT_SIZE;
      }
   }
   // printf("end\n");
}

mcc_capture_config_t mcc_capture_config = {
    .pin[0] = 4,
    .pin[1] = 5,
    .pin[2] = 6,
    .pin[3] = -1,
    .pin[4] = -1,
    .pin[5] = -1,
    .pin[6] = -1,
    .pin[7] = -1,
    .pin[8] = -1,
    .pin[9] = -1,
    .pin[10] = -1,
    .pin[11] = -1,
    .pin[12] = -1,
    .pin[13] = -1,
    .pin[14] = -1,
    .pin[15] = -1,
    .meashure_timeout = 20,// 200 millisek
    .buf0 = (uint8_t*)sample_buf[0].dma_capture_start;
    .buf1 = (uint8_t*)sample_buf[1].dma_capture_start;
    .mcc_capture_cb = mcc_decode_cb};


extern void test();
void app_main(void)
{
    gpio_reset_pin(5);
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    gpio_reset_pin(6);
    gpio_set_direction(6, GPIO_MODE_OUTPUT);
 
    xTaskCreate(rmt_mcc_tx_task, "rmt tx", 4096, NULL, 5, NULL);

    vTaskDelay(100);

    //esp_err_t ret = start_mcc_capture(&mcc_capture_config);

    test();

}