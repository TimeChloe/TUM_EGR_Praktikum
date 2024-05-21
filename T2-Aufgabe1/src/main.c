/**
 * EGR Praktikum
 * Termin 2 - Aufgabe 1
 * Modified by: Yue Zhang
 * Date: 2024-05-07
 * Task: 1.button1: if you press the button1, all LEDs will be turned off during the button is pressed.
 *       2.button2: if you press the button2, the color changing speed will be increased.
 *       3.button3: if you press the button3, the interrupt will be triggered and all LEDs will be turned off. Then you need to restart the program.
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "Delay_ms.h"
#include "egr_gpio.h"

//define the pins
#define LED_BLUE_PIN 8
#define LED_RED_PIN 9
#define LED_GREEN_PIN 10
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4

// Global variable to control LED activation
volatile int leds_enabled = 1;

// define the interrupt handler
void EXTI4_IRQHandler(void) // EXTI4 is the interrupt for the emergency stop button
{   
    leds_enabled = 0;

    // turn off all LEDs
    gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);

    // clear the interrupt flag
    uint32_t volatile *address = (uint32_t *)(EXTI_BASE + 0x14);  // 0x14 is the offset for the pending register
    *address |= (1 << 4);  // clear the interrupt flag for EXTI4
}

int main(void)
{
    //active the GPIO A module
    activate_gpio_a();

    //define the variables
    uint32_t delay = 500;  // initial delay time 500ms
    uint32_t led_color = 0;  // initial led color is red
    uint32_t button1_state = 0;  // initial button1 state is 0 (not pressed)
    uint32_t button2_state = 0;  // initial button2 state is 0 (not pressed)


    // set the LED pins as output mode
    gpio_mode(GPIOA_BASE, LED_BLUE_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, LED_RED_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, LED_GREEN_PIN, GPIO_OUTPUT);

    // set the button pins as input mode
    gpio_mode(GPIOA_BASE, BUTTON1_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, BUTTON2_PIN, GPIO_INPUT);
    gpio_mode(GPIOA_BASE, BUTTON3_PIN, GPIO_INPUT);

    // set the button pins as pull-up
    gpio_pull(GPIOA_BASE, BUTTON1_PIN, GPIO_PULL_UP);
    gpio_pull(GPIOA_BASE, BUTTON2_PIN, GPIO_PULL_UP);
    gpio_pull(GPIOA_BASE, BUTTON3_PIN, GPIO_PULL_UP);


    //Interrupt
    //active the SYSCFG module
    activate_SYSCFG();

    // assign GPIO Pin EXTI line
    configure_PA4_EXTI4();   //PA4 is connected to EXTI4

    // EXTI line configuration
    configure_EXTI4_line(); //falling edge will trigger the interrupt

    // enable the interrupt in the NVIC and set its priority
    // Priorität festlegen
    NVIC_SetPriority(EXTI4_IRQn, 1);
    // Interrupt aktivieren
    NVIC_EnableIRQ(EXTI4_IRQn);

    while (1)
    {
        // read the button states
        button1_state = gpio_read(GPIOA_BASE, BUTTON1_PIN);
        button2_state = gpio_read(GPIOA_BASE, BUTTON2_PIN);

    // if button1 is pressed, turn off all LEDs
    if (button1_state == 0 && leds_enabled)
    {
        gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
        gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_LOW);
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);
    }
    else if (leds_enabled)
    {
        // if button1 is not pressed, start the color changing sequence
        switch (led_color)
        {
            case 0:  // red
                gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_HIGH);
                delay_ms(delay);
                gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
                break;
            case 1:  // green
                gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_HIGH);
                delay_ms(delay);
                gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_LOW);
                break;
            case 2:  // blue
                gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);
                delay_ms(delay);
                gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);
                break;
        }

        led_color = (led_color + 1) % 3;
    }

        // if button2 is pressed, increase the color changing speed
        if (button2_state == 0 && delay > 100)
        {
            delay -= 100;
        }
        
    }

    return 0;
}