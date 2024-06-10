#include "dcmotor.h"
#include "egr_gpio.h"
#include "timer.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int motor1_speed = 0;
static int motor2_speed = 0;



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//DCMOTOR FUNCTIONS
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void DCMotor_Init(void)
{
    // Activate GPIO module
    activate_gpio_b();

    // Assign pin alternate function
    gpio_mode(GPIOB_BASE, MOTOR1_IN1, GPIO_OUTPUT);
    gpio_mode(GPIOB_BASE, MOTOR1_IN2, GPIO_OUTPUT);
    gpio_mode(GPIOB_BASE, MOTOR2_IN1, GPIO_OUTPUT);
    gpio_mode(GPIOB_BASE, MOTOR2_IN2, GPIO_OUTPUT);

    // Activate and configure timer
    activate_timer3();
    activate_timer4();

    set_pwm_mode_timer3();
    set_pwm_mode_timer4();

    start_timer3();
    start_timer4();

    TIMER_MAX_COMPARE = 1000;
    TIMER_MIN_COMPARE = 0;

    // Set prescaler and auto-reload value
    set_prescaler_timer3(15); // 16 MHz / 16 = 1 MHz
    set_prescaler_timer4(15); // 16 MHz / 16 = 1 MHz

    set_auto_reload_timer3(TIMER_MAX_COMPARE); // 1 MHz / 1000 = 1 kHz
    set_auto_reload_timer4(TIMER_MAX_COMPARE); // 1 MHz / 1000 = 1 kHz
    set_compare_timer3(TIMER_MIN_COMPARE);
    set_compare_timer3_channel2(TIMER_MIN_COMPARE);
    set_compare_timer4(TIMER_MIN_COMPARE);
    set_compare_timer4_channel2(TIMER_MIN_COMPARE);

    // Set initial direction to Forward
    DCMotor_SetDirection(1, Dir_Forward);
    DCMotor_SetDirection(2, Dir_Forward);

    // Set initial speed to 0
    DCMotor_SetSpeed(1, 0);
    DCMotor_SetSpeed(2, 0);
}


// use H-bridge to control the direction of the motor
void DCMotor_SetDirection(int motor, int direction)
{
    if (motor == 1) {
        if (direction == Dir_Forward) {
            gpio_mode(GPIOB_BASE, MOTOR1_IN1, GPIO_ALTERNATE);
            gpio_alternate_function(GPIOB_BASE, MOTOR1_IN1, GPIO_AF2);
            gpio_clear_pin(GPIOB_BASE, MOTOR1_IN2);
        } else if (direction == Dir_Backward) {
            gpio_clear_pin(GPIOB_BASE, MOTOR1_IN1);
            gpio_mode(GPIOB_BASE, MOTOR1_IN2, GPIO_ALTERNATE);
            gpio_alternate_function(GPIOB_BASE, MOTOR1_IN2, GPIO_AF2);
        } else if (direction == Dir_Stop) {
            gpio_clear_pin(GPIOB_BASE, MOTOR1_IN1);
            gpio_clear_pin(GPIOB_BASE, MOTOR1_IN2);
        }
    } else if (motor == 2) {
        if (direction == Dir_Forward) {
            gpio_mode(GPIOB_BASE, MOTOR2_IN1, GPIO_ALTERNATE);
            gpio_alternate_function(GPIOB_BASE, MOTOR2_IN1, GPIO_AF2);
            gpio_clear_pin(GPIOB_BASE, MOTOR2_IN2);
        } else if (direction == Dir_Backward) {
            gpio_clear_pin(GPIOB_BASE, MOTOR2_IN1);
            gpio_mode(GPIOB_BASE, MOTOR2_IN2, GPIO_ALTERNATE);
            gpio_alternate_function(GPIOB_BASE, MOTOR2_IN2, GPIO_AF2);
        } else if (direction == Dir_Stop) {
            gpio_clear_pin(GPIOB_BASE, MOTOR2_IN1);
            gpio_clear_pin(GPIOB_BASE, MOTOR2_IN2);
        }
    }
}


// Set the speed of the motor
// speed should be a value between -100 and 100
void DCMotor_SetSpeed(int motor, int speed)
{
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    if (speed < MIN_SPEED) speed = MIN_SPEED;

    // Convert speed to a compare value
    uint32_t compare = (abs(speed)) * (TIMER_MAX_COMPARE - TIMER_MIN_COMPARE) / 100;

    if (motor == 1) {
        // Set PWM duty cycle for motor 1
        if (speed == 0) {
            DCMotor_SetDirection(1, Dir_Stop);
        } else if (speed > 0) {
            DCMotor_SetDirection(1, Dir_Forward);
        } else {
            DCMotor_SetDirection(1, Dir_Backward);
        }
        set_compare_timer3(compare);
        set_compare_timer3_channel2(compare);
        clear_update_event_timer3();
        generate_update_event_timer3();
    } else if (motor == 2) {
        // Set PWM duty cycle for motor 2
        if (speed == 0) {
            DCMotor_SetDirection(2, Dir_Stop);
        } else if (speed > 0) {
            DCMotor_SetDirection(2, Dir_Forward);
        } else {
            DCMotor_SetDirection(2, Dir_Backward);
        }
        set_compare_timer4(compare);
        set_compare_timer4_channel2(compare);
        clear_update_event_timer4();
        generate_update_event_timer4();
    }
}

void DCMotor_IncreaseSpeed(int motor)
{
    if (motor == 1) {
        motor1_speed = motor1_speed + SPEED_STEP;
        DCMotor_SetSpeed(1, motor1_speed);
    } else if (motor == 2) {
        motor2_speed = motor2_speed + SPEED_STEP;
        DCMotor_SetSpeed(2, motor2_speed);
    }
}

void DCMotor_DecreaseSpeed(int motor)
{
    if (motor == 1) {
        motor1_speed = motor1_speed - SPEED_STEP;
        DCMotor_SetSpeed(1, motor1_speed);
    } else if (motor == 2) {
        motor2_speed = motor2_speed - SPEED_STEP;
        DCMotor_SetSpeed(2, motor2_speed);
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++