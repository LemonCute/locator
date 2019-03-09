#ifndef __CONTROL_H
#define __CONTROL_H 			   
#include "sys.h"  
#include "usrGPIO.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//使用SysTick的普通计数模式对延迟进行管理（适合STM32F10x系列）
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.8
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 
#define  unsigned int u16
	



void ss_sort(u16 e[5]);
void machine_move(u16 steps0,u16 steps1,u16 steps2,u16 steps3,u16 steps4);



#endif
