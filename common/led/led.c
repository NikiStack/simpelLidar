#include "./include/led.h"
#include <wiringPi.h>
#include "./../../main/include/common_type.h"

void led_init(void)
{
	pinMode (7, OUTPUT);
}

void led_on(void)
{
	digitalWrite(7, HIGH);
}

void led_off(void)
{
	digitalWrite(7, LOW);
}
