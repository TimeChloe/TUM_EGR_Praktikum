#include <stdint.h>
#include "egr_gpio.h"
#include "stm32g431xx.h"
#include "egr_i2c.h"


uint32_t received_data[3];
uint8_t data_index = 0;


void activate_i2c1(void)
{
    // activate I2C1 module    P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + RCC_APB1ENR1); //0x40021000 is the base address of the RCC module
    *adresse |= (1 << 21);
}


// I2C1 clock source selection     PCLK selected as I2C1 clock source  P327 为什么选PCLK呢 在script的框图里面有解释
void clock_select_i2c1(void)
{

    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + RCC_CCIPR); //0x40021000 is the base address of the RCC module
    *adresse &= ~(1 << 13);
    *adresse &= ~ (1 << 12);
}


void I2C_Init(void) {
    // RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;
    activate_gpio_a();
    activate_gpio_b();
    activate_i2c1();

    clock_select_i2c1();

    // reset_i2c1();



    //configure GPIOA pin 15 as I2C1_SCL
    // PA15 -> I2C1_SCL  //I2C needs to be configured as open-drain
    gpio_mode(GPIOA_BASE, 15, GPIO_ALTERNATE);
    gpio_alternate_function(GPIOA_BASE, 15, GPIO_AF4);
    gpio_output_type(GPIOA_BASE, 15, GPIO_OPEN_DRAIN);
    gpio_output_speed(GPIOA_BASE, 15, GPIO_HIGH_SPEED);
    gpio_pull(GPIOA_BASE, 15, GPIO_PULL_UP);


    //configure GPIOB pin 7 as I2C1_SDA
    // PB7 -> I2C1_SDA
    gpio_mode(GPIOB_BASE, 7, GPIO_ALTERNATE);
    gpio_alternate_function(GPIOB_BASE, 7, GPIO_AF4);
    gpio_output_type(GPIOB_BASE, 7, GPIO_OPEN_DRAIN);
    gpio_output_speed(GPIOB_BASE, 7, GPIO_HIGH_SPEED);
    gpio_pull(GPIOB_BASE, 7, GPIO_PULL_UP);


    // configure I2C1
    // I2C1->CR1 &= ~I2C_CR1_PE; // disable I2C1
    uint32_t volatile *cr1_address = (uint32_t *)(I2C1_BASE + I2C_CR1);
    *cr1_address &= ~(1 << 0);

    delay_ms (1);

    // I2C1->CR1 = I2C_CR1_ANFOFF; // activate analog noise filter
    *cr1_address &= ~(1 << 12);

    // I2C1->CR1 &= ~I2C_CR1_DNF; // disable digital noise filter
    // 清除I2C_CR1寄存器的DNF位（8到11位）
    *cr1_address &= ~(0xF << 8);

    // PRESC = 3
    uint32_t volatile *timingr_address = (uint32_t *)(I2C1_BASE + I2C_TIMINGR);
    *timingr_address |= (3 << 28);
    // SCLDEL = 0x4
    *timingr_address |= (4 << 20);
    // SDADEL = 0x2
    *timingr_address |= (2 << 16);
    // SCLH = 0xC3
    *timingr_address |= (0xC3 << 8);
    // SCLL = 0xC7
    *timingr_address |= (0xC7 << 0);


    // I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; // disable clock stretching
    *cr1_address |=  (1 << 17);

    // I2C1->CR1 |= I2C_CR1_PE; // enable I2C1
    *cr1_address |= (1 << 0);

}

void I2C1_EV_IRQHandler(void)
{
    uint32_t volatile *isr_address = (uint32_t *)(I2C1_BASE + I2C_ISR);
    uint32_t volatile *icr_address = (uint32_t *)(I2C1_BASE + I2C_ICR);
    uint32_t volatile *cr2_address = (uint32_t *)(I2C1_BASE + I2C_CR2);
    uint32_t volatile *cr1_address = (uint32_t *)(I2C1_BASE + I2C_CR1);
    uint32_t volatile *rxdr_address = (uint32_t *)(I2C1_BASE + I2C_RXDR);


    // if (I2C_ISR & I2C_ISR_RXNE)
    if (*isr_address & (1 << 2))
    {
        // received_data[data_index++] = I2C_RXDR;
        received_data[data_index++] = *rxdr_address;

        if (data_index >= 3)
        {
            // finished receiving data
            // disable RXNE interrupt
            // I2C_CR1 &= ~I2C_CR1_RXIE;
            *cr1_address &= ~(1 << 2);

            // wait 5ms to ensure data transmission is complete
            delay_ms(5);

            // clear stop condition flag
            // I2C_ICR |= I2C_ICR_STOPCF;
            *icr_address |= (1 << 5);

            // reset CR2 register
            // I2C_CR2 = 0;
            *cr2_address = 0;

            // reset data index
            data_index = 0;
        }
    }
}


