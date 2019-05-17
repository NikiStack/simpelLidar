#include <wiringPi.h>
#include <wiringSerial.h>
#include <softPwm.h>
#include "./include/slam.h"

#define LIDAR_BUAD	230400
#define LIDAR_DEVICE_ID	"/dev/ttyS1"
#define LIDAR_PWM_INIT_DUTY	0
#define LIDAR_PWM_CYCLE	100
#define LIDAR_PWM_EN	0 //slam pwm pin num

/*
uart
extern int   serialOpen      (const char *device, const int baud) ;
extern void  serialClose     (const int fd) ;
extern void  serialFlush     (const int fd) ;
extern void  serialPutchar   (const int fd, const unsigned char c) ;
extern void  serialPuts      (const int fd, const char *s) ;
extern void  serialPrintf    (const int fd, const char *message, ...) ;
extern int   serialDataAvail (const int fd) ;
extern int   serialGetchar   (const int fd) ;

pwm
extern int  softPwmCreate (int pin, int value, int range) ;
extern void softPwmWrite  (int pin, int value) ;
extern void softPwmStop   (int pin) ;

*/

static int fd ;

/*init slam uart --"/dev/ttyS1"*/
int slam_uart_init(void)
{
	char ret = 0;

	if ((fd = serialOpen(LIDAR_DEVICE_ID, LIDAR_BUAD)) < 0)
	{
		printf("Unable to open serial device: %s\n", LIDAR_DEVICE_ID);
		ret = -1 ;
	}

	pinMode(LIDAR_PWM_EN, OUTPUT);
	softPwmCreate(LIDAR_PWM_EN, LIDAR_PWM_INIT_DUTY, LIDAR_PWM_CYCLE);	
	return ret;
}

int slam_speed_adj(unsigned char speed)
{
	char ret = 0;
	if(speed > 100 || speed < 0)
	{
		ret = -1;
	}
	softPwmWrite(LIDAR_PWM_EN, speed);
	return ret;
}

void slam_start(void)
{
	serialPutchar(fd, 'b');
}
void slam_srop(void)
{
	serialPutchar(fd, 'e');
}
void slam_lidar_poll(void)
{
	piThreadCreate(slam_poll_thread);
}
//new speed timer thread
PI_THREAD (slam_poll_thread) 
{
	piHiPri(88);

	for(;;)
	{
		
	}
}

