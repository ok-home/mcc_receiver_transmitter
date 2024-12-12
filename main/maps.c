#include <stdint.h>
#include <stdio.h>
#include "esp_timer.h"
#include "mcc_encoder.h"
//0-36
const uint16_t miles[37] = 
 {
    0x5a3,0x64b,0x563,0x6a3,0x653,0x4ab,0x533,0xdb,0x16b,0x49b,0x4b3,0x693,0x26b,0x29b,0x4d3,0x1ab,
    0x553,0x54b,0x633,0x46b,0x51b,0x15b,0x663,0x68b,0x743,0x58b,0x593,0x713,0x43b,0x723,0x70b,0x62b,
    0x4cb,0x52b,0x61b,0x6c3,0x45b
 };
//1-330
const uint16_t spid[331] = 
{
0x00, // zero spid not in table
0x00f,0x017,0x01b,0x01d,0x01e,0x027,0x02b,0x02d,0x02e,0x033,0x035,0x036,0x039,0x03a,0x03c,0x047,
0x04b,0x04d,0x04e,0x053,0x055,0x056,0x059,0x05a,0x05c,0x063,0x065,0x066,0x069,0x06a,0x06c,0x071,
0x072,0x074,0x078,0x087,0x08b,0x08d,0x08e,0x093,0x095,0x096,0x099,0x09a,0x09c,0x0a3,0x0a5,0x0a6,
0x0a9,0x0aa,0x0ac,0x0b1,0x0b2,0x0b4,0x0b8,0x0c3,0x0c5,0x0c6,0x0c9,0x0ca,0x0cc,0x0d1,0x0d2,0x0d4,
0x0d8,0x0e1,0x0e2,0x0e4,0x0e8,0x0f0,0x107,0x10b,0x10d,0x10e,0x113,0x115,0x116,0x119,0x11a,0x11c,
0x123,0x125,0x126,0x129,0x12a,0x12c,0x131,0x132,0x134,0x138,0x143,0x145,0x146,0x149,0x14a,0x14c,
0x151,0x152,0x154,0x158,0x161,0x162,0x164,0x168,0x170,0x183,0x185,0x186,0x189,0x18a,0x18c,0x191,
0x192,0x194,0x198,0x1a1,0x1a2,0x1a4,0x1a8,0x1b0,0x1c1,0x1c2,0x1c4,0x1c8,0x1d0,0x1e0,0x207,0x20b,
0x20d,0x20e,0x213,0x215,0x216,0x219,0x21a,0x21c,0x223,0x225,0x226,0x229,0x22a,0x22c,0x231,0x232,
0x234,0x238,0x243,0x245,0x246,0x249,0x24a,0x24c,0x251,0x252,0x254,0x258,0x261,0x262,0x264,0x268,
0x270,0x283,0x285,0x286,0x289,0x28a,0x28c,0x291,0x292,0x294,0x298,0x2a1,0x2a2,0x2a4,0x2a8,0x2b0,
0x2c1,0x2c2,0x2c4,0x2c8,0x2d0,0x2e0,0x303,0x305,0x306,0x309,0x30a,0x30c,0x311,0x312,0x314,0x318,
0x321,0x322,0x324,0x328,0x330,0x341,0x342,0x344,0x348,0x350,0x360,0x381,0x382,0x384,0x388,0x390,
0x3a0,0x3c0,0x407,0x40b,0x40d,0x40e,0x413,0x415,0x416,0x419,0x41a,0x41c,0x423,0x425,0x426,0x429,
0x42a,0x42c,0x431,0x432,0x434,0x438,0x443,0x445,0x446,0x449,0x44a,0x44c,0x451,0x452,0x454,0x458,
0x461,0x462,0x464,0x468,0x470,0x483,0x485,0x486,0x489,0x48a,0x48c,0x491,0x492,0x494,0x498,0x4a1,
0x4a2,0x4a4,0x4a8,0x4b0,0x4c1,0x4c2,0x4c4,0x4c8,0x4d0,0x4e0,0x503,0x505,0x506,0x509,0x50a,0x50c,
0x511,0x512,0x514,0x518,0x521,0x522,0x524,0x528,0x530,0x541,0x542,0x544,0x548,0x550,0x560,0x581,
0x582,0x584,0x588,0x590,0x5a0,0x5c0,0x603,0x605,0x606,0x609,0x60a,0x60c,0x611,0x612,0x614,0x618,
0x621,0x622,0x624,0x628,0x630,0x641,0x642,0x644,0x648,0x650,0x660,0x681,0x682,0x684,0x688,0x690,
0x6a0,0x6c0,0x701,0x702,0x704,0x708,0x710,0x720,0x740,0x780
};

