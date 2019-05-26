#include <stdio.h>
#include <wiringPi.h>
#include "../common/beep/include/beep.h"
#include "../common/led/include/led.h"
#include "../common/speed/include/speed.h"
#include "../common/motor/include/motor.h"
#include "../common/slam/include/slam.h"
#include <softPwm.h>
#include <wiringPi.h>

int main(void)
{
 	wiringPiSetup();
	motor_init();
	motor_encoder_init();
	slam_uart_init();
	slam_stop();
	delay(500);
	slam_start();
	delay(500);
	for(;;)
	{
		slam_lidar_poll();
		delay(100);
	}
}
