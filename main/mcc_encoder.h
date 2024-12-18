#pragma once
#include "driver/rmt_types.h"

#define RMT_BIT_WIDTH (1667)
#define RMT_TX_CLK  (80*1000*1000)
#define RMT_TX_PIN (4)
#define TX_BLOCK_SYMBOL (64)

#define BIN_SIZE (3)
#define TIME_SLOT_SIZE (BIN_SIZE * 16)
#define MCC_WORD_SIZE (TIME_SLOT_SIZE * 11)
#define FRAME_SIZE (13 * 16 * 3)




#if 0
typedef union {
    struct {
        uint16_t duration0 : 15; /*!< Duration of level0 */
        uint16_t level0 : 1;     /*!< Level of the first part */
        uint16_t duration1 : 15; /*!< Duration of level1 */
        uint16_t level1 : 1;     /*!< Level of the second part */
    };
    uint32_t val; /*!< Equivalent unsigned value for the RMT symbol */
} rmt_symbol_word_t;
#endif

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

typedef struct mcc_code_word {
        uint16_t miles;
        uint16_t spid;
        uint8_t yz_mod;
} mcc_code_word_t;

#define DMA_FRAME 4032
typedef  union {
    struct  {
           uint16_t rollback_buf[TIME_SLOT_SIZE*10];
           uint16_t dma_capture_start[DMA_FRAME/2-TIME_SLOT_SIZE*10];//2016 *16 bit
           uint16_t to_rollback_buf[TIME_SLOT_SIZE*10];
         };
    uint16_t buff[DMA_FRAME/2+TIME_SLOT_SIZE*10];
} mcc_capture_buf_t;




// mcc word decode one channel bit to bit
int mcc_word_decode(uint8_t channel, uint8_t bit);
// encode mcc word to  rmt words
void rmt_mcc_word_encode(mcc_code_word_t* mcc_word, rmt_mcc_word_t* rmt_word);

