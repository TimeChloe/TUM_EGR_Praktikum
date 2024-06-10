/**
 * EGR Praktikum
 * Termin 3 - Aufgabe 3
 * Autor: Yue Zhang
 * Description: Aufgabe 3: Potentiometer
 *              reads the voltage at the potentiometer and outputs it to the UART
 * Last modified: 16.05.2024
 */

#include <stdint.h>
#include <string.h>
#include "egr_adc.h"
#include "egr_gpio.h"
#include "UART.h"
#include "dcmotor.h"


int main(void)
{   
    //     for (int i = 0; i < 10; i++)
    // {
    //     // Array für Zeichenkette
    //     char testString[14] = "Hello World!\n";
    //     // Zeichenkette an PC senden:
    //     UARTsend((uint8_t*) testString, strlen(testString));
    //     delay_ms(500);
    // }
    activate_gpio_a();
    activate_gpio_b();

    DCMotor_All_Stop();
    // Initialize the motor control
    // place code to run once here
    ADC_Init();

    // // Array für Zeichenkette
    // char testString[14] = "Hello World!\n";
    // // Zeichenkette an PC senden:
    // UARTsend((uint8_t*) testString, strlen(testString));

    // Start ADC conversion
    ADC_start();

    while (1)
    {
        // Read ADC value
        uint32_t adc_value = read_adc_value_single_mode();

        // Convert ADC value to string
        char adc_string[30];
        sprintf(adc_string, "ADC Value: %ld \r\n", adc_value);

        // Send string via UART
        UARTsend((uint8_t*) adc_string, strlen(adc_string));

        // Optional delay to avoid spamming
        delay_ms(1000); // 1 second delay
    }

    return 0;
}