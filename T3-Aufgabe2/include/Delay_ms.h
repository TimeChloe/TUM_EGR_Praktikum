#ifndef DELAY_MS_H
#define DELAY_MS_H

#include "stdint.h"

// using TIM2

void delay_ms(uint32_t milliseconds)
{

    // enable TIM2 in RCC (RCC_APB1ENR1)
    uint32_t volatile *adr = (uint32_t *)(0x40021000 + 0x58);
    *adr |= (1 << 0);

    // set prescaler in TIM2_PSC
    adr = (uint32_t *)(0x40000000 + 0x028);
    *adr = 15999;

    // set auto-reload
    adr = (uint32_t *)(0x40000000 + 0x02C);
    *adr = milliseconds;

    // clear CNT
    adr = (uint32_t *)(0x40000000 + 0x024);
    *adr = 0;

    // generate update event
    adr = (uint32_t *)(0x40000000 + 0x014);
    *adr |= (1 << 0);

    // clear update event
    adr = (uint32_t *)(0x40000000 + 0x010);
    *adr &= ~(1 << 0);

    // enable timer
    adr = (uint32_t *)(0x40000000 + 0x000);
    *adr |= (1 << 0);

    // wait for timer to finish
    adr = (uint32_t *)(0x40000000 + 0x010);
    while (((*adr >> 0) & 1) != 1)
        ;
    *adr &= ~(1 << 0);

    // reset prescaler
    adr = (uint32_t *)(0x40000000 + 0x028);
    *adr = 0;

    // reset auto-reload
    adr = (uint32_t *)(0x40000000 + 0x02C);
    *adr = 0;

    // disable timer
    adr = (uint32_t *)(0x40000000 + 0x000);
    *adr &= ~(1 << 0);
}

#endif