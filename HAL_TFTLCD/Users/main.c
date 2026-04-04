#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SPI/spi.h"
#include "./stm32f1xx_hal.h"
#include "./BSP/ST7789V/st7789v.h"
#include "./image.h"

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    usart_init(115200);

    st7789v_init();
    
    st7789v_clear(0xFFFF);
    st7789v_clear(0x0000);

//    // 准备一行数据（用于演示）
//    uint16_t row_data[LCD_WIDTH];
//    for (uint16_t i = 0; i < LCD_WIDTH; i++) 
//    {
//        row_data[i] = 0xF800;   // 红色
//    }
//    
//    uint8_t dma_row_buffer[LCD_WIDTH*2];
//    for(uint16_t i=0;i<LCD_WIDTH;i++)
//    {
//        dma_row_buffer[2*i] = (row_data[i] >> 8)&0xFF;
//        dma_row_buffer[2*i+1] = row_data[i] & 0xFF;
//    }
//    
//    SPI_W_RS(0);
//    st7789v_setwindows(0,0,LCD_WIDTH,LCD_HEIGHT);
//    SPI_W_RS(1);
//    
//    for(uint16_t j=0;j< LCD_HEIGHT;j++)
//    {
//        while(__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_BSY));
//        HAL_SPI_Transmit_DMA(&hspi2,dma_row_buffer, LCD_WIDTH * 2);
//    }

    
    st7789v_display_image_dma(g_image, LCD_WIDTH, LCD_HEIGHT);
    while(1)
    {
        
//        delay_ms(500);
//        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
    }
}
