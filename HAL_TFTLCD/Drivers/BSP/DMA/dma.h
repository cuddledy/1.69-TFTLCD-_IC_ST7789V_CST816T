#ifndef __DMA_H
#define __DMA_H
#include "./SYSTEM/sys/sys.h"

extern SPI_HandleTypeDef hspi2;
extern volatile uint8_t dma_transfer_complete;

void dma_spi2_init(void);

#endif