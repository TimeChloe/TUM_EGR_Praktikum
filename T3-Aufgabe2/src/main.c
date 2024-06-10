/**
 * EGR Praktikum
 * Termin 3 - Aufgabe 2
 * Datei: main.c
 * Date: 16.05.2024
 * Autor: Yue Zhang
 * Description: DC-Motor Geschwindigkeitssteuerung
 *               Taster 1: Increase Speed
 *               Taster 2: Decrease Speed
 *               Taster 3: Stop Motor 
 * Last modified: 16.05.2024
 */

#include <stdint.h>
#include "dcmotor.h"
#include "timer.h"
#include "egr_gpio.h"
#include "Delay_ms.h"

#define BUTTON1_INCREASE 2   // GPIO Pin for Increase Button
#define BUTTON2_DECREASE 3  // GPIO Pin for Decrease Button
#define BUTTON3_STOP 4  // GPIO Pin for Stop Button

#define SPEED_STEP 10  // Speed Step for Increase/Decrease

void setup_buttons(void) {
    // activate GPIO A Module
    activate_gpio_a();

    // Set Buttons as Input
    gpio_mode (GPIOA_BASE, BUTTON1_INCREASE, GPIO_INPUT);
    gpio_mode (GPIOA_BASE, BUTTON2_DECREASE, GPIO_INPUT);
    gpio_mode (GPIOA_BASE, BUTTON3_STOP, GPIO_INPUT);

    // Set Buttons as Pull-Up
    gpio_pull (GPIOA_BASE, BUTTON1_INCREASE, GPIO_PULL_UP);
    gpio_pull (GPIOA_BASE, BUTTON2_DECREASE, GPIO_PULL_UP);
    gpio_pull (GPIOA_BASE, BUTTON3_STOP, GPIO_PULL_UP);

}

bool read_button(uint32_t pin) {
    return gpio_read(GPIOA_BASE, pin) == PIN_LOW;
}

int main(void) {

    // 初始化硬件
    DCMotor_Init();

    setup_buttons();

    while (1) {
        // 检查增速按钮
        if (read_button(BUTTON1_INCREASE)) {
            DCMotor_IncreaseSpeed(MOTOR1);
            DCMotor_IncreaseSpeed(MOTOR2);
        }

        // 检查减速按钮
        if (read_button(BUTTON2_DECREASE)) {
            DCMotor_DecreaseSpeed(MOTOR1);
            DCMotor_DecreaseSpeed(MOTOR2);
        }

        // 检查停止按钮
        if (read_button(BUTTON3_STOP)) {
            DCMotor_SetDirection(MOTOR1, Dir_Stop);
            DCMotor_SetDirection(MOTOR2, Dir_Stop);
        }


        // 延迟一段时间以防止按钮抖动
        delay_ms(50);
    }
}