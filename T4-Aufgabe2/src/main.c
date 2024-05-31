/**
 * EGR Praktikum
 * Termin 4 - Aufgabe 2
 */

#include <stdint.h>
#include "egr_i2c.h"
#include "UART.h"
#include <string.h>

int main(void)
{   
    uint32_t measurement;

    int data = 1; 

    I2C_Init();
    I2C1_SendStartCommand(SLA_ADDRESS, data, 1);
    i2c_master_receive(SLA_ADDRESS, 3);

    // 通过移位和或运算恢复原始的测量值
    measurement = ((char)received_data[0] << 16) | ((char)received_data[1] << 8) | (char)received_data[2];
    // while (1)
    // {
        for (int i = 0; i < 50; i++)
        {
            char print_string[150];
            sprintf(print_string, 
                    "\r\nDistance: %ld\r\n", 
                    measurement);

            // Send combined string via UART
            UARTsend((uint8_t*) print_string, strlen(print_string));

            // Delay to prevent flooding the UART
            delay_ms(100);
        }


    // I2C1_SendStartCommand(SLA_ADDRESS, data, 1);
    // i2c_master_receive(SLA_ADDRESS, 3);

    // measurement = ((char)received_data[0] << 16) | ((char)received_data[1] << 8) | (char)received_data[2];

    // }

    return 0;
}