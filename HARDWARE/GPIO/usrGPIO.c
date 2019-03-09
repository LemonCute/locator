#include "usrGPIO.h"

//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PG和PF为输出口.并使能这两个口的时钟	   PB	    
//GPIO初始化
void usr_GPIO_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
	//////////////////方向
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED0-->PG.15 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIOG.15
	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG.15 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    		 //LED1-->PG.14 端口配置, 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_14); 						 //PG.14 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PG.13 端口配置, 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_13); 						 //PG.13 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED1-->PG.12 端口配置, 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_12); 						 //PG.12 输出高 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED1-->PG.11 端口配置, 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_11); 						 //PG.11 输出高 

////////////////////使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //LED1-->PF.0 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_0); 						 //PF.0 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //LED1-->PF.1 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_1); 						 //PF.1 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PF.2 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_2); 						 //PF.2 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //LED1-->PF.3 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_3); 						 //PF.3 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PF.4 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_4); 						 //PF.4 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PF.5 端口配置, 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOF,GPIO_Pin_5); 						 //PF.5 输出高 

/////////////////////////////	PWM
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //PB.0端口配置, 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 									 //PB.0 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //PB.1端口配置, 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为51MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_1); 									 //PB.1 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //PB.2端口配置, 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为52MHz
	GPIO_SetBits(GPIOB,GPIO_Pin_2); 									 //PB.2 输出高 
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PB.3端口配置, 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为53MHz
	GPIO_ResetBits(GPIOB,GPIO_Pin_5); 									 //PB.3 输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //PB.4端口配置, 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为54MHz
	GPIO_SetBits(GPIOG,GPIO_Pin_0); 									 //PB.4 输出高 
	

	
}
 
