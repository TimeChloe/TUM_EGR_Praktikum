/**
 * EGR Praktikum
 * Termin 4 - Aufgabe 1
 */

#include "egr_gpio.h"

// 定义I2C1时钟频率为10kHz
#define I2C_TIMING 0xB0420F13
#define I2C1_BASE 0x40005400
#define RCC_APB1ENR1 0x58
#define SLA_ADDRESS 0x57

// void I2C1_Init(void) {
//     // 使能I2C1和GPIOB、GPIOA的时钟
//     RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;

//     // 配置GPIOA的I2C1_SCL引脚
//     // PA15 -> I2C1_SCL
//     GPIOA->MODER &= ~GPIO_MODER_MODER15;
//     GPIOA->MODER |= GPIO_MODER_MODER15_1; // 选择AF模式
//     GPIOA->OTYPER |= GPIO_OTYPER_OT_15; // 选择开漏模式
//     GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15; // 高速
//     GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15;
//     GPIOA->AFR[1] |= (4 << (15-8)*4); // 选择AF4 (I2C1)

//     // 配置GPIOB的I2C1_SDA引脚
//     // PB7 -> I2C1_SDA
//     GPIOB->MODER &= ~GPIO_MODER_MODER7;
//     GPIOB->MODER |= GPIO_MODER_MODER7_1; // 选择AF模式
//     GPIOB->OTYPER |= GPIO_OTYPER_OT_7; // 选择开漏模式
//     GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7; // 高速
//     GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7;
//     GPIOB->AFR[0] |= (4 << (7)*4); // 选择AF4 (I2C1)

//     // 复位I2C1
//     RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
//     RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

//     // 配置I2C1
//     I2C1->CR1 &= ~I2C_CR1_PE; // 禁用I2C1
//     I2C1->TIMINGR = I2C_TIMING; // 设置时序寄存器
//     I2C1->CR1 = I2C_CR1_ANFOFF; // 启用模拟滤波器
//     I2C1->CR1 &= ~I2C_CR1_DNF; // 禁用数字滤波器
//     I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; // 禁用时钟拉伸
//     I2C1->CR1 |= I2C_CR1_PE; // 启用I2C1
// }

void I2C_Init(void) {
    // 使能I2C1和GPIOB、GPIOA的时钟
    // RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;
    activate_i2c1();
    activate_gpio_a();
    activate_gpio_b();

    // 配置GPIOA的I2C1_SCL引脚
    // PA15 -> I2C1_SCL
    gpio_mode(GPIOA_BASE, 15, GPIO_ALTERNATE);
    gpio_output_type(GPIOA_BASE, 15, GPIO_OPEN_DRAIN);
    gpio_output_speed(GPIOA_BASE, 15, GPIO_VERY_HIGH_SPEED);
    gpio_pull(GPIOA_BASE, 15, GPIO_NO_PULL);
    gpio_alternate_function(GPIOA_BASE, 15, GPIO_AF4);

    // 配置GPIOB的I2C1_SDA引脚
    // PB7 -> I2C1_SDA
    gpio_mode(GPIOB_BASE, 7, GPIO_ALTERNATE);
    gpio_output_type(GPIOB_BASE, 7, GPIO_OPEN_DRAIN);
    gpio_output_speed(GPIOB_BASE, 7, GPIO_VERY_HIGH_SPEED);
    gpio_pull(GPIOB_BASE, 7, GPIO_NO_PULL);
    gpio_alternate_function(GPIOB_BASE, 7, GPIO_AF4);


    // 配置I2C1
    // I2C1->CR1 &= ~I2C_CR1_PE; // 禁用I2C1
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x00);
    *adresse &= ~(1 << 0);

    // I2C1->CR1 = I2C_CR1_ANFOFF; // 启用模拟滤波器
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x00);
    *adresse |= (1 << 12);

    // I2C1->CR1 &= ~I2C_CR1_DNF; // 禁用数字滤波器
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x00);
    *adresse &= ~(1 << 8);
    *adresse &= ~(1 << 9);
    *adresse &= ~(1 << 10);
    *adresse &= ~(1 << 11);

    // PRESC = 3
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x10);
    *adresse |= (3 << 28);

    // SCLDEL = 0x4
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x10);
    *adresse |= (4 << 20);

    // SDADEL = 0x2
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x10);
    *adresse |= (2 << 16);

    // SCLH = 0xC3
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x10);
    *adresse |= (0xC3 << 8);

    // SCLL = 0xC7
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x10);
    *adresse |= (0xC7 << 0);

    // I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; // 禁用时钟拉伸
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x00);
    *adresse &= ~(1 << 17);

    // I2C1->CR1 |= I2C_CR1_PE; // 启用I2C1
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x00);
    *adresse |= (1 << 0);

}

void activate_i2c1(void)
{
    // activate I2C1 module
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + RCC_APB1ENR1); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR register
    *adresse |= (1 << 21);
}

void I2C1_SendStartCommand(uint32_t sla_address, uint8_t* data, uint8_t num_bytes) {
    // 配置传输参数

    // I2C1->CR2 = (I2C_ADDRESS << 1) & I2C_CR2_SADD; // 设置从设备地址
    uint32_t volatile *adresse = (uint32_t *)(I2C1_BASE + 0x04);
    // 将从设备地址写入 CR2 寄存器的 SADD[7:1]
    *adresse = (SLA_ADDRESS << 1) & 0xFF;  
    // In 7-bit addressing mode (ADD10 = 0)
    *adresse &= ~(1 << 10);

    // I2C1->CR2 |= (num_bytes << I2C_CR2_NBYTES_Pos); // 设置传输字节数
    // 将传输字节数写入 CR2 寄存器的 NBYTES[7:0]
    *adresse |= (num_bytes << 16);


    // I2C1->CR2 |= autoend ? I2C_CR2_AUTOEND : 0; // 设置AUTOEND
    *adresse |= (1 << 25);


    // I2C1->CR2 &= ~I2C_CR2_RD_WRN; // 设置为写模式
    *adresse &= ~(1 << 10);

    // I2C1->CR2 |= I2C_CR2_START; // 产生开始条件
    *adresse |= (1 << 13);
    

    for (int i = 0; i < num_bytes; i++) {
        // 等待传输缓冲区为空
        while (!(I2C1->ISR & I2C_ISR_TXIS)) {
            if (I2C1->ISR & I2C_ISR_NACKF) {
                // 检测到NACK信号，终止传输
                return;
            }
        }

        // 发送数据
        I2C1->TXDR = data[i];
    }

    // 等待传输完成
    while (!(I2C1->ISR & I2C_ISR_TC)) {
        if (I2C1->ISR & I2C_ISR_NACKF) {
            // 检测到NACK信号，终止传输
            return;
        }
    }

    // 如果设置了AUTOEND，则不需要手动发送停止条件
    if (!autoend) {
        I2C1->CR2 |= I2C_CR2_STOP; // 发送停止条件
    }

    // 清除停止条件标志
    while (I2C1->ISR & I2C_ISR_STOPF) {}
    I2C1->ICR |= I2C_ICR_STOPCF;

    // 重置I2C_CR2寄存器
    I2C1->CR2 = 0;
}

#include <stdint.h>

int main(void)
{
    // place code to run once here

    while (1)
    {
        // place your loop code here
    }

    return 0;
}