#ifndef __usrGPIO_H
#define __usrGPIO_H	 
#include "sys.h"
									  
////////////////////////////////////////////////////////////////////////////////// 
#define direction0 PGout(15)// PG15 -> 	dx			方向
#define direction1 PGout(14)// PG14	->	dy
#define direction2 PGout(13)// PG13	->	dz
#define direction3 PGout(12)// PG12	->	Alpha
#define direction4 PGout(11)// PG11	->	Beta

#define machine_enable0 PFout(0)// PF0	-> dx				使能
#define machine_enable1 PFout(1)// PF1	-> dy
#define machine_enable2 PFout(2)// PF2	-> dz
#define machine_enable3 PFout(3)// PF3	-> Alpha
#define machine_enable4 PFout(4)// PF4	-> Beta

#define PWM_channel0 PBout(0)		// PB0	-> dx			脉冲
#define PWM_channel1 PBout(1)		// PB1	-> dy
#define PWM_channel2 PBout(2)		// PB2	-> dz
#define PWM_channel3 PBout(5)		// PB5	-> Alpha
#define PWM_channel4 PGout(0)		// PG0	-> Beta

void usr_GPIO_Init(void);//

		 				    
#endif
