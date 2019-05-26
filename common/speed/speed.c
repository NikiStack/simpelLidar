#include <stdio.h>
#include <wiringPi.h>
#include "./include/speed.h"

static struct ENCODER_STATUS s_speed_data[3];

#if 0
void motor_encoder_a_1_isr(void);
void motor_encoder_b_1_isr(void);
void motor_encoder_a_2_isr(void);
void motor_encoder_b_2_isr(void);
void motor_encoder_a_3_isr(void);
void motor_encoder_b_3_isr(void);
#endif
void motor_encoder_speed_get_ctl(void);
void motor_encoder_timer(unsigned int timeout_ms);
void motor_encoder_exit(void);
static PI_THREAD (soft_timer_thread);
PI_THREAD(soft_gpio_status_get_thread);


void motor_encoder_init(void)
{

	/*encoder init*/
	pinMode(MOTOR_ENCODER_A_1, INPUT);
	pinMode(MOTOR_ENCODER_B_1, INPUT);
	pinMode(MOTOR_ENCODER_A_2, INPUT);
	pinMode(MOTOR_ENCODER_B_2, INPUT);
	pinMode(MOTOR_ENCODER_A_3, INPUT);
	pinMode(MOTOR_ENCODER_B_3, INPUT);
#if 0

	/*input pin up to vcc*/
	pullUpDnControl(MOTOR_ENCODER_A_1, PUD_UP);
	pullUpDnControl(MOTOR_ENCODER_B_1, PUD_UP);
	pullUpDnControl(MOTOR_ENCODER_A_2, PUD_UP);
	pullUpDnControl(MOTOR_ENCODER_B_2, PUD_UP);
	pullUpDnControl(MOTOR_ENCODER_A_3, PUD_UP);
	pullUpDnControl(MOTOR_ENCODER_B_3, PUD_UP);

	//INT_EDGE_FALLING：下降沿触发
	//INT_EDGE_RISING：上升沿触发
	//INT_EDGE_BOTH ：上下降都会触发
	//INT_EDGE_SETUP：编程时用不到
	wiringPiISR(MOTOR_ENCODER_A_1, INT_EDGE_BOTH, &motor_encoder_a_1_isr);
	wiringPiISR(MOTOR_ENCODER_B_1, INT_EDGE_BOTH, &motor_encoder_b_1_isr);
	wiringPiISR(MOTOR_ENCODER_A_2, INT_EDGE_BOTH, &motor_encoder_a_2_isr);
	wiringPiISR(MOTOR_ENCODER_B_2, INT_EDGE_BOTH, &motor_encoder_b_2_isr);
	wiringPiISR(MOTOR_ENCODER_A_3, INT_EDGE_BOTH, &motor_encoder_a_3_isr);
	wiringPiISR(MOTOR_ENCODER_B_3, INT_EDGE_BOTH, &motor_encoder_b_3_isr);
#endif
	//creat a pthread for speed timer
	piThreadCreate(soft_timer_thread);
	piThreadCreate(soft_gpio_status_get_thread);

}
//velocity(mm/ms)
double* motor_velocity_get(void)
{
	double velocity[3] = {0};
	double *p = 0;
	for(int i = 0; i < 3; i++)
	{
		velocity[i] = ((s_speed_data[i].EMCODER_TIME_FOR_SPEED/ENCODER_PPR)*MOTOR_GIRTH)/MOTOR_SPEED_TEST_TIME;
	}
	p = velocity;
	return p;
}
/*exit motor encoder process*/
void motor_encoder_exit(void)
{
	//close threads and other variable
}