void I2C1_SendStartCommand(uint32_t sla_address, int data, uint8_t num_bytes) {
    // 配置传输参数
    uint32_t volatile *isr_address = (uint32_t *)(I2C1_BASE + I2C_ISR);
    uint32_t volatile *txdr_address = (uint32_t *)(I2C1_BASE + I2C_TXDR);

    // I2C1->CR2 = (I2C_ADDRESS << 1) & I2C_CR2_SADD; // set slave address
    uint32_t volatile *cr2_address = (uint32_t *)(I2C1_BASE + I2C_CR2);
    // 将从设备地址写入 CR2 寄存器的 SADD[7:1]
    // write slave address to CR2 register SADD[7:1]
    *cr2_address |= (sla_address << 1) & 0xFF;  
    // In 7-bit addressing mode (ADD10 = 0)
    *cr2_address &= ~(1 << 11);


    // I2C1->CR2 |= (num_bytes << I2C_CR2_NBYTES_Pos); // 设置传输字节数 // set number of bytes to be transmitted
    // 将传输字节数写入 CR2 寄存器的 NBYTES[7:0]  Bits 23:16 //write number of bytes to be transmitted to CR2 register NBYTES[7:0]
    *cr2_address |= (num_bytes << 16);

    // I2C1->CR2 |= autoend ? I2C_CR2_AUTOEND : 1; // 设置AUTOEND
    // set AUTOEND as 1
    *cr2_address |= (1 << 25);

    // I2C1->CR2 &= ~I2C_CR2_RD_WRN; // 设置为写模式 // set write mode
    *cr2_address &= ~(1 << 10);

    // I2C1->CR2 |= I2C_CR2_START; // 产生开始条件   // generate start condition
    *cr2_address |= (1 << 13);


    for (int i = 0; i < num_bytes; i++) {
        // 检查是否接收到NACK信号，如果接收到NACK信号，终止传输 // check if NACK is received, if NACK is received, terminate transmission
        if (*isr_address & (1 << 4)) {
            return;
        }

        // 检查传输缓冲区是否为空，如果传输缓冲区不为空，等待   // check if transmit buffer is empty, if transmit buffer is not empty, wait 
        while (!(*isr_address & (1 << 1))) {
            delay_ms(1);
            // Array für Zeichenkette
            char testString[14] = "Hello World!\n";
            // Zeichenkette an PC senden:
            UARTsend((uint8_t*) testString, strlen(testString));
            delay_ms(20);
        }

        // send data
        *txdr_address = data;

        // wait 5ms to ensure data transmission is complete
        delay_ms(5);
    }

    // // clear stop condition flag
    // I2C1->ICR |= I2C_ICR_STOPCF;
    uint32_t volatile *icr_address = (uint32_t *)(I2C1_BASE + I2C_ICR);
    *icr_address |= (1 << 5);

    // reset CR2 register
    // I2C1->CR2 = 0;
    *cr2_address &= ~0xFFFFFFFF;  // 将 CR2 寄存器的所有位都清零

}

void i2c_master_receive(uint32_t sla_address, uint8_t num_bytes)
{
    // Set NBYTES to 3, AUTOEND=1
    uint32_t volatile *cr2_address = (uint32_t *)(I2C1_BASE + I2C_CR2);
    *cr2_address |= (num_bytes << 16);
    *cr2_address |= (1 << 25);

    // configure slave address and read direction
    *cr2_address |= (1 << 10);
    // write slave address to CR2 register SADD[7:1]
    *cr2_address |= (sla_address << 1) & 0xFF;  

    // activate i2c transmission
    // I2C_CR2 |= I2C_CR2_START;
    *cr2_address |= (1 << 13);

    // activate RXNE interrupt
    // I2C_CR1 |= I2C_CR1_RXIE;
    uint32_t volatile *cr1_address = (uint32_t *)(I2C1_BASE + I2C_CR1);
    *cr1_address |= (1 << 2);

    // configure I2C1_EV_IRQn
    NVIC_SetPriority(I2C1_EV_IRQn, 1);
    NVIC_EnableIRQ(I2C1_EV_IRQn);
}