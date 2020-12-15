#ifndef LT_PROTOCOL_H_
#define LT_PROTOCOL_H_
#include "sys.h"
#include "stdio.h"
#include "usart.h"

#define	MAX_LED_NUM		0xFF													//���LED������Ӱ��ȫ��ģʽ���Զ������һ�ε�����LED���ܣ�

#define	TSET			00														//����ģʽ
#define	MONOCHROME		01														//��ɫģʽ
#define	FULL_COLOR1		02														//ȫ��ģʽ1
#define	FULL_COLOR2		03														//ȫ��ģʽ2
#define	MULTI_SEG		04														//���ģʽ

#define	LEN_RT			7+2														//�ظ�����ĳ���+0x0d0a
#define	SUCCESS			00														//ִ�гɹ�
#define	FAILD			01														//ִ��ʧ��	20201215��δ�õ�
#define	UNSUPPORT		02														//��֧�ֵ�ģʽ	
#define	XORERR			03														//���У��ʧ��
#define	OVERLENGTH		04														//ָ��ȳ���
#define	NOPROGRAM		05														//Ԥ����򲻴���



/*��������*/
void LtCmdLightLed(u8 *inBuff);
void TestModLight(u8 *inBuff);
void MonochromeModLight(u8 *inBuff);
void FullColor1Light(u8 *inBuff);
void FullColor2Light(u8 *inBuff);
void MultiSegmentLight(u8 *inBuff);
void UartReturnCmd(u8 cmd, u8 Rt);

#endif
