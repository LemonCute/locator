#ifndef __CONTROL_H
#define __CONTROL_H 			   
#include "sys.h"  
#include "usrGPIO.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й����ʺ�STM32F10xϵ�У�
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.8
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 
#define  unsigned int u16
	



void ss_sort(u16 e[5]);
void machine_move(u16 steps0,u16 steps1,u16 steps2,u16 steps3,u16 steps4);



#endif
