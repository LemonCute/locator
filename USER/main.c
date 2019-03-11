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
 ALIENTEKս��STM32������ʵ��9 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

u8 Echo=0; //�Ƿ�������

u16 mt[5]={0};
u32 BASEt=0;
u8 i=0;


struct comm_node
{
	u8 MacNum;
	u8 direction;
	u8 steps[2];	
};
struct command_data
{
	u16 command_head;  						//��ͷ
	struct comm_node mac_comm[5];//������
	u8 comm_crc;									//crcУ��
	u16 command_end;						 //������	
};

struct command_data uart_command={0};
struct command_data *uart_command_pointer;

u8 strTest[128]={0};
u16 steps0=0,steps1=0,steps2=0,steps3=0,steps4=0;										//���ڽ��� �ƶ�����

void UART_Send_Array(unsigned char *send_array,unsigned char num); 	//�������� һ������(������) �������鳤��1-255 
void Remote(void);																									//ң��������
void machine_move(u16 dx,u16 dy,u16 dz,u16 alpha,u16 beta);
void RS485receive();

int main(void){		
	u8 key=20,T,nrfsend_time,Answer;
	u16 uart_len=0;						//���ڽ������ݳ���	
	
	delay_init();	    	 			//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 			//���ڳ�ʼ��Ϊ115200
	usr_GPIO_Init();					//ʹ��PG15 14 13 12 11��PF0 1 2 3 4 5
	usart3_init(115200);			//��ʼ������3 	
	TIM3_Int_Init(10,72-1);		//100KHz  ��ʱ
	Remote_Init();						//������ճ�ʼ��		 	PB9	
	
	RS485_Init(9600);	//��ʼ��RS485	

		machine_enable0=0;machine_enable1=0;machine_enable2=0;
		machine_enable3=0;machine_enable4=0;						//��ʼ�����ʹ��	
	
 while(1)
	{
				
//---------------------���ڽ���---------------------------------/					
		if(USART_RX_STA&0X8000){ 									//���յ�һ��������						
				int len=0;														//���ճ���
				len = (USART_RX_STA&0X3FFF)-1;				//ȥ�����յ� 0x0d
				USART_RX_BUF[len]=0;	//��ӽ�����
				memcpy(strTest,USART_RX_BUF,len);			//�ַ�������
				uart_len=len;	
				USART_RX_STA=0;					
			if (Echo){																//��Դ�����ƻ���							
				for(i=0;i<uart_len;i++){
					printf("%02X ",USART_RX_BUF[i]);		//����		
					}
				}					
		}								
				
//----------------------------���ڲ���--------------------------/								
//					ָ���ʽ 7e 11 ����+����+����*10 У��*1 ff 7f
					
	if(uart_len>5)	//uart_len==25)																			//���յ�����Ϊ25��ʱ��ʼ����
			{	
				uart_command_pointer = (struct command_data*) &strTest[0];		//������ �����ַ���תΪ�ṹ����
			
			if(uart_command_pointer->command_head==0x117E)									//У��֡ͷ���쳣������
				{
					if(uart_command_pointer->mac_comm[0].MacNum==0x00)					//0�ŵ��
						{
						direction0=uart_command_pointer->mac_comm[0].direction;		//����											
						steps0=uart_command_pointer->mac_comm[0].steps[0]*256 + uart_command_pointer->mac_comm[0].steps[1];	//����
						}				
					if(uart_command_pointer->mac_comm[1].MacNum==0x01)					//1�ŵ��
						{
						direction1=uart_command_pointer->mac_comm[1].direction;		//����
						steps1 =uart_command_pointer->mac_comm[1].steps[0]*256 + uart_command_pointer->mac_comm[1].steps[1];	//����				
						}				
					if(uart_command_pointer->mac_comm[2].MacNum==0x02)						//2�ŵ��
						{
						direction2=uart_command_pointer->mac_comm[2].direction;			//����
						steps2 =uart_command_pointer->mac_comm[2].steps[0]*256 + uart_command_pointer->mac_comm[2].steps[1];	//����					
						}				
					if(uart_command_pointer->mac_comm[3].MacNum==0x03)						//3�ŵ��
						{
						direction3 = uart_command_pointer->mac_comm[3].direction;			//����  
						steps3 =uart_command_pointer->mac_comm[3].steps[0]*256 + uart_command_pointer->mac_comm[3].steps[1];	//����					
						}				
					if(uart_command_pointer->mac_comm[4].MacNum==0x04)						//4�ŵ��
						{
						direction4 = uart_command_pointer->mac_comm[4].direction;		//����	
						steps4 =uart_command_pointer->mac_comm[4].steps[0]*256 + uart_command_pointer->mac_comm[4].steps[1];	//����					
						}																			
			machine_move(steps0,steps1,steps2,steps3,steps4);									//������� ͬ���������								
			}else 	printf("send error");	
				
			uart_len=0;		//������ո�λ			
		}
//------------------------------------ң��������---------------------------/
			Remote();			
	}
}

