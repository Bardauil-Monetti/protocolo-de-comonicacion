#include "spi.h"
#include "stm32f103xb.h"
//pines validos para SPI1
#define SS1 4
#define SCK1 5
#define MISO1 6
#define MOSI1 7
//pines validos para SPI2
#define SS2 12
#define SCK2 13
#define MISO2 14
#define MOSI2 15


void spi_init(SPI_TypeDef* SPIx) {
    if(SPIx==SPI1){
        RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
        RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
        GPIOA->CRL&=~(0xF<<(SS1*4)); //limpio
        GPIOA->CRL|=(0x1<<(SS1*4)); //salida push pull
        GPIOA->CRL&=~(0xF<<(SCK1*4)); //limpio
        GPIOA->CRL|=(0xB<<(SCK1*4)); //salida de funcion alternativa push pull
        GPIOA->CRL&=~(0xF<<(MISO1*4)); //limpio
        GPIOA->CRL|=(0x4<<(MISO1*4)); //floating input
        GPIOA->CRL&=~(0xF<<(MOSI1*4)); //limpio
        GPIOA->CRL|=(0xB<<(MOSI1*4)); //salida de funcion alternativa push pull
    }else if(SPIx==SPI2){
        RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
        RCC->APB1ENR|=RCC_APB1ENR_SPI2EN;
        GPIOB->CRH&=~(0xF<<((SS2%8)*4)); //limpio
        GPIOB->CRH|=(0x1<<((SS2%8)*4)); //salida push pull
        GPIOB->CRH&=~(0xF<<((SCK2%8)*4)); //limpio
        GPIOB->CRH|=(0xB<<((SCK2%8)*4)); //salida de funcion alternativa push pull
        GPIOB->CRH&=~(0xF<<((MISO2%8)*4)); //limpio
        GPIOB->CRH|=(0x4<<((MISO2%8)*4)); //floating input
        GPIOB->CRH&=~(0xF<<((MOSI2%8)*4)); //limpio
        GPIOB->CRH|=(0xB<<((MOSI2%8)*4)); //salida de funcion alternativa push pull
    }
        SPIx->CR1 = 0;                                  // LimpiO LA CONFIGURACION
        SPIx->CR1 |= SPI_CR1_MSTR;                      // 
        SPIx->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM;         // 
        SPIx->CR1 |= SPI_CR1_BR_1;                      // 
        SPIx->CR1 &= ~SPI_CR1_CPOL;                     // 
        SPIx->CR1 &= ~SPI_CR1_CPHA;                     //
        SPIx->CR1 |= SPI_CR1_SPE;                       // habilita periférico
}

void SPI_SS(SPI_TypeDef *spi){
    if (spi == SPI1){
        GPIOA->BSRR = (1 << (4 + 16));   // PA4 -> selecciona esclavo
    }
    else if (spi == SPI2){
        GPIOB->BSRR = (1 << (12 + 16));  // PB12
    }
}
void SPI_DS(SPI_TypeDef *spi){
    if (spi == SPI1){
        GPIOA->BSRR = (1 << 4);   // PA4  -> desactiva esclavo
    }
    else if (spi == SPI2){
        GPIOB->BSRR = (1 << 12);  // PB12 
    }
}
char SPI_RW_Byte(SPI_TypeDef *spi, char data){
    while (!(spi->SR & SPI_SR_TXE));   // Espera que SE HALLA TERMINADO LA TRASMISION DE DATOS Y EL DR ESTE LIBRE 
    spi->DR = data;                    
    while (!(spi->SR & SPI_SR_RXNE));  // Espera A QUE  HALLA DATOS RECIBIDO 
    return spi->DR;                    // Devuelve LOS DATOS LEIDOS
}
