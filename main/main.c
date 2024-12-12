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
/*
#include "soc/gpio_sig_map.h"
#include "soc/gpio_periph.h"
#include "soc/io_mux_reg.h"
#define gpio_matrix_in(a, b, c) esp_rom_gpio_connect_in_signal(a, b, c)
#define gpio_matrix_out(a, b, c, d) esp_rom_gpio_connect_out_signal(a, b, c, d)
*/

#include "mcc_encoder.h"
#include "maps.c"

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
uint16_t mcc_get_miles_code( uint16_t mil)
{  // from 0 to 36
    return miles[mil];   //010 0110 1011
}
uint16_t mcc_get_spid_code (uint16_t spi)
{ // from 1 to 330
    return spid[spi];   //100 0000 0111
}
void rmt_mcc_word_encode(mcc_code_word_t* tst_word, rmt_mcc_word_t* rmt_word){
    // clear all bits
    uint16_t miles = mcc_get_miles_code(tst_word->miles);
    uint16_t spid = mcc_get_spid_code(tst_word->spid);
    uint8_t y_mod = ((tst_word->yz_mod)>>4)&0xf;
    uint8_t z_mod = (tst_word->yz_mod)&0xf;
    // set miles & clear spid
    for(int i=0;i<11;i++)
    {
        rmt_word->mcc_word[i].t_bit0.level0=miles&1;
        miles >>= 1;
        rmt_word->mcc_word[i].t_bit0.level1=0;
        rmt_word->mcc_word[i].t_bit0.duration0=RMT_BIT_WIDTH;
        rmt_word->mcc_word[i].t_bit0.duration1=RMT_BIT_WIDTH*5;

        rmt_word->mcc_word[i].t_bit6.level0=0;
        rmt_word->mcc_word[i].t_bit6.level1=0;
        rmt_word->mcc_word[i].t_bit6.duration0=RMT_BIT_WIDTH;
        rmt_word->mcc_word[i].t_bit6.duration1=RMT_BIT_WIDTH;

        rmt_word->mcc_word[i].t_bit8.level0=0;
        rmt_word->mcc_word[i].t_bit8.level1=0;
        rmt_word->mcc_word[i].t_bit8.duration0=RMT_BIT_WIDTH;
        rmt_word->mcc_word[i].t_bit8.duration1=RMT_BIT_WIDTH;

        rmt_word->mcc_word[i].t_bit10.level0=0;
        rmt_word->mcc_word[i].t_bit10.level1=0;
        rmt_word->mcc_word[i].t_bit10.duration0=RMT_BIT_WIDTH;
        rmt_word->mcc_word[i].t_bit10.duration1=RMT_BIT_WIDTH*5;
    }
        rmt_word->rmt_stop.val = 0;

    for(int i=10; i >= 0;i--){
        if((spid>>i)&1)
        {
            if(z_mod&1)
            {
                rmt_word->mcc_word[i].t_bit10.level0=1;
            }
            else 
            {
            if( y_mod&1)
                {
                    rmt_word->mcc_word[i].t_bit6.level0=1;
                }
                  else
                {
                    rmt_word->mcc_word[i].t_bit8.level0=1;
                }
            }
        z_mod >>=1;
        y_mod >>=1;
        }
    }
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
        .loop_count = 0,
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
#include "logic_analyzer_ws_server.h"
extern void test_time(void);
void app_main(void)
{
    gpio_reset_pin(5);
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    logic_analyzer_ws_server();
    xTaskCreate(rmt_mcc_tx_task, "rmt tx", 4096, NULL, 5, NULL);
    test_time();
}