//----------------------------------���������--------------------------------//




//------------------------------------485������--------------------------------//
		void RS485receive(){
	
				float pitch,roll,heading;
				char pitchsign,rollsign,headingsign; 		
				u8 rs485buf[5]={104,4,0,4,8};	
				u8 tem_buf[20]={0};
				u8 key,len,T;
				
				RS485_Send_Data(rs485buf,len);
				delay_ms(1000);
				RS485_Receive_Data(RS485_RX_BUF,&key);
				
			for(i = 0;i<key;i++){
				tem_buf[i]=RS485_RX_BUF[i];						
			}		
			
			for(i = 0;i<key;i++){
				if(tem_buf[i]==0x68)	
					T=i;						//�ж��������ͷλ��	
			}
			if(tem_buf[T+0]==0x68){			
					if(tem_buf[T+4]>>4)  	 pitchsign=1;
						else 								 pitchsign=0;					
				pitch=(tem_buf[T+4]&0x0F)*100+(tem_buf[T+5]>>4)*10+(tem_buf[T+5]&0x0F); //����λ
				pitch=pitch+(tem_buf[T+6]>>4)*0.1+(tem_buf[T+6]&0x0F)*0.01;							//С��λ								
				if( !pitchsign ) 						pitch=pitch*(-1);
				
				
				if(tem_buf[T+7]>>4)   			rollsign=1;
						else 										rollsign=0;					
				roll=(tem_buf[T+7]&0x0F)*100+(tem_buf[T+8]>>4)*10+(tem_buf[T+8]&0x0F); //����λ
				roll=roll+(tem_buf[T+9]>>4)*0.1+(tem_buf[T+9]&0x0F)*0.01;								//С��λ
				if(rollsign) 							roll=roll*(-1);
				
				
				if(tem_buf[T+10]>>4)  		 headingsign=1;
						else 									 headingsign=0;					
				heading=(tem_buf[T+10]&0x0F)*100+(tem_buf[T+11]>>4)*10+(tem_buf[T+11]&0x0F); 	//����λ
				heading=heading+(tem_buf[T+12]>>4)*0.1+(tem_buf[T+12]&0x0F)*0.01;							//С��λ
				if(headingsign ) 				heading=heading*(-1);																//�����û�и���			
				if(heading<180)			heading=heading*(-1);	
								else 				heading=360-heading;	
								
//			for(i = 0;i<key;i++)	{printf(" %02X",tem_buf[i]); } 						//�����ʼ���� ������...
				printf("angle,%.2f,%.2f,%.2f",roll,pitch,heading);	//���͸����Ǻͺ�����λ��
		}
	}
//-----------------------------------485����--------------------------------------/

 
	

