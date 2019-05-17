#include "./include/motor.h"


//static int motor_set_wheel_action(unsigned char, unsigned int, unsigned char);
void motor_exit(void);

void motor_init(void)
{
	/*gpio init*/
	pinMode(MOTOR_EN_1, OUTPUT);
	pinMode(MOTOR_LEFT_1, OUTPUT);
	pinMode(MOTOR_RIGHT_1, OUTPUT);
	pinMode(MOTOR_EN_2, OUTPUT);
	pinMode(MOTOR_LEFT_2, OUTPUT);
	pinMode(MOTOR_RIGHT_2, OUTPUT);
	pinMode(MOTOR_EN_3, OUTPUT);
	pinMode(MOTOR_LEFT_3, OUTPUT);
	pinMode(MOTOR_RIGHT_3, OUTPUT);

	//PWM
	softPwmCreate(MOTOR_EN_1, MOTOR_PWM_INIT_DUTY, MOTOR_PWM_CYCLE);	
	softPwmCreate(MOTOR_EN_2, MOTOR_PWM_INIT_DUTY, MOTOR_PWM_CYCLE);	
	softPwmCreate(MOTOR_EN_3, MOTOR_PWM_INIT_DUTY, MOTOR_PWM_CYCLE);	

	//init motor speed 0
	softPwmWrite(MOTOR_EN_1, MOTOR_PWM_INIT_DUTY);
	softPwmWrite(MOTOR_EN_2, MOTOR_PWM_INIT_DUTY);
	softPwmWrite(MOTOR_EN_3, MOTOR_PWM_INIT_DUTY);
	
#if 0 //for test
	digitalWrite(MOTOR_LEFT_1, HIGH);
	digitalWrite(MOTOR_LEFT_2, HIGH);
	digitalWrite(MOTOR_LEFT_3, HIGH);

	digitalWrite(MOTOR_RIGHT_1, LOW);
	digitalWrite(MOTOR_RIGHT_2, LOW);
	digitalWrite(MOTOR_RIGHT_3, LOW);

	softPwmWrite(MOTOR_EN_1, 50);
	softPwmWrite(MOTOR_EN_2, 10);
	softPwmWrite(MOTOR_EN_3, 100);
#else
//	motor_set_wheel_action(1, 2, 100);
//	motor_set_wheel_action(2, 1, 100);
//	motor_set_wheel_action(3, 2, 100);
#endif
}

void motor_exit(void)
{
	softPwmStop(MOTOR_EN_1);
	softPwmStop(MOTOR_EN_2);
	softPwmStop(MOTOR_EN_3);
}

int motor_set_wheel_action(unsigned char wheel, unsigned int direction, unsigned char speed)
{
	unsigned char ret = 0;

	if(speed < 0 || speed > 100)
	{
		printf("motor speed input error!\n");
		ret =  -1;
	}

	switch (wheel)
	{
		case 1://Motor 1
			switch (direction)
			{
				case 0://stop
					MOTOR_STOP(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				case 1://back
					MOTOR_TURN_BACK(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					//printf("motor 1 direction set back!\n");
					break;
				case 2://front
					//digitalWrite(MOTOR_LEFT_1, LOW);
					//digitalWrite(MOTOR_RIGHT_1, HIGH);
					MOTOR_TURN_FRONT(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					//printf("motor 1 direction set front!\n");
					break;
				default:
					printf("motor direction set error!\n");
					break;
			}
			//set motor speed
			softPwmWrite(MOTOR_EN_1, speed);
			printf ("set motor 1 speed id %d", speed);
			break;
		case 2://Motor 2
			switch (direction)
			{
				case 0://stop
					MOTOR_STOP(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				case 1://back
					MOTOR_TURN_BACK(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				case 2://front
					MOTOR_TURN_FRONT(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				default:
					printf("motor direction set error!\n");
					break;
			}
			//set motor speed
			softPwmWrite(MOTOR_EN_1, speed);
			break;
		case 3://Motor 3
			switch (direction)
			{
				case 0://stop
					MOTOR_STOP(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				case 1://back
					MOTOR_TURN_BACK(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				case 2://front
					MOTOR_TURN_FRONT(MOTOR_LEFT_1, MOTOR_RIGHT_1);
					break;
				default:
					printf("motor direction set error!\n");
					break;
			}
			//set motor speed
			softPwmWrite(MOTOR_EN_1, speed);
			break;
		default:
			printf("motor wheel input error!\n");
			break;
	}
	return ret;
}

