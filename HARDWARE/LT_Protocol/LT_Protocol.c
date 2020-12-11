#include "LT_Protocol.h"


/*******************************************************************************
���ƣ�void UART_IRQ_LTcmd(void)
���ܣ����ڽ���LT����ʱ���жϷ�������
��Σ���
���Σ���
���أ���
*******************************************************************************/
void UART_IRQ_LTcmd(void)
{
	u8 res;	
	u8 i;
	u8 OK[4]={0x4F,0x4B,0x0d,0x0a};		//ok/r/n
	
	//��Э�飬���4�ֽ�
	if(USART1->SR&(1<<5))	//���յ�����
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
					USART_RX_STA|=0x8000;	//��������� 
				}
			}
			else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
				USART_RX_STA++;
				if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
			}
		}  		 									     
	}

	if(USART_RX_STA&0x8000)		//���ڽ������
	{
		PAout(8)=1;		//����
		
		if(USART_RX_BUF[(USART_RX_STA&0X3FFF)-4]==0x31)					//�ж��ǡ�1��
		{
			state=USART_RX_BUF[(USART_RX_STA&0X3FFF)-3]-0x30;			//��ȡstate
			delay_us(200);												//������ʱ����485û�лظ���ȷ������
			for(i=0;i<4;i++)											//�ظ�OK
			{
				USART1->DR=OK[i]; 										//����һ���ֽڵ�����
				while((USART1->SR&0X40)==0);							//�ȴ����ͽ��������SUART1->SR�ĵ�6λ����������ɱ�־TC��			
			}
		}
		USART_RX_STA=0;													//����ձ�־�ͼ���
		memset(USART_RX_BUF,0,USART_REC_LEN);							//���ջ�����0
		
		PAout(8)=0;		//����
	}
}
