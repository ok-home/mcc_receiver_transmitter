#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include <string.h>
#include "mcc_encoder.h"
#include "logic_analyzer_hal.h"
// 0-36
const uint16_t miles[37] =
    {
        0x5a3, 0x64b, 0x563, 0x6a3, 0x653, 0x4ab, 0x533, 0xdb, 0x16b, 0x49b, 0x4b3, 0x693, 0x26b, 0x29b, 0x4d3, 0x1ab,
        0x553, 0x54b, 0x633, 0x46b, 0x51b, 0x15b, 0x663, 0x68b, 0x743, 0x58b, 0x593, 0x713, 0x43b, 0x723, 0x70b, 0x62b,
        0x4cb, 0x52b, 0x61b, 0x6c3, 0x45b //,0x421 - //37 boresearch
};
// 1-330
const uint16_t spid[331] =
    {
        0x00, // zero spid not in table
        0x00f, 0x017, 0x01b, 0x01d, 0x01e, 0x027, 0x02b, 0x02d, 0x02e, 0x033, 0x035, 0x036, 0x039, 0x03a, 0x03c, 0x047,
        0x04b, 0x04d, 0x04e, 0x053, 0x055, 0x056, 0x059, 0x05a, 0x05c, 0x063, 0x065, 0x066, 0x069, 0x06a, 0x06c, 0x071,
        0x072, 0x074, 0x078, 0x087, 0x08b, 0x08d, 0x08e, 0x093, 0x095, 0x096, 0x099, 0x09a, 0x09c, 0x0a3, 0x0a5, 0x0a6,
        0x0a9, 0x0aa, 0x0ac, 0x0b1, 0x0b2, 0x0b4, 0x0b8, 0x0c3, 0x0c5, 0x0c6, 0x0c9, 0x0ca, 0x0cc, 0x0d1, 0x0d2, 0x0d4,
        0x0d8, 0x0e1, 0x0e2, 0x0e4, 0x0e8, 0x0f0, 0x107, 0x10b, 0x10d, 0x10e, 0x113, 0x115, 0x116, 0x119, 0x11a, 0x11c,
        0x123, 0x125, 0x126, 0x129, 0x12a, 0x12c, 0x131, 0x132, 0x134, 0x138, 0x143, 0x145, 0x146, 0x149, 0x14a, 0x14c,
        0x151, 0x152, 0x154, 0x158, 0x161, 0x162, 0x164, 0x168, 0x170, 0x183, 0x185, 0x186, 0x189, 0x18a, 0x18c, 0x191,
        0x192, 0x194, 0x198, 0x1a1, 0x1a2, 0x1a4, 0x1a8, 0x1b0, 0x1c1, 0x1c2, 0x1c4, 0x1c8, 0x1d0, 0x1e0, 0x207, 0x20b,
        0x20d, 0x20e, 0x213, 0x215, 0x216, 0x219, 0x21a, 0x21c, 0x223, 0x225, 0x226, 0x229, 0x22a, 0x22c, 0x231, 0x232,
        0x234, 0x238, 0x243, 0x245, 0x246, 0x249, 0x24a, 0x24c, 0x251, 0x252, 0x254, 0x258, 0x261, 0x262, 0x264, 0x268,
        0x270, 0x283, 0x285, 0x286, 0x289, 0x28a, 0x28c, 0x291, 0x292, 0x294, 0x298, 0x2a1, 0x2a2, 0x2a4, 0x2a8, 0x2b0,
        0x2c1, 0x2c2, 0x2c4, 0x2c8, 0x2d0, 0x2e0, 0x303, 0x305, 0x306, 0x309, 0x30a, 0x30c, 0x311, 0x312, 0x314, 0x318,
        0x321, 0x322, 0x324, 0x328, 0x330, 0x341, 0x342, 0x344, 0x348, 0x350, 0x360, 0x381, 0x382, 0x384, 0x388, 0x390,
        0x3a0, 0x3c0, 0x407, 0x40b, 0x40d, 0x40e, 0x413, 0x415, 0x416, 0x419, 0x41a, 0x41c, 0x423, 0x425, 0x426, 0x429,
        0x42a, 0x42c, 0x431, 0x432, 0x434, 0x438, 0x443, 0x445, 0x446, 0x449, 0x44a, 0x44c, 0x451, 0x452, 0x454, 0x458,
        0x461, 0x462, 0x464, 0x468, 0x470, 0x483, 0x485, 0x486, 0x489, 0x48a, 0x48c, 0x491, 0x492, 0x494, 0x498, 0x4a1,
        0x4a2, 0x4a4, 0x4a8, 0x4b0, 0x4c1, 0x4c2, 0x4c4, 0x4c8, 0x4d0, 0x4e0, 0x503, 0x505, 0x506, 0x509, 0x50a, 0x50c,
        0x511, 0x512, 0x514, 0x518, 0x521, 0x522, 0x524, 0x528, 0x530, 0x541, 0x542, 0x544, 0x548, 0x550, 0x560, 0x581,
        0x582, 0x584, 0x588, 0x590, 0x5a0, 0x5c0, 0x603, 0x605, 0x606, 0x609, 0x60a, 0x60c, 0x611, 0x612, 0x614, 0x618,
        0x621, 0x622, 0x624, 0x628, 0x630, 0x641, 0x642, 0x644, 0x648, 0x650, 0x660, 0x681, 0x682, 0x684, 0x688, 0x690,
        0x6a0, 0x6c0, 0x701, 0x702, 0x704, 0x708, 0x710, 0x720, 0x740, 0x780};
