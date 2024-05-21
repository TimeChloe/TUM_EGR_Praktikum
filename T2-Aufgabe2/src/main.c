/**
 * EGR Praktikum
 * Termin 2 - Aufgabe 2
 * Modified by: Yue Zhang
 * Date: 2024-05-08
 * Funtion for this task:  use the Timer-Interrupts to control the blue LED.
 *                         the blue LED will blink every 1 second, which means 0.5 second on and 0.5 second off.
 *                         The blue LED will be turned on when the compare match interrupt is triggered and turned off when the update interrupt is triggered.
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "egr_gpio.h"
#include "timer.h"

//define the pins
#define LED_BLUE_PIN 8

void gpio_init(void)
{
    //active the GPIO A module
    activate_gpio_a();

    // set the LED pins as output mode
    gpio_mode(GPIOA_BASE, LED_BLUE_PIN, GPIO_OUTPUT);
}

void timer2_init(void) {
    // Enable Timer 2 clock
    activate_timer2();

    // Set prescaler
    set_prescaler_timer2(1000 - 1);  // Set prescaler to 1000 (16 MHz / 1000 = 16 kHz)

    // Set auto-reload and compare values
    set_auto_reload_timer2(16000); // Auto-reload at 16000 counts (1 second)
    set_compare_timer2(8000);      // Compare at 8000 counts (0.5 second)

    generate_update_event_timer2();

    clear_update_event_timer2();

    // Enable timer and its interrupt
    enable_timer2_interrupts (true, true);

    // Set up NVIC
    NVIC_SetPriority(TIM2_IRQn, 2);  // Set priority level to 2
    NVIC_EnableIRQ(TIM2_IRQn);       // Enable Timer 2 interrupt in NVIC

    // Start the timer
    start_timer2();
}

// define the ISR 
void TIM2_IRQHandler(void) {
    uint32_t volatile *address = (uint32_t *)(TIM2_BASE + SR_OFFSET);  // 0x10 is the offset for SR (Status Register)

    // Check if update interrupt flag is set
    if (*address & (1 << 0)) {  
        *address &= ~(1 << 0);  // Clear the update interrupt flag
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);  // Turn off the LED
    }

    // Check if compare match interrupt flag is set
    if (*address & (1 << 1)) {
        *address &= ~(1 << 1);  // Clear the compare match interrupt flag
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);  // Turn on the LED
    }

}



int main(void)
{
    //initialize the GPIO
    gpio_init();

    //initialize the timer
    timer2_init();

    while (1)
    {
        // do nothing 
    }

    return 0;
}