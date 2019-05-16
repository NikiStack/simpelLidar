#ifndef __SPEED_H__
#define __SPEED_H__

#define MOTOR_SPEED_TEST_TIME 50	//ms
#define ENCODER_PPR	160*4
#define MOTOR_DIAMETER	48	//mm
#define PI	3.14159
#define MOTOR_GIRTH	301.6	//2*PI*MOTOR_DIAMETER	//mm

//MOTOR 1 测试用编码器测速
#define MOTOR_ENCODER_A_1	6
#define MOTOR_ENCODER_B_1	10
#define MOTOR_ENCODER_A_2	4
#define MOTOR_ENCODER_B_2	5
#define MOTOR_ENCODER_A_3	1
#define MOTOR_ENCODER_B_3	16

#define		BIT0CHK( C )	(( C )& 0x01 )
#define		BIT0CLR( C )	(( C )&= 0xfe )
#define		BIT1CHK( C )	(( C )& 0x02 )
#define		BIT1CLR( C )	(( C )&= 0xfd )
#define		BIT0SET( C )	(( C )|= 0x01 )
#define		BIT1SET( C )	(( C )|= 0x02 )
#define		BIT2SET( C )	(( C )|= 0x04 )
#define		BIT3SET( C )	(( C )|= 0x08 )
#define		BIT4SET( C )	(( C )|= 0x10 )
#define		BIT5SET( C )	(( C )|= 0x20 )
#define		BIT6SET( C )	(( C )|= 0x40 )
#define		BIT7SET( C )	(( C )|= 0x80 )

typedef enum
{
	ENCODER_ROTARY_FRONT	= 0x00								
,	ENCODER_ROTARY_BACK
,	ENCODER_ROTARY_STOP								
}ENCODER_DUTY_MODE;

struct ENCODER_STATUS
{
	unsigned char ENCODER_A;
	unsigned char ENCODER_B;
	unsigned int ENCODER_COUNT;
	ENCODER_DUTY_MODE ENCODER_DIRECTION;
	unsigned int EMCODER_TIME_FOR_SPEED;
	unsigned int EMCODER_TIME_OUT;
	int ENCODER_COUNT_DIRECTION;
};

extern void motor_encoder_init(void);
extern double* motor_velocity_get(void);
#endif
