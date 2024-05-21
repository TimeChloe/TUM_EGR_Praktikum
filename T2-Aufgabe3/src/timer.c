#include "timer.h"
#include "stm32g431xx.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                         FUNCTION DEFINITIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Enable the timer 2
void activate_timer2(void)
{
    // activate Timer 2 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse |= (1 << 0);
}


// Set the prescaler value of the timer 2
void set_prescaler_timer2(uint32_t prescaler) {
    // Set the prescaler value of the timer 2
    uint32_t volatile *prescaler_address = (uint32_t *)(TIM2_BASE + PDCR_OFFSET); // 0x28 is the offset for the PSC register
    *prescaler_address = prescaler;
}

// Set the auto-reload value of the timer 2
void set_auto_reload_timer2(uint32_t auto_reload) {
    uint32_t volatile *auto_reload_address = (uint32_t *)(TIM2_BASE + ARR_OFFSET); // 0x2C is the offset for the ARR register
    *auto_reload_address = auto_reload;  // Set auto-reload at calculated counts
}


// Set the compare value of the timer 2
void set_compare_timer2(uint32_t compare) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM2_BASE + CCR1_OFFSET); // 0x34 is the offset for the CCR1 register
    *compare_address = compare;
}

// Enable interrupts for Timer 2
void enable_timer2_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt) {
    uint32_t volatile *interrupt_enable_address = (uint32_t *)(TIM2_BASE + TIM2_DIER_OFFSET);
    
    if (enable_update_interrupt) {
        *interrupt_enable_address |= (1 << 0);  // Enable update interrupt (UIE)
    }
    
    if (enable_compare_interrupt) {
        *interrupt_enable_address |= (1 << 1);  // Enable compare interrupt (CC1IE)
    }
}


// Generate an update event to update the timer's registers
void generate_update_event_timer2(void) {
    uint32_t volatile *event_address = (uint32_t *)(TIM2_BASE + EGR_OFFSET);  // 0x14 is the offset for the EGR register
    *event_address |= (1 << 0);  // Generate an update event
}
// Clear update event interrupt flag
void clear_update_event_timer2(void) {
    uint32_t volatile *status_address = (uint32_t *)(TIM2_BASE + SR_OFFSET);  // 0x10 is the offset for the SR register
    *status_address &= ~(1 << 0);  // Clear the update event interrupt flag
}

// start the timer 2
void start_timer2(void) {
    uint32_t volatile *control_address = (uint32_t *)(TIM2_BASE + CR1_OFFSET);  // 0x00 is the offset for the CR1 register
    *control_address |= (1 << 0);  // Enable the timer
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




