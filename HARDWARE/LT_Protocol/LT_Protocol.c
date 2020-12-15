#include "LT_Protocol.h"




/*******************************************************************************
���ƣ�void LtCmdLightLed(u8 *inBuff)
���ܣ�LTָ������������еƹ���ơ�
��Σ�u8 *InBuff��LTָ�����ڵ���ʼ��ַ��
���Σ���
���أ���
*******************************************************************************/
void LtCmdLightLed(u8 *inBuff)
{
	/*���ݲ�ͬģʽ�ֱ������ִ��*/
	switch(inBuff[2])															//�ƹ�ģʽ����
	{
		case TSET:
			TestModLight(inBuff);												//����value�ֶβ���ִ��
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
			UartReturnCmd(inBuff[2], UNSUPPORT);								//���ڻظ�����֧�ֵ�ģʽ
			break;
	}
}

/*******************************************************************************
���ƣ�void TestModLight(u8 *inBuff)
���ܣ�LTָ�����ģʽ����LED������value�ֶβ���ִ�С�
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void TestModLight(u8 *inBuff)
{
	u8		i,err = 0;
	u8		arry[3] = {0};
	
	/*��ʼ���*/
	USART1->CR1&=0xffffffdf;    												//���ջ������ǿ��жϽ���
	for(i=0;i<MAX_LED_NUM;i++)
	{
		LED0_Send_pixel(arry);													//��Ϩ��MAX_LED_NUM��LED
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
	if(err)	UartReturnCmd(inBuff[2], NOPROGRAM);								//���ڻظ���Ԥ����򲻴���
	else UartReturnCmd(inBuff[2], SUCCESS);										//���ڻظ����ɹ�
	
	USART1->CR1|=1<<5;    														//���ջ������ǿ��ж�ʹ��
}

/*******************************************************************************
���ƣ�void MonochromeModLight(u8 *inBuff)
���ܣ�LTָ�ɫģʽ����LED����ɫģʽ�£�ָ���<len>�̶�Ϊ12(0x0C)��ʵ�ʿ���MAX_LED_NUM����
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void MonochromeModLight(u8 *inBuff)
{
	u16		i=0,j,num=0;
	u8		arry[3] = {0};
	
	/*��ʼ���*/
	USART1->CR1&=0xffffffdf;    												//���ջ������ǿ��жϽ���
	num = (inBuff[9]<<8)+inBuff[10];											//����Ӧ����led����
	for(j=0;j<MAX_LED_NUM;j++)													//ʵ�ʿ���MAX_LED_NUM��led
	{
		if(i<num)
		{
			LED0_Send_pixel(inBuff+6);
//			LED1_Send_pixel(inBuff+6);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//Ϩ������LED
//			LED1_Send_pixel(arry);
		}
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//���ڻظ����ɹ�
	
	USART1->CR1|=1<<5;    														//���ջ������ǿ��ж�ʹ��
}

/*******************************************************************************
���ƣ�void FullColor1Light(u8 *inBuff)
���ܣ�LTָ��ȫ��ģʽ1����LED��ȫ��ģʽ1�£�ָ���<len>Ϊ4n+6��nΪ����LED������ʵ�ʿ���MAX_LED_NUM����
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void FullColor1Light(u8 *inBuff)
{
	u8		i=0,j,offset;
	u8		arry[3] = {0};
	
	/*��ʼ���*/
	USART1->CR1&=0xffffffdf;    												//���ջ������ǿ��жϽ���
	for(j=0;j<MAX_LED_NUM;j++)													//ʵ�ʿ���MAX_LED_NUM��led
	{
		offset = 5+i*4;															//��һ��Ҫ������LEDλ���ֽڵ�ƫ����
		if(j==inBuff[offset]-1)													//��ȱ�ʾ��ǰLED��Ҫ����
		{
			LED0_Send_pixel(inBuff+offset+1);									//offset+1ΪRGBֵƫ����
//			LED1_Send_pixel(inBuff+offset+1);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//Ϩ������LED
//			LED1_Send_pixel(arry;
		}
		
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//���ڻظ����ɹ�
	
	USART1->CR1|=1<<5;    														//���ջ������ǿ��ж�ʹ��
}

/*******************************************************************************
���ƣ�void FullColor2Light(u8 *inBuff)
���ܣ�LTָ��ȫ��ģʽ2����LED��ȫ��ģʽ2�£�ָ���<len>Ϊ5n+6��nΪ����LED������ʵ�ʿ���MAX_LED_NUM����
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void FullColor2Light(u8 *inBuff)
{
	u8		i=0;
	u16		j,offset;
	u8		arry[3] = {0};
	
	/*��ʼ���*/
	USART1->CR1&=0xffffffdf;    												//���ջ������ǿ��жϽ���	
	for(j=0;j<MAX_LED_NUM;j++)													//ʵ�ʿ���MAX_LED_NUM��led
	{
		offset = 5+i*5;															//��һ��Ҫ������LEDλ���ֽڵ�ƫ����
		if(j==(inBuff[offset]<<8)+inBuff[offset+1]-1)							//��ȱ�ʾ��ǰLED��Ҫ����
		{
			LED0_Send_pixel(inBuff+offset+2);
//			LED1_Send_pixel(inBuff+offset+2);
			i++;
		}
		else
		{
			LED0_Send_pixel(arry);												//Ϩ������LED
//			LED1_Send_pixel(arry;
		}
		
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//���ڻظ����ɹ���
	
	USART1->CR1|=1<<5;    														//���ջ������ǿ��ж�ʹ��
}

/*******************************************************************************
���ƣ�void FullColor2Light(u8 *inBuff)
���ܣ�LTָ����ģʽ����LED�����ģʽ�£�ָ���<len>Ϊ4n+6��nΪ����LED������
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void MultiSegmentLight(u8 *inBuff)
{
	u8		i,j;
	u8		arry[3] = {0};
	u8		numSegment = ((inBuff[3]<<8)+inBuff[4]-6)/4;						//����Ӧ����led����
	
	/*��ʼ���*/
	USART1->CR1&=0xffffffdf;    												//���ջ������ǿ��жϽ���
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
		LED0_Send_pixel(arry);													//Ϩ�����MAX_LED_NUM��LED
	}
	UartReturnCmd(inBuff[2], SUCCESS);											//���ڻظ����ɹ�
	
	USART1->CR1|=1<<5;    														//���ջ������ǿ��ж�ʹ��
}

/*******************************************************************************
���ƣ�void UartReturnCmd(u8 cmd, u8 Rt)
���ܣ�ͨ�����ڻظ�RTָ�
��Σ�u8 cmd��RT��<value>������
���Σ���
���أ���
*******************************************************************************/
void UartReturnCmd(u8 cmd, u8 Rt)
{
	u8		i;
	u8		arry[LEN_RT] = {0x52,0x54,0x00,0x00,0x07,0x00,0x01,0x0d,0x0a};				
	
	PAout(8)=1;																	//485����Ϊ����
	delay_us(200);																//������ʱ����485û�лظ���ȷ������
	
	arry[2] = cmd;
	arry[5] = Rt;																//��֡��cmd��RT��<value>����
	arry[6] = XOR_check(arry, LEN_RT-1-2);										//��֡��XORУ�� -2����Ϊ�������0D0A
	
	for(i=0;i<LEN_RT;i++)														
	{
		USART1->DR=arry[i]; 													//����һ���ֽڵ�����
		while((USART1->SR&0X40)==0);											//�ȴ����ͽ��������SUART1->SR�ĵ�6λ����������ɱ�־TC��			
	}
	
	PAout(8)=0;																	//485����Ϊ������
}

