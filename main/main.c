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
	//motor_init();
	//motor_encoder_init();
	slam_uart_init();
	//slam_lidar_poll();
	slam_stop();
	delay(500);
	slam_start();
	delay(500);
//	slam_lidar_poll();
	for(;;)
	{
		//motor_set_wheel_action(0, 1, 100);
		//slam_stop();
		//motor_set_wheel_action(1, 0, 50);
		//motor_set_wheel_action(2, 1, 10);
		//printf("uart send 'e' to lidar!\n");
		//delay(1000);
		//motor_set_wheel_action(1, 1, 100);

		//slam_start();
		slam_lidar_poll();
		//printf("\n");
		//printf("uart send 'b' to lidar!\n");
		delay(100);
		//motor_set_wheel_action(1, 2, 100);
		//delay(5000);
		//motor_exit();
		//delay(10000);
	}
}
