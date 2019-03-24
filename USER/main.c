#include "sys.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "usart3.h"
#include "usrGPIO.h" 
#include "rs485.h"
#include "spi.h"
#include "key.h"
#include "led.h"
#include "remote.h"
/************************************************
 ALIENTEK战舰STM32开发板实验9 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

u8 Echo=1; //是否开启回显
u8 mode=1; //定义当前模式

u16 mt[5]={0};
u32 BASEt=0;
u8 i=0;
u8 key;  //遥控按键
u8 sended_flag=1;
u8 send_times=1;

struct comm_node
{
	u8 MacNum;
	u8 direction;
	u8 steps[2];	
};
struct command_data
{
	u16 command_head;  						//字头
	struct comm_node mac_comm[5];//控制字
	u8 comm_crc;									//crc校验
	u16 command_end;						 //结束符	
};

struct command_data uart_command={0};
struct command_data *uart_command_pointer;

u8 strTest[128]={0};
u16 steps0=0,steps1=0,steps2=0,steps3=0,steps4=0;										//串口接收 移动距离

void UART_Send_Array(unsigned char *send_array,unsigned char num); 	//两个参数 一是数组(的数据) 二是数组长度1-255 
void Remote_mode1(void);	
void Remote_mode2(void);																									//遥控器部分
void machine_move(u16 dx,u16 dy,u16 dz,u16 alpha,u16 beta);
void RS485receive(void);
void Remote_op(void);		


int main(void){		
	u16 uart_len=0;						//串口接收数据长度	
	
	delay_init();	    	 			//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 			//串口初始化为115200
	usr_GPIO_Init();					//使用PG15 14 13 12 11，PF0 1 2 3 4 5
	usart3_init(115200);			//初始化串口3 	
	TIM3_Int_Init(10,72-1);		//100KHz  定时
	Remote_Init();						//红外接收初始化		 	PB9	
	
	RS485_Init(9600);	//初始化RS485	

		machine_enable0=0;machine_enable1=0;machine_enable2=0;
		machine_enable3=0;machine_enable4=0;						//初始电机不使能	
	
 while(1)
	{
				
//---------------------串口接收---------------------------------/					
		if(USART_RX_STA&0X8000){ 									//接收到一次数据了						
				int len=0;														//接收长度
				len = (USART_RX_STA&0X3FFF)-1;				//去掉接收的 0x0d
				USART_RX_BUF[len]=0;	//添加结束符
				memcpy(strTest,USART_RX_BUF,len);			//字符串拷贝
				uart_len=len;	
				USART_RX_STA=0;		
			
			if (Echo){																//电源键控制回显							
				for(i=0;i<uart_len;i++){
					printf("%02X ",USART_RX_BUF[i]);		//回显		
					}
				}	
			
		}								
				
//----------------------------串口部分--------------------------/								
//					指令格式 7e 11 机号+方向+步长*10 校验*1 ff 7f
					
	if(uart_len>5)	//uart_len==25)																			//接收到数据为25个时则开始处理
			{	
				uart_command_pointer = (struct command_data*) &strTest[0];		//将接受 到的字符串转为结构体型
			
			if(uart_command_pointer->command_head==0x117E)									//校验帧头，异常则抛弃
				{
					if(uart_command_pointer->mac_comm[0].MacNum==0x00)					//0号电机
						{
						direction0=uart_command_pointer->mac_comm[0].direction;		//方向											
						steps0=uart_command_pointer->mac_comm[0].steps[0]*256 + uart_command_pointer->mac_comm[0].steps[1];	//步长
						}				
					if(uart_command_pointer->mac_comm[1].MacNum==0x01)					//1号电机
						{
						direction1=uart_command_pointer->mac_comm[1].direction;		//方向
						steps1 =uart_command_pointer->mac_comm[1].steps[0]*256 + uart_command_pointer->mac_comm[1].steps[1];	//步长				
						}				
					if(uart_command_pointer->mac_comm[2].MacNum==0x02)						//2号电机
						{
						direction2=uart_command_pointer->mac_comm[2].direction;			//方向
						steps2 =uart_command_pointer->mac_comm[2].steps[0]*256 + uart_command_pointer->mac_comm[2].steps[1];	//步长					
						}				
					if(uart_command_pointer->mac_comm[3].MacNum==0x03)						//3号电机
						{
						direction3 = uart_command_pointer->mac_comm[3].direction;			//方向  
						steps3 =uart_command_pointer->mac_comm[3].steps[0]*256 + uart_command_pointer->mac_comm[3].steps[1];	//步长					
						}				
					if(uart_command_pointer->mac_comm[4].MacNum==0x04)						//4号电机
						{
						direction4 = uart_command_pointer->mac_comm[4].direction;		//方向	
						steps4 =uart_command_pointer->mac_comm[4].steps[0]*256 + uart_command_pointer->mac_comm[4].steps[1];	//步长					
						}		
						
			machine_move(steps0,steps1,steps2,steps3,steps4);									//排序距离 同步输出函数		
				printf("");		
			}else 	printf("send error");	
				
			uart_len=0;		//长度清空复位			
		}
			
//------------------------------------遥控器部分---------------------------/
			if(mode==1){
					Remote_mode1();
			}else if(mode==2){
				Remote_mode2();
			}
			else{		
				Remote_op();
			}
	}
}

//----------------------------------主程序结束--------------------------------//



//------------------------------------485读六轴--------------------------------//
		void RS485receive(){
	
				float pitch,roll,heading;
				char pitchsign,rollsign,headingsign; 		
				u8 rs485buf[5]={104,4,0,4,8};	
				u8 tem_buf[20]={0};
				u8 key,len,T;
				
				if(sended_flag)
				if(send_times==1)
					printf("ok,angle,270,12,340"),send_times=2;	
				else if(send_times==2)
				printf("ok,angle,200,12,230"),send_times=3;
				else if(send_times++==3)
				printf("ok,angle,160,12,200"),send_times=1;
				
					delay_ms(400);
				/*
				RS485_Send_Data(rs485buf,len);
				delay_ms(1000);
				RS485_Receive_Data(RS485_RX_BUF,&key);
				
			for(i = 0;i<key;i++){
				tem_buf[i]=RS485_RX_BUF[i];						
			}		
			
			for(i = 0;i<key;i++){
				if(tem_buf[i]==0x68)	
					T=i;						//判断六轴的字头位置	
				printf(" %02X",tem_buf[i]); 
			}
			
			if(tem_buf[T+0]==0x68){			
					if(tem_buf[T+4]>>4)  	 pitchsign=1;
						else 								 pitchsign=0;					
				pitch=(tem_buf[T+4]&0x0F)*100+(tem_buf[T+5]>>4)*10+(tem_buf[T+5]&0x0F); //整数位
				pitch=pitch+(tem_buf[T+6]>>4)*0.1+(tem_buf[T+6]&0x0F)*0.01;							//小数位								
				if( !pitchsign ) 						pitch=pitch*(-1);
				
				
				if(tem_buf[T+7]>>4)   			rollsign=1;
						else 										rollsign=0;					
				roll=(tem_buf[T+7]&0x0F)*100+(tem_buf[T+8]>>4)*10+(tem_buf[T+8]&0x0F); //整数位
				roll=roll+(tem_buf[T+9]>>4)*0.1+(tem_buf[T+9]&0x0F)*0.01;								//小数位
				if(rollsign) 							roll=roll*(-1);
				
				
				if(tem_buf[T+10]>>4)  		 headingsign=1;
						else 									 headingsign=0;					
				heading=(tem_buf[T+10]&0x0F)*100+(tem_buf[T+11]>>4)*10+(tem_buf[T+11]&0x0F); 	//整数位
				heading=heading+(tem_buf[T+12]>>4)*0.1+(tem_buf[T+12]&0x0F)*0.01;							//小数位
				if(headingsign ) 				heading=heading*(-1);																//航向角没有负数			
				if(heading<180)			heading=heading*(-1);	
								else 				heading=360-heading;	
								
			for(i = 0;i<key;i++)	{printf(" %02X",tem_buf[i]); } 						//六轴初始数据 测试用...
				printf("ok,angle,%.2f,%.2f,%.2f",roll,pitch,heading);	//发送俯仰角和航向到上位机
		}*/
	}
