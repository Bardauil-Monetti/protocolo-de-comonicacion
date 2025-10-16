#ifndef USART_H
#define USART_H
#include "stm32f103xb.h"

void usart_init(USART_TypeDef* USARTx, int baudrate);
void usart_send_char(USART_TypeDef* USARTx, char c);
void usart_send_string(USART_TypeDef* USARTx, char* str);
char usart_recibir_char(USART_TypeDef* USARTx);

#endif