typedef struct id_code
{
   uint16_t id;
   uint16_t code;
} id_code_t;

const id_code_t miles_id_sort[38] = {
    {0, 0x5a3}, {1, 0x64b}, {2, 0x563}, {3, 0x6a3}, {4, 0x653}, {5, 0x4ab}, {6, 0x533}, {7, 0xdb}, {8, 0x16b}, {9, 0x49b}, {10, 0x4b3}, {11, 0x693}, {12, 0x26b}, {13, 0x29b}, {14, 0x4d3}, {15, 0x1ab}, {16, 0x553}, {17, 0x54b}, {18, 0x633}, {19, 0x46b}, {20, 0x51b}, {21, 0x15b}, {22, 0x663}, {23, 0x68b}, {24, 0x743}, {25, 0x58b}, {26, 0x593}, {27, 0x713}, {28, 0x43b}, {29, 0x723}, {30, 0x70b}, {31, 0x62b}, {32, 0x4cb}, {33, 0x52b}, {34, 0x61b}, {35, 0x6c3}, {36, 0x45b}, {37, 0x421} // 37 = BORESIGHT (Continuous Transmission)
};

const id_code_t spid_id_sort[331] = {
    {0, 0x000},
    {1, 0x00f},
    {2, 0x017},
    {3, 0x01b},
    {4, 0x01d},
    {5, 0x01e},
    {6, 0x027},
    {7, 0x02b},
    {8, 0x02d},
    {9, 0x02e},
    {10, 0x033},
    {11, 0x035},
    {12, 0x036},
    {13, 0x039},
    {14, 0x03a},
    {15, 0x03c},
    {16, 0x047},
    {17, 0x04b},
    {18, 0x04d},
    {19, 0x04e},
    {20, 0x053},
    {21, 0x055},
    {22, 0x056},
    {23, 0x059},
    {24, 0x05a},
    {25, 0x05c},
    {26, 0x063},
    {27, 0x065},
    {28, 0x066},
    {29, 0x069},
    {30, 0x06a},
    {31, 0x06c},
    {32, 0x071},
    {33, 0x072},
    {34, 0x074},
    {35, 0x078},
    {36, 0x087},
    {37, 0x08b},
    {38, 0x08d},
    {39, 0x08e},
    {40, 0x093},
    {41, 0x095},
    {42, 0x096},
    {43, 0x099},
    {44, 0x09a},
    {45, 0x09c},
    {46, 0x0a3},
    {47, 0x0a5},
    {48, 0x0a6},
    {49, 0x0a9},
    {50, 0x0aa},
    {51, 0x0ac},
    {52, 0x0b1},
    {53, 0x0b2},
    {54, 0x0b4},
    {55, 0x0b8},
    {56, 0x0c3},
    {57, 0x0c5},
    {58, 0x0c6},
    {59, 0x0c9},
    {60, 0x0ca},
    {61, 0x0cc},
    {62, 0x0d1},
    {63, 0x0d2},
    {64, 0x0d4},
    {65, 0x0d8},
    {66, 0x0e1},
    {67, 0x0e2},
    {68, 0x0e4},
    {69, 0x0e8},
    {70, 0x0f0},
    {71, 0x107},
    {72, 0x10b},
    {73, 0x10d},
    {74, 0x10e},
    {75, 0x113},
    {76, 0x115},
    {77, 0x116},
    {78, 0x119},
    {79, 0x11a},
    {80, 0x11c},
    {81, 0x123},
    {82, 0x125},
    {83, 0x126},
    {84, 0x129},
    {85, 0x12a},
    {86, 0x12c},
    {87, 0x131},
    {88, 0x132},
    {89, 0x134},
    {90, 0x138},
    {91, 0x143},
    {92, 0x145},
    {93, 0x146},
    {94, 0x149},
    {95, 0x14a},
    {96, 0x14c},
    {97, 0x151},
    {98, 0x152},
    {99, 0x154},
    {100, 0x158},
    {101, 0x161},
    {102, 0x162},
    {103, 0x164},
    {104, 0x168},
    {105, 0x170},
    {106, 0x183},
    {107, 0x185},
    {108, 0x186},
    {109, 0x189},
    {110, 0x18a},
    {111, 0x18c},
    {112, 0x191},
    {113, 0x192},
    {114, 0x194},
    {115, 0x198},
    {116, 0x1a1},
    {117, 0x1a2},
    {118, 0x1a4},
    {119, 0x1a8},
    {120, 0x1b0},
    {121, 0x1c1},
    {122, 0x1c2},
    {123, 0x1c4},
    {124, 0x1c8},
    {125, 0x1d0},
    {126, 0x1e0},
    {127, 0x207},
    {128, 0x20b},
    {129, 0x20d},
    {130, 0x20e},
    {131, 0x213},
    {132, 0x215},
    {133, 0x216},
    {134, 0x219},
    {135, 0x21a},
    {136, 0x21c},
    {137, 0x223},
    {138, 0x225},
    {139, 0x226},
    {140, 0x229},
    {141, 0x22a},
    {142, 0x22c},
    {143, 0x231},
    {144, 0x232},
    {145, 0x234},
    {146, 0x238},
    {147, 0x243},
    {148, 0x245},
    {149, 0x246},
    {150, 0x249},
    {151, 0x24a},
    {152, 0x24c},
    {153, 0x251},
    {154, 0x252},
    {155, 0x254},
    {156, 0x258},
    {157, 0x261},
    {158, 0x262},
    {159, 0x264},
    {160, 0x268},
    {161, 0x270},
    {162, 0x283},
    {163, 0x285},
    {164, 0x286},
    {165, 0x289},
    {166, 0x28a},
    {167, 0x28c},
    {168, 0x291},
    {169, 0x292},
    {170, 0x294},
    {171, 0x298},
    {172, 0x2a1},
    {173, 0x2a2},
    {174, 0x2a4},
    {175, 0x2a8},
    {176, 0x2b0},
    {177, 0x2c1},
    {178, 0x2c2},
    {179, 0x2c4},
    {180, 0x2c8},
    {181, 0x2d0},
    {182, 0x2e0},
    {183, 0x303},
    {184, 0x305},
    {185, 0x306},
    {186, 0x309},
    {187, 0x30a},
    {188, 0x30c},
    {189, 0x311},
    {190, 0x312},
    {191, 0x314},
    {192, 0x318},
    {193, 0x321},
    {194, 0x322},
    {195, 0x324},
    {196, 0x328},
    {197, 0x330},
    {198, 0x341},
    {199, 0x342},
    {200, 0x344},
    {201, 0x348},
    {202, 0x350},
    {203, 0x360},
    {204, 0x381},
    {205, 0x382},
    {206, 0x384},
    {207, 0x388},
    {208, 0x390},
    {209, 0x3a0},
    {210, 0x3c0},
    {211, 0x407},
    {212, 0x40b},
    {213, 0x40d},
    {214, 0x40e},
    {215, 0x413},
    {216, 0x415},
    {217, 0x416},
    {218, 0x419},
    {219, 0x41a},
    {220, 0x41c},
    {221, 0x423},
    {222, 0x425},
    {223, 0x426},
    {224, 0x429},
    {225, 0x42a},
    {226, 0x42c},
    {227, 0x431},
    {228, 0x432},
    {229, 0x434},
    {230, 0x438},
    {231, 0x443},
    {232, 0x445},
    {233, 0x446},
    {234, 0x449},
    {235, 0x44a},
    {236, 0x44c},
    {237, 0x451},
    {238, 0x452},
    {239, 0x454},
    {240, 0x458},
    {241, 0x461},
    {242, 0x462},
    {243, 0x464},
    {244, 0x468},
    {245, 0x470},
    {246, 0x483},
    {247, 0x485},
    {248, 0x486},
    {249, 0x489},
    {250, 0x48a},
    {251, 0x48c},
    {252, 0x491},
    {253, 0x492},
    {254, 0x494},
    {255, 0x498},
    {256, 0x4a1},
    {257, 0x4a2},
    {258, 0x4a4},
    {259, 0x4a8},
    {260, 0x4b0},
    {261, 0x4c1},
    {262, 0x4c2},
    {263, 0x4c4},
    {264, 0x4c8},
    {265, 0x4d0},
    {266, 0x4e0},
    {267, 0x503},
    {268, 0x505},
    {269, 0x506},
    {270, 0x509},
    {271, 0x50a},
    {272, 0x50c},
    {273, 0x511},
    {274, 0x512},
    {275, 0x514},
    {276, 0x518},
    {277, 0x521},
    {278, 0x522},
    {279, 0x524},
    {280, 0x528},
    {281, 0x530},
    {282, 0x541},
    {283, 0x542},
    {284, 0x544},
    {285, 0x548},
    {286, 0x550},
    {287, 0x560},
    {288, 0x581},
    {289, 0x582},
    {290, 0x584},
    {291, 0x588},
    {292, 0x590},
    {293, 0x5a0},
    {294, 0x5c0},
    {295, 0x603},
    {296, 0x605},
    {297, 0x606},
    {298, 0x609},
    {299, 0x60a},
    {300, 0x60c},
    {301, 0x611},
    {302, 0x612},
    {303, 0x614},
    {304, 0x618},
    {305, 0x621},
    {306, 0x622},
    {307, 0x624},
    {308, 0x628},
    {309, 0x630},
    {310, 0x641},
    {311, 0x642},
    {312, 0x644},
    {313, 0x648},
    {314, 0x650},
    {315, 0x660},
    {316, 0x681},
    {317, 0x682},
    {318, 0x684},
    {319, 0x688},
    {320, 0x690},
    {321, 0x6a0},
    {322, 0x6c0},
    {323, 0x701},
    {324, 0x702},
    {325, 0x704},
    {326, 0x708},
    {327, 0x710},
    {328, 0x720},
    {329, 0x740},
    {330, 0x780},
};

