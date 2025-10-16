#include "i2c.h"
#include "stm32f103xb.h"

//pines validos para I2C1
#define SCL1 6
#define SDA1 7
//pines validos para I2C2
#define SCL2 10
#define SDA2 11

void i2c_init(I2C_TypeDef* I2Cx){
    if(I2Cx==I2C1){
        RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
        RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;
        GPIOB->CRL&=~(0xF<<((SCL1)*4)); //limpio
        GPIOB->CRL|=(0xF<<((SCL1)*4)); //salida alternativa Open-Drain
        GPIOB->CRL&=~(0xF<<((SDA1)*4)); //limpio
        GPIOB->CRL|=(0xF<<((SDA1)*4)); //salida alternativa 0pen-Drain
    }
    else if(I2Cx==I2C2){
        RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
        RCC->APB1ENR|=RCC_APB1ENR_I2C2EN;
        GPIOB->CRH&=~(0xF<<((SCL2%8)*4)); //limpio
        GPIOB->CRH|=(0xF<<((SCL2%8)*4)); //salida alternativa Open-Drain
        GPIOB->CRH&=~(0xF<<((SDA2%8)*4)); //limpio
        GPIOB->CRH|=(0xF<<((SDA2%8)*4)); //salida alternativa Open-Drain
    }
    I2Cx->CR2=36; //Frecuencia del periférico en MHz
    I2Cx->CCR=180; //Controla velocidad I2C
    I2Cx->TRISE=37; //Tiempo de subida máximo permitido
    I2Cx->CR1|=I2C_CR1_PE; //Habilita I2C
}

void i2c_start(I2C_TypeDef* I2Cx) {
    I2Cx->CR1 |= I2C_CR1_START; //Genera el start
    while (!(I2Cx->SR1 & I2C_SR1_SB)); //Espera a que se genere el start
}

void i2c_stop(I2C_TypeDef* I2Cx) {
    I2Cx->CR1 |= I2C_CR1_STOP; //Genera el stop
}

void i2c_send_direccion(I2C_TypeDef* I2Cx, char direccion) {
    I2Cx->DR = direccion; //Envia dirección del esclavo
    while (!(I2Cx->SR1 & I2C_SR1_ADDR)); //Espera a que se haya transmitido la dirección
}

void i2c_send_byte(I2C_TypeDef* I2Cx, char dato) {
    while (!(I2Cx->SR1 & I2C_SR1_TXE)); //Espera a que el registro este vacio
    I2Cx->DR = dato; //Envía el dato
    while(!(I2Cx->SR1 & I2C_SR1_BTF));
}

char i2c_recibir_byte_ack(I2C_TypeDef* I2Cx) {
    I2Cx->CR1 |= I2C_CR1_ACK; //Habilita ACK (confirmación de recepción)
    while (!(I2Cx->SR1 & I2C_SR1_RXNE)); //Espera a que se reciba el dato
    return I2Cx->DR; //Devuelve el dato recibido
}

char i2c_recibir_byte_nack(I2C_TypeDef* I2Cx) {
    I2Cx->CR1 &= ~I2C_CR1_ACK; //Deshabilita ACK
    while (!(I2Cx->SR1 & I2C_SR1_RXNE)); //Espera a que llegue el dato
    return I2Cx->DR; //Devuelve el dato recibido sin ACK
}