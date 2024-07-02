/*
 * robo_ctr.h
 *
 *  Created on: 26.06.2024
 *      Author: Yue Zhang
 * Last Change: 26.06.2024
 */

#ifndef ROBO_CTR_H
#define ROBO_CTR_H

#include <stdint.h>
#include "egr_gpio.h"
#include "timer.h"
#include "dcmotor.h"


#define Robo_forward    1
#define Robo_left       2 //robot running direction
#define Robo_right      3
#define Robo_backward   4
#define Robo_stop       5
#define Robo_slow_forward 6
#define Robo_slow_left  7
#define Robo_slow_right 8


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void RoboCtr_Init(void);
void Robo_Direction(int direction);
void Robo_Direction2(int direction);
void check_sensor(uint32_t *is_sensor);

#endif /* ROBO_CTR_H */

