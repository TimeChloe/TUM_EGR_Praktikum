#include "timer.h"

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

void activate_timer3(void)
{
    // activate Timer 3 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse |= (1 << 1);
}

void activate_timer4(void)
{
    // activate Timer 4 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse |= (1 << 2);
}

void deactivate_timer2(void)
{
    // deactivate Timer 2 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse &= ~(1 << 0);
}

void deactivate_timer3(void)
{
    // deactivate Timer 3 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse &= ~(1 << 1);
}

void deactivate_timer4(void)
{
    // deactivate Timer 4 module   P124
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + APB1ENR1_OFFSET); //0x40021000 is the base address of the RCC module, 0x58 is the offset for the APB1ENR1 register
    *adresse &= ~(1 << 2);
}



// Set the prescaler value of the timer 2
void set_prescaler_timer2(uint32_t prescaler) {
    // Set the prescaler value of the timer 2
    uint32_t volatile *prescaler_address = (uint32_t *)(TIM2_BASE + PDCR_OFFSET); // 0x28 is the offset for the PSC register
    *prescaler_address = prescaler;
}

void set_prescaler_timer3(uint32_t prescaler) {
    // Set the prescaler value of the timer 3
    uint32_t volatile *prescaler_address = (uint32_t *)(TIM3_BASE + PDCR_OFFSET); // 0x28 is the offset for the PSC register
    *prescaler_address = prescaler;
}

void set_prescaler_timer4(uint32_t prescaler) {
    // Set the prescaler value of the timer 4
    uint32_t volatile *prescaler_address = (uint32_t *)(TIM4_BASE + PDCR_OFFSET); // 0x28 is the offset for the PSC register
    *prescaler_address = prescaler;
}


// Set the auto-reload value of the timer 2
void set_auto_reload_timer2(uint32_t auto_reload) {
    uint32_t volatile *auto_reload_address = (uint32_t *)(TIM2_BASE + ARR_OFFSET); // 0x2C is the offset for the ARR register
    *auto_reload_address = auto_reload;  // Set auto-reload at calculated counts
}

// Set the auto-reload value of the timer 3
void set_auto_reload_timer3(uint32_t auto_reload) {
    uint32_t volatile *auto_reload_address = (uint32_t *)(TIM3_BASE + ARR_OFFSET); // 0x2C is the offset for the ARR register
    *auto_reload_address = auto_reload;  // Set auto-reload at calculated counts
}

// Set the auto-reload value of the timer 4
void set_auto_reload_timer4(uint32_t auto_reload) {
    uint32_t volatile *auto_reload_address = (uint32_t *)(TIM4_BASE + ARR_OFFSET); // 0x2C is the offset for the ARR register
    *auto_reload_address = auto_reload;  // Set auto-reload at calculated counts
}


// Set the compare value of the timer 2
void set_compare_timer2(uint32_t compare) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM2_BASE + CCR1_OFFSET); // 0x34 is the offset for the CCR1 register
    *compare_address = compare;
}

// Set the compare value of the timer 3
void set_compare_timer3(uint32_t compare) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM3_BASE + CCR1_OFFSET); // 0x34 is the offset for the CCR1 register
    *compare_address = compare;
}

// Set the compare value of the timer 4
void set_compare_timer4(uint32_t compare) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM4_BASE + CCR1_OFFSET); // 0x34 is the offset for the CCR1 register
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

// Enable interrupts for Timer 3
void enable_timer3_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt) {
    uint32_t volatile *interrupt_enable_address = (uint32_t *)(TIM3_BASE + TIM2_DIER_OFFSET);
    
    if (enable_update_interrupt) {
        *interrupt_enable_address |= (1 << 0);  // Enable update interrupt (UIE)
    }
    
    if (enable_compare_interrupt) {
        *interrupt_enable_address |= (1 << 1);  // Enable compare interrupt (CC1IE)
    }
}

