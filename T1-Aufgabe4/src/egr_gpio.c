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
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C);
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
