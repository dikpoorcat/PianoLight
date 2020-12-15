#include "LT_Protocol.h"




/*******************************************************************************
名称：void LtCmdLightLed(u8 *inBuff)
功能：LT指令解析，并进行灯光控制。
入参：u8 *InBuff，LT指令所在的起始地址。
出参：无
返回：无
*******************************************************************************/
void LtCmdLightLed(u8 *inBuff)
{
	/*根据不同模式分别解析并执行*/
	switch(inBuff[2])															//灯光模式类型
	{
		case TSET:
			TestModLight(inBuff);												//根据value字段参数执行
			break;
		case MONOCHROME:
			MonochromeModLight(inBuff);
			break;
		case FULL_COLOR1:
			FullColor1Light(inBuff);
			break;
		case FULL_COLOR2:
			FullColor2Light(inBuff);
			break;
		case MULTI_SEG:
			MultiSegmentLight(inBuff);
			break;
		default:
			UartReturnCmd(inBuff[2], UNSUPPORT);								//串口回复：不支持的模式
			break;
	}
}

/*******************************************************************************
名称：void TestModLight(u8 *inBuff)
功能：LT指令测试模式点亮LED。根据value字段参数执行。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void TestModLight(u8 *inBuff)
{
	u8		i,err = 0;
	u8		arry[3] = {0};
	
	/*开始点灯*/
	USART1->CR1&=0xffffffdf;    												//接收缓冲区非空中断禁用
	for(i=0;i<MAX_LED_NUM;i++)
	{
		LED0_Send_pixel(arry);													//先熄灭MAX_LED_NUM个LED
	}
	for(i=0;i<5;i++)
	{
		switch(inBuff[5])
		{
			case 0:
				GuZhang();														
				break;
			case 1:
				CongDian();															
				break;
			case 2:							
				DaiJi();															
				break;
			case 3:
				ZanTing();															
				break;
			case 4:
				ZhengChang();														
				break;
			default:
				err = 0xFF;
				break;	
		}
	}
	if(err)	UartReturnCmd(inBuff[2], NOPROGRAM);								//串口回复：预设程序不存在
	else UartReturnCmd(inBuff[2], SUCCESS);										//串口回复：成功
	
	USART1->CR1|=1<<5;    														//接收缓冲区非空中断使能
}

/*******************************************************************************
名称：void MonochromeModLight(u8 *inBuff)
功能：LT指令单色模式点亮LED。单色模式下，指令长度<len>固定为12(0x0C)。实际控制MAX_LED_NUM个。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void MonochromeModLight(u8 *inBuff)
{
	u16		i=0,j,num=0;
	u8		arry[3] = {0};
	
	/*开始点灯*/
	USART1->CR1&=0xffffffdf;    												//接收缓冲区非空中断禁用
	num = (inBuff[9]<<8)+inBuff[10];											//计算应点亮led个数
	for(j=0;j<MAX_LED_NUM;j++)													//实际控制MAX_LED_NUM个led
	{
		if(i<num)
		{
			LED0_Send_pixel(inBuff+6);
//			LED1_Send_pixel(inBuff+6);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//熄灭其他LED
//			LED1_Send_pixel(arry);
		}
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//串口回复：成功
	
	USART1->CR1|=1<<5;    														//接收缓冲区非空中断使能
}

/*******************************************************************************
名称：void FullColor1Light(u8 *inBuff)
功能：LT指令全彩模式1点亮LED。全彩模式1下，指令长度<len>为4n+6，n为被控LED个数。实际控制MAX_LED_NUM个。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void FullColor1Light(u8 *inBuff)
{
	u8		i=0,j,offset;
	u8		arry[3] = {0};
	
	/*开始点灯*/
	USART1->CR1&=0xffffffdf;    												//接收缓冲区非空中断禁用
	for(j=0;j<MAX_LED_NUM;j++)													//实际控制MAX_LED_NUM个led
	{
		offset = 5+i*4;															//下一个要点亮的LED位置字节的偏移量
		if(j==inBuff[offset]-1)													//相等表示当前LED是要点亮
		{
			LED0_Send_pixel(inBuff+offset+1);									//offset+1为RGB值偏移量
//			LED1_Send_pixel(inBuff+offset+1);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//熄灭其他LED
//			LED1_Send_pixel(arry;
		}
		
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//串口回复：成功
	
	USART1->CR1|=1<<5;    														//接收缓冲区非空中断使能
}