// Enable interrupts for Timer 4
void enable_timer4_interrupts(bool enable_update_interrupt, bool enable_compare_interrupt) {
    uint32_t volatile *interrupt_enable_address = (uint32_t *)(TIM4_BASE + TIM2_DIER_OFFSET);
    
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

// Generate an update event to update the timer's registers
void generate_update_event_timer3(void) {
    uint32_t volatile *event_address = (uint32_t *)(TIM3_BASE + EGR_OFFSET);  // 0x14 is the offset for the EGR register
    *event_address |= (1 << 0);  // Generate an update event
}

// Generate an update event to update the timer's registers
void generate_update_event_timer4(void) {
    uint32_t volatile *event_address = (uint32_t *)(TIM4_BASE + EGR_OFFSET);  // 0x14 is the offset for the EGR register
    *event_address |= (1 << 0);  // Generate an update event
}


// Clear update event interrupt flag
void clear_update_event_timer2(void) {
    uint32_t volatile *status_address = (uint32_t *)(TIM2_BASE + SR_OFFSET);  // 0x10 is the offset for the SR register
    *status_address &= ~(1 << 0);  // Clear the update event interrupt flag
}

// Clear update event interrupt flag
void clear_update_event_timer3(void) {
    uint32_t volatile *status_address = (uint32_t *)(TIM3_BASE + SR_OFFSET);  // 0x10 is the offset for the SR register
    *status_address &= ~(1 << 0);  // Clear the update event interrupt flag
}

// Clear update event interrupt flag
void clear_update_event_timer4(void) {
    uint32_t volatile *status_address = (uint32_t *)(TIM4_BASE + SR_OFFSET);  // 0x10 is the offset for the SR register
    *status_address &= ~(1 << 0);  // Clear the update event interrupt flag
}

// Set the timer 3 mode to PWM
void set_pwm_mode_timer3(void) {
    // Activate PWM Mode (TIMx_CCMR1 or TIMx_CCMR2)
    uint32_t volatile *ccmr_address = (uint32_t *)(TIM3_BASE + CCMR1_OFFSET);
    *ccmr_address |= (6 << 4)| (6 << 12);  // Set OC1M and OC2M bits to 110 (PWM mode 1 for both channels)


    // Enable Preload (TIMx_CCMR1 or TIMx_CCMR2)
    //这行代码在同一个CCMR1寄存器中设置输出比较1预加载使能位（OC1PE）。启用这个功能后，CCR的值不会立即更新，而是在下一个更新事件发生时才更新。这确保了PWM波形的平滑切换，避免了任何可能的信号抖动。
    *ccmr_address |= (1 << 3)| (1 << 11);  // Enable OC1PE and OC2PE (Output Compare Preload enable for both channels)

    // Enable Auto-reload (TIMx_CR1)
    //这行代码设置控制寄存器1（CR1）中的自动重载预加载使能位（ARPE）。与OC1PE相似，这确保了自动重载寄存器（ARR）的值也在下一个更新事件时才更新，有助于同步周期更新。
    uint32_t volatile *cr1_address = (uint32_t *)(TIM3_BASE + CR1_OFFSET);
    *cr1_address |= (1 << 7);  // Enable ARPE (Auto-reload preload enable)

    // Set Output Polarity and enable output for both channels
    uint32_t volatile *ccer_address = (uint32_t *)(TIM3_BASE + CCER_OFFSET);
    *ccer_address &= ~((1 << 1) | (1 << 5));  // Set CC1P and CC2P bits to 0 (Output is active high for both channels)
    *ccer_address |= (1 << 0) | (1 << 4);  // Enable CC1E and CC2E (Capture/Compare output enable for both channels)


    // Initialize Timer (UG bit TIMx_EGR)
    //这行代码设置事件生成寄存器（EGR），通过设置更新生成位（UG）为1，强制产生一个更新事件。这个操作使得所有预加载的寄存器（如ARR、CCR）的新值立即生效，初始化定时器的配置。
    uint32_t volatile *egr_address = (uint32_t *)(TIM3_BASE + EGR_OFFSET);
    *egr_address |= (1 << 0);  // Generate an update event to initialize the timer
}

//Set the timer 4 mode to PWM
void set_pwm_mode_timer4(void) {
    // Activate PWM Mode (TIMx_CCMR1 or TIMx_CCMR2)
    uint32_t volatile *ccmr_address = (uint32_t *)(TIM4_BASE + CCMR1_OFFSET);
    *ccmr_address |= (6 << 4)| (6 << 12);  // Set OC1M and OC2M bits to 110 (PWM mode 1 for both channels)

    // Enable Preload (TIMx_CCMR1 or TIMx_CCMR2)
    *ccmr_address |= (1 << 3)| (1 << 11);  // Enable OC1PE and OC2PE (Output Compare Preload enable for both channels)

    // Enable Auto-reload (TIMx_CR1)
    uint32_t volatile *cr1_address = (uint32_t *)(TIM4_BASE + CR1_OFFSET);
    *cr1_address |= (1 << 7);  // Enable ARPE (Auto-reload preload enable)

    // Set Output Polarity and enable output for both channels
    uint32_t volatile *ccer_address = (uint32_t *)(TIM4_BASE + CCER_OFFSET);
    *ccer_address &= ~((1 << 1) | (1 << 5));  // Set CC1P and CC2P bits to 0 (Output is active high for both channels)
    *ccer_address |= (1 << 0) | (1 << 4);  // Enable CC1E and CC2E (Capture/Compare output enable for both channels)

    // Initialize Timer (UG bit TIMx_EGR)
    uint32_t volatile *egr_address = (uint32_t *)(TIM4_BASE + EGR_OFFSET);
    *egr_address |= (1 << 0);  // Generate an update event to initialize the timer


}



// start the timer 2
void start_timer2(void) {
    uint32_t volatile *control_address = (uint32_t *)(TIM2_BASE + CR1_OFFSET);  // 0x00 is the offset for the CR1 register
    *control_address |= (1 << 0);  // Enable the timer
}

// start the timer 3
void start_timer3(void) {
    uint32_t volatile *control_address = (uint32_t *)(TIM3_BASE + CR1_OFFSET);  // 0x00 is the offset for the CR1 register
    *control_address |= (1 << 0);  // Enable the timer
}

// start the timer 4
void start_timer4(void) {
    uint32_t volatile *control_address = (uint32_t *)(TIM4_BASE + CR1_OFFSET);  // 0x00 is the offset for the CR1 register
    *control_address |= (1 << 0);  // Enable the timer
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




