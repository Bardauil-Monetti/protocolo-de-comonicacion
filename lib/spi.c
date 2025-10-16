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
    SPIx->CR1=0; //limpia configuracion
    SPIx->CR1|=SPI_CR1_MSTR; //mdodo maestro
    SPIx->CR1|=SPI_CR1_BR_0; //velocidad del reloj
    SPIx->CR1|=SPI_CR1_SSI;
    SPIx->CR1|=SPI_CR1_SSM; //gestion de software
    SPIx->CR1|=SPI_CR1_SPE; //hablita SPI
}

void spi_ss(SPI_TypeDef* SPIx) {
    if(SPIx==SPI1){
        GPIOA->BSRR=(1<<(SS1+16));
    }
    else if(SPIx==SPI2) GPIOB->BSRR=(1<<(SS2+16));
}

void spi_ds(SPI_TypeDef* SPIx) {
    if(SPIx==SPI1){
        GPIOA->BSRR=(1<<SS1);
    } 
    else if(SPIx==SPI2){
        GPIOB->BSRR=(1<<SS2);
    }
}

char spi_rw_byte(SPI_TypeDef* SPIx, char data) {
    while(!(SPIx->SR&SPI_SR_TXE)); //Espera a que haya datos transmitidos y que DR este libre
    SPIx->DR=data;
    while(!(SPIx->SR&SPI_SR_RXNE)); //Espera a que haya datos recibidos
    return SPIx->DR;
}