PI_THREAD(soft_gpio_status_get_thread)
{
	piHiPri(88);
	static unsigned char digital_read_arr_new[3][2] = {{0,0},{0,0},{0,0}};
	static unsigned char digital_read_arr_old[3][2] = {{0,0},{0,0},{0,0}};
	const unsigned char digital_gpio_arr[3][2] = {{MOTOR_ENCODER_A_1, MOTOR_ENCODER_B_1},{MOTOR_ENCODER_A_2, MOTOR_ENCODER_B_2},{MOTOR_ENCODER_A_3, MOTOR_ENCODER_B_3}};
	static unsigned int speed_zero_detect = 0;
	for(;;)
	{
		delay(1);
		//check motor rotary direction
		for(int i = 0; i < 3; i++)
		{
			//digital_read_arr_new[i][0] = digitalRead(digital_gpio_arr[i][0]);
			//digital_read_arr_new[i][1] = digitalRead(digital_gpio_arr[i][1]);
			if((digital_read_arr_old[i][0] != digitalRead(digital_gpio_arr[i][0])) ||
			(digital_read_arr_old[i][1] != digitalRead(digital_gpio_arr[i][1])))
			//if((digital_read_arr_new[i][0] != digital_read_arr_old[i][0]) || (digital_read_arr_new[i][1] != digital_read_arr_old[i][1]))
			{
				speed_zero_detect = 0;
				digital_read_arr_old[i][0] = digitalRead(digital_gpio_arr[i][0]);
				digital_read_arr_old[i][1] = digitalRead(digital_gpio_arr[i][1]);
				
				//digital_read_arr_old[i][0] = digital_read_arr_new[i][0];
				//digital_read_arr_old[i][1] = digital_read_arr_new[i][1];
				if(digital_read_arr_old[i][0])                                                                                                                                                                
				{
					BIT0SET(s_speed_data[i].ENCODER_A);
					if(BIT0CHK(s_speed_data[i].ENCODER_B))
					{
						//C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_BACK;
						s_speed_data[i].ENCODER_COUNT_DIRECTION --;
					}
					else
					{
						//C.C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_FRONT;
						s_speed_data[i].ENCODER_COUNT_DIRECTION ++;
					}
				}
				else
				{
					BIT0CLR(s_speed_data[i].ENCODER_A);
					if(BIT0CHK(s_speed_data[i].ENCODER_B))
					{
						//C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_FRONT;
						s_speed_data[i].ENCODER_COUNT_DIRECTION ++;
					}
					else
					{
						//C.C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_BACK;
						s_speed_data[i].ENCODER_COUNT_DIRECTION --;
					}
				}
				if(digital_read_arr_old[i][1])                                                                                                                                                                
				{
					BIT0SET(s_speed_data[i].ENCODER_B);
					if(BIT0CHK(s_speed_data[i].ENCODER_A))
					{
						//C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_FRONT;
						s_speed_data[i].ENCODER_COUNT_DIRECTION ++;
					}
					else
					{
						//C.C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_BACK;
						s_speed_data[i].ENCODER_COUNT_DIRECTION --;
					}
				}
				else
				{
					BIT0CLR(s_speed_data[i].ENCODER_B);
					if(BIT0CHK(s_speed_data[i].ENCODER_A))
					{
						//C.C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_BACK;
						s_speed_data[i].ENCODER_COUNT_DIRECTION --;
					}
					else
					{
						//C.W.rotation
						//s_speed_data[i].ENCODER_DIRECTION = ENCODER_ROTARY_FRONT;
						s_speed_data[i].ENCODER_COUNT_DIRECTION ++;
					}
				}
				//get count for speed calculate
				s_speed_data[i].ENCODER_COUNT ++;

			}
			else
			{
				//do nothing
				speed_zero_detect ++;
				if(speed_zero_detect > 50)
				{
					s_speed_data[i].ENCODER_COUNT = 0;
					s_speed_data[i].ENCODER_COUNT_DIRECTION = 0;
				}	
			}
		}
	}
}
//new speed timer thread
PI_THREAD (soft_timer_thread) 
{
	piHiPri(89);

	for(;;)
	{
		motor_encoder_timer(MOTOR_SPEED_TEST_TIME);
	}
}
/*1ms CYC process*/
void motor_encoder_speed_get_ctl(void)
{
	
}


/*timer for speed calculate*/
void motor_encoder_timer(unsigned int timeout_ms)
{
	unsigned int timeOut = millis() + timeout_ms;
	while(1)
	{
		if(millis() > timeOut)
		{
			//printf("Enter motor encoder timer!\n");
			for(int i = 0; i < 3; i++)
			{
				timeOut = millis() + timeout_ms;
				s_speed_data[i].EMCODER_TIME_FOR_SPEED = s_speed_data[i].ENCODER_COUNT;
				s_speed_data[i].ENCODER_COUNT = 0;
				/*display the pulse every 100ms time*/
				if(s_speed_data[i].ENCODER_COUNT_DIRECTION >= 0)
				{
					printf("get data %d is %d\n",i, s_speed_data[i].EMCODER_TIME_FOR_SPEED);
				}
				else
				{
					printf("get data %d is -%d\n",i, s_speed_data[i].EMCODER_TIME_FOR_SPEED);
				}

			}
		}
	}

}
#if 0
/*interrupt process*/
void motor_encoder_a_1_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_A_1))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_A_1);
		if(BIT0CHK(s_speed_data.ENCODER_B_1))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_A_1);
		if(BIT0CHK(s_speed_data.ENCODER_B_1))
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_BACK;
		}
		else
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_FRONT;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_1 ++;
}

void motor_encoder_b_1_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_B_1))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_B_1);
		if(BIT0CHK(s_speed_data.ENCODER_A_1))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_B_1);
		if(BIT0CHK(s_speed_data.ENCODER_A_1))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_1 = ENCODER_ROTARY_BACK;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_1 ++;
}

void motor_encoder_a_2_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_A_2))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_A_2);
		if(BIT0CHK(s_speed_data.ENCODER_B_2))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_A_2);
		if(BIT0CHK(s_speed_data.ENCODER_B_2))
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_BACK;
		}
		else
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_FRONT;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_2 ++;
}

void motor_encoder_b_2_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_B_2))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_B_2);
		if(BIT0CHK(s_speed_data.ENCODER_A_2))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_B_2);
		if(BIT0CHK(s_speed_data.ENCODER_A_2))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_2 = ENCODER_ROTARY_BACK;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_2 ++;
}

void motor_encoder_a_3_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_A_3))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_A_3);
		if(BIT0CHK(s_speed_data.ENCODER_B_3))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_A_3);
		if(BIT0CHK(s_speed_data.ENCODER_B_3))
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_BACK;
		}
		else
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_FRONT;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_3 ++;
}

void motor_encoder_b_3_isr(void)
{
	//check motor rotary direction
	if(digitalRead(MOTOR_ENCODER_B_3))                                                                                                                                                                                  
	{
		BIT0SET(s_speed_data.ENCODER_B_3);
		if(BIT0CHK(s_speed_data.ENCODER_A_3))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_BACK;
		}
	}
	else
	{
		BIT0CLR(s_speed_data.ENCODER_B_3);
		if(BIT0CHK(s_speed_data.ENCODER_A_3))
		{
			//C.C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_FRONT;
		}
		else
		{
			//C.W.rotation
			s_speed_data.ENCODER_DIRECTION_3 = ENCODER_ROTARY_BACK;
		}
	}
	//get count for speed calculate
	s_speed_data.ENCODER_COUNT_3 ++;
}

#endif
