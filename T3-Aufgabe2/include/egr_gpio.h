/*
 * egr_gpio.h
 *
 *  Created on: 24.04.2024
 *      Author: Yue Zhang
 * Last Change: 24.04.2024
 */

#ifndef EGR_GPIO_H
#define EGR_GPIO_H

#include <stdint.h>

//GPIOA Base Addresses
#define GPIOA_BASE 0x48000000
#define GPIOB_BASE 0x48000400

//SYSFG Base Addresses
#define SYSCFG_BASE 0x40010000

//define EXTI base address
#define EXTI_BASE 0x40010400

// Pin Values
#define 	PIN_HIGH 		1
#define 	PIN_LOW 		0

//GPIO Mode Input or Output
#define 	GPIO_OUTPUT		0
#define 	GPIO_INPUT		1
#define     GPIO_ALTERNATE	2
#define     GPIO_ANALOG     3

//GPIO Output type
#define 	GPIO_PUSH_PULL	0
#define 	GPIO_OPEN_DRAIN	1

//GPIO Output Speed
#define 	GPIO_LOW_SPEED	0
#define 	GPIO_MEDIUM_SPEED	1
#define 	GPIO_HIGH_SPEED	2
#define 	GPIO_VERY_HIGH_SPEED	3

//GPIO Pull Up/Pull Down
#define 	GPIO_NO_PULL	0
#define 	GPIO_PULL_UP	1
#define 	GPIO_PULL_DOWN	2
#define 	GPIO_RESERVED	3

//GPIO Alternate Function
#define 	GPIO_AF0	0
#define 	GPIO_AF1	1
#define 	GPIO_AF2	2
#define 	GPIO_AF3	3
#define 	GPIO_AF4	4
#define 	GPIO_AF5	5
#define 	GPIO_AF6	6
#define 	GPIO_AF7	7
#define 	GPIO_AF8	8
#define 	GPIO_AF9	9
#define 	GPIO_AF10	10
#define 	GPIO_AF11	11
#define 	GPIO_AF12	12
#define 	GPIO_AF13	13
#define 	GPIO_AF14	14
#define 	GPIO_AF15	15

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// GPIO FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void activate_gpio_a(void);

void activate_gpio_b(void);

void activate_SYSCFG(void);

void gpio_mode(uint32_t gpio_base, uint32_t pin, uint32_t mode);

uint32_t gpio_read(uint32_t gpio_base, uint32_t pin);

void gpio_write(uint32_t gpio_base, uint32_t pin, uint32_t value);

void gpio_output_type(uint32_t gpio_base, uint32_t pin, uint32_t type);

void gpio_output_speed(uint32_t gpio_base, uint32_t pin, uint32_t speed);

void gpio_pull(uint32_t gpio_base, uint32_t pin, uint32_t pull);

void gpio_set_pin(uint32_t gpio_base, uint32_t pin);

void gpio_clear_pin(uint32_t gpio_base, uint32_t pin);

void gpio_alternate_function(uint32_t gpio_base, uint32_t pin, uint32_t af);

void configure_PA4_EXTI4(void);

void configure_EXTI4_line(void);

#endif /* EGR_GPIO_H */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++