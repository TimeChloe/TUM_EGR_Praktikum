/**
 * EGR Praktikum
 * Termin 3 - Aufgabe 4
 * Autor: Yue Zhang
 * Description: Aufgabe 4: Helligkeitssensor
 *             reads the voltage at the brightness sensor and outputs it to the UART
 */

#include <stdint.h>
#include <string.h>
#include "egr_adc.h"
#include "egr_gpio.h"
#include "UART.h"
#include "dcmotor.h"


int main(void)
{
    // shut down all the motors
    activate_gpio_a();
    activate_gpio_b();
    DCMotor_All_Stop();

    ADC_Init(3, MODE_SINGLE);

    // ADC start
    ADC_start(MODE_SINGLE);


    while (1)
    {
        // Read ADC value
        uint32_t results[3];
        read_adc_value_single_mode(results, 3);

        // Convert ADC values to string
        char adc_string[100];

        sprintf(adc_string, "\r\nBrightness sensor1: %ld, sensor2: %ld, sensor3: %ld \r\n", results[0], results[1], results[2]);

        // sprintf(adc_string, "Brightness sensor1: %ld, sensor2: %ld, sensor3: %ld \r\n", results[0], results[1], results[2]);

        // Send string via UART
        UARTsend((uint8_t*) adc_string, strlen(adc_string));
        delay_ms(100);
    }

    return 0;
}