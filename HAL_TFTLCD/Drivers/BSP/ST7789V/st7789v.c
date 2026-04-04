#include "./st7789v.h"
#include "./BSP/LCD/font.h"
#include "./BSP/SPI/spi.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/DMA/dma.h"
#include "string.h"

//写指令
void st7789v_write_cmd(uint8_t cmd)
{
    SPI_W_RS(0);  // 设置为命令模式
    SPI_W_CS(0);
    spi_send_byte(cmd);
    //SPI_W_CS(1);
}

//写数据
void st7789v_write_data(uint8_t data)
{
    SPI_W_RS(1);  // 设置为数据模式
    SPI_W_CS(0);
    spi_send_byte(data);
    //SPI_W_CS(1);
}

//写16位数据，用于RGB565颜色
void st7789v_write_data16(uint16_t data)
{
    SPI_W_RS(1);
    SPI_W_CS(0);
    uint8_t high = (data >> 8)&0xFF;
    uint8_t low  = data & 0xFF;
    st7789v_write_data(high);
    st7789v_write_data(low);
    SPI_W_CS(1);
}

//硬件复位
void st7789v_reset(void)
{
    SPI_W_RST(1);
    delay_ms(100);
    SPI_W_RST(0);    //复位
    delay_ms(100);
    SPI_W_RST(1);
    delay_ms(100);
}

//初始化寄存器
void st7789v_initreg(void)
{
    st7789v_write_cmd(0x36);    //内存访问控制
    st7789v_write_data(0x00);    //空操作
    
    st7789v_write_cmd(0x3A);    //像素格式
    st7789v_write_data(0x05);    //低3位：控制接口格式（011=12bpp,101=16bpp,110=18bpp）
                                 //高3位：RGB接口格式（101=16bpp,110=18bpp）
    st7789v_write_cmd(0xB2);    //前后肩设置
    st7789v_write_data(0x0B);
    st7789v_write_data(0x0B);
    st7789v_write_data(0x00);
    st7789v_write_data(0x33);
    st7789v_write_data(0x35);
    
    st7789v_write_cmd(0xB7);    //栅极电压设置
    st7789v_write_data(0x11);
    
    st7789v_write_cmd(0xBB);    //VCOM电压
    st7789v_write_data(0x35);
    
    st7789v_write_cmd(0xC0);    //LCM控制
    st7789v_write_data(0x2C);
    
    st7789v_write_cmd(0xC2);    //使能VDV/VRH写
    st7789v_write_data(0x01);
    
    st7789v_write_cmd(0xC3);    //VRH（AVDD）设置
    st7789v_write_data(0x0D);
    
    st7789v_write_cmd(0xC4);    //VDV（灰度电压）设置
    st7789v_write_data(0x20);
    
    st7789v_write_cmd(0xC6);    //正常模式帧率
    st7789v_write_data(0x13);
    
    st7789v_write_cmd(0xD0);    //电源控制1
    st7789v_write_data(0xA4);
    st7789v_write_data(0xA1);

    st7789v_write_cmd(0xD6);
    st7789v_write_data(0xA1);

    st7789v_write_cmd(0xE0);    //正Gamma校正
    st7789v_write_data(0xF0);
    st7789v_write_data(0x06);
    st7789v_write_data(0x0B);
    st7789v_write_data(0x0A);
    st7789v_write_data(0x09);
    st7789v_write_data(0x26);
    st7789v_write_data(0x29);
    st7789v_write_data(0x33);
    st7789v_write_data(0x41);
    st7789v_write_data(0x18);
    st7789v_write_data(0x16);
    st7789v_write_data(0x15);
    st7789v_write_data(0x29);
    st7789v_write_data(0x2D);

    st7789v_write_cmd(0xE1);    //负Gamma校正
    st7789v_write_data(0xF0);
    st7789v_write_data(0x04);
    st7789v_write_data(0x08);
    st7789v_write_data(0x08);
    st7789v_write_data(0x07);
    st7789v_write_data(0x03);
    st7789v_write_data(0x28);
    st7789v_write_data(0x32);
    st7789v_write_data(0x40);
    st7789v_write_data(0x3B);
    st7789v_write_data(0x19);
    st7789v_write_data(0x18);
    st7789v_write_data(0x2A);
    st7789v_write_data(0x2E);

    st7789v_write_cmd(0xE4);    //栅极控制
    st7789v_write_data(0x25);
    st7789v_write_data(0x00);
    st7789v_write_data(0x00);

    st7789v_write_cmd(0x21);    //颜色反转开启

    st7789v_write_cmd(0x11);    //睡眠退出
    delay_ms(120);
    st7789v_write_cmd(0x29);    //显示开启
}

