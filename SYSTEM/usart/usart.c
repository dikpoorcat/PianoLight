#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ�����ʺ�STM32F10xϵ�У�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
//V1.6�޸�˵�� 20150109
//uart_init����ȥ���˿���PE�ж�
//V1.7�޸�˵�� 20150322
//�޸�OS_CRITICAL_METHOD���ж�Ϊ��SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////




#if EN_USART1_RX   //���ʹ���˽���
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8		USART_RX_BUF[USART_REC_LEN];     										//���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�ָ��ͷLT
//bit13		���յ�ָ��ͷL
//bit12~0��	���յ�����Ч�ֽ���Ŀ 8191
u16		USART_RX_STA=0;       													//����״̬���


/*******************************************************************************
���ƣ�void USART1_IRQHandler(void)
���ܣ�����1�жϷ������
��Σ���
���Σ���
���أ���
*******************************************************************************/
void USART1_IRQHandler(void)
{
	u8				res=0;
	static u16		cmd_len;       												//���ڴ洢��ǰ�����
	
	if(USART1->SR&(1<<5))														//���յ�����
	{	 
		res=USART1->DR; 														
		if(USART_RX_STA&0x4000)													//���ѽ��յ�LT
		{
			/*�ȴ����յ��㹻�ĳ��ȣ��ͱ��Ϊ���ս���*/
			if((USART_RX_STA&0x1FFF)==6)										//��ʱ���յ�ָ���
			{
				cmd_len = (USART_RX_BUF[3]<<8)+USART_RX_BUF[4];					//����Ӧ���ճ��ȣ�ָ��ȣ�
				if(cmd_len>USART_REC_LEN)
				{
					USART_RX_STA=0;												//��������޷����գ����¿�ʼ
					//�Ӹ���־������λ���ظ�һ�� ��������޷�����
					cmd_len = 0;												//����
				}
			}
			if( cmd_len!=0 && ((USART_RX_STA&0x1FFF)+1)>=cmd_len )				//�����յ��㹻�ĳ��ȣ���ʱ�ѽ��յ�<XOR>��+1����ΪUSART_RX_STA��++�ں��棩
			{
				if(res == XOR_check(USART_RX_BUF,cmd_len-1))					//XORУ��
				{
					USART_RX_STA|=0x8000;										//���������
				}
				else
				{
					USART_RX_STA=0;												//XORУ��ʧ�ܣ����¿�ʼ
					//�Ӹ���־������λ���ظ�һ�� XORУ��ʧ��
				}					
			}
		}
		else 																	//��û�յ�LT
		{	
			if(USART_RX_STA&0x2000)												//���ѽ��յ�L
			{
				/*�ȴ�����T*/
				if(res!=0x54) USART_RX_STA=0;									//���մ���L����T�������¿�ʼ
				else
				{
					USART_RX_STA|=0x4000;										//���Ϊ���յ���LT
					USART_RX_BUF[3] = 0;										//��һ��len�ֶΣ���ֹ����
					USART_RX_BUF[4] = 0;										//��һ��len�ֶΣ���ֹ����
				}
			}
			/*�ȴ�����L*/
			else if(res==0x4C) 
			{
				USART_RX_STA=0x2000;											//���Ϊ���յ���L�������ý��ճ���
			}	
		} 		 									     
	}

	USART_RX_BUF[USART_RX_STA&0x1FFF]=res;										//д����ջ���
	USART_RX_STA++; 
	if((USART_RX_STA&0x1FFF)>(USART_REC_LEN-1))
	{
		USART_RX_STA=0;															//������������¿�ʼ
		memset(USART_RX_BUF,0,USART_REC_LEN);									//��ս��ջ���
	}
}
#endif
	
//	u8 res;	
//	u8 i;
//	u8 OK[4]={0x4F,0x4B,0x0d,0x0a};		//ok/r/n
//	
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	
//	//��Э�飬���4�ֽ�
//	if(USART1->SR&(1<<5))	//���յ�����
//	{	 
//		res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//					if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
//					USART_RX_STA|=0x8000;	//��������� 
//				}
//			}
//			else //��û�յ�0X0D
//			{	
//				if(res==0x0d)USART_RX_STA|=0x4000;
//				USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//				USART_RX_STA++;
//				if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//			}
//		}  		 									     
//	}

//	if(USART_RX_STA&0x8000)		//���ڽ������
//	{
//		PAout(8)=1;		//����
//		
//		if(USART_RX_BUF[(USART_RX_STA&0X3FFF)-4]==0x31)					//�ж��ǡ�1��
//		{
//			state=USART_RX_BUF[(USART_RX_STA&0X3FFF)-3]-0x30;			//��ȡstate
//			delay_us(200);												//������ʱ����485û�лظ���ȷ������
//			for(i=0;i<4;i++)											//�ظ�OK
//			{
//				USART1->DR=OK[i]; 										//����һ���ֽڵ�����
//				while((USART1->SR&0X40)==0);							//�ȴ����ͽ��������SUART1->SR�ĵ�6λ����������ɱ�־TC��			
//			}
//		}
//		USART_RX_STA=0;													//����ձ�־�ͼ���
//		memset(USART_RX_BUF,0,USART_REC_LEN);							//���ջ�����0
//		
//		PAout(8)=0;		//����
//	}	
////����������	
////	if(USART1->SR&(1<<5))	//���յ�����
////	{	 
////		res=USART1->DR; 
////		if((USART_RX_STA&0x8000)==0)//����δ���
////		{
////			if(USART_RX_STA&0x4000)//���յ���0x0d
////			{
////				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
////				else USART_RX_STA|=0x8000;	//��������� 
////			}else //��û�յ�0X0D
////			{	
////				if(res==0x0d)USART_RX_STA|=0x4000;
////				else
////				{
////					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
////					USART_RX_STA++;
////					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
////				}		 
////			}
////		}  		 									     
////	}
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 




//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
}
