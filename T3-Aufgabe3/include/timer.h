/*
 * timer.h
 *
 *  Created on: 08.05.2024
 *      Author: Yue Zhang
 * Last Change: 08.05.2024
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

#define TIM2_base 0x40000000
#define TIM3_base 0x40000400
#define TIM4_base 0x40000800

#define APB1ENR1_OFFSET 0x58
#define PDCR_OFFSET 0x28
#define ARR_OFFSET 0x2C
#define CCR1_OFFSET 0x34
#define TIM2_DIER_OFFSET 0x0C
#define EGR_OFFSET 0x14
#define SR_OFFSET 0x10
#define CR1_OFFSET 0x00
#define CCMR1_OFFSET 0x18
#define CCER_OFFSET 0x20



int TIMER_MAX_COMPARE;
int TIMER_MIN_COMPARE;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TIMER FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void activate_timer2(void);
void activate_timer3(void);
void activate_timer4(void);

void deactivate_timer2(void);
void deactivate_timer3(void);
void deactivate_timer4(void);

void set_prescaler_timer2(uint32_t prescaler);
void set_prescaler_timer3(uint32_t prescaler);
void set_prescaler_timer4(uint32_t prescaler);

void set_auto_reload_timer2(uint32_t auto_reload);
void set_auto_reload_timer3(uint32_t auto_reload);
void set_auto_reload_timer4(uint32_t auto_reload);


void set_compare_timer2(uint32_t compare);
void set_compare_timer3(uint32_t compare);
void set_compare_timer4(uint32_t compare);

void enable_timer2_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt);
void enable_timer3_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt);
void enable_timer4_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt);

void generate_update_event_timer2(void);
void generate_update_event_timer3(void);
void generate_update_event_timer4(void);

void clear_update_event_timer2(void);
void clear_update_event_timer3(void);
void clear_update_event_timer4(void);

void set_pwm_mode_timer3(void);
void set_pwm_mode_timer4(void);


void start_timer2(void);
void start_timer3(void);
void start_timer4(void);



#endif /* TIMER_H */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

