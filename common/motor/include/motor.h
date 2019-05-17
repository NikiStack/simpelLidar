#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <wiringPi.h>
#include <softPwm.h>


#define MOTOR_EN_1		8
#define MOTOR_LEFT_1	9
#define MOTOR_RIGHT_1	7

#define MOTOR_EN_2		3
#define MOTOR_LEFT_2	2
#define MOTOR_RIGHT_2	0

#define MOTOR_EN_3		12
#define MOTOR_LEFT_3	13
#define MOTOR_RIGHT_3	14

#define MOTOR_PWM_CYCLE	100	//10ms cycle pwm
#define MOTOR_PWM_INIT_DUTY	0	//init duty is 0

#define MOTOR_TURN_FRONT(x,y)	{digitalWrite(x, HIGH);digitalWrite(y, LOW);}
#define MOTOR_TURN_BACK(x,y)	{digitalWrite(x, LOW);digitalWrite(y, HIGH);}
#define MOTOR_STOP(x,y)			{digitalWrite(x, HIGH);digitalWrite(y, HIGH);}

extern void motor_init(void);
extern void motor_exit(void);
extern int motor_set_wheel_action(unsigned char, unsigned int, unsigned char);
#endif