//-----------------------------------485结束--------------------------------------/


//------------------------电机的串口响应程序----------------------------------/
void machine_move(u16 dx,u16 dy,u16 dz,u16 alpha,u16 beta)
{	
	#define T  30																		//定义周期的最小值为 2*T*10Us
	u16 MT[5]={0};
	u32 countpulse,countUs;	
	u16 maxdistance=0;   //,maxdegree=0
	
			if(alpha>maxdistance)	maxdistance=alpha;			//找出最大值	
			if(beta>maxdistance)	maxdistance=beta;			
			if(dx>maxdistance)		maxdistance=dx;
			if(dy>maxdistance)		maxdistance=dy;
			if(dz>maxdistance)		maxdistance=dz;					
		
	if(maxdistance>0)
	{
		countpulse=(maxdistance*6400)/400;								//电机200个脉冲上升沿转一圈-上升(下降)4 mm=400*	10uM			
		countUs=countpulse*2*T;														//最小时间10Us*T 取最小时间作为 运动最快的脉冲	
																																	
		if(dx>0) 		{		 MT[0]=T*maxdistance/dx;   		 	}	////考虑变速问题 需要修改频率pmw
		if(dy>0)		{		 MT[1]=T*maxdistance/dy;			 	}	//
		if(dz>0)		{		 MT[2]=T*maxdistance/dz;			 	}	//
		if(alpha>0)	{	 	 MT[3]=T*maxdistance/alpha;			}	//电机转一圈 alpha/beta 变化4° 	
		if(beta>0)	{	 	 MT[4]=T*maxdistance/beta;		 	}	
		
			machine_enable0=1;machine_enable1=1;machine_enable2=1; //运动中 锁死
			machine_enable3=1; 	machine_enable4=1;								//同时使能 5 个电机
		
	TIM_Cmd(TIM3, ENABLE);		//定时器3 开始计数
		
	while(BASEt<countUs)		//(BASEt*10<countUs)		
		{				
	if(mt[0]>=MT[0]&&MT[0])		{	PWM_channel0=!PWM_channel0;	mt[0]=0;	}		//由引脚电平MT[]时间翻转一次
	if(mt[1]>=MT[1]&&MT[1])		{	PWM_channel1=!PWM_channel1;	mt[1]=0;	}		//产生指定频率的 PWM 驱动步进电机
	if(mt[2]>=MT[2]&&MT[2])		{	PWM_channel2=!PWM_channel2;	mt[2]=0;	}
	if(mt[3]>=MT[3]&&MT[3])		{	PWM_channel3=!PWM_channel3;	mt[3]=0;	}
	if(mt[4]>=MT[4]&&MT[4])		{	PWM_channel4=!PWM_channel4; mt[4]=0;	}			
		}					
			TIM_Cmd(TIM3, DISABLE);			
			machine_enable0=0;
			machine_enable1=0;
			machine_enable2=0;
			machine_enable3=0;
			machine_enable4=0;	

	if (Echo)			
		printf("\r\nBASEt=%d MT=%d，%d，%d，%d，%d ",BASEt,MT[0],MT[1],MT[2],MT[3],MT[4]);		//回显 测试用...
			BASEt=0; 	MT[0]=0;	MT[1]=0;	MT[2]=0;	MT[3]=0;	MT[4]=0;	
								dx	 =0;	dy   =0;	dz   =0;	alpha=0;	beta =0;
								mt[0]=0;	mt[1]=0;	mt[2]=0;	mt[3]=0;	mt[4]=0;												//清数据
			printf("move,finish");		//-----------------应答 移动完成
		}

	else	printf("error");	//-----------------应答 发送位移为全 0
}
///-------------------------电机串口响应结束-------------------------------/


