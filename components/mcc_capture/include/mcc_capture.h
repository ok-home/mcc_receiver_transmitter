#pragma once

#include "stdint.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

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

#ifdef __cplusplus
}
#endif