const uint16_t model[] = 
{//|  0  |    1                                            6                 8                10                                               
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //00
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //00
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //01
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //02
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //03
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //04
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //05
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //06
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //07
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //08
   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //09
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,1,1,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //10
   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, //00
};

#if 1
#define WIN_SIZE (3)
#define BIN_SIZE (3)
#define TIME_SLOT_SIZE (BIN_SIZE*16)
#define MCC_WORD_SIZE (TIME_SLOT_SIZE*11)
#define FRAME_SIZE (13*16*3)

typedef union {
   struct {
      uint32_t pattern_found:1;
      uint32_t miles_decoded:1;
      uint32_t spid_decoded:1;
      uint32_t yz_decoded:1;
      uint32_t end_of_data_frame:1;
   };
   uint32_t val;
} decode_state_t;

typedef struct decode {
   decode_state_t decode_state;
   mcc_code_word_t decode_result;
   uint16_t *decode_bit_offset;
   uint32_t decode_channel;
} decode_t;

decode_t channel_array[4] = {0};

uint32_t decode_channel(uint32_t channel,uint16_t const *ptr)
{
   uint8_t y_mode = 0;
   uint8_t z_mode = 0;
   uint8_t yz_mode = 0;
   uint16_t bit_mask = 0;
   bit_mask = 1<<channel;
   uint16_t *mem_ptr = ptr;//channel_array[channel].decode_bit_offset;
   if(ptr <= channel_array[channel].decode_bit_offset) return -1; // already decoded
   if((*mem_ptr&bit_mask) == 0){
      channel_array[channel].decode_bit_offset = ptr;
      return -1; // not found
   }
   uint32_t miles = 0;
   uint32_t spid = 0;
   // miles
   for(int i=0; i<11 ; i++) 
   {
      uint32_t bit0 = ((mem_ptr[(TIME_SLOT_SIZE*i)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+1]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+2]>>channel)&1);
      if ( bit0 > 1) 
      {
         miles |= 1<<11;
      }
      miles >>=1;

      uint32_t bit6 = ((mem_ptr[(TIME_SLOT_SIZE*i)+(BIN_SIZE*6)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+1+(BIN_SIZE*6)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+2+(BIN_SIZE*6)]>>channel)&1);
      uint32_t bit8 = ((mem_ptr[(TIME_SLOT_SIZE*i)+(BIN_SIZE*8)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+1+(BIN_SIZE*8)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+2+(BIN_SIZE*8)]>>channel)&1);
      uint32_t bit10 = ((mem_ptr[(TIME_SLOT_SIZE*i)+(BIN_SIZE*10)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+1+(BIN_SIZE*10)]>>channel)&1) + ((mem_ptr[(TIME_SLOT_SIZE*i)+2+(BIN_SIZE*10)]>>channel)&1);

      bit6 = bit6 > 1 ? 1:0;
      bit8 = bit8 > 1 ? 1:0;
      bit10 = bit10 > 1 ? 1:0;

    if(bit6 || bit8 || bit10)
    {
        y_mode <<= 1;
        z_mode <<= 1;
        y_mode |= bit6;
        z_mode |= bit10;

        spid |= 1<<11;
    }
        spid >>=1;

//    printf("i=%d bit0 %d bit6 %d bit8 %d bit10 %d\n",i,bp[i].bit0,bp[i].bit6,bp[i].bit8,bp[i].bit10);
   }
   yz_mode = (y_mode<<4) | z_mode ;
   //printf("miles =%lx yz=%x spid = %lx\n",miles,yz_mode,spid,);
   if( (miles & 0x7) != 3) return -1;
//check miles
//
//   
   //printf("found miles %lx\n",miles);
   
   channel_array[channel].decode_bit_offset = mem_ptr+MCC_WORD_SIZE;
   return 0;

}

void test_time(void)
{
   uint64_t t0 = esp_timer_get_time();
   for(int i=0;i<16;i++)
   {
      decode_channel(0,&model[48]);
   }
   uint64_t t1 = esp_timer_get_time();
   printf("time %llu\n",t1-t0);

}

#endif