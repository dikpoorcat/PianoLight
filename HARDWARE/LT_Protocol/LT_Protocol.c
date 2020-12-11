#include "LT_Protocol.h"


/*******************************************************************************
名称：void UART_IRQ_LTcmd(void)
功能：串口接收LT命令时的中断服务函数。
入参：无
出参：无
返回：无
*******************************************************************************/
void UART_IRQ_LTcmd(void)
{
	u8 res;	
	u8 i;
	u8 OK[4]={0x4F,0x4B,0x0d,0x0a};		//ok/r/n
	
	//新协议，最简4字节
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
					USART_RX_STA|=0x8000;	//接收完成了 
				}
			}
			else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
				USART_RX_STA++;
				if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
			}
		}  		 									     
	}

	if(USART_RX_STA&0x8000)		//串口接收完成
	{
		PAout(8)=1;		//发送
		
		if(USART_RX_BUF[(USART_RX_STA&0X3FFF)-4]==0x31)					//判断是‘1’
		{
			state=USART_RX_BUF[(USART_RX_STA&0X3FFF)-3]-0x30;			//读取state
			delay_us(200);												//加入延时后解决485没有回复正确的问题
			for(i=0;i<4;i++)											//回复OK
			{
				USART1->DR=OK[i]; 										//发送一个字节到串口
				while((USART1->SR&0X40)==0);							//等待发送结束（检测SUART1->SR的第6位，即发送完成标志TC）			
			}
		}
		USART_RX_STA=0;													//清接收标志和记数
		memset(USART_RX_BUF,0,USART_REC_LEN);							//接收缓存清0
		
		PAout(8)=0;		//接收
	}
}
