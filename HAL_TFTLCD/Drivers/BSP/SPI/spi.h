#ifndef __SPI_H
#define __SPI_H
#include "./SYSTEM/sys/sys.h"
#include "./stm32f1xx_hal.h"

#define SPI_W_CS(x) do{ x ? \
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); \
                    }while(0)
#define SPI_W_RST(x) do{ x ? \
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); \
                    }while(0)

#define SPI_W_RS(x) do{ x ? \
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); \
                    }while(0)

void spi_init(void);
void spi_send_byte(uint8_t bytedata);

#endif