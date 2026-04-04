#include "./SYSTEM/usart/usart.h"

UART_HandleTypeDef g_uart_init_handle;
uint8_t g_rx_buf[1]={0};
uint8_t g_uart1_rx_flag=0;

void usart_init(uint32_t Baud)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    g_uart_init_handle.Instance=USART1;
    g_uart_init_handle.Init.BaudRate=Baud;
    g_uart_init_handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    g_uart_init_handle.Init.Mode=UART_MODE_TX_RX;
    g_uart_init_handle.Init.Parity=UART_PARITY_NONE;
    g_uart_init_handle.Init.StopBits=UART_STOPBITS_1;
    g_uart_init_handle.Init.WordLength=UART_WORDLENGTH_8B;
    HAL_UART_Init(&g_uart_init_handle);
    
    gpio_init_struct.Mode=GPIO_MODE_AF_PP;
    gpio_init_struct.Pin=GPIO_PIN_9;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);
    
    gpio_init_struct.Mode=GPIO_MODE_AF_INPUT;
    gpio_init_struct.Pin=GPIO_PIN_10;
    gpio_init_struct.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);
    
    
    HAL_UART_Receive_IT(&g_uart_init_handle,(uint8_t*)g_rx_buf,1);
    // 设置中断优先级
    HAL_NVIC_SetPriority(USART1_IRQn, 13, 0);
        // 使能中断
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    
}


void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart_init_handle);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        g_uart1_rx_flag = 1; // 设置标志位
        // 重新启动中断接收，实现连续接收
        HAL_UART_Receive_IT(&g_uart_init_handle, (uint8_t*)g_rx_buf, 1);
    }
}

// 获取接收标志
uint8_t uart_getRxflag(void)
{
    if(g_uart1_rx_flag == 1)
    {
        g_uart1_rx_flag = 0;
        return 1;
    }
    return 0;
}

uint8_t uart_getdata(void)
{
    return g_rx_buf[0];

}

/****************发送******************/
//发送一个字节
void uart_sendbyte(uint8_t byte)
{
    HAL_UART_Transmit(&g_uart_init_handle,&byte,1,1000);
    while((__HAL_UART_GET_FLAG(&g_uart_init_handle, UART_FLAG_TXE))==RESET);
}

//发送数组
void uart_sendarray(uint8_t *array,uint16_t length)
{
    uint16_t i;
    for(i=0;i<length;i++)
    {
        uart_sendbyte(array[i]);
    }
}

//发送字符串
void uart_sendstring(char *String)
{
    uint8_t i;
    for(i=0;String[i]!='\0';i++)
    {
        uart_sendbyte(String[i]);
    }
}

//幂函数
uint32_t uart_pow(uint32_t x,uint32_t y)
{
    uint32_t result=1;
    while(y--)
    {
        result*=x;
    }
    return result;
}

//发送数字
void uart_sendnum(uint32_t number,uint8_t length)
{
    uint8_t i;
    for(i=0;i<length;i++)
    {
        uart_sendbyte(number/uart_pow(10,length-i-1)%10+'0');
    }
}

//重定向printf
int fputc(int ch,FILE *f)
{
    uart_sendbyte((uint8_t)ch);
    return ch;
}

//封装sprintf
void uart_printf(char *format,...)
{
    char string[100];
    __va_list arg;
    va_start(arg,format);
    vsprintf(string,format,arg);
    va_end(arg);
    uart_sendstring(string);
}