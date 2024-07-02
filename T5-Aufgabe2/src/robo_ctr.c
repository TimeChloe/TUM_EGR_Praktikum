#include "robo_ctr.h"
#include "dcmotor.h"
#include "egr_adc.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//ROBO_CTR FUNCTIONS
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void RoboCtr_Init(void)
{
    // Initialize the DC motors
    DCMotor_Init();
    ADC_Init(3, MODE_SINGLE);
    ADC_start(MODE_SINGLE);
    DCMotor_SetDirection(1, Dir_Forward);
    DCMotor_SetDirection(2, Dir_Forward);
}

void Robo_Direction(int direction)
{
    switch (direction)
    {
    case Robo_forward:
        DCMotor_SetSpeed(1, 3);
        DCMotor_SetSpeed(2, 3);
        break;
    case Robo_left:
        DCMotor_SetSpeed(1, 3);
        DCMotor_SetSpeed(2, 0);
        break;
    case Robo_right:
        DCMotor_SetSpeed(1, 0);
        DCMotor_SetSpeed(2, 3);
        break;
    case Robo_backward:
        DCMotor_SetSpeed(1, -3);
        DCMotor_SetSpeed(2, -3);
        break;
    case Robo_stop:
        DCMotor_SetSpeed(1, 0);
        DCMotor_SetSpeed(2, 0);
        break;
    case Robo_slow_forward:
        DCMotor_SetSpeed(1, 2);
        DCMotor_SetSpeed(2, 2);
        break;
    default:
        break;
    }
}


void Robo_Direction2(int direction)
{
    switch (direction)
    {
    case Robo_forward:
        DCMotor_SetSpeed(1, 4);
        DCMotor_SetSpeed(2, 4);
        break;
    case Robo_left:
        DCMotor_SetSpeed(1, 3);
        DCMotor_SetSpeed(2, -3);
        break;
    case Robo_right:
        DCMotor_SetSpeed(1, -3);
        DCMotor_SetSpeed(2, 3);
        break;
    case Robo_backward:
        DCMotor_SetSpeed(1, -3);
        DCMotor_SetSpeed(2, -3);
        break;
    case Robo_stop:
        DCMotor_SetSpeed(1, 0);
        DCMotor_SetSpeed(2, 0);
        break;
    case Robo_slow_left:
        DCMotor_SetSpeed(1, 2);
        DCMotor_SetSpeed(2, 0);
        break;
    case Robo_slow_right:
        DCMotor_SetSpeed(1, 0);
        DCMotor_SetSpeed(2, 2);
        break;
    default:
        break;
    }
}

void check_sensor(uint32_t *is_sensor)
{
    uint32_t BrightnessSensor[3];
    read_BrightnessSensor(BrightnessSensor, 3);
    for (int i = 0; i < 3; i++)
    {
        if (BrightnessSensor[i] > 150)
        {
            is_sensor[i] = 1; // 1 for black
        }
        else
        {
            is_sensor[i] = 0; // 0 for white
        }
    }
}
