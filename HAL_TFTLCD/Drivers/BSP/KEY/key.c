#include "./key.h"
#include "./SYSTEM/delay/delay.h"
#include "stm32f1xx_hal.h"

void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    gpio_init_struct.Pin=GPIO_PIN_4;
    gpio_init_struct.Mode=GPIO_MODE_INPUT;
    gpio_init_struct.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    
    gpio_init_struct.Pin=GPIO_PIN_0;
    gpio_init_struct.Mode=GPIO_MODE_INPUT;
    gpio_init_struct.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);
    
}

uint8_t key_scan(uint8_t mode)
{
    if(mode==0)
    {
        if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0)
        {
            delay_ms(10);   //消抖
            if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0)
            {
                while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0);   //松手才会改变灯的状态
                return 1;   //按键按下
            }
        }
        
    }
    else if(mode==1)
    {
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
        {
            delay_ms(10);   //消抖
            if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
            {   
                return 1;   //按键按下
            }
        }
    }
    return 0;   //按键没有按下
}