//-------------------遥控器部分-------------------------------------------/
void Remote_mode1()				
{	
	#define M 5
//	u8 key;     			//遥控按键使用
	char *str=0;			//遥控的键值含义
	u8 key1=Remote_Scan();

		if(key1)
		{	 	
		TIM_Cmd(TIM3, ENABLE);
		if(mt[0]>=M)		{	PWM_channel0=!PWM_channel0;	mt[0]=0;	}
		if(mt[1]>=M)		{	PWM_channel1=!PWM_channel1;	mt[1]=0;	}
		if(mt[2]>=M)		{	PWM_channel2=!PWM_channel2;	mt[2]=0;	}
		if(mt[3]>=M*2)		{	PWM_channel3=!PWM_channel3;	mt[3]=0;	}
		if(mt[4]>=M*2)		{	PWM_channel4=!PWM_channel4; mt[4]=0;	}	
			switch(key1)
				{
					
				case 0:str="ERROR";			break;			   
				case 162:str="POWER";		Echo =!Echo;break;	     
				case 98:str="UP";				direction0=1;machine_enable0=1;break;	  			 	//上 
				case 2:str="PLAY";			RS485receive();break;		 
				case 226:str="ALIENTEK";printf("mode,02,55,00,end,");delay_ms(100);printf("point,move"),mode=2;break;		  
				case 194:str="RIGHT";		direction1=1;machine_enable1=1;break;	 	 				//右 
				case 34:str="LEFT";			direction1=0;machine_enable1=1;break;			  		//左
				case 224:str="VOL-";		direction2=0;machine_enable2=1;break;			  		//前
				case 168:str="DOWN";		direction0=0;machine_enable0=1;break;		  			//下 
				case 144:str="VOL+";		direction2=1;machine_enable2=1;break;		    		//后
				case 104:str="1";				direction3=1;machine_enable3=1;break;		  			//1
				case 152:str="2";				break;	   
				case 176:str="3";				direction3=0;machine_enable3=1;break;	    			//3
				case 48:str="4";				direction4=1;machine_enable4=1;break;						//4    
				case 24:str="5";				break;		    
				case 122:str="6";				direction4=0;machine_enable4=1;break;						//6	  
					
				case 16:str="7";				mode=1;break;			   					
				case 56:str="8";				printf("point,move");mode=2;break;	 
				case 90:str="9";				printf("point,move");mode=3;break;
				case 66:str="0";				break;
				case 82:str="DELETE";		break;		 
				}
				key=key1;sended_flag=1;
		}
		else 
			{		
				u16 distant=BASEt/M/2*400/6400*100;
				
				TIM_Cmd(TIM3, DISABLE);
					if(sended_flag){
				if(key==98||key==168) printf("mode,01,00,%d,%d,end,",direction0,distant);
				else if(key==34||key==194)		 printf("mode,01,01,%d,%d,end,",direction1,distant);	
				else if(key==224||key==144)		 printf("mode,01,02,%d,%d,end,",direction2,distant);		
				else if(key==104||key==176)		 printf("mode,01,03,%d,%d,end,",direction3,distant);		
				else if(key==48||key==122)		 printf("mode,01,04,%d,%d,end,",direction4,distant);			
				//printf("key==%d",key);
				sended_flag=0;BASEt=0;
					}
				machine_enable0=0;
				machine_enable1=0;
				machine_enable2=0;
				machine_enable3=0;
				machine_enable4=0;	
				};		
}
//------------------------------------------------------------------------------//



