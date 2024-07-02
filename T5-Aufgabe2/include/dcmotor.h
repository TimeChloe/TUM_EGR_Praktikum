/*
 * dcmotor.h
 *
 *  Created on: 16.05.2024
 *      Author: Yue Zhang
 * Last Change: 16.05.2024
 */

#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <stdint.h>
#include "egr_gpio.h"
#include "timer.h"





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SPEED_STEP 10
#define MAX_SPEED 100
#define MIN_SPEED -100

#define MOTOR1 1
#define MOTOR2 2

#define MOTOR1_IN1 5
#define MOTOR1_IN2 4

#define MOTOR2_IN1 6
#define MOTOR2_IN2 7

#define Dir_Forward 1
#define Dir_Backward 2
#define Dir_Stop 3
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DCMOTOR FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Initialize the motor control
void DCMotor_Init(void);


// Set the speed of the motor1
// speed should be a value between -100 and 100
void DCMotor_SetSpeed(int motor, int speed);
void DCMotor_IncreaseSpeed(int motor);
void DCMotor_DecreaseSpeed(int motor);
void DCMotor_SetDirection(int motor, int direction);

void DCMotor_All_Stop(void);


#endif /* DCMOTOR_H */
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
