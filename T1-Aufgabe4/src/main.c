/**
 * EGR Praktikum
 * Termin 1 - Aufgabe 4
 * Modified by: Yue Zhang
 * Date: 2024-05-07
 * Function for this task:  button 1  the LED will close the color change
 *                          button 2 the LED will speed up the color change
 *                          button 3 the LED will slow down the color change
 */

#include "egr_gpio.h"
#include "Delay_ms.h"

//define the pins
#define LED_BLUE_PIN 8
#define LED_RED_PIN 9
#define LED_GREEN_PIN 10
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4

int main(void)
{
    //active the GPIO A module
    activate_gpio_a();

    //define the variables
    uint32_t delay = 500;  // initial delay time 500ms
    uint32_t led_color = 0;  // initial led color is red
    uint32_t button1_state = 0;  // initial button1 state is 0 (not pressed)
    uint32_t button2_state = 0;  // initial button2 state is 0 (not pressed)
    uint32_t button3_state = 0; // initial button3 state is 0 (not pressed)

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

    // // turn on the blue LED
    // gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);

    while (1)
    {
        // read the button states
        button1_state = gpio_read(GPIOA_BASE, BUTTON1_PIN);
        button2_state = gpio_read(GPIOA_BASE, BUTTON2_PIN);
        button3_state = gpio_read(GPIOA_BASE, BUTTON3_PIN);

    // if button1 is pressed, turn off all LEDs
    if (button1_state == 0)
    {
        gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
        delay_ms(delay);
        gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_LOW);
        delay_ms(delay);
        gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);
        delay_ms(delay);
    }
    else
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

        // if button3 is pressed, decrease the color changing speed
        if (button3_state == 0 && delay < 1000)
        {
            delay += 100;
        }
        
    }

    return 0;
}