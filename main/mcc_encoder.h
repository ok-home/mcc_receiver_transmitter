#pragma once

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include <driver/gpio.h>
#include "hal/gpio_types.h"
#include "hal/gpio_ll.h"
#include "soc/gpio_struct.h"
#include "soc/i2s_struct.h"
#include "soc/dport_reg.h"
#include "soc/gpio_reg.h"
#include "esp32/rom/lldesc.h"
#include "esp_private/periph_ctrl.h"

#include "driver/rmt_types.h"



#define RMT_BIT_WIDTH (1667)
#define RMT_TX_CLK  (80*1000*1000)
#define RMT_TX_PIN (4)
#define TX_BLOCK_SYMBOL (64)

#define BIN_SIZE (3)
#define TIME_SLOT_SIZE (BIN_SIZE * 16)
#define MCC_WORD_SIZE (TIME_SLOT_SIZE * 11)

#define DMA_FRAME (4032)

#define MCC_HW_DEFAULT_SAMPLE_RATE (144000)
#define MCC_HW_DEFAULT_CHANNELS  (16)


#define MCC_HW_CLK_SAMPLE_RATE 160000000
#define MCC_HW_MIN_GPIO -1
#define MCC_HW_MAX_GPIO 48


#define MCC_PIN_0 (-1)
#define MCC_PIN_1 (-1)
#define MCC_PIN_2 (-1)
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


#define CONFIG_ANALYZER_PCLK_PIN (40)

#define CONFIG_ANALYZER_USE_LEDC_TIMER_FOR_PCLK
#define CONFIG_ANALYZER_LEDC_TIMER_NUMBER (3)
#define CONFIG_ANALYZER_LEDC_CHANNEL_NUMBER (7)

//#define CONFIG_ANALYZER_SEPARATE_MODE



typedef struct rmt_mcc_timeslot {
        rmt_symbol_word_t t_bit0 ; // bit0 -> (level0=1, duration0 = 1), bit1-5 -> (level1=0,duration1=5)
        rmt_symbol_word_t t_bit6 ; // bit6 -> (level0=1, duration0 = 1), bit7 -> (level1=0,duration1=1)
        rmt_symbol_word_t t_bit8 ; // bit8 -> (level0=1, duration0 = 1), bit9 -> (level1=0,duration1=1)
        rmt_symbol_word_t t_bit10 ; // bit10 -> (level0=1, duration0 = 1), bit11-15 -> (level1=0,duration1=5)
} rmt_mcc_timeslot_t;

typedef struct rmt_mcc_word {
   rmt_mcc_timeslot_t mcc_word[11];
   rmt_symbol_word_t rmt_stop;
} rmt_mcc_word_t;

typedef  union {
    struct  {
           uint16_t rollback_buf[TIME_SLOT_SIZE*11];
           uint16_t dma_capture_start[DMA_FRAME/2-TIME_SLOT_SIZE*11];//2016 *16 bit
           uint16_t to_rollback_buf[TIME_SLOT_SIZE*11];
         };
    uint16_t buff[DMA_FRAME/2+TIME_SLOT_SIZE*11];
} mcc_capture_buf_t;

typedef struct mcc_code_word {
        uint16_t miles;
        uint16_t spid;
        uint8_t yz_mod;
} mcc_code_word_t;

typedef struct id_code
{
   uint32_t id;
   uint32_t code;
} id_code_t;



    /**
     * @brief logic analyzer callback
     *
     * if param == 0 -> timeout detected
     *
     */
    typedef void (*mcc_capture_cb_t)(int frame);

    typedef struct
    {
        int pin[16];                           // GPIO pin ESP32=>(0-39) ESP32S3=>(0-48) , -1 - disable , on 8bit mode use lower 8 pin [0,7]
        int meashure_timeout;                  // MAX meashured time in FreeRtos Tick - if timeout = 0 - stop&reset logic analyser
        uint8_t *buf0;
        uint8_t *buf1;
        mcc_capture_cb_t mcc_capture_cb; // logic analyzer callback
    } mcc_capture_config_t;

    
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
    esp_err_t start_mcc_capture(mcc_capture_config_t *config);

    void mcc_capture_stop(void);


    typedef struct
    {
        uint8_t *buf[2];
        lldesc_t *dma; // Pointer of dma frame
    } mcc_frame_t;

    /**
     *  @brief logic analyzer config i2s
     *        configure all i2s struct,before stert
     *
     *  @param- int data_pins   - pointer of data GPIO array pin[16] ( 0-15 )
     *  @param- int pin_trigger - trigger GPIO ( -1 disable )
     *  @param- int sample_rate - real sample rate in HZ
     *  @param- mcc_frame_t *frame - pointer of dma frame ( dma desriptor, sample buffer, sample buffer len )
     *
     *  @return
     */
    void mcc_capture_ll_config(int *data_pins, int sample_rate, int channels, mcc_frame_t *frame);
    /**
     *  @brief logic analyzer start meashure
     *
     */
    void mcc_capture_ll_start();

    /**
     *  @brief logic analyzer stop meashure
     *
     */
    void mcc_capture_ll_stop();
    /**
     *  @brief logic analyzer init dma eof isr
     *          isr after full dma transfer
     *  @param-  TaskHandle_t task  - notify main task after full dma transfer
     *
     *  @return
     */
    esp_err_t mcc_capture_ll_init_dma_eof_isr(TaskHandle_t task);
    /**
     *  @brief logic analyzer free dma eof isr
     *
     *  @return
     */
    void mcc_capture_ll_deinit_dma_eof_isr();
    /**
     *  @brief logic analyzer return real sample rate
     *
     *  @param  int sample_rate  - config sample rate
     *
     *  @return  real sample rate
     */
    int mcc_capture_ll_get_sample_rate(int sample_rate);



// mcc word decode one channel bit to bit
void mcc_word_decode( uint16_t *ptr);
// encode mcc word to  rmt words
void rmt_mcc_word_encode(mcc_code_word_t* mcc_word, rmt_mcc_word_t* rmt_word);