//初始化
void st7789v_init(void)
{
    spi_init();
    delay_ms(100); // 添加延时确保SPI初始化完成
    
    st7789v_reset();
    
    st7789v_write_cmd(0x36);    //控制显存的读写方向
    st7789v_write_data(0x00);    //正常从上到下、从左到右扫描
    
    st7789v_initreg();
}

//设置显示窗口
void st7789v_setwindows(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend)
{
    
    //横坐标
    st7789v_write_cmd(0x2A);
    st7789v_write_data(xstart >> 8);
    st7789v_write_data(xstart);
    st7789v_write_data((xend-1) >> 8);
    st7789v_write_data(xend-1);

    //纵坐标
    st7789v_write_cmd(0x2B);
    st7789v_write_data((ystart+20) >> 8);
    st7789v_write_data(ystart+20);
    st7789v_write_data((yend+20-1) >> 8);
    st7789v_write_data(yend+20-1);
    
    st7789v_write_cmd(0x2C);    //开始写内存
}

// 清屏函数（填充指定颜色）
void st7789v_clear(uint16_t color)
{
    uint16_t i,j;
    st7789v_setwindows(0,0,LCD_WIDTH,LCD_HEIGHT);
    for(i=0;i<LCD_WIDTH;i++)
    {
        for(j=0;j<LCD_HEIGHT;j++)
        {
            st7789v_write_data16(color);
        }
    } 
}

//将图片缓存区的数据发送到屏幕
void st7789v_display(uint16_t *Image)
{
    uint16_t i,j;
    st7789v_setwindows(0, 0,LCD_WIDTH,LCD_HEIGHT);
    
    for(i = 0; i < LCD_HEIGHT; i++)       // 通常外层循环行(Y)，内层循环列(X)
    {    
        for(j = 0; j < LCD_WIDTH; j++) 
        {    
            uint16_t color = Image[i * LCD_WIDTH + j]; // 统一使用 WIDTH 计算偏移
            st7789v_write_data16(color);
        }
    }

}

//窗口显示
void st7789v_display_windows(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t *Image)
{
    // display
    uint32_t Addr = 0;
    uint16_t i,j;
    
    st7789v_setwindows(xstart, ystart, xend , xend);
    SPI_W_RS(1);
    
    for (i = ystart; i < yend - 1; i++) {
        Addr = ystart + i * LCD_WIDTH ;
        for(j=xstart;j<yend-1;j++) {
            st7789v_write_data16(*(Image+Addr+j));
        }
    }
}

//画点
void st7789v_drawpoint(uint16_t X, uint16_t Y, uint16_t Color)
{
    st7789v_setwindows(X, Y, X, Y);
    st7789v_write_data16(Color);
}

/**
 * @brief   在指定位置显示一个 16×8 的 ASCII 字符（逐列低位在前字库）
 * @param   x       : 起始 X 坐标（左上角）
 * @param   y       : 起始 Y 坐标（左上角）
 * @param   ch      : 要显示的字符
 * @param   fc      : 字体颜色（前景色）
 * @param   bc      : 背景颜色
 */
void st7789v_show_char(uint16_t x, uint16_t y, char ch, uint16_t fc, uint16_t bc)
{
    uint8_t col, row;
    uint16_t col_data;
    uint16_t px, py;

    if (ch < ' ' || ch > '~') ch = ' ';
    uint8_t index = ch - ' ';

    for (col = 0; col < 8; col++)
    {
        // 每列占 2 个字节，低字节为上半部分（行0-7），高字节为下半部分（行8-15）
        uint8_t low_byte = ascii_1608[index][col * 2];
        uint8_t high_byte = ascii_1608[index][col * 2 + 1];
        col_data = ((uint16_t)high_byte << 8) | low_byte; // 组合成 16 位列数据，低位在上半部分

        px = x + col;
        if (px >= LCD_WIDTH) continue;

        for (row = 0; row < 16; row++)
        {
            py = y + row;
            if (py >= LCD_HEIGHT) continue;
            if (col_data & (1 << row))   // 低位对应第一行
                st7789v_drawpoint(px, py, fc);
            else
                st7789v_drawpoint(px, py, bc);
        }
    }
}

/**
 * @brief   显示字符串（16×8 字体）
 * @param   x, y    : 起始坐标
 * @param   str     : 要显示的字符串
 * @param   fc      : 字体颜色
 * @param   bc      : 背景颜色
 */
