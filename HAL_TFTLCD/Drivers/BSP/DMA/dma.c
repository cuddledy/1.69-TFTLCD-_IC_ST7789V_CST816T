#include "./dma.h"
#include "./SYSTEM/usart/usart.h"

DMA_HandleTypeDef hdma_spi2_tx;
// 用于DMA传输完成回调的静态变量
volatile uint8_t dma_transfer_complete = 0;

void dma_spi2_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    hdma_spi2_tx.Instance=DMA1_Channel5;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi2_tx.Init.Mode = DMA_NORMAL;        // 单次传输，不是循环
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_spi2_tx);
    
    //将DMA句柄关联到SPI
    __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);
    
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    
}

void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

// DMA传输完成中断回调
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2) 
    {
        dma_transfer_complete = 1;
    }
}