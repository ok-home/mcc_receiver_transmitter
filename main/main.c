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

#include "mcc_enc_dec.h"


//static const char *TAG = "MCC TEST";

static rmt_channel_handle_t tx_chan_handle = NULL;
static rmt_encoder_handle_t tx_encoder = NULL;

void rmt_mcc_tx_init(void)
{
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_TX_PIN,
        .mem_block_symbols = TX_BLOCK_SYMBOL, // for esp32c3 48 !!!!!!! esp32s -> 64 !!!!!!!
        .flags.invert_out = false,
        .resolution_hz = RMT_TX_CLK,
        .trans_queue_depth = 5, // set the maximum number of transactions that can pend in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &tx_chan_handle));

    rmt_copy_encoder_config_t tx_encoder_config = {};
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&tx_encoder_config, &tx_encoder));
    ESP_ERROR_CHECK(rmt_enable(tx_chan_handle));
}

void rmt_mcc_tx_task(void *p)
{
    rmt_mcc_word_t rmt_word = {0};
    mcc_code_word_t tst_word = {
        .miles = 12,
        .spid = 211,
        .yz_mod = 0x1A,
    };
    rmt_mcc_tx_init();
    rmt_transmit_config_t rmt_tx_config = {
        .loop_count = 2,
    };
    rmt_mcc_word_encode(&tst_word, &rmt_word);
    while (1)
    {
        gpio_set_level(5, 1);
        ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &rmt_word, sizeof(rmt_word), &rmt_tx_config));
        rmt_tx_wait_all_done(tx_chan_handle, portMAX_DELAY);
        gpio_set_level(5, 0);
        printf("done\n");
        vTaskDelay(500);
    }
}

static mcc_capture_buf_t sample_buf[2] = {0};
extern uint16_t *decode_offset[16];

void  mcc_decode_cb(int frame)
{

    if (frame >= 0) // 2016
    {
        frame &= 1;
        uint16_t *sample_buf16 = sample_buf[frame].buff;
        memcpy((uint8_t *)sample_buf[frame ^ 1].rollback_buf, (uint8_t *)sample_buf[frame].to_rollback_buf, (TIME_SLOT_SIZE * 11) * 2);
        for (int ch = 0; ch < 16; ch++)
        {
            // from previous buff
            decode_offset[ch] = (decode_offset[ch] == NULL) ? NULL : decode_offset[ch] - sample_buf[frame ^ 1].to_rollback_buf + sample_buf[frame].buff;
        }
        for (int i = 0; i < DMA_FRAME / 2; i++)
        {
            mcc_word_decode(&sample_buf16[i]);
        }
    }
}

mcc_capture_config_t mcc_capture_config = {
    .pin[0] = 4,
    .pin[1] = 7,
    .pin[2] = 4,
    .pin[3] = 7,
    .pin[4] = 7,
    .pin[5] = 7,
    .pin[6] = 7,
    .pin[7] = 7,
    .pin[8] = 7,
    .pin[9] = 4,
    .pin[10] = 7,
    .pin[11] = 7,
    .pin[12] = 4,
    .pin[13] = 7,
    .pin[14] = 7,
    .pin[15] = 7,
    .meashure_timeout = 200, // 200 millisek
    .buf0 = (uint8_t *)sample_buf[0].dma_capture_start,
    .buf1 = (uint8_t *)sample_buf[1].dma_capture_start,
    .mcc_capture_cb = mcc_decode_cb};

// extern void test();
void app_main(void)
{
    gpio_reset_pin(5);
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    gpio_reset_pin(6);
    gpio_set_direction(6, GPIO_MODE_OUTPUT);
    gpio_reset_pin(7);
    gpio_set_direction(7, GPIO_MODE_OUTPUT);
    gpio_reset_pin(8);
    gpio_set_direction(8, GPIO_MODE_OUTPUT);

    gpio_set_level(7, 0);
    gpio_set_level(8, 1);

    xTaskCreate(rmt_mcc_tx_task, "rmt tx", 4096, NULL, 5, NULL);

    vTaskDelay(10);

    start_mcc_capture(&mcc_capture_config);

    // test();
}