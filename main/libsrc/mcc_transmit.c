
#include "driver/rmt_rx.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"

#include "mcc_rx_tx.h"
#include "mcc_enc_dec.h"


static rmt_channel_handle_t tx_chan_handle = NULL;
static rmt_encoder_handle_t tx_encoder = NULL;

void rmt_mcc_transmit_init(void)
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
    // rmt_mcc_transmit_init();
    rmt_transmit_config_t rmt_tx_config = {
        .loop_count = tst_word.loop_count,
    };
    rmt_mcc_word_encode(&tst_word, &rmt_word);
    while (1)
    {
        ESP_ERROR_CHECK(rmt_transmit(tx_chan_handle, tx_encoder, &rmt_word, sizeof(rmt_word), &rmt_tx_config));
        rmt_tx_wait_all_done(tx_chan_handle, portMAX_DELAY);
        vTaskDelay(500);
    }
}
#endif
void rmt_mcc_word_transmit_data(mcc_code_word_tx_t mcc_tx_word)
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

