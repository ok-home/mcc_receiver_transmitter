#pragma once

#include <stdint.h>
#include <esp_err.h>

// GPIO connected to IR sensor -> 1
// GPIO internal connected to test rmt tx -> 0
#define MCC_CONNECT_IR_GPIO 0

//  GPIO conected to IR sensor
#define MCC_PIN_0 (4)
#define MCC_PIN_1 (4)
#define MCC_PIN_2 (4)
#define MCC_PIN_3 (-1)
#define MCC_PIN_4 (-1)
#define MCC_PIN_5 (-1)
#define MCC_PIN_6 (-1)
#define MCC_PIN_7 (-1)
#define MCC_PIN_8 (-1)
#define MCC_PIN_9 (-1)
#define MCC_PIN_10 (-1)
#define MCC_PIN_11 (-1)
#define MCC_PIN_12 (-1)
#define MCC_PIN_13 (-1)
#define MCC_PIN_14 (-1)
#define MCC_PIN_15 (-1)

// mcc transmitter GPIO
#define MCC_TX_PIN (4)

// mcc  receiver queue name
#define MCC_RX_QUEUE_NAME mcc_rx_queue

// transmitter mcc word struct 
typedef struct mcc_code_word_tx
{
    uint16_t miles;
    uint16_t spid;
    uint8_t yz_mod;
    uint8_t loop_count; // number of repetitions mcc word 4/8/128 .... 0-> single word
    uint16_t delay_mks; // 0-> mcc word, 1-> special transmit delay interval between mcc word 
} mcc_code_word_tx_t;

// receiver mcc word struct from MCC_RX_QUEUE_NAME queue
typedef struct mcc_code_word_rx
{
    uint16_t miles;
    uint16_t spid;
    uint8_t yz_mod;
    uint8_t channel;
} mcc_code_word_rx_t;

// pin definition struct to start_mcc_receive_loop
typedef struct
{
    int pin[16]; // GPIO pin ESP32=>(0-39) ESP32S3=>(0-48) , -1 - disable 
} mcc_capture_config_t;

/**
 * @brief Start mcc capture
 *        mcc data returned in queue MCC_RX_QUEUE_NAME with mcc_code_word_rx_t format structure
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
esp_err_t start_mcc_receive_loop(mcc_capture_config_t *config);

void rmt_mcc_transmit_init(void);

void rmt_mcc_word_transmit_data(mcc_code_word_tx_t mcc_tx_word);