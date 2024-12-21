#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc_enc_dec.h"


extern id_code_t miles_id_sort[];
extern id_code_t miles_code_sort[];
extern id_code_t spid_id_code_sort[];

extern QueueHandle_t mcc_rx_queue;

uint16_t *decode_offset[16] = {0};

// comparator for binsearch
static int id_code_compare(const void *a, const void *b)
{
   id_code_t *aa = (id_code_t *)a;
   id_code_t *bb = (id_code_t *)b;
   return (aa->code - bb->code);
}
// mcc word decode one channel bit to bit
void mcc_word_decode( uint16_t *ptr)
{
   mcc_code_word_rx_t result;
   for (uint32_t channel = 0; channel < 16; channel++)
   {
 
      uint32_t y_mode = 0;
      uint32_t z_mode = 0;
      uint32_t yz_mode = 0;
      uint32_t miles = 0;
      uint32_t spid = 0;

      //uint16_t *ptr = ptr; // channel_array[channel].decode_bit_offset;

      if (ptr <= decode_offset[channel])
         continue; // already decoded
      if ((*ptr & (1 << channel)) == 0)
      {
         decode_offset[channel] = ptr;
         continue; // not found
      }
      // miles
      for (int i = 0; i < 11; i++)
      {
         uint32_t bit0 = ((ptr[(TIME_SLOT_SIZE * i)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 1] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 2] >> channel) & 1);
         if (bit0 > 1)
         {
            miles |= 1 << 11;
         }
         miles >>= 1;
      }
      if ((miles & 0x7) != 3)
      {
         continue;
      }

      id_code_t code_miles = {0, miles};
      id_code_t *id_miles = (id_code_t *)bsearch(&code_miles, miles_code_sort, 38, sizeof(id_code_t), id_code_compare);
      if (id_miles == 0)
      {
         printf("err miles\n");
         continue;
      }

      for (int i = 0; i < 11; i++)
      {
         uint32_t bit6 = ((ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 6)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 6)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 6)] >> channel) & 1);
         uint32_t bit8 = ((ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 8)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 8)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 8)] >> channel) & 1);
         uint32_t bit10 = ((ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 10)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 10)] >> channel) & 1) + ((ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 10)] >> channel) & 1);

         bit6 = bit6 > 1 ? 1 : 0;
         bit8 = bit8 > 1 ? 1 : 0;
         bit10 = bit10 > 1 ? 1 : 0;

         if (bit6 || bit8 || bit10)
         {
            y_mode <<= 1;
            z_mode <<= 1;
            y_mode |= bit6;
            z_mode |= bit10;

            spid |= 1 << 11;
         }
         spid >>= 1;
      }
      yz_mode = (y_mode << 4) | z_mode;

      id_code_t code_spid = {0, spid};
      id_code_t *id_spid = (id_code_t *)bsearch(&code_spid, spid_id_code_sort, 331, sizeof(id_code_t), id_code_compare);
      if (id_spid == 0)
      {
         printf("err spid\n");
         continue;
      }
//      printf("id_miles = %ld yz=%lx id_spid=%ld\n", id_miles->id, yz_mode, id_spid->id);

      result.miles = id_miles->id;
      result.spid = id_spid->id;
      result.yz_mod = yz_mode;
      result.channel = channel;
      xQueueSend(mcc_rx_queue,&result,0);

      decode_offset[channel] = ptr + MCC_WORD_SIZE - TIME_SLOT_SIZE;
      continue;
   }
}
static uint16_t mcc_get_miles_code(uint16_t mil)
{                                  // from 0 to 36
   return miles_id_sort[mil].code; // 010 0110 1011
}
static uint16_t mcc_get_spid_code(uint16_t spi)
{                                      // from 1 to 330
   return spid_id_code_sort[spi].code; // 100 0000 0111
}
// encode mcc word to  rmt words
void rmt_mcc_word_encode(mcc_code_word_tx_t *mcc_word, rmt_mcc_word_t *rmt_word)
{
   // clear all bits
   uint16_t miles = mcc_get_miles_code(mcc_word->miles);
   uint16_t spid = mcc_get_spid_code(mcc_word->spid);
   uint8_t y_mod = ((mcc_word->yz_mod) >> 4) & 0xf;
   uint8_t z_mod = (mcc_word->yz_mod) & 0xf;
   // set miles & clear spid
   for (int i = 0; i < 11; i++)
   {
      rmt_word->mcc_word[i].t_bit0.level0 = miles & 1;
      miles >>= 1;
      rmt_word->mcc_word[i].t_bit0.level1 = 0;
      rmt_word->mcc_word[i].t_bit0.duration0 = RMT_BIT_WIDTH;
      rmt_word->mcc_word[i].t_bit0.duration1 = RMT_BIT_WIDTH * 5;

      rmt_word->mcc_word[i].t_bit6.level0 = 0;
      rmt_word->mcc_word[i].t_bit6.level1 = 0;
      rmt_word->mcc_word[i].t_bit6.duration0 = RMT_BIT_WIDTH;
      rmt_word->mcc_word[i].t_bit6.duration1 = RMT_BIT_WIDTH;

      rmt_word->mcc_word[i].t_bit8.level0 = 0;
      rmt_word->mcc_word[i].t_bit8.level1 = 0;
      rmt_word->mcc_word[i].t_bit8.duration0 = RMT_BIT_WIDTH;
      rmt_word->mcc_word[i].t_bit8.duration1 = RMT_BIT_WIDTH;

      rmt_word->mcc_word[i].t_bit10.level0 = 0;
      rmt_word->mcc_word[i].t_bit10.level1 = 0;
      rmt_word->mcc_word[i].t_bit10.duration0 = RMT_BIT_WIDTH;
      rmt_word->mcc_word[i].t_bit10.duration1 = RMT_BIT_WIDTH * 5;
   }
   rmt_word->rmt_stop.val = 0;
   // set spid & yz_mode
   for (int i = 10; i >= 0; i--)
   {
      if ((spid >> i) & 1)
      {
         if (z_mod & 1)
         {
            rmt_word->mcc_word[i].t_bit10.level0 = 1;
         }
         else
         {
            if (y_mod & 1)
            {
               rmt_word->mcc_word[i].t_bit6.level0 = 1;
            }
            else
            {
               rmt_word->mcc_word[i].t_bit8.level0 = 1;
            }
         }
         z_mod >>= 1;
         y_mod >>= 1;
      }
   }
}

