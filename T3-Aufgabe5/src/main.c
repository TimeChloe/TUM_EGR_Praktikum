/**
 * EGR Praktikum
 * Termin 3 - Aufgabe 5
 */

#include <stdint.h>
#include "UART.h"
#include "egr_adc.h"
#include "egr_gpio.h"
#include "dcmotor.h"
#include "timer.h"


int main(void)
{
    // shut down all the motors
    activate_gpio_a();
    activate_gpio_b();
    DCMotor_All_Stop();

    ADC_Init(3, MODE_SINGLE);
    ADC_start(MODE_SINGLE);

    while (1)
    {   
        // Read Potentiometer value
        uint32_t PotentiometerValue = read_Potentiometer();
        
        // Read Brightness Sensor values
        uint32_t BrightnessSensor[3];
        read_BrightnessSensor(BrightnessSensor, 3);
        
        // Combine all values into one string
        char combined_string[150];
        sprintf(combined_string, 
                "\r\nPotentiometer Value: %ld, Brightness sensor1: %ld, sensor2: %ld, sensor3: %ld \r\n", 
                PotentiometerValue, 
                BrightnessSensor[0], 
                BrightnessSensor[1], 
                BrightnessSensor[2]);

        // Send combined string via UART
        UARTsend((uint8_t*) combined_string, strlen(combined_string));

        // Delay to prevent flooding the UART
        delay_ms(100);
    }

    return 0;
}