#include "./BSP/SPI/spi.h"
#include "./SYSTEM/usart/usart.h"
SPI_HandleTypeDef hspi2;

void spi_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // ========== 3. 背光控制引脚 PB3 ==========
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pin = GPIO_PIN_3;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);  // 默认开启背光
    
    //RST->PD2，复位，0复位
    gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pin=GPIO_PIN_1;
    gpio_init_struct.Pull=GPIO_PULLUP;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    
    //SCL->PB13，SPI时钟端，上升沿写入,复用推挽输出
    gpio_init_struct.Mode=GPIO_MODE_AF_PP;
    gpio_init_struct.Pin=GPIO_PIN_13;
    gpio_init_struct.Pull=GPIO_PULLUP;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);
    
    //SDA->PB15，SPI数据端，复用推挽输出
    gpio_init_struct.Mode=GPIO_MODE_AF_PP;
    gpio_init_struct.Pin=GPIO_PIN_15;
    gpio_init_struct.Pull=GPIO_NOPULL;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);
    
    // ========== 7. MISO (PB14) 复用输入 ==========
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;   // SPI 模式下 MISO 为复用推挽（实际是输入）
    gpio_init_struct.Pin = GPIO_PIN_14;
    gpio_init_struct.Pull = GPIO_PULLUP;       // 可加内部上拉
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    
    //DC->PD1，数据命令选择端，0选择命令，1选择数据
    gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pin=GPIO_PIN_1;
    gpio_init_struct.Pull=GPIO_PULLUP;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    
    //CS->PB12，片选，推挽输出
    gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pin=GPIO_PIN_12;
    gpio_init_struct.Pull=GPIO_PULLUP;
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);
    
    //配置SPI2句柄
    hspi2.Instance=SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;           // 主机模式
    hspi2.Init.Direction = SPI_DIRECTION_2LINES; // 全双工 (虽然 LCD 只写，但这是标准配置)
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;     // 8位数据
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;   // SCK 空闲时为低电平 (Mode 0)
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;              // 软件管理 NSS (CS)
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; // 调整波特率分频系数
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;      // MSB 先发
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;      // 禁用 TI 模式
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // 禁用 CRC
    hspi2.Init.CRCPolynomial = 0;               // CRC 多项式 (禁用时无关)
    
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
    
     if(HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        // SPI初始化失败
        printf("SPI Init Error\r\n");
    }
}

void spi_send_byte(uint8_t bytedata)
{
    //SPI_W_CS(0);
    if (HAL_SPI_Transmit(&hspi2, &bytedata, 1, 500) != HAL_OK)
    {
        // 传输错误处理 (可选)
        // Error_Handler();
        printf("Error_Handler\r\n");
        
    }
    //SPI_W_CS(1);
}
