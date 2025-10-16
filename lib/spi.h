#ifndef SPI_H
#define SPI_H
#include "stm32f103xb.h"

void spi_init(SPI_TypeDef* SPIx);
void spi_ss(SPI_TypeDef* SPIx);
void spi_ds(SPI_TypeDef* SPIx);
char spi_rw_byte(SPI_TypeDef* SPIx, char data);

#endif