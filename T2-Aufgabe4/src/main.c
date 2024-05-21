/**
 * EGR Praktikum
 * Termin 2 - Aufgabe 4
 * Modified by: Yue Zhang
 * Date: 2024-05-08
 */

/**
 * Function for this task:  button 1  make the servo move right
 *                          button 2 make the servo move to the middle position
 *                          button 3 make the servo move left
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "egr_gpio.h"
#include "timer.h"

//define the pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define LED_BLUE_PIN 8
#define SERVO_PIN 12

void gpio_init(void)
{
    //active the GPIO A module
    activate_gpio_a();

    // set the LED pins as output mode
    gpio_mode(GPIOA_BASE, BUTTON1_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, BUTTON2_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, BUTTON3_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, LED_BLUE_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, SERVO_PIN, GPIO_OUTPUT);

    gpio_pull(GPIOA_BASE, BUTTON1_PIN, GPIO_PULL_UP);
    gpio_pull(GPIOA_BASE, BUTTON2_PIN, GPIO_PULL_UP);
    gpio_pull(GPIOA_BASE, BUTTON3_PIN, GPIO_PULL_UP);


}

void timer2_init(void) {
    // Enable Timer 2 clock
    activate_timer2();

    // Set prescaler
    set_prescaler_timer2(16 - 1);  // Set prescaler to 16 (16 MHz / 16 = 1 MHz)

    // Set auto-reload and compare values
    set_auto_reload_timer2(20000); // period of 20ms
    set_compare_timer2(1500);      // 1.5ms pulse width

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
    uint32_t volatile *address = (uint32_t *)(TIM2_BASE + SR_OFFSET);

    if (*address & (1 << 0)) {  // auto-reload interrupt
        *address &= ~(1 << 0);  // clear the auto-reload interrupt flag
        gpio_write(GPIOA_BASE, SERVO_PIN, PIN_HIGH);  // set the SERVO to high
    }

    if (*address & (1 << 1)) {  // compare match interrupt
        *address &= ~(1 << 1);  // clear the compare match interrupt flag
        gpio_write(GPIOA_BASE, SERVO_PIN, PIN_LOW);  // set the SERVO to low
    }
}

void set_servo_position(uint32_t pulse_width) {
    set_compare_timer2(pulse_width);
    generate_update_event_timer2();
    clear_update_event_timer2();
}


int main(void)
{
    //initialize the GPIO
    gpio_init();

    //initialize the timer
    timer2_init();

    set_servo_position(1500); // intialize the servo to middle position

    while (1)
    {

        if (gpio_read(GPIOA_BASE, BUTTON1_PIN) == 0) {  // Move servo right to 180 degrees
            set_servo_position(1000);  // 1ms pulse width
        } else if (gpio_read(GPIOA_BASE, BUTTON2_PIN) == 0) {  // Move servo to middle position
            set_servo_position(1500);  // 1.5ms pulse width
        } else if (gpio_read(GPIOA_BASE, BUTTON3_PIN) == 0) {  // Move servo left to 0 degrees
            set_servo_position(2000);  // 2ms pulse width
        }

    }

    return 0;
}