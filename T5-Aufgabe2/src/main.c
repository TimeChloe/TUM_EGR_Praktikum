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
#include "UART.h"

#define BUTTON1 2   // GPIO Pin for start
#define BUTTON2 3  // GPIO Pin for mode change mode 1
#define BUTTON3 4   //GPIO Pin for mode change mode 2
#define LED_BLUE_PIN 8
#define LED_RED_PIN 9
#define LED_GREEN_PIN 10
#define SYSTEM_CLOCK 16000000 // assuming 16 MHz system clock

// global variable to check if the timer has started
volatile bool start = false;

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

void timer_5s(void) {
    // 激活 Timer 2
    activate_timer2();

    // 设置预分频器
    uint32_t prescaler = (SYSTEM_CLOCK / 1000) - 1; // 预分频器设为16000-1，使定时器计数频率为1 kHz (1 ms)
    set_prescaler_timer2(prescaler);

    // 设置自动重装载值为5000，使定时器在5秒后溢出
    set_auto_reload_timer2(5000 - 1);

    generate_update_event_timer2();

    clear_update_event_timer2();

    // 使能更新中断
    enable_timer2_interrupts(true, false);

    // Set up NVIC
    NVIC_SetPriority(TIM2_IRQn, 1);  // Set priority level to 1
    NVIC_EnableIRQ(TIM2_IRQn);       // Enable Timer 2 interrupt in NVIC
}

// Timer 2 interrupt service routine (ISR)
void TIM2_IRQHandler(void) {
    gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_GREEN_PIN, PIN_HIGH);
    start = true;   
    clear_update_event_timer2();
    deactivate_timer2();
}
    



int main(void)
{
    uint32_t sensor_states[3];
    RoboCtr_Init();

    int mode = 1;

    setup_buttons();
    setup_led();
    timer_5s();

    // if the button is pressed, start a timer for 5 seconds, if not pressed, wait for the button to be pressed
    while (read_button(BUTTON1) == 0)
    {
        // Wait for the button to be pressed
        gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_HIGH);
    }
    gpio_write(GPIOA_BASE, LED_RED_PIN, PIN_LOW);
    gpio_write(GPIOA_BASE, LED_BLUE_PIN, PIN_HIGH);

    start_timer2();

    while (!start)
    {
    }
    // delay_ms(5000);


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