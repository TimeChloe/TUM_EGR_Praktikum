/*
 * egr_i2c.h
 *
 *  Created on: 27.05.2024
 *      Author: Yue Zhang
 * Last Change: 27.05.2024
 */

#ifndef EGR_I2C_H
#define EGR_I2C_H

#include <stdint.h>



// 定义I2C1时钟频率为10kHz
#define I2C1_BASE 0x40005400
#define RCC_APB1ENR1 0x58
#define I2C_TIMINGR 0x10
#define I2C_ISR 0x18
#define I2C_ICR 0x1C
#define I2C_TXDR 0x28
#define I2C_RXDR 0x24
#define I2C_CR2 0x04
#define I2C_CR1 0x00

#define SLA_ADDRESS 0x57



void activate_i2c1(void);
void deactivate_i2c1(void);
void I2C1_EV_IRQHandler(void);
void I2C1_SendStartCommand(uint32_t sla_address, uint8_t* data, uint8_t num_bytes);
void i2c_master_receive(uint32_t sla_address, uint8_t num_bytes);




#endif /* EGR_GPIO_H */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++