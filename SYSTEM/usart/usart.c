#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化（适合STM32F10x系列）		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//V1.6修改说明 20150109
//uart_init函数去掉了开启PE中断
//V1.7修改说明 20150322
//修改OS_CRITICAL_METHOD宏判断为：SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////




#if EN_USART1_RX   //如果使能了接收
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8		USART_RX_BUF[USART_REC_LEN];     										//接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到指令头LT
//bit13		接收到指令头L
//bit12~0，	接收到的有效字节数目 8191
u16		USART_RX_STA=0;       													//接收状态标记


/*******************************************************************************
名称：void USART1_IRQHandler(void)
功能：串口1中断服务程序。
入参：无
出参：无
返回：无
*******************************************************************************/
void USART1_IRQHandler(void)
{
	u8				res=0;
	static u16		cmd_len;       												//用于存储当前命令长度
	
	if(USART1->SR&(1<<5))														//接收到数据
	{	 
		res=USART1->DR; 														
		if(USART_RX_STA&0x4000)													//若已接收到LT
		{
			/*等待接收到足够的长度，就标记为接收结束*/
			if((USART_RX_STA&0x1FFF)==6)										//此时接收到指令长度
			{
				cmd_len = (USART_RX_BUF[3]<<8)+USART_RX_BUF[4];					//计算应接收长度（指令长度）
				if(cmd_len>USART_REC_LEN)
				{
					USART_RX_STA=0;												//命令过长无法接收，重新开始
					//加个标志，给上位机回复一下 命令过长无法接收
					cmd_len = 0;												//重置
				}
			}
			if( cmd_len!=0 && ((USART_RX_STA&0x1FFF)+1)>=cmd_len )				//若接收到足够的长度，此时已接收到<XOR>（+1是因为USART_RX_STA的++在后面）
			{
				if(res == XOR_check(USART_RX_BUF,cmd_len-1))					//XOR校验
				{
					USART_RX_STA|=0x8000;										//接收完成了
				}
				else
				{
					USART_RX_STA=0;												//XOR校验失败，重新开始
					//加个标志，给上位机回复一下 XOR校验失败
				}					
			}
		}
		else 																	//还没收到LT
		{	
			if(USART_RX_STA&0x2000)												//若已接收到L
			{
				/*等待接收T*/
				if(res!=0x54) USART_RX_STA=0;									//接收错误（L后不是T），重新开始
				else
				{
					USART_RX_STA|=0x4000;										//标记为接收到了LT
					USART_RX_BUF[3] = 0;										//清一下len字段，防止意外
					USART_RX_BUF[4] = 0;										//清一下len字段，防止意外
				}
			}
			/*等待接收L*/
			else if(res==0x4C) 
			{
				USART_RX_STA=0x2000;											//标记为接收到了L，并重置接收长度
			}	
		} 		 									     
	}

	USART_RX_BUF[USART_RX_STA&0x1FFF]=res;										//写入接收缓存
	USART_RX_STA++; 
	if((USART_RX_STA&0x1FFF)>(USART_REC_LEN-1))
	{
		USART_RX_STA=0;															//接收溢出，重新开始
		memset(USART_RX_BUF,0,USART_REC_LEN);									//清空接收缓存
	}
}
#endif
	
//	u8 res;	
//	u8 i;
//	u8 OK[4]={0x4F,0x4B,0x0d,0x0a};		//ok/r/n
//	
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	
//	//新协议，最简4字节
//	if(USART1->SR&(1<<5))	//接收到数据
//	{	 
//		res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//					if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
//					USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			}
//			else //还没收到0X0D
//			{	
//				if(res==0x0d)USART_RX_STA|=0x4000;
//				USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//				USART_RX_STA++;
//				if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//			}
//		}  		 									     
//	}

//	if(USART_RX_STA&0x8000)		//串口接收完成
//	{
//		PAout(8)=1;		//发送
//		
//		if(USART_RX_BUF[(USART_RX_STA&0X3FFF)-4]==0x31)					//判断是‘1’
//		{
//			state=USART_RX_BUF[(USART_RX_STA&0X3FFF)-3]-0x30;			//读取state
//			delay_us(200);												//加入延时后解决485没有回复正确的问题
//			for(i=0;i<4;i++)											//回复OK
//			{
//				USART1->DR=OK[i]; 										//发送一个字节到串口
//				while((USART1->SR&0X40)==0);							//等待发送结束（检测SUART1->SR的第6位，即发送完成标志TC）			
//			}
//		}
//		USART_RX_STA=0;													//清接收标志和记数
//		memset(USART_RX_BUF,0,USART_REC_LEN);							//接收缓存清0
//		
//		PAout(8)=0;		//接收
//	}	
////开发板例子	
////	if(USART1->SR&(1<<5))	//接收到数据
////	{	 
////		res=USART1->DR; 
////		if((USART_RX_STA&0x8000)==0)//接收未完成
////		{
////			if(USART_RX_STA&0x4000)//接收到了0x0d
////			{
////				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
////				else USART_RX_STA|=0x8000;	//接收完成了 
////			}else //还没收到0X0D
////			{	
////				if(res==0x0d)USART_RX_STA|=0x4000;
////				else
////				{
////					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
////					USART_RX_STA++;
////					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
////				}		 
////			}
////		}  		 									     
////	}
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 




//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
}