/*******************************************************************************
名称：void FullColor2Light(u8 *inBuff)
功能：LT指令全彩模式2点亮LED。全彩模式2下，指令长度<len>为5n+6，n为被控LED个数。实际控制MAX_LED_NUM个。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void FullColor2Light(u8 *inBuff)
{
	u8		i=0;
	u16		j,offset;
	u8		arry[3] = {0};
	
	/*开始点灯*/
	USART1->CR1&=0xffffffdf;    												//接收缓冲区非空中断禁用	
	for(j=0;j<MAX_LED_NUM;j++)													//实际控制MAX_LED_NUM个led
	{
		offset = 5+i*5;															//下一个要点亮的LED位置字节的偏移量
		if(j==(inBuff[offset]<<8)+inBuff[offset+1]-1)							//相等表示当前LED是要点亮
		{
			LED0_Send_pixel(inBuff+offset+2);
//			LED1_Send_pixel(inBuff+offset+2);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//熄灭其他LED
//			LED1_Send_pixel(arry;
		}
		
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//串口回复：成功、
	
	USART1->CR1|=1<<5;    														//接收缓冲区非空中断使能
}

/*******************************************************************************
名称：void FullColor2Light(u8 *inBuff)
功能：LT指令多段模式点亮LED。多段模式下，指令长度<len>为4n+6，n为被控LED段数。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void MultiSegmentLight(u8 *inBuff)
{
	u8		i,j;
	u8		arry[3] = {0};
	u8		numSegment = ((inBuff[3]<<8)+inBuff[4]-6)/4;						//计算应点亮led段数
	
	/*开始点灯*/
	USART1->CR1&=0xffffffdf;    												//接收缓冲区非空中断禁用
	for(i=0;i<numSegment;i++)
	{
		for(j=0;j<inBuff[5+i*4];j++)
		{
			LED0_Send_pixel(inBuff+6+i*4);
//			LED1_Send_pixel(inBuff+6+i*4);
		}
	}
	for(i=0;i<MAX_LED_NUM;i++)
	{
		LED0_Send_pixel(arry);													//熄灭后面MAX_LED_NUM个LED
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//串口回复：成功
	
	USART1->CR1|=1<<5;    														//接收缓冲区非空中断使能
}

/*******************************************************************************
名称：void UartReturnCmd(u8 cmd, u8 Rt)
功能：通过串口回复RT指令。
入参：u8 cmd，RT的<value>参数。
出参：无
返回：无
*******************************************************************************/
void UartReturnCmd(u8 cmd, u8 Rt)
{
	u8		i;
	u8		arry[LEN_RT] = {0x52,0x54,0x00,0x00,0x07,0x00,0x01,0x0d,0x0a};				
	
	PAout(8)=1;																	//485设置为发送
	delay_us(200);																//加入延时后解决485没有回复正确的问题
	
	arry[2] = cmd;
	arry[5] = Rt;																//组帧：cmd，RT的<value>参数
	arry[6] = XOR_check(arry, LEN_RT-1-2);										//组帧：XOR校验 -2是因为后面加了0D0A
	
	for(i=0;i<LEN_RT;i++)														
	{
		USART1->DR=arry[i]; 													//发送一个字节到串口
		while((USART1->SR&0X40)==0);											//等待发送结束（检测SUART1->SR的第6位，即发送完成标志TC）			
	}
	
	PAout(8)=0;																	//485设置为发接收
}

