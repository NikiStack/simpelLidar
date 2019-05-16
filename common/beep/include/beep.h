#ifndef __BEEP_H__
#define __BEEP_H__
#include "./../../../main/include/common_type.h"

#define BEEP_NUM		5


extern void beep_init(void);
extern void beep_off(void);
extern void beep_on_duty(uint32_t duty);


#endif