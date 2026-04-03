#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./stm32f1xx_hal.h"
#include "./BSP/ST7789V/st7789v.h"

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);

    st7789v_init();
    
    st7789v_clear(0xFFFF);
    st7789v_clear(0xF800);
    st7789v_clear(0xFD20);
    st7789v_clear(0xFFE0);
    st7789v_clear(0x07E0);
    st7789v_clear(0x001F);
    st7789v_clear(0x4810);
    st7789v_clear(0x911F);
    st7789v_clear(0x0000);
//    st7789v_drawpoint(24,24,0xf800);
//    lcd_printf(10,20,0x0000, 0xFFFF,"Hellow World!");   // 白色字体，黑色背景
//    st7789v_show_num(10, 40, 12345, 0x0000, 0xFFFF);        // 绿色数字
//    st7789v_show_hex(10, 60, 0xABCD, 4, 0x0000, 0xFFFF);    // 红色十六进制（4位，显示"ABCD"）
    while(1)
    {
        
    }
}
