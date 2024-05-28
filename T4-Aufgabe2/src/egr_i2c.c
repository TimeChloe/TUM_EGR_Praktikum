#include <stdint.h>
#include "egr_gpio.h"
#include "stm32g431xx.h"
#include "egr_i2c.h"


uint32_t received_data[3];
uint8_t data_index = 0;


void activate_i2c1(void)
{
    // activate I2C1 module
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + RCC_APB1ENR1); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR register
    *adresse |= (1 << 21);
}

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
    // gpio_output_speed(GPIOA_BASE, 15, GPIO_VERY_HIGH_SPEED);
    // gpio_pull(GPIOA_BASE, 15, GPIO_NO_PULL);
    gpio_alternate_function(GPIOA_BASE, 15, GPIO_AF4);

    // 配置GPIOB的I2C1_SDA引脚
    // PB7 -> I2C1_SDA
    gpio_mode(GPIOB_BASE, 7, GPIO_ALTERNATE);
    gpio_output_type(GPIOB_BASE, 7, GPIO_OPEN_DRAIN);
    // gpio_output_speed(GPIOB_BASE, 7, GPIO_VERY_HIGH_SPEED);
    // gpio_pull(GPIOB_BASE, 7, GPIO_NO_PULL);
    gpio_alternate_function(GPIOB_BASE, 7, GPIO_AF4);


    // 配置I2C1
    // I2C1->CR1 &= ~I2C_CR1_PE; // 禁用I2C1
    uint32_t volatile *cr1_address = (uint32_t *)(I2C1_BASE + I2C_CR1);
    *cr1_address &= ~(1 << 0);

    // I2C1->CR1 = I2C_CR1_ANFOFF; // 启用模拟滤波器
    *cr1_address |= (1 << 12);

    // I2C1->CR1 &= ~I2C_CR1_DNF; // 禁用数字滤波器
    *cr1_address &= ~(1 << 8);
    *cr1_address &= ~(1 << 9);
    *cr1_address &= ~(1 << 10);
    *cr1_address &= ~(1 << 11);

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


    // I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; // 禁用时钟拉伸
    *cr1_address &= ~(1 << 17);

    // I2C1->CR1 |= I2C_CR1_PE; // 启用I2C1
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
        // 读取数据
        received_data[data_index++] = *rxdr_address;

        if (data_index >= 3)
        {
            // 接收完成
            // 禁用RXNE中断
            // I2C_CR1 &= ~I2C_CR1_RXIE;
            *cr1_address &= ~(1 << 2);

            // 等待5毫秒
            delay_ms(5);

            // 清除停止标志
            // I2C_ICR |= I2C_ICR_STOPCF;
            *icr_address |= (1 << 5);

            // 重置I2C_CR2寄存器
            // I2C_CR2 = 0;
            *cr2_address = 0;

            // 重置数据索引
            data_index = 0;
        }
    }
}


void I2C1_SendStartCommand(uint32_t sla_address, uint8_t* data, uint8_t num_bytes) {
    // 配置传输参数
    uint32_t volatile *isr_address = (uint32_t *)(I2C1_BASE + I2C_ISR);
    uint32_t volatile *txdr_address = (uint32_t *)(I2C1_BASE + I2C_TXDR);


    // I2C1->CR2 = (I2C_ADDRESS << 1) & I2C_CR2_SADD; // 设置从设备地址
    uint32_t volatile *cr2_address = (uint32_t *)(I2C1_BASE + I2C_CR2);
    // 将从设备地址写入 CR2 寄存器的 SADD[7:1]
    *cr2_address = (sla_address << 1) & 0xFF;  
    // In 7-bit addressing mode (ADD10 = 0)
    *cr2_address &= ~(1 << 11);


    // I2C1->CR2 |= (num_bytes << I2C_CR2_NBYTES_Pos); // 设置传输字节数
    // 将传输字节数写入 CR2 寄存器的 NBYTES[7:0]
    *cr2_address |= (num_bytes << 16);

    // I2C1->CR2 |= autoend ? I2C_CR2_AUTOEND : 0; // 设置AUTOEND
    *cr2_address |= (1 << 25);

    // I2C1->CR2 &= ~I2C_CR2_RD_WRN; // 设置为写模式
    *cr2_address &= ~(1 << 10);

    // I2C1->CR2 |= I2C_CR2_START; // 产生开始条件
    *cr2_address |= (1 << 13);

    //I2C_ISR_TXIS

    for (int i = 0; i < num_bytes; i++) {
        // 检查是否接收到NACK信号，如果接收到NACK信号，终止传输
        if (*isr_address & (1 << 4)) {
            return;
        }

        // 检查传输缓冲区是否为空，如果传输缓冲区不为空，等待
        while (!(*isr_address & (1 << 1))) {
            delay_ms(1);
        }

        // 发送数据
        *txdr_address = data[i];

        // 等待5ms确保数据传输完成
        delay_ms(5);
    }

    // // 清除停止条件标志
    // I2C1->ICR |= I2C_ICR_STOPCF;
    uint32_t volatile *icr_address = (uint32_t *)(I2C1_BASE + I2C_ICR);
    *icr_address |= (1 << 5);

    // 重置I2C_CR2寄存器
    // I2C1->CR2 = 0;
    *cr2_address = 0;
}

void i2c_master_receive(uint32_t sla_address, uint8_t num_bytes)
{
    // 设置NBYTES为3，AUTOEND=1
    uint32_t volatile *cr2_address = (uint32_t *)(I2C1_BASE + I2C_CR2);
    *cr2_address = (num_bytes << 16);
    *cr2_address |= (1 << 25);


    // 配置从地址和传输方向（读）
    *cr2_address |= (1 << 10);
    // 将从设备地址写入 CR2 寄存器的 SADD[7:1]
    *cr2_address = (sla_address << 1) & 0xFF;  

    // 启动I2C传输
    // I2C_CR2 |= I2C_CR2_START;
    *cr2_address |= (1 << 13);

    // 启用RXNE中断
    // I2C_CR1 |= I2C_CR1_RXIE;
    uint32_t volatile *cr1_address = (uint32_t *)(I2C1_BASE + I2C_CR1);
    *cr1_address |= (1 << 2);

    // 配置中断优先级和启用中断
    NVIC_SetPriority(I2C1_EV_IRQn, 1);
    NVIC_EnableIRQ(I2C1_EV_IRQn);
}