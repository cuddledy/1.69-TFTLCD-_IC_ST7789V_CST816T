#ifndef __ST7789V_H
#define __ST7789V_H

#include "./SYSTEM/sys/sys.h"
#include <stdio.h>
#include <stdarg.h>

#define LCD_WIDTH   240
#define LCD_HEIGHT  280

//写指令
void st7789v_write_cmd(uint8_t cmd);

//写数据
void st7789v_write_data(uint8_t data);

//写16位数据，用于RGB565颜色
void st7789v_write_data16(uint16_t data);

//硬件复位
void st7789v_reset(void);

// 清屏函数（填充指定颜色）
void st7789v_clear(uint16_t color);
//初始化
void st7789v_init(void);

//画点
void st7789v_drawpoint(uint16_t X, uint16_t Y, uint16_t Color);

void st7789v_show_char(uint16_t x, uint16_t y, char ch, uint16_t fc, uint16_t bc);
void st7789v_show_string(uint16_t x, uint16_t y, const char *str, uint16_t fc, uint16_t bc);
void st7789v_show_num(uint16_t x, uint16_t y, uint32_t num, uint16_t fc, uint16_t bc);
void st7789v_show_hex(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint16_t fc, uint16_t bc);

void lcd_printf(uint16_t x, uint16_t y,uint16_t fc, uint16_t bc, char *format, ...); // 参数增加了 x, y 坐标
#endif