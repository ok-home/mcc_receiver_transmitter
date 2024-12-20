#pragma once
#include "mcc_encoder.h"
const id_code_t miles_id_sort[38] = {
    {0, 0x5a3}, {1, 0x64b}, {2, 0x563}, {3, 0x6a3}, {4, 0x653}, {5, 0x4ab}, {6, 0x533}, {7, 0xdb}, {8, 0x16b}, {9, 0x49b}, {10, 0x4b3}, {11, 0x693}, {12, 0x26b}, {13, 0x29b}, {14, 0x4d3}, {15, 0x1ab}, {16, 0x553}, {17, 0x54b}, {18, 0x633}, {19, 0x46b}, {20, 0x51b}, {21, 0x15b}, {22, 0x663}, {23, 0x68b}, {24, 0x743}, {25, 0x58b}, {26, 0x593}, {27, 0x713}, {28, 0x43b}, {29, 0x723}, {30, 0x70b}, {31, 0x62b}, {32, 0x4cb}, {33, 0x52b}, {34, 0x61b}, {35, 0x6c3}, {36, 0x45b}, {37, 0x421} // 37 = BORESIGHT (Continuous Transmission)
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
    {24, 0x743}};
const id_code_t spid_id_code_sort[331] = {
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
    {330, 0x780}};
#if 0
// model for test encoder
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
#endif