#include <stdio.h>
#include <wiringPi.h>
#include "../common/beep/include/beep.h"
#include "../common/led/include/led.h"
#include "../common/speed/include/speed.h"
#include "../common/motor/include/motor.h"
#include <softPwm.h>
#include <wiringPi.h>

int main(void)
{
 	wiringPiSetup();
	motor_init();
	motor_encoder_init();

	for(;;)
	{
		//motor_set_wheel_action(0, 1, 100);
		//motor_set_wheel_action(1, 0, 50);
		//motor_set_wheel_action(2, 1, 10);
		delay(5000);
		motor_set_wheel_action(1, 1, 100);
		delay(5000);
		motor_set_wheel_action(1, 2, 100);
		delay(5000);
		motor_exit();
		delay(10000);
	}
}