void st7789v_show_string(uint16_t x, uint16_t y, const char *str, uint16_t fc, uint16_t bc)
{
    while (*str)
    {
        st7789v_show_char(x, y, *str, fc, bc);
        x += 8;                        // 字符宽度 8 像素
        if (x + 8 > LCD_WIDTH)         // 超出宽度换行
        {
            x = 0;
            y += 16;                   // 行高 16 像素
            if (y + 16 > LCD_HEIGHT) break;
        }
        str++;
    }
}

/**
 * @brief   显示数字（十进制，16×8 字体）
 * @param   x, y    : 起始坐标
 * @param   num     : 要显示的数字（32 位无符号）
 * @param   fc      : 字体颜色
 * @param   bc      : 背景颜色
 */
void st7789v_show_num(uint16_t x, uint16_t y, uint32_t num, uint16_t fc, uint16_t bc)
{
    char buf[12];
    sprintf(buf, "%u", num);
    st7789v_show_string(x, y, buf, fc, bc);
}

/**
 * @brief   显示十六进制数（16×8 字体）
 * @param   x, y    : 起始坐标
 * @param   num     : 要显示的数字（32 位无符号）
 * @param   len     : 固定显示位数（1~8），不足前面补零，0 表示不补零
 * @param   fc      : 字体颜色
 * @param   bc      : 背景颜色
 */
void st7789v_show_hex(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint16_t fc, uint16_t bc)
{
    char buf[12];
    if (len == 0)
        sprintf(buf, "%X", num);
    else
        sprintf(buf, "%0*X", len, num);
    st7789v_show_string(x, y, buf, fc, bc);
}

//封装
void lcd_printf(uint16_t x, uint16_t y,uint16_t fc, uint16_t bc, char *format, ...) // 参数增加了 x, y 坐标
{
    char string[100];
    __va_list arg; // 定义一个va_list变量，用于遍历可变参数列表
    
    va_start(arg, format); // 初始化arg，使其指向第一个可变参数
    vsprintf(string, format, arg); // 将格式化后的字符串写入string数组
    va_end(arg); // 清理arg，结束对可变参数列表的遍历

    // 调用 lcd_show_string 显示字符串
    // x, y 是坐标，strlen(string) 是字符串的实际长度
    st7789v_show_string(x, y,(char *)string,fc,bc);
}

//void read_id(void)
//{
//    uint8_t id[3];
//    SPI_W_CS(0);
//    SPI_W_RS(0);                // 命令模式
//    spi_send_byte(0x04);        // RDDID
//    SPI_W_RS(1);                // 数据模式
//    // 第一个 dummy 字节（丢弃）
//    spi_send_read_byte(0xFF);
//    // 读取真正的 ID1, ID2, ID3
//    id[0] = spi_send_read_byte(0xFF);
//    id[1] = spi_send_read_byte(0xFF);
//    id[2] = spi_send_read_byte(0xFF);
//    SPI_W_CS(1);
//    lcd_printf(10, 80, 0x0000, 0xFFFF, "LCD ID: %02X %02X %02X", id[0], id[1], id[2]);
//}

/**
 * @brief 通过DMA分块显示存储在Flash中的图片（RGB565）
 * @param img  图片数组指针（必须为 const，存储在Flash）
 * @param width  图片宽度（像素）
 * @param height 图片高度（像素）
 */
void st7789v_display_image_dma(const uint8_t *img, uint16_t width, uint16_t height)
{
    if (width > LCD_WIDTH || height > LCD_HEIGHT) return;

    // 设置显示窗口
    st7789v_setwindows(0, 0, width, height);
    SPI_W_RS(1);   // 数据模式

    // 行缓冲区（SRAM中，1920字节）
    uint8_t row_buffer[10*width * 2];

    for (uint16_t y = 0; y < height/10; y++) {
        // 从Flash中读取一行，转换为字节流
        for (uint16_t x = 0; x < 10*width*2; x++) {
            row_buffer[x]=img[y*10*width*2+x];
        }
        
//        // 等待上一行DMA完成（第一行跳过）
//        if (y > 0) {
//            while (!dma_transfer_complete);
//            dma_transfer_complete = 0;
//        }

        // 启动当前行的DMA传输
        while(__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_BSY));
        HAL_SPI_Transmit_DMA(&hspi2,row_buffer, 10*width * 2);
    }

    // 等待最后一行完成
    while (!dma_transfer_complete);
    dma_transfer_complete = 0;
//    SPI_W_CS(1);   // 拉高CS
}