const id_code_t miles_code_sort[38] = {
    {7, 0x0db},
    {21, 0x15b},
    {8, 0x16b},
    {15, 0x1ab},
    {12, 0x26b},
    {13, 0x29b},
    {37, 0x421},
    {28, 0x43b},
    {36, 0x45b},
    {19, 0x46b},
    {9, 0x49b},
    {5, 0x4ab},
    {10, 0x4b3},
    {32, 0x4cb},
    {14, 0x4d3},
    {20, 0x51b},
    {33, 0x52b},
    {6, 0x533},
    {17, 0x54b},
    {16, 0x553},
    {2, 0x563},
    {25, 0x58b},
    {26, 0x593},
    {0, 0x5a3},
    {34, 0x61b},
    {31, 0x62b},
    {18, 0x633},
    {1, 0x64b},
    {4, 0x653},
    {22, 0x663},
    {23, 0x68b},
    {11, 0x693},
    {3, 0x6a3},
    {35, 0x6c3},
    {30, 0x70b},
    {27, 0x713},
    {29, 0x723},
    {24, 0x743},
};

const id_code_t spid_code_sort[331] = {
    {0, 0x000},
    {1, 0x00f},
    {2, 0x017},
    {3, 0x01b},
    {4, 0x01d},
    {5, 0x01e},
    {6, 0x027},
    {7, 0x02b},
    {8, 0x02d},
    {9, 0x02e},
    {10, 0x033},
    {11, 0x035},
    {12, 0x036},
    {13, 0x039},
    {14, 0x03a},
    {15, 0x03c},
    {16, 0x047},
    {17, 0x04b},
    {18, 0x04d},
    {19, 0x04e},
    {20, 0x053},
    {21, 0x055},
    {22, 0x056},
    {23, 0x059},
    {24, 0x05a},
    {25, 0x05c},
    {26, 0x063},
    {27, 0x065},
    {28, 0x066},
    {29, 0x069},
    {30, 0x06a},
    {31, 0x06c},
    {32, 0x071},
    {33, 0x072},
    {34, 0x074},
    {35, 0x078},
    {36, 0x087},
    {37, 0x08b},
    {38, 0x08d},
    {39, 0x08e},
    {40, 0x093},
    {41, 0x095},
    {42, 0x096},
    {43, 0x099},
    {44, 0x09a},
    {45, 0x09c},
    {46, 0x0a3},
    {47, 0x0a5},
    {48, 0x0a6},
    {49, 0x0a9},
    {50, 0x0aa},
    {51, 0x0ac},
    {52, 0x0b1},
    {53, 0x0b2},
    {54, 0x0b4},
    {55, 0x0b8},
    {56, 0x0c3},
    {57, 0x0c5},
    {58, 0x0c6},
    {59, 0x0c9},
    {60, 0x0ca},
    {61, 0x0cc},
    {62, 0x0d1},
    {63, 0x0d2},
    {64, 0x0d4},
    {65, 0x0d8},
    {66, 0x0e1},
    {67, 0x0e2},
    {68, 0x0e4},
    {69, 0x0e8},
    {70, 0x0f0},
    {71, 0x107},
    {72, 0x10b},
    {73, 0x10d},
    {74, 0x10e},
    {75, 0x113},
    {76, 0x115},
    {77, 0x116},
    {78, 0x119},
    {79, 0x11a},
    {80, 0x11c},
    {81, 0x123},
    {82, 0x125},
    {83, 0x126},
    {84, 0x129},
    {85, 0x12a},
    {86, 0x12c},
    {87, 0x131},
    {88, 0x132},
    {89, 0x134},
    {90, 0x138},
    {91, 0x143},
    {92, 0x145},
    {93, 0x146},
    {94, 0x149},
    {95, 0x14a},
    {96, 0x14c},
    {97, 0x151},
    {98, 0x152},
    {99, 0x154},
    {100, 0x158},
    {101, 0x161},
    {102, 0x162},
    {103, 0x164},
    {104, 0x168},
    {105, 0x170},
    {106, 0x183},
    {107, 0x185},
    {108, 0x186},
    {109, 0x189},
    {110, 0x18a},
    {111, 0x18c},
    {112, 0x191},
    {113, 0x192},
    {114, 0x194},
    {115, 0x198},
    {116, 0x1a1},
    {117, 0x1a2},
    {118, 0x1a4},
    {119, 0x1a8},
    {120, 0x1b0},
    {121, 0x1c1},
    {122, 0x1c2},
    {123, 0x1c4},
    {124, 0x1c8},
    {125, 0x1d0},
    {126, 0x1e0},
    {127, 0x207},
    {128, 0x20b},
    {129, 0x20d},
    {130, 0x20e},
    {131, 0x213},
    {132, 0x215},
    {133, 0x216},
    {134, 0x219},
    {135, 0x21a},
    {136, 0x21c},
    {137, 0x223},
    {138, 0x225},
    {139, 0x226},
    {140, 0x229},
    {141, 0x22a},
    {142, 0x22c},
    {143, 0x231},
    {144, 0x232},
    {145, 0x234},
    {146, 0x238},
    {147, 0x243},
    {148, 0x245},
    {149, 0x246},
    {150, 0x249},
    {151, 0x24a},
    {152, 0x24c},
    {153, 0x251},
    {154, 0x252},
    {155, 0x254},
    {156, 0x258},
    {157, 0x261},
    {158, 0x262},
    {159, 0x264},
    {160, 0x268},
    {161, 0x270},
    {162, 0x283},
    {163, 0x285},
    {164, 0x286},
    {165, 0x289},
    {166, 0x28a},
    {167, 0x28c},
    {168, 0x291},
    {169, 0x292},
    {170, 0x294},
    {171, 0x298},
    {172, 0x2a1},
    {173, 0x2a2},
    {174, 0x2a4},
    {175, 0x2a8},
    {176, 0x2b0},
    {177, 0x2c1},
    {178, 0x2c2},
    {179, 0x2c4},
    {180, 0x2c8},
    {181, 0x2d0},
    {182, 0x2e0},
    {183, 0x303},
    {184, 0x305},
    {185, 0x306},
    {186, 0x309},
    {187, 0x30a},
    {188, 0x30c},
    {189, 0x311},
    {190, 0x312},
    {191, 0x314},
    {192, 0x318},
    {193, 0x321},
    {194, 0x322},
    {195, 0x324},
    {196, 0x328},
    {197, 0x330},
    {198, 0x341},
    {199, 0x342},
    {200, 0x344},
    {201, 0x348},
    {202, 0x350},
    {203, 0x360},
    {204, 0x381},
    {205, 0x382},
    {206, 0x384},
    {207, 0x388},
    {208, 0x390},
    {209, 0x3a0},
    {210, 0x3c0},
    {211, 0x407},
    {212, 0x40b},
    {213, 0x40d},
    {214, 0x40e},
    {215, 0x413},
    {216, 0x415},
    {217, 0x416},
    {218, 0x419},
    {219, 0x41a},
    {220, 0x41c},
    {221, 0x423},
    {222, 0x425},
    {223, 0x426},
    {224, 0x429},
    {225, 0x42a},
    {226, 0x42c},
    {227, 0x431},
    {228, 0x432},
    {229, 0x434},
    {230, 0x438},
    {231, 0x443},
    {232, 0x445},
    {233, 0x446},
    {234, 0x449},
    {235, 0x44a},
    {236, 0x44c},
    {237, 0x451},
    {238, 0x452},
    {239, 0x454},
    {240, 0x458},
    {241, 0x461},
    {242, 0x462},
    {243, 0x464},
    {244, 0x468},
    {245, 0x470},
    {246, 0x483},
    {247, 0x485},
    {248, 0x486},
    {249, 0x489},
    {250, 0x48a},
    {251, 0x48c},
    {252, 0x491},
    {253, 0x492},
    {254, 0x494},
    {255, 0x498},
    {256, 0x4a1},
    {257, 0x4a2},
    {258, 0x4a4},
    {259, 0x4a8},
    {260, 0x4b0},
    {261, 0x4c1},
    {262, 0x4c2},
    {263, 0x4c4},
    {264, 0x4c8},
    {265, 0x4d0},
    {266, 0x4e0},
    {267, 0x503},
    {268, 0x505},
    {269, 0x506},
    {270, 0x509},
    {271, 0x50a},
    {272, 0x50c},
    {273, 0x511},
    {274, 0x512},
    {275, 0x514},
    {276, 0x518},
    {277, 0x521},
    {278, 0x522},
    {279, 0x524},
    {280, 0x528},
    {281, 0x530},
    {282, 0x541},
    {283, 0x542},
    {284, 0x544},
    {285, 0x548},
    {286, 0x550},
    {287, 0x560},
    {288, 0x581},
    {289, 0x582},
    {290, 0x584},
    {291, 0x588},
    {292, 0x590},
    {293, 0x5a0},
    {294, 0x5c0},
    {295, 0x603},
    {296, 0x605},
    {297, 0x606},
    {298, 0x609},
    {299, 0x60a},
    {300, 0x60c},
    {301, 0x611},
    {302, 0x612},
    {303, 0x614},
    {304, 0x618},
    {305, 0x621},
    {306, 0x622},
    {307, 0x624},
    {308, 0x628},
    {309, 0x630},
    {310, 0x641},
    {311, 0x642},
    {312, 0x644},
    {313, 0x648},
    {314, 0x650},
    {315, 0x660},
    {316, 0x681},
    {317, 0x682},
    {318, 0x684},
    {319, 0x688},
    {320, 0x690},
    {321, 0x6a0},
    {322, 0x6c0},
    {323, 0x701},
    {324, 0x702},
    {325, 0x704},
    {326, 0x708},
    {327, 0x710},
    {328, 0x720},
    {329, 0x740},
    {330, 0x780},
};

