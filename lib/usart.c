#include "usart.h"
#include "stm32f103xb.h"

//pines validos para TX
# define TX1 10
# define TX2 3
# define TX3 4
//pines validos para RX
# define RX1 5
# define RX2 8
# define RX3 10


void usart_init(USART_TypeDef *usart, int baudrate) {
    int pclk = 0;


    if (usart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;      // Habilita puerto A
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    // Habilita USART1

        // PA9 → TX (salida alternativa push-pull)
        GPIOA->CRH &= ~(0xF << (4 * 1));
        GPIOA->CRH |= (0xB << (4 * 1));

        // PA10 → RX (entrada flotante)
        GPIOA->CRH &= ~(0xF << (4 * 2));
        GPIOA->CRH |= (0x4 << (4 * 2));

        pclk = 72000000; // Frecuencia de APB2
        usart->BRR = (16 * baudrate) / frecuencia(pclk); // Tu fórmula original
    }


    if (usart == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    // Habilita USART2
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;      // Habilita puerto A

        // PA2 → TX
        GPIOA->CRL &= ~(0xF << (4 * 2));
        GPIOA->CRL |= (0xB << (4 * 2));

        // PA3 → RX
        GPIOA->CRL &= ~(0xF << (4 * 3));
        GPIOA->CRL |= (0x4 << (4 * 3));

        pclk = 36000000; // Frecuencia de APB1
        usart->BRR = (16 * baudrate) / frecuencia(pclk);
    }

   
    if (usart == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;    // Habilita USART3
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;      // Habilita puerto B

        // PB10 → TX
        GPIOB->CRH &= ~(0xF << (4 * 2));
        GPIOB->CRH |= (0xB << (4 * 2));

        // PB11 → RX
        GPIOB->CRH &= ~(0xF << (4 * 3));
        GPIOB->CRH |= (0x4 << (4 * 3));

        pclk = 36000000; // Frecuencia de APB1
        usart->BRR = (16 * baudrate) / frecuencia(pclk);
    }

    // Habilitar USART 
    usart->CR1 |= USART_CR1_TE; // Habilita transmisión
    usart->CR1 |= USART_CR1_RE; // Habilita recepción
    usart->CR1 |= USART_CR1_UE; // Enciende USART
}

void usart_send_char(USART_TypeDef *usart, char data) {
    while (!(usart->SR & USART_SR_TXE)); // Espera a que TXE esté libre
    usart->DR = data;                    // Envía el carácter
}



char usart_send_string(USART_TypeDef *usart, char *str) {
    while (*str) {
        while (!(usart->SR & USART_SR_TXE)); // Espera a que TXE sea 1
        usart->DR =*str++;//envia una cadena de caracteres 
}
}