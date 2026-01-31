/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --bpp 1 --size 12 --font /home/labcnc/Desktop/LVGLui/assets/whitrabt.ttf -o /home/labcnc/Desktop/LVGLui/assets/ui_font_whitrabt12.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_WHITRABT12
#define UI_FONT_WHITRABT12 1
#endif

#if UI_FONT_WHITRABT12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xcf,

    /* U+0022 "\"" */
    0xb4,

    /* U+0023 "#" */
    0x52, 0xbe, 0xa5, 0x7d, 0x4a,

    /* U+0024 "$" */
    0x10, 0xfb, 0x43, 0xe1, 0x42, 0x9f, 0x8,

    /* U+0025 "%" */
    0xce, 0x46, 0x66, 0x62, 0x31,

    /* U+0026 "&" */
    0x45, 0x29, 0xc4, 0x56, 0x5d,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0xea, 0xab,

    /* U+0029 ")" */
    0xd5, 0x57,

    /* U+002A "*" */
    0x53, 0xbe, 0xe5, 0x0,

    /* U+002B "+" */
    0x21, 0x9, 0xf2, 0x10,

    /* U+002C "," */
    0xf8,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x4, 0x10, 0x84, 0x21, 0x8, 0x20,

    /* U+0030 "0" */
    0x7a, 0x18, 0xe7, 0xe7, 0x18, 0x5e,

    /* U+0031 "1" */
    0x59, 0x24, 0x97,

    /* U+0032 "2" */
    0x7b, 0x10, 0x43, 0x18, 0xc6, 0x1f,

    /* U+0033 "3" */
    0x7f, 0x10, 0x47, 0x4, 0x14, 0x5e,

    /* U+0034 "4" */
    0x14, 0xa5, 0x29, 0x7c, 0x42,

    /* U+0035 "5" */
    0xfe, 0x8, 0x3e, 0x4, 0x10, 0x7e,

    /* U+0036 "6" */
    0xf2, 0x8, 0x3e, 0x86, 0x18, 0x5e,

    /* U+0037 "7" */
    0xfc, 0x10, 0x43, 0x18, 0xc6, 0x30,

    /* U+0038 "8" */
    0xfe, 0x18, 0x7f, 0x86, 0x18, 0x5e,

    /* U+0039 "9" */
    0xfa, 0x18, 0x7f, 0x4, 0x10, 0x7f,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0x3e,

    /* U+003C "<" */
    0x12, 0x48, 0x84, 0x21,

    /* U+003D "=" */
    0xf8, 0x3e,

    /* U+003E ">" */
    0x8c, 0x63, 0x36, 0xc8,

    /* U+003F "?" */
    0x7b, 0x10, 0x43, 0x18, 0x40, 0x4,

    /* U+0040 "@" */
    0x7a, 0x19, 0xeb, 0xae, 0x68, 0x3f,

    /* U+0041 "A" */
    0x7a, 0x18, 0x7f, 0x86, 0x18, 0x61,

    /* U+0042 "B" */
    0xfe, 0x18, 0x7f, 0x86, 0x18, 0x7e,

    /* U+0043 "C" */
    0x7a, 0x28, 0x20, 0x82, 0x8, 0xde,

    /* U+0044 "D" */
    0xfa, 0x18, 0x61, 0x86, 0x18, 0x7e,

    /* U+0045 "E" */
    0xfe, 0x8, 0x3c, 0x82, 0x8, 0x3f,

    /* U+0046 "F" */
    0xfe, 0x8, 0x3c, 0x82, 0x8, 0x20,

    /* U+0047 "G" */
    0x7e, 0x18, 0x23, 0x86, 0x18, 0x5e,

    /* U+0048 "H" */
    0x86, 0x18, 0x7f, 0x86, 0x18, 0x61,

    /* U+0049 "I" */
    0xe9, 0x24, 0x97,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x4, 0x14, 0x5e,

    /* U+004B "K" */
    0x8e, 0x6b, 0x38, 0xe2, 0xc9, 0xa3,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x3f,

    /* U+004D "M" */
    0x87, 0x3b, 0x61, 0x86, 0x18, 0x61,

    /* U+004E "N" */
    0x87, 0x9b, 0x67, 0x8e, 0x18, 0x61,

    /* U+004F "O" */
    0x7a, 0x18, 0x61, 0x86, 0x18, 0x5e,

    /* U+0050 "P" */
    0xfe, 0x18, 0x7e, 0x82, 0x8, 0x20,

    /* U+0051 "Q" */
    0x7a, 0x18, 0x61, 0x86, 0x79, 0x9d,

    /* U+0052 "R" */
    0xfe, 0x18, 0x7f, 0x86, 0x18, 0x61,

    /* U+0053 "S" */
    0xfe, 0x18, 0x3e, 0x4, 0x18, 0x5e,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x84,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x5e,

    /* U+0056 "V" */
    0x86, 0x18, 0x61, 0x87, 0x37, 0x8c,

    /* U+0057 "W" */
    0x8c, 0x63, 0x5a, 0xd6, 0xbf,

    /* U+0058 "X" */
    0x86, 0x14, 0x8c, 0x31, 0x28, 0x61,

    /* U+0059 "Y" */
    0x8c, 0x76, 0xe2, 0x10, 0x84,

    /* U+005A "Z" */
    0xfc, 0x10, 0x84, 0x21, 0x8, 0x3f,

    /* U+005B "[" */
    0xea, 0xab,

    /* U+005C "\\" */
    0x82, 0x4, 0x8, 0x10, 0x20, 0x41,

    /* U+005D "]" */
    0xd5, 0x57,

    /* U+005E "^" */
    0x3, 0xb7, 0x10,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0x34,

    /* U+0061 "a" */
    0x3c, 0x10, 0x5f, 0xc5, 0xf0,

    /* U+0062 "b" */
    0x82, 0xf, 0xa1, 0x86, 0x18, 0x7e,

    /* U+0063 "c" */
    0x7a, 0x28, 0x20, 0x89, 0xe0,

    /* U+0064 "d" */
    0x4, 0x17, 0xe1, 0x86, 0x18, 0x5f,

    /* U+0065 "e" */
    0x7a, 0x18, 0x7f, 0x83, 0xc0,

    /* U+0066 "f" */
    0x74, 0xe4, 0x44, 0x44,

    /* U+0067 "g" */
    0xfe, 0x18, 0x7f, 0x4, 0x13, 0xc0,

    /* U+0068 "h" */
    0x82, 0xf, 0xa1, 0x86, 0x18, 0x61,

    /* U+0069 "i" */
    0x43, 0x24, 0x97,

    /* U+006A "j" */
    0x8, 0x6, 0x10, 0x84, 0x29, 0x70,

    /* U+006B "k" */
    0x88, 0x9b, 0xee, 0xb9,

    /* U+006C "l" */
    0xc9, 0x24, 0x97,

    /* U+006D "m" */
    0xfd, 0x6b, 0x5a, 0xd4,

    /* U+006E "n" */
    0xfa, 0x18, 0x61, 0x86, 0x10,

    /* U+006F "o" */
    0x7a, 0x18, 0x61, 0x85, 0xe0,

    /* U+0070 "p" */
    0xfe, 0x18, 0x7e, 0x82, 0x8, 0x0,

    /* U+0071 "q" */
    0xfe, 0x18, 0x7f, 0x4, 0x10, 0x40,

    /* U+0072 "r" */
    0xf4, 0xa1, 0x8, 0x40,

    /* U+0073 "s" */
    0x7e, 0x8, 0x1e, 0xf, 0xe0,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x43,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x85, 0xf0,

    /* U+0076 "v" */
    0x86, 0x18, 0x73, 0x78, 0xc0,

    /* U+0077 "w" */
    0x8d, 0x6b, 0x5a, 0xfc,

    /* U+0078 "x" */
    0x8e, 0xdc, 0xed, 0xc4,

    /* U+0079 "y" */
    0x86, 0x18, 0x7f, 0x4, 0x13, 0xc0,

    /* U+007A "z" */
    0xf8, 0xcc, 0xcc, 0x7c,

    /* U+007B "{" */
    0x69, 0x64, 0x91,

    /* U+007C "|" */
    0xff,

    /* U+007D "}" */
    0xc9, 0x34, 0x94,

    /* U+007E "~" */
    0x71, 0xa8, 0x30,

    /* U+007F "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 110, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 110, .box_w = 3, .box_h = 2, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 4, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 9, .adv_w = 110, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 21, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 110, .box_w = 1, .box_h = 2, .ofs_x = 3, .ofs_y = 6},
    {.bitmap_index = 27, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 29, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 35, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 39, .adv_w = 110, .box_w = 2, .box_h = 3, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 40, .adv_w = 110, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 41, .adv_w = 110, .box_w = 2, .box_h = 2, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 69, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 74, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 110, .box_w = 2, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 108, .adv_w = 110, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 110, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 114, .adv_w = 110, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 124, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 136, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 246, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 282, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 110, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 110, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 293, .adv_w = 110, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 294, .adv_w = 110, .box_w = 2, .box_h = 3, .ofs_x = 3, .ofs_y = 6},
    {.bitmap_index = 295, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 110, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 332, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 347, .adv_w = 110, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 374, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 380, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 110, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 417, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 110, .box_w = 1, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 425, .adv_w = 110, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 110, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 431, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 96, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_whitrabt12 = {
#else
lv_font_t ui_font_whitrabt12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_WHITRABT12*/

