/**
 * EGR Praktikum
 * Termin 4 - Aufgabe 2
 */

#include <stdint.h>
#include "egr_i2c.h"
#include "UART.h"
#include "egr_gpio.h"
#include <string.h>

extern volatile uint8_t received_data[3];




int main(void)
{   
    uint32_t measurement;

    I2C_Init();
    // I2C1_SendStartCommand(SLA_ADDRESS, 1);
    // i2c_master_receive(SLA_ADDRESS, 3);

    // by shifting the received data, we can combine the 3 bytes into a single 24-bit value
    // measurement = ((char)received_data[0] << 16) | ((char)received_data[1] << 8) | (char)received_data[2];
    // delay_ms(200);
    while (1)
        {
        // //reset measurement
        // measurement = 2;
        // received_data[0] = 0;
        // received_data[1] = 0;
        // received_data[2] = 0;
        // // I2C_Init();
        I2C1_SendStartCommand(SLA_ADDRESS, 1);

        // // delay_ms(10);
        i2c_master_receive(SLA_ADDRESS, 3);
        // measurement = ((uint32_t)received_data[0] << 16) | ((uint32_t)received_data[1] << 8) | (uint32_t)received_data[2];
        // // 将测量值从微米转换为厘米
        // double measurement_centimeters = measurement / 10000.0;
        // char print_string[150];
        // // 使用%f来正确格式化浮点数
        // sprintf(print_string, 
        //         "\r\nDistance: %.2f cm\r\n", 
        //         measurement_centimeters);

        // // Send combined string via UART
        // UARTsend((uint8_t*) print_string, strlen(print_string));
        // delay_ms (200);

        //read the received data from the I2C
        uint8_t received_data_i2c[3];
        received_data_i2c[0] = received_data[0];
        received_data_i2c[1] = received_data[1];
        received_data_i2c[2] = received_data[2];

        // 分别打印每一个received_data的值在一行中
        char data_string[150];
        sprintf(data_string, "Received data: %d, %d, %d\r\n", received_data_i2c[0], received_data_i2c[1], received_data_i2c[2]);
        UARTsend((uint8_t*) data_string, strlen(data_string));
        delay_ms(50);
        

        // Delay to prevent flooding the UART
        delay_ms(500);
        }

    return 0;
}