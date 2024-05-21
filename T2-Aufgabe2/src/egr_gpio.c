/*
 * egr_gpio.c
 *
 *  Created on: 24.04.2024
 *      Author: Yue Zhang
 * Last Change: 24.04.2024
 */

#include "egr_gpio.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// FUNCTION DEFINITIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void activate_gpio_a(void)
{
    // activate GPIO A module
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C); //0x40021000 is the base address of the RCC module, 0x4C is the offset for the AHB2ENR register
    *adresse |= (1 << 0);
}

void activate_SYSCFG(void)
{
    // activate SYSCFG module
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x60);  //0x40021000 is the base address of the RCC module, 0x60 is the offset for the APB2ENR register
    *adresse |= (1 << 0);
}


void gpio_mode(uint32_t gpio_base, uint32_t pin, uint32_t mode)
{
    // set the mode of the pin(GPIO) as input or output
    if (mode == GPIO_INPUT)
    {
        // set the pin as input
        uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x00);
        *adresse &= ~(1 << (2*pin));
        *adresse &= ~(1 << (2*pin + 1));
    }
    else if (mode == GPIO_OUTPUT)
    {
        // set the pin as output
        uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x00);
        *adresse |= (1 << (2*pin));
        *adresse &= ~(1 << (2*pin + 1));
    }
}

uint32_t gpio_read(uint32_t gpio_base, uint32_t pin)
{
    // read the value of the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x10);
    return (*adresse & (1 << pin)) >> pin;
}

void gpio_write(uint32_t gpio_base, uint32_t pin, uint32_t value)
{
    // write the value to the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x14);
    if (value == PIN_HIGH)
    {
        *adresse |= (1 << pin);
    }
    else if (value == PIN_LOW)
    {
        *adresse &= ~(1 << pin);
    }
}

void gpio_pull(uint32_t gpio_base, uint32_t pin, uint32_t pull)
{
    // set the pull-up/pull-down of the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x0C);
    if (pull == GPIO_PULL_UP)
    {
        *adresse &= ~(1 << (2*pin + 1));
        *adresse |= (1 << (2*pin));
    }
    else if (pull == GPIO_PULL_DOWN)
    {
        *adresse |= (1 << (2*pin + 1));
        *adresse &= ~(1 << (2*pin));
    }
}

void gpio_output_type(uint32_t gpio_base, uint32_t pin, uint32_t type)
{
    // set the output type of the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x04);
    if (type == GPIO_PUSH_PULL)
    {
        *adresse &= ~(1 << (pin));
    }
    else if (type == GPIO_OPEN_DRAIN)
    {
        *adresse |= (1 << (pin));
    }
}


//still not sure about the following two functions
void gpio_output_speed(uint32_t gpio_base, uint32_t pin, uint32_t speed)
{
    // set the output speed of the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x08);
    *adresse &= ~(3 << (2*pin));
    *adresse |= (speed << (2*pin));
}

void gpio_alternate_function(uint32_t gpio_base, uint32_t pin, uint32_t af)
{
    // set the alternate function of the pin(GPIO)
    uint32_t volatile *adresse = (uint32_t *)(gpio_base + 0x20);
    *adresse &= ~(0xF << (4*pin));
    *adresse |= (af << (4*pin));
}

void configure_PA4_EXTI4(void)
{
    // connect EXTI4 line to the PA4 pin
    uint32_t volatile *address = (uint32_t *)(SYSCFG_BASE + 0x0C);  // 0x0C is the offset for the EXTICR2 register
    *address &= ~(0xF << (4 * (4 % 4)));  // clear the EXTI4 configuration
    *address |= (0 << (4 * (4 % 4)));  // set the EXTI4 configuration to PA4, 0000 means PA4 pin
}


void configure_EXTI4_line(void)
{
    // unmask the EXTI4 line
    uint32_t volatile *address = (uint32_t *)(EXTI_BASE + 0x00);  // 0x00 is the offset for the interrupt mask register
    *address |= (1 << 4);  // unmask the EXTI4 line  P145

    // rising edge will trigger the interrupt
    // set the EXTI4 line to trigger on the rising edge
    address = (uint32_t *)(EXTI_BASE + 0x08);  // 0x08 is the offset for the rising trigger selection register
    // *address |= (1 << 4);  
    *address &= ~(1 << 4);  // clear the EXTI4 line to trigger on the rising edge, button is pull-up, so it is falling edge

    // clear the EXTI4 line to trigger on the falling edge
    address = (uint32_t *)(EXTI_BASE + 0x0C);  // 0x0C is the offset for the falling trigger selection register
    // *address &= ~(1 << 4);  
    *address |= (1 << 4);   // set the EXTI4 line to trigger on the falling edge
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