void Remote_mode2(){	

//	u8 key;     			//遥控按键使用
	char *str=0;			//遥控的键值含义
	key=Remote_Scan();
 	
			switch(key)
				{
				case 0:str="ERROR";			break;			   
				case 162:str="POWER";		Echo =!Echo;break;	    
				case 98:str="UP";				printf("mode,02,03,00,120,end,");break;	  			 	//上 
				case 2:str="PLAY";			RS485receive();break;		 
				case 226:str="ALIENTEK"; break;		  
				case 194:str="RIGHT";		printf("mode,02,04,01,120,end,"); break;	 				//右 
				case 34:str="LEFT";			printf("mode,02,04,00,120,end,");	break;			  		//左
				case 224:str="VOL-";		printf("mode,01,02,00,end,");break;			  		//前
				case 168:str="DOWN";		printf("mode,02,03,01,120,end,");break;		  			//下 
																		
				case 144:str="VOL+";		printf("mode,01,02,00,end,");break;		    		//后
				case 104:str="1";		    printf("mode,02,55,00,end,");break;		  			//1
				case 152:str="2";				break;	   
				case 176:str="3";		     printf("mode,02,55,01,end,"); break;	    			//3
				case 48:str="4";				printf("mode,01,04,00,end,");break;						//4    
				case 24:str="5";				break;		    
				case 122:str="6";				printf("mode,01,04,01,00,end,");break;						//6	  
					
				case 16:str="7";				mode=1;break;			   					
				case 56:str="8";			mode=2;;break;
				case 90:str="9";				mode=3;break;
				case 66:str="0";				break;
				case 82:str="DELETE";		break;		 
				}

}
//------------------------------------------------------------------------------//