const uint16_t model[] =
    {
        //|  0  |    1                                            6                 8                10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 01
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 02
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 03
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 04
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 05
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 06
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 07
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 08
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 09
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 01
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 02
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 03
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 04
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 05
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 06
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 07
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 08
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 09
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 01
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 02
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 03
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 04
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 05
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 06
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 07
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 08
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 09
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
};

#if 1
#define WIN_SIZE (3)
#define BIN_SIZE (3)
#define TIME_SLOT_SIZE (BIN_SIZE * 16)
#define MCC_WORD_SIZE (TIME_SLOT_SIZE * 11)
#define FRAME_SIZE (13 * 16 * 3)

typedef union
{
   struct
   {
      uint32_t pattern_found : 1;
      uint32_t miles_decoded : 1;
      uint32_t spid_decoded : 1;
      uint32_t yz_decoded : 1;
      uint32_t end_of_data_frame : 1;
   };
   uint32_t val;
} decode_state_t;

typedef struct decode
{
   decode_state_t decode_state;
   mcc_code_word_t decode_result;
   uint16_t *decode_bit_offset;
   uint32_t decode_channel;
} decode_t;

decode_t channel_array[4] = {0};

int id_code_compare(const void *a, const void *b)
{
   id_code_t *aa = (id_code_t *)a;
   id_code_t *bb = (id_code_t *)b;
   return (aa->code - bb->code);
}

