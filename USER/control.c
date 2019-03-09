#include "control.h"
#include "delay.h"














































//#define  unsigned int u16
//	
//u16 count_steps[5];
//u16 steps_for_sort[5]={0};

//void ss_sort(u16 e[5])  
//{  
//    u16 i,j,k,t;  
//    for(i=0;i<4;i++)		//控制n-1趟的选择步骤
//    {//在e[i],e[i+1]...e[n-1]中选键值最小的结点
//			for(k=i,j=i+1;j<5;j++)  
//				if(e[k]>e[j])  
//						k=j;  
//			if(k!=i)//e[i]和e[k]交换
//			{     
//					t=e[i];  
//					e[i]=e[k];  
//					e[k]=t;  
//			}
//			count_steps[i]=k;
//    }  
//}
//void machine_move(u16 steps0,u16 steps1,u16 steps2,u16 steps3,u16 steps4)
//{
//	//步长排序
//	//顺序执行
//	//差速控制步长
//	steps_for_sort[0]=steps0;
//	steps_for_sort[1]=steps1;
//	steps_for_sort[2]=steps2;
//	steps_for_sort[3]=steps3;
//	steps_for_sort[4]=steps4;
//	ss_sort(steps_for_sort);
//	
//	machine_enable0=1;
//	machine_enable1=1;
//	machine_enable2=1;
//	machine_enable3=1;
//	machine_enable4=1;
//	delay_ms(steps_for_sort[0]*100);
//	switch(count_steps[0])
//	{
//		case 0:machine_enable0=0;break;
//		case 1:machine_enable1=0;break;
//		case 2:machine_enable2=0;break;
//		case 3:machine_enable3=0;break;
//		case 4:machine_enable4=0;break;
//		default:machine_enable0=0;machine_enable1=0;machine_enable2=0;machine_enable3=0;machine_enable4=0;break;
//	}
//	delay_ms((steps_for_sort[1]-steps_for_sort[0])*100);
//	switch(count_steps[1])
//	{
//		case 0:machine_enable0=0;break;
//		case 1:machine_enable1=0;break;
//		case 2:machine_enable2=0;break;
//		case 3:machine_enable3=0;break;
//		case 4:machine_enable4=0;break;
//		default:machine_enable0=0;machine_enable1=0;machine_enable2=0;machine_enable3=0;machine_enable4=0;break;
//	}
//	delay_ms((steps_for_sort[2]-steps_for_sort[1])*100);
//	switch(count_steps[2])
//	{
//		case 0:machine_enable0=0;break;
//		case 1:machine_enable1=0;break;
//		case 2:machine_enable2=0;break;
//		case 3:machine_enable3=0;break;
//		case 4:machine_enable4=0;break;
//		default:machine_enable0=0;machine_enable1=0;machine_enable2=0;machine_enable3=0;machine_enable4=0;break;
//	}
//	delay_ms((steps_for_sort[3]-steps_for_sort[2])*100);
//	switch(count_steps[3])
//	{
//		case 0:machine_enable0=0;break;
//		case 1:machine_enable1=0;break;
//		case 2:machine_enable2=0;break;
//		case 3:machine_enable3=0;break;
//		case 4:machine_enable4=0;break;
//		default:machine_enable0=0;machine_enable1=0;machine_enable2=0;machine_enable3=0;machine_enable4=0;break;
//	}
//	delay_ms((steps_for_sort[4]-steps_for_sort[3])*100);
//	switch(count_steps[4])
//	{
//		case 0:machine_enable0=0;break;
//		case 1:machine_enable1=0;break;
//		case 2:machine_enable2=0;break;
//		case 3:machine_enable3=0;break;
//		case 4:machine_enable4=0;break;
//		default:machine_enable0=0;machine_enable1=0;machine_enable2=0;machine_enable3=0;machine_enable4=0;break;
//	}
//	machine_enable0=0;
//	machine_enable1=0;
//	machine_enable2=0;
//	machine_enable3=0;
//	machine_enable4=0;

//}
