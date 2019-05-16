#include "./include/beep.h"
#include <wiringPi.h>
#include "./../../main/include/common_type.h"

void beep_init(void)
{
	pinMode (BEEP_NUM, PWM_OUTPUT);
}

void beep_on_duty(uint32_t duty)
{
	if(duty < 1024)
	{
		pwmWrite (BEEP_NUM, duty);
	}
	else
	{
		printf("set value error! beep_on_duty(x); //x<1024\n");
	}
}

void beep_off(void)
{
	pwmWrite (BEEP_NUM, 0);
}