//------------------------����Ĵ�����Ӧ����----------------------------------/
void machine_move(u16 dx,u16 dy,u16 dz,u16 alpha,u16 beta)
{	
	#define T  30																		//�������ڵ���СֵΪ 2*T*10Us
	u16 MT[5]={0};
	u32 countpulse,countUs;	
	u16 maxdistance=0,maxdegree=0;
	
			if(alpha>maxdistance)	maxdistance=alpha;			//�ҳ����ֵ	
			if(beta>maxdistance)	maxdistance=beta;			
			if(dx>maxdistance)		maxdistance=dx;
			if(dy>maxdistance)		maxdistance=dy;
			if(dz>maxdistance)		maxdistance=dz;					
		
	if(maxdistance>0)
	{
		countpulse=(maxdistance*6400)/400;								//���200������������תһȦ-����(�½�)4 mm=400*	10uM			
		countUs=countpulse*2*T;														//��Сʱ��10Us*T ȡ��Сʱ����Ϊ �˶���������	
																																	
		if(dx>0) 		{		 MT[0]=T*maxdistance/dx;   		 	}	////���Ǳ������� ��Ҫ�޸�Ƶ��pmw
		if(dy>0)		{		 MT[1]=T*maxdistance/dy;			 	}	//
		if(dz>0)		{		 MT[2]=T*maxdistance/dz;			 	}	//
		if(alpha>0)	{	 	 MT[3]=T*maxdistance/alpha;			}	//���תһȦ alpha/beta �仯4�� 	
		if(beta>0)	{	 	 MT[4]=T*maxdistance/beta;		 	}	
			machine_enable0=1;machine_enable1=1;machine_enable2=1; //�˶��� ����
			machine_enable3=1; 	machine_enable4=1;								//ͬʱʹ�� 5 �����
		
	TIM_Cmd(TIM3, ENABLE);		//��ʱ��3 ��ʼ����
	while(BASEt<countUs)		//(BASEt*10<countUs)		
		{				
	if(mt[0]>=MT[0]&&MT[0])		{	PWM_channel0=!PWM_channel0;	mt[0]=0;	}		//�����ŵ�ƽMT[]ʱ�䷭תһ��
	if(mt[1]>=MT[1]&&MT[1])		{	PWM_channel1=!PWM_channel1;	mt[1]=0;	}		//����ָ��Ƶ�ʵ� PWM �����������
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
		printf("\r\nBASEt=%d MT=%d��%d��%d��%d��%d ",BASEt,MT[0],MT[1],MT[2],MT[3],MT[4]);		//���� ������...
			BASEt=0; 	MT[0]=0;	MT[1]=0;	MT[2]=0;	MT[3]=0;	MT[4]=0;	
								dx	 =0;	dy   =0;	dz   =0;	alpha=0;	beta =0;
								mt[0]=0;	mt[1]=0;	mt[2]=0;	mt[3]=0;	mt[4]=0;												//������
			printf("OK");		//-----------------Ӧ�� �ƶ����
		}

	else	printf("OK");	//-----------------Ӧ�� ����λ��Ϊȫ 0
}
///-------------------------���������Ӧ����-------------------------------/


//-------------------ң��������-------------------------------------------/
void Remote(){	

	u8 key;     			//ң�ذ���ʹ��
	char *str=0;			//ң�صļ�ֵ����
	key=Remote_Scan();
 	
			switch(key)
				{
				case 0:str="ERROR";			break;			   
				case 162:str="POWER";		Echo =!Echo;break;	    
				case 98:str="UP";				machine_enable0=1;break;	  			 	//�� 
				case 2:str="PLAY";			RS485receive();break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";		machine_enable1=1;break;	 	 				//�� 
				case 34:str="LEFT";			machine_enable1=1;break;			  		//��
				case 224:str="VOL-";		machine_enable2=1;break;			  		//ǰ
				case 168:str="DOWN";		machine_enable0=1;break;		  			//�� 
				case 144:str="VOL+";		machine_enable2=1;break;		    		//��
				case 104:str="1";				machine_enable3=1;break;		  			//1
				case 152:str="2";				break;	   
				case 176:str="3";				machine_enable3=1;break;	    			//3
				case 48:str="4";				machine_enable4=1;break;						//4    
				case 24:str="5";				break;		    
				case 122:str="6";				machine_enable4=1;break;						//6	  
				case 16:str="7";				break;			   					
				case 56:str="8";				break;	 
				case 90:str="9";				break;
				case 66:str="0";				break;
				case 82:str="DELETE";		break;		 
				}

}
//------------------------------------------------------------------------------//

//-------------------ң��������-------------------------------------------/
void Remote_op()				
{	
	#define M 5
	u8 key;     			//ң�ذ���ʹ��
	char *str=0;			//ң�صļ�ֵ����
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
				case 98:str="UP";				direction0=1;machine_enable0=1;break;	  			 	//�� 
				case 2:str="PLAY";			RS485receive();break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";		direction1=1;machine_enable1=1;break;	 	 				//�� 
				case 34:str="LEFT";			direction1=0;machine_enable1=1;break;			  		//��
				case 224:str="VOL-";		direction2=0;machine_enable2=1;break;			  		//ǰ
				case 168:str="DOWN";		direction0=0;machine_enable0=1;break;		  			//�� 
				case 144:str="VOL+";		direction2=1;machine_enable2=1;break;		    		//��
				case 104:str="1";				direction3=1;machine_enable3=1;break;		  			//1
				case 152:str="2";				break;	   
				case 176:str="3";				direction3=0;machine_enable3=1;break;	    			//3
				case 48:str="4";				direction4=1;machine_enable4=1;break;						//4    
				case 24:str="5";				break;		    
				case 122:str="6";				direction4=0;machine_enable4=1;break;						//6	  
				case 16:str="7";				break;			   					
				case 56:str="8";				break;	 
				case 90:str="9";				break;
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
