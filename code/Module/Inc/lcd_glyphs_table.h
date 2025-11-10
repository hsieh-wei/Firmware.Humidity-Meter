#ifndef LCD_GLYPHS_TABLE_H
#define LCD_GLYPHS_TABLE_H

#include <stdint.h>

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *data;
} LCD_GLYPHS_HANDLE;

// LCD_Font_7x10 _: [0]0x0000, [1]0x0000, [2]0x0000, [3]0x0000, [4]0x0000, [5]0x0000, [6]0x0000, [7]0x0000, [8]0x0000, [9]"0xFE00",
// uint16_t *data means row(x), [0]  means column(y), [9]"0xFE00" can be observed that it is MSB
extern LCD_GLYPHS_HANDLE LCD_Font_7x10;
extern LCD_GLYPHS_HANDLE LCD_Font_11x18;
extern LCD_GLYPHS_HANDLE LCD_Font_16x26;
extern LCD_GLYPHS_HANDLE LCD_Font_16x26;

#endif // LCD_GLYPHS_TABLE_H