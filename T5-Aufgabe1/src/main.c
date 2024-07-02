/**
 * EGR Praktikum
 * Termin 5 - Aufgabe 1
 */

#include <stdint.h>
#include "UART.h"
#include "egr_adc.h"
#include "egr_gpio.h"
#include "dcmotor.h"
#include "timer.h"
#include "robo_ctr.h"

int main(void)
{
    uint32_t sensor_states[3];
    RoboCtr_Init();


    while (1)
    {   
        // Read Brightness Sensor values
        check_sensor(sensor_states);

        // Check if the robot is on the line, sensor3 is the middle sensor, sensor2 is the left sensor, sensor1 is the right sensor
        if (sensor_states[2] == 1 && sensor_states[1] == 0 && sensor_states[0] == 0)
        {
            Robo_Direction(Robo_forward);
        }
        else if (sensor_states[2] == 0 && sensor_states[1] == 0 && sensor_states[0] == 1)
        {
            Robo_Direction(Robo_right);
        }
        else if (sensor_states[2] == 0 && sensor_states[1] == 1 && sensor_states[0] == 0)
        {
            Robo_Direction(Robo_left);
        }
        else
        {
            Robo_Direction(Robo_left);
        }


    }

    return 0;
}