uint32_t decode_channel(uint32_t channel, uint16_t const *ptr)
{
   uint8_t y_mode = 0;
   uint8_t z_mode = 0;
   uint8_t yz_mode = 0;
   uint16_t bit_mask = 0;
   bit_mask = 1 << channel;
   uint16_t *mem_ptr = ptr; // channel_array[channel].decode_bit_offset;
   if (ptr <= channel_array[channel].decode_bit_offset)
      return -1; // already decoded
   if ((*mem_ptr & bit_mask) == 0)
   {
      channel_array[channel].decode_bit_offset = ptr;
      return -1; // not found
   }
   uint32_t miles = 0;
   uint32_t spid = 0;
   // miles
   for (int i = 0; i < 11; i++)
   {
      uint32_t bit0 = ((mem_ptr[(TIME_SLOT_SIZE * i)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 1] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 2] >> channel) & 1);
      if (bit0 > 1)
      {
         miles |= 1 << 11;
      }
      miles >>= 1;
   }
   if ((miles & 0x7) != 3)
      return -1;
   id_code_t code_miles = {0, miles};
   id_code_t *id_miles = (id_code_t *)bsearch(&code_miles, miles_code_sort, 38, sizeof(id_code_t), id_code_compare);
   if (id_miles == 0)
   {
      return -1;
   }

   for (int i = 0; i < 11; i++)
   {
      uint32_t bit6 = ((mem_ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 6)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 6)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 6)] >> channel) & 1);
      uint32_t bit8 = ((mem_ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 8)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 8)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 8)] >> channel) & 1);
      uint32_t bit10 = ((mem_ptr[(TIME_SLOT_SIZE * i) + (BIN_SIZE * 10)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 1 + (BIN_SIZE * 10)] >> channel) & 1) + ((mem_ptr[(TIME_SLOT_SIZE * i) + 2 + (BIN_SIZE * 10)] >> channel) & 1);

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
   id_code_t *id_spid = (id_code_t *)bsearch(&code_spid, spid_code_sort, 331, sizeof(id_code_t), id_code_compare);
   if (id_spid == 0)
   {
      return -1;
   }
   printf("id_miles = %d yz=%x id_spid=%d\n", id_miles->id, yz_mode, id_spid->id);

   channel_array[channel].decode_bit_offset = mem_ptr + MCC_WORD_SIZE;
   return 0;
}

