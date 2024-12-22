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

// static const char *TAG = "MCC TEST";

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
        printf("receive->channel = %d miles = %d yz=%x spid=%d\n", result.channel, result.miles, result.yz_mod, result.spid);
    }
}
void app_main(void)
{

    rmt_mcc_transmit_init();

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
        rmt_mcc_word_transmit_data(tst_word);

        tst_word.delay_mks = 1600;
        rmt_mcc_word_transmit_data(tst_word);

        tst_word.miles = 10;
        tst_word.spid = 126;
        tst_word.yz_mod = 0x96;
        tst_word.loop_count = 4;
        tst_word.delay_mks = 0;
        rmt_mcc_word_transmit_data(tst_word);


        vTaskDelay(500);
        printf("\n");
    }
}