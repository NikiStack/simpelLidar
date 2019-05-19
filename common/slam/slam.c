#include <wiringPi.h>
#include <wiringSerial.h>
#include <softPwm.h>
#include "./include/slam.h"
#include <unistd.h>

#define LIDAR_BUAD	230400
#define LIDAR_DEVICE_ID	"/dev/ttyUSB0"
#define LIDAR_PWM_INIT_DUTY	0
#define LIDAR_PWM_CYCLE	100
#define LIDAR_PWM_EN	1 //slam pwm pin num
#define uint16_t unsigned int
#define uint8_t	unsigned char
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

PI_THREAD (slam_poll_thread); 
/*init slam uart --"/dev/ttyUSB0"*/
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
	//pwmWrite(LIDAR_PWM_EN,1023);
	softPwmWrite(LIDAR_PWM_EN, 10);
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
//	serialPutchar(fd, 'b');
//	serialPuts(fd,"b\n");
	write(fd,"b\n", 1 );
	
}
void slam_stop(void)
{
	//serialPutchar(fd, 'e');
//	serialPuts(fd, "e\n");
	write(fd,"e\n", 1 );
}
void slam_lidar_poll(void)
{
	piThreadCreate(slam_poll_thread);
}
//new speed timer thread
PI_THREAD (slam_poll_thread) 
{
	piHiPri(88);
	char temp_char;
	int start_count = 0;
	uint8_t got_scan = 0;
	char good_sets = 0;
	unsigned int motor_speed = 0;
	int index;
	unsigned char raw_bytes[2520]={0};
	char ret;
	int rpms;
	while(!got_scan)
	{
		read(fd, &raw_bytes[start_count], 1);
		if(0 == start_count)
		{
			if(0xFA == raw_bytes[start_count])
			{
				start_count = 1;
				//printf("start_count = 1\n");
			}
		}
		else if(1 == start_count)
		{
			if(0xA0 == raw_bytes[start_count])
			{
				start_count = 0;
				got_scan = 1;
				
				read(fd, &raw_bytes[2], 2518);
				for(uint16_t i = 0; i < 2520; i=i+42)
        			{
          				if(raw_bytes[i] == 0xFA && raw_bytes[i+1] == (0xA0 + i / 42)) //&& CRC check
          				{
            					good_sets++;
            					motor_speed += (raw_bytes[i+3] << 8) + raw_bytes[i+2]; //accumulate count for avg. time increment
            					rpms=(raw_bytes[i+3]<<8|raw_bytes[i+2])/10;

						for(uint16_t j = i+4; j < i+40; j=j+6)
            					{
            						index = 6*(i/42) + (j-4-i)/6;

              						// Four bytes per reading
              						uint8_t byte0 = raw_bytes[j];
              						uint8_t byte1 = raw_bytes[j+1];
              						uint8_t byte2 = raw_bytes[j+2];
              						uint8_t byte3 = raw_bytes[j+3];

              						// Remaining bits are the range in mm
             						uint16_t intensity = (byte1 << 8) + byte0;

          						// Last two bytes represent the uncertanty or intensity, might also be pixel area of target...
							// uint16_t intensity = (byte3 << 8) + byte2;
							uint16_t range = (byte3 << 8) + byte2;

							if(index == 20)
							printf ("r[%d]=%f,\n",359-index, range / 1000.0);
						}
					}
        			}
			}
		}
		else
		{
			start_count = 0;
		}
	}
}	