#endif
typedef struct bins
{
   uint8_t bit0 : 1;
   uint8_t bit6 : 1;
   uint8_t bit8 : 1;
   uint8_t bit10 : 1;
   uint8_t bit_spare : 4;
} bins_t;

typedef struct channel_decode
{
   uint8_t started;
   uint8_t cnt_bins;
   uint8_t cnt_timeslots;
   uint8_t cnt_last_bit; // 0-3
   uint8_t last_bit;     // last 3 bit
   uint8_t y_mode;
   uint8_t z_mode;
   uint8_t yz_mode;
   uint16_t miles;
   uint16_t spid;
   bins_t mcc_word; // [timeslot][bins]
} channel_decode_t;

channel_decode_t channel_data[16] = {0};

int decode(uint8_t channel, uint8_t bit)
{
   channel_decode_t *data = &channel_data[channel];
   bit &= 1;
   if (data->started == 0 && bit == 0)
   {
      return 0;
   }
   else
   {
      data->started = true;
   } // first bit
   data->last_bit += bit;
   data->cnt_last_bit += 1;
   // printf("bit=%d last=%d cnt=%d\n",bit,data->last_bit,data->cnt_last_bit);

   if (data->cnt_last_bit >= 3)
   {
      uint8_t bit_val = data->last_bit > 1 ? 1 : 0;
      // printf("bit_val=%d last=%d cnt_bin=%d slot=%d\n",bit_val,data->last_bit,data->cnt_bins,data->cnt_timeslots);
      switch (data->cnt_bins)
      {
      case 0:
         data->mcc_word.bit0 = bit_val;
         if (data->mcc_word.bit0)
         {
            data->miles |= 1 << 11;
         }
         data->miles >>= 1;
         if (data->cnt_timeslots == 2) // check miles start 110
         {
            if (data->miles >> 8 != 3)
            {
               // clear all
               printf("clear miles = %x \n", data->miles >> 8);
               memset(data, 0, sizeof(channel_decode_t));
               return -1;
            }
         }
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins += 1;
         break;
      case 6:
         data->mcc_word.bit6 = bit_val;
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins += 1;
         break;
      case 8:
         data->mcc_word.bit8 = bit_val;
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins += 1;
         break;
      case 10:
         data->mcc_word.bit10 = bit_val;
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins += 1;
         break;
      case 15:
         if (data->mcc_word.bit6 || data->mcc_word.bit8 || data->mcc_word.bit10)
         {
            data->spid |= 1 << 11;

            data->y_mode <<= 1;
            data->z_mode <<= 1;
            data->y_mode |= data->mcc_word.bit6;
            data->z_mode |= data->mcc_word.bit10;
         }
         data->spid >>= 1;

         if (data->cnt_timeslots == 10) // all mcc word
         {
            data->yz_mode = (data->y_mode << 4) | data->z_mode;
            //               printf("miles = %x, yz=%x, spid=%x\n",data->miles,data->yz_mode,data->spid);

            id_code_t code_miles = {0, data->miles};
            id_code_t *id_miles = (id_code_t *)bsearch(&code_miles, miles_code_sort, 38, sizeof(id_code_t), id_code_compare);

            id_code_t code_spid = {0, data->spid};
            id_code_t *id_spid = (id_code_t *)bsearch(&code_spid, spid_code_sort, 331, sizeof(id_code_t), id_code_compare);

            if (id_miles && id_spid)
            {
               printf("id_miles = %d yz=%x id_spid=%d\n", id_miles->id, data->yz_mode, id_spid->id);
            }
            else
            {
               printf("not found\n");
            }

            memset(data, 0, sizeof(channel_decode_t));
            return 0;
         }
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins = 0;
         data->cnt_timeslots += 1;
         break;
      default:
         data->last_bit = 0;
         data->cnt_last_bit = 0;
         data->cnt_bins += 1;
         break;
      }
   }
   return 0;
}

