#include "usrGPIO.h"

//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PG��PFΪ�����.��ʹ���������ڵ�ʱ��	   PB	    
//GPIO��ʼ��
void usr_GPIO_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	//////////////////����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED0-->PG.15 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOG.15
	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG.15 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    		 //LED1-->PG.14 �˿�����, �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_14); 						 //PG.14 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PG.13 �˿�����, �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_13); 						 //PG.13 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED1-->PG.12 �˿�����, �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_12); 						 //PG.12 ����� 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED1-->PG.11 �˿�����, �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_11); 						 //PG.11 ����� 

////////////////////ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //LED1-->PF.0 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_0); 						 //PF.0 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //LED1-->PF.1 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_1); 						 //PF.1 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PF.2 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_2); 						 //PF.2 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //LED1-->PF.3 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_3); 						 //PF.3 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PF.4 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_4); 						 //PF.4 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PF.5 �˿�����, �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_5); 						 //PF.5 ����� 

/////////////////////////////	PWM
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //PB.0�˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 									 //PB.0 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //PB.1�˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ51MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_1); 									 //PB.1 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //PB.2�˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ52MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_2); 									 //PB.2 ����� 
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PB.3�˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ53MHz
	GPIO_ResetBits(GPIOB,GPIO_Pin_5); 									 //PB.3 ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //PB.4�˿�����, �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ54MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_0); 									 //PB.4 ����� 
	

	
}
 