//------------------- 遥控器部分--手动 -------------------------------------------/
void Remote_op(void)				
{	
	#define M 5
	   			//遥控按键使用
	char *str=0;			//遥控的键值含义
	key=Remote_Scan();
		if(key)
		{	 	
		TIM_Cmd(TIM3, ENABLE);
		if(mt[0]>=M)		{	PWM_channel0=!PWM_channel0;	mt[0]=0;	}
		if(mt[1]>=M)		{	PWM_channel1=!PWM_channel1;	mt[1]=0;	}
		if(mt[2]>=M)		{	PWM_channel2=!PWM_channel2;	mt[2]=0;	}
		if(mt[3]>=M*2)		{	PWM_channel3=!PWM_channel3;	mt[3]=0;	}
		if(mt[4]>=M*2)		{	PWM_channel4=!PWM_channel4; mt[4]=0;	}	
			switch(key)
				{
				case 0:str="ERROR";			break;			   
				case 162:str="POWER";		Echo =!Echo;break;	    
				case 98:str="UP";				direction0=1;machine_enable0=1;break;	  			 	//上 
				case 2:str="PLAY";			RS485receive();break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";		direction1=1;machine_enable1=1;break;	 	 				//右 
				case 34:str="LEFT";			direction1=0;machine_enable1=1;break;			  		//左
				case 224:str="VOL-";		direction2=0;machine_enable2=1;break;			  		//前
				case 168:str="DOWN";		direction0=0;machine_enable0=1;break;		  			//下 
				case 144:str="VOL+";		direction2=1;machine_enable2=1;break;		    		//后
				case 104:str="1";				direction3=1;machine_enable3=1;break;		  			//1
				case 152:str="2";				break;	   
				case 176:str="3";				direction3=0;machine_enable3=1;break;	    			//3
				case 48:str="4";				direction4=1;machine_enable4=1;break;						//4    
				case 24:str="5";				break;		    
				case 122:str="6";				direction4=0;machine_enable4=1;break;						//6	  
					
				case 16:str="7";				mode=1;break;			   					
				case 56:str="8";				mode=2;break;	 
				case 90:str="9";				mode=3;break;
				case 66:str="0";				break;
				case 82:str="DELETE";		break;		 
				}
		}
		else 
			{			
				TIM_Cmd(TIM3, DISABLE);
				machine_enable0=0;
				machine_enable1=0;
				machine_enable2=0;
				machine_enable3=0;
				machine_enable4=0;	
				};		
}
//------------------------------------------------------------------------------//