void mcc_cb(uint8_t *samle_buf, int samples, int sample_rate, int channels)
{
   // printf("buf %p samples=%d sample_rate=%d channels=%d\n",samle_buf, samples,sample_rate,channels);
   uint16_t *samle_buf16 = (uint16_t *)samle_buf;
   if (samples)
   {
      for (int i = 0; i < samples / 2; i++)
      {
         decode(0, samle_buf16[i] & (1));
         // decode_channel(0, &samle_buf16[i]);
         // printf("%x\n",samle_buf16[i]& (1));
      }
   }
   // printf("end\n");
}

logic_analyzer_config_t config = {
    .pin[0] = 4,
    .pin[1] = 5,
    .pin[2] = 6,
    .pin_trigger = -1,
    .trigger_edge = 1,
    .number_channels = 16,
    .sample_rate = 144000,
    .meashure_timeout = 20,
    .number_of_samples = 4032,
    .samples_to_psram = 0,
    .logic_analyzer_cb = mcc_cb};
#include "esp_heap_caps.h"
void test_time(void)
{
   /*  uint64_t t0 = esp_timer_get_time();
     for (int j = 0; j < 2; j++)
     {
           channel_array[0].decode_bit_offset = NULL;

        for (int i = 0; i < sizeof(model)/2-MCC_WORD_SIZE; i++)
        {
        //   decode_channel(0, &model[i]);
        }
     }
        uint64_t t1 = esp_timer_get_time();
           printf("time %llu\n", t1 - t0);

     t0 = esp_timer_get_time();
     for (int j = 0; j < 2; j++)
     {
        memset(channel_data,0,sizeof(channel_data));
        for (int i = 0; i < sizeof(model)/2; i++)
        {
           decode(0, model[i]);
        }
     }
        t1 = esp_timer_get_time();
           printf("time %llu\n", t1 - t0);
  */

   esp_err_t ret = start_logic_analyzer(&config);
   printf("ret=%d\n", ret);
}