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
#include "mcc_rx_tx.h"

static const char *TAG = "MCC";
// pin definition on include/mcc_rx_tx.h
mcc_capture_config_t mcc_capture_config = {
    .pin[0] = MCC_PIN_0,
    .pin[1] = MCC_PIN_1,
    .pin[2] = MCC_PIN_2,
    .pin[3] = MCC_PIN_3,
    .pin[4] = MCC_PIN_4,
    .pin[5] = MCC_PIN_5,
    .pin[6] = MCC_PIN_6,
    .pin[7] = MCC_PIN_7,
    .pin[8] = MCC_PIN_8,
    .pin[9] = MCC_PIN_9,
    .pin[10] = MCC_PIN_10,
    .pin[11] = MCC_PIN_11,
    .pin[12] = MCC_PIN_12,
    .pin[13] = MCC_PIN_13,
    .pin[14] = MCC_PIN_14,
    .pin[15] = MCC_PIN_15,
};

QueueHandle_t MCC_RX_QUEUE_NAME;

void mcc_receive_task(void *p)
{
    MCC_RX_QUEUE_NAME = xQueueCreate(16, sizeof(mcc_code_word_rx_t));
    mcc_code_word_rx_t result;
    start_mcc_receive_loop(&mcc_capture_config);

    while (1)
    {
        xQueueReceive(MCC_RX_QUEUE_NAME, &result, portMAX_DELAY);
        ESP_LOGI(TAG,"receive->channel = %d miles = %d yz=%x spid=%d", result.channel, result.miles, result.yz_mod, result.spid);
    }
}
void app_main(void)
{
    // attention -> in test mode rmt_mcc_transmit_init must be called before create mcc_receive_task
    rmt_mcc_transmit_init(); 
    // create receiver loop task
    xTaskCreate(mcc_receive_task, "mcc_receive_task", 2048 * 8, NULL, 5, NULL);

    // test transmit mcc word
    while (1)
    {
        mcc_code_word_tx_t tst_word = { //transmit 2 * 12.1A.211 word 
            .miles = 12,
            .spid = 211,
            .yz_mod = 0x1A,
            .loop_count = 2,
            .delay_mks = 0,
        };
        rmt_mcc_word_transmit_data(tst_word);

        tst_word.delay_mks = 1600;
        rmt_mcc_word_transmit_data(tst_word); // transmit 1600 mks interval

        tst_word.miles = 10;
        tst_word.spid = 126;
        tst_word.yz_mod = 0x96;
        tst_word.loop_count = 4;
        tst_word.delay_mks = 0;
        rmt_mcc_word_transmit_data(tst_word); //transmit 4 * 10.96.126 word 

        vTaskDelay(500); // delay 5 sek
        ESP_LOGI(TAG,"next packet");
    }
}