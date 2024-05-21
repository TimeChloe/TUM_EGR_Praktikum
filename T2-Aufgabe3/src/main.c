/**
 * EGR Praktikum
 * Termin 2 - Aufgabe 3
 * Modified by: Yue Zhang
 * Date: 2024-05-08
 */

/**
 * Function for this task: button 1  the LED will be dimmer
 * button 2 the LED will be brighter
 */
// 为了解决因为程序运行速度快而导致的按钮按下时LED亮度迅速变化的问题，可以引入一个软件防抖动机制和延时，以便只在按钮状态稳定时调整LED亮度。这样可以确保按钮在短时间内多次读取结果一致时，才认为是有效的操作。

#include <stdint.h>
#include "stm32g431xx.h"
#include "egr_gpio.h"
#include "timer.h"
#include "Delay_ms.h"

//define the pins
#define LED_BLUE_PIN 8
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

void gpio_init(void)
{
    //active the GPIO A module
    activate_gpio_a();

    // set the LED pins as output mode
    gpio_mode(GPIOA_BASE, LED_BLUE_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, BUTTON1_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, BUTTON2_PIN, GPIO_INPUT);

    gpio_pull(GPIOA_BASE, BUTTON1_PIN, GPIO_PULL_UP);
    gpio_pull(GPIOA_BASE, BUTTON2_PIN, GPIO_PULL_UP);
}

void timer2_init(void) {
    // Enable Timer 2 clock
    activate_timer2();

    // Set prescaler
    set_prescaler_timer2(1600 - 1);  // Set prescaler to 1600 (16 MHz / 1600 = 10 kHz)

    // Set auto-reload and compare values
    set_auto_reload_timer2(160); // 
    set_compare_timer2(80);      // 

    generate_update_event_timer2();

    clear_update_event_timer2();

    // Enable timer and its interrupt
    enable_timer2_interrupts (true, true);

    // Set up NVIC
    NVIC_SetPriority(TIM2_IRQn, 1);  // Set priority level to 1
    NVIC_EnableIRQ(TIM2_IRQn);       // Enable Timer 2 interrupt in NVIC

    // Start the timer
    start_timer2();
}

// define the ISR 
void TIM2_IRQHandler(void) {
    uint32_t volatile *address = (uint32_t *)(TIM2_BASE + SR_OFFSET); // get the status register address

    if (*address & (1 << 0)) {  // auto-reload interrupt
        *address &= ~(1 << 0);  // clear the auto-reload interrupt flag
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);  // set the LED to high
    }

    if (*address & (1 << 1)) {  // compare match interrupt
        *address &= ~(1 << 1);  // clear the compare match interrupt flag
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);  // set the LED to low
    }
}

void increase_duty_cycle(void) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM2_BASE + CCR1_OFFSET);  // get the compare register address
    uint32_t compare = *compare_address;  // get the current compare value
    if (compare < 150) {  // 
        compare += 5;  // 
        if (compare > 150) {
            compare = 150;  // prevent from exceeding 150
        }
        set_compare_timer2(compare);  // set the new compare value

        generate_update_event_timer2();  // generate an update event
        clear_update_event_timer2();  // clear the update event
    }
}


void decrease_duty_cycle(void) {
    uint32_t volatile *compare_address = (uint32_t *)(TIM2_BASE + CCR1_OFFSET);  // get the compare register address
    uint32_t compare = *compare_address;  // get the current compare value
    if (compare > 20) {  // 
        compare -= 5;  // 
        if (compare < 20) {
            compare = 20;  // prevent from exceeding 20
        }
        set_compare_timer2(compare);  // set the new compare value

        generate_update_event_timer2();  // generate an update event
        clear_update_event_timer2();  // clear the update event
    }
}


// prevent the button from bouncing
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

void debounce_delay(void) {
    // Simple delay for debouncing
    volatile int count = 50000;  // Delay loop count
    while (count--);
}

int read_button_stable(uint32_t gpio_base, uint32_t pin) {
    // Read the button with a simple debounce logic
    if (gpio_read(gpio_base, pin) == 0) {
        debounce_delay();  // Wait to see if the button state remains
        if (gpio_read(gpio_base, pin) == 0) {
            // Wait a bit more to confirm the button is definitely pressed
            debounce_delay();
            return 1;
        }
    }
    return 0;
}



int main(void)
{
    //initialize the GPIO
    gpio_init();

    //initialize the timer
    timer2_init();

    while (1)
    {
        if (read_button_stable(GPIOA_BASE, BUTTON1_PIN) == 0) {  
            increase_duty_cycle();  
            debounce_delay();  

        }
        if (read_button_stable(GPIOA_BASE, BUTTON2_PIN) == 0) {  
            decrease_duty_cycle();  
            debounce_delay();  
        }
    }

    return 0;
}