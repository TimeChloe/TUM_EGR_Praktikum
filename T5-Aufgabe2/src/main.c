/**
 * EGR Praktikum
 * Termin 5 - Aufgabe 2
 */

#include <stdint.h>
#include "egr_adc.h"
#include "egr_gpio.h"
#include "dcmotor.h"
#include "timer.h"
#include "robo_ctr.h"

#define BUTTON1 2   // GPIO Pin for start
#define BUTTON2 3  // GPIO Pin for mode change mode 1
#define BUTTON3 4   //GPIO Pin for mode change mode 2
#define LED_BLUE_PIN 8
#define LED_RED_PIN 9
#define LED_GREEN_PIN 10


void setup_buttons(void) {
    // activate GPIO A Module
    activate_gpio_a();

    // Set Buttons as Input
    gpio_mode (GPIOA_BASE, BUTTON1, GPIO_INPUT);

    // Set Buttons as Pull-Up
    gpio_pull (GPIOA_BASE, BUTTON1, GPIO_PULL_UP);
}

bool read_button(uint32_t pin) {
    return gpio_read(GPIOA_BASE, pin) == PIN_LOW;
}

void setup_led(void) {
    // active the GPIO A module
    activate_gpio_a();

    // set the LED pins as output mode
    gpio_mode(GPIOA_BASE, LED_BLUE_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, LED_RED_PIN, GPIO_OUTPUT);
    gpio_mode(GPIOA_BASE, LED_GREEN_PIN, GPIO_OUTPUT);

}

    
    

int main(void)
{
    uint32_t sensor_states[3];
    RoboCtr_Init();

    int mode = 1;

    setup_buttons();
    setup_led();

    // if the button is pressed, start a timer for 5 seconds, if not pressed, wait for the button to be pressed
    while (read_button(BUTTON1) == 0)
    {
        // Wait for the button to be pressed
        gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_HIGH);
    }
    gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);

    // TO DO: Timer implementation
    delay_ms(5000);
    gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_HIGH);




    
    if (mode == 0) {

        while (1)
        {   
            // Read Brightness Sensor values
            check_sensor(sensor_states);

            // Check if the robot is on the line, sensor3 is the middle sensor, sensor2 is the left sensor, sensor1 is the right sensor
            if (sensor_states[2] == 1 && sensor_states[1] == 1 && sensor_states[0] == 0)
            {
                Robo_Direction(Robo_forward);
            }
            else if (sensor_states[2] == 1 && sensor_states[1] == 0 && sensor_states[0] == 1)
            {
                Robo_Direction(Robo_right);
            }
            else if (sensor_states[2] == 0 && sensor_states[1] == 1 && sensor_states[0] == 0)
            {
                Robo_Direction(Robo_left);
            }
            else if (sensor_states[2] == 1 && sensor_states[1] == 1 && sensor_states[0] == 1)
            {
                Robo_Direction(Robo_right);
            }
            else
            {
                Robo_Direction(Robo_left);
            }
        }
    } else {
        while (1)
        {   
            // Read Brightness Sensor values
            check_sensor(sensor_states);

            // Check if the robot is on the line, sensor3 is the middle sensor, sensor2 is the left sensor, sensor1 is the right sensor
            if (sensor_states[2] == 1 && sensor_states[1] == 1 && sensor_states[0] == 0)
            {
                Robo_Direction2(Robo_forward);
            }
            else if (sensor_states[2] == 1 && sensor_states[1] == 0 && sensor_states[0] == 1)
            {
                Robo_Direction2(Robo_right);
            }
            else if (sensor_states[2] == 0 && sensor_states[1] == 1 && sensor_states[0] == 0)
            {
                Robo_Direction2(Robo_left);
            }
            else if (sensor_states[2] == 1 && sensor_states[1] == 1 && sensor_states[0] == 1)
            {
                Robo_Direction2(Robo_right);
            }
            else
            {
                Robo_Direction2(Robo_left);
            }
        }
    }





    return 0;
}