#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
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

// static const char *TAG = "MCC TEST";

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
        .trans_queue_depth = 1, // set the maximum number of transactions that can pend in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &tx_chan_handle));

    rmt_copy_encoder_config_t tx_encoder_config = {};
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&tx_encoder_config, &tx_encoder));
    ESP_ERROR_CHECK(rmt_enable(tx_chan_handle));
}
#if 0
void rmt_mcc_tx_task(void *p)
{
    rmt_mcc_word_t rmt_word = {0};
    mcc_code_word_tx_t tst_word = {
        .miles = 12,
        .spid = 211,
        .yz_mod = 0x1A,
        .loop_count = 1,
        .delay_mks = 0,
    };
    // rmt_mcc_tx_init();
    rmt_transmit_config_t rmt_tx_config = {
        .loop_count = tst_word.loop_count,
    };
    rmt_mcc_word_encode(&tst_word, &rmt_word);
    while (1)
    {
        ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &rmt_word, sizeof(rmt_word), &rmt_tx_config));
        rmt_tx_wait_all_done(tx_chan_handle, portMAX_DELAY);
        printf("done\n");
        vTaskDelay(500);
    }
}
#endif
void rmt_mcc_word_transmit(mcc_code_word_tx_t mcc_tx_word)
{
    rmt_transmit_config_t rmt_tx_config = {0};
    if (mcc_tx_word.delay_mks == 0) // transmit mcc word
    {
        rmt_mcc_word_t rmt_word = {0};
        rmt_tx_config.loop_count = mcc_tx_word.loop_count;
        rmt_mcc_word_encode(&mcc_tx_word, &rmt_word);
        ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &rmt_word, sizeof(rmt_word), &rmt_tx_config));
    }
    else // transmit delay
    {
        rmt_symbol_word_t delay_word[2] = {0};
        delay_word[0].duration0 = mcc_tx_word.delay_mks / 20;
        delay_word[0].level0 = 0;
        delay_word[0].duration1 = 0;
        delay_word[0].level1 = 0;

        delay_word[1].duration0 = 0;
        delay_word[1].level0 = 0;
        delay_word[1].duration1 = 0;
        delay_word[1].level1 = 0;

        ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &delay_word, sizeof(delay_word), &rmt_tx_config));
    };
    rmt_tx_wait_all_done(tx_chan_handle, portMAX_DELAY);
}

static mcc_capture_buf_t sample_buf[2] = {0};
extern uint16_t *decode_offset[16];

void mcc_decode_cb(int frame)
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
    .pin[9] = 7,
    .pin[10] = 7,
    .pin[11] = 7,
    .pin[12] = 7,
    .pin[13] = 7,
    .pin[14] = 7,
    .pin[15] = 7,
    .meashure_timeout = 200, // 200 millisek
    .buf0 = (uint8_t *)sample_buf[0].dma_capture_start,
    .buf1 = (uint8_t *)sample_buf[1].dma_capture_start,
    .mcc_capture_cb = mcc_decode_cb};

// extern void test();
QueueHandle_t mcc_rx_queue;

void mcc_receive_task(void *p)
{
    mcc_rx_queue = xQueueCreate(16, sizeof(mcc_code_word_rx_t));
    mcc_code_word_rx_t result;
    start_mcc_capture(&mcc_capture_config);

    while (1)
    {
        xQueueReceive(mcc_rx_queue, &result, portMAX_DELAY);
        printf("receive->channel = %d miles = %d yz=%x spid=%d\n", result.channel, result.miles, result.yz_mod, result.spid);
    }
}
void app_main(void)
{

    rmt_mcc_tx_init();

    xTaskCreate(mcc_receive_task, "mcc_receive_task", 2048 * 8, NULL, 5, NULL);

    while (1)
    {
        mcc_code_word_tx_t tst_word = {
            .miles = 12,
            .spid = 211,
            .yz_mod = 0x1A,
            .loop_count = 2,
            .delay_mks = 0,
        };
        rmt_mcc_word_transmit(tst_word);

        tst_word.delay_mks = 1600;
        rmt_mcc_word_transmit(tst_word);

        tst_word.miles = 10;
        tst_word.spid = 126;
        tst_word.yz_mod = 0x96;
        tst_word.loop_count = 4;
        tst_word.delay_mks = 0;
        rmt_mcc_word_transmit(tst_word);


        vTaskDelay(500);
        printf("\n");
    }
}