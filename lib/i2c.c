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
 I2C1->CR1 |= I2C_CR1_PE;                       // Habilita I2C
I2C1->CR2 = 36;                                // Frecuencia del reloj APB1 
I2C1->CCR = 180;                               // Velocidad estándar 100 kHz puede cer de 400k que es mas rapida
I2C1->TRISE = 37;                              // Tiempo de subida máximo

    }
void i2c_start(I2C_TypeDef *i2c) {
    i2c->CR1 |= I2C_CR1_START;                 // Genera señal START
    while (!(i2c->SR1 & I2C_SR1_SB));          // Espera a que se genere el  START
}

void i2c_stop(I2C_TypeDef *i2c) {
    i2c->CR1 |= I2C_CR1_STOP;                  // Genera el  stop
}
void i2c_send_direccion(I2C_TypeDef *i2c, char direccion) {
    i2c->DR = direccion;                       // Escribe la direccion del esclavo
    while (!(i2c->SR1 & I2C_SR1_ADDR));        // espera a que se ejecute la trasmicion de datos y el ack se ponga en uno
    (void)i2c->SR2;                            // lee los estados y despues borra las flags 
                                               // se usa sr2 por que se leeb los estados generales y despues se borran todos los estados 
}
void i2c_send_byte(I2C_TypeDef *i2c, char dato) {
               
    while (!(i2c->SR1 & I2C_SR1_TXE));         // Espera a que el registro este vacio 
       i2c->DR = dato;                            // envio los datos de registro
           while (!(i2c->SR1 & I2C_SR1_BTF));  
}
char i2c_recibir_byte_ack(I2C_TypeDef *i2c) {
    i2c->CR1 |= I2C_CR1_ACK;                   // Habilita ACK
    while (!(i2c->SR1 & I2C_SR1_RXNE));        // Espera a  QUE SE HAYAN RECIBIDO TODOS LOS DATOS Y ESPERA A QUE HAYA BITS DISPONIBLES
    return i2c->DR;                            // Devuelve el BIT recibido
}
char i2c_recibir_byte_nack(I2C_TypeDef *i2c) {
    i2c->CR1 &= ~I2C_CR1_ACK;                  // Desactiva ACK, enviará NACK
                                               //SI EL ACK ESTA EN UNO SE PUENDEN CEGUIR MANDANDO O RECIBIENDO DATOS SI NO ESTA HACTIBADO SE MANDA UN NACK UNA CONDICION QUE NO PERMITE MANDAR MAS DATOS NI TAMPOCO RECIBIR  
    while (!(i2c->SR1 & I2C_SR1_RXNE));        // Espera LA recepción 
    return i2c->DR;                            // Devuelve último dato recibido POR QYE DESPUES SE MUESTRA EL NACK 
}