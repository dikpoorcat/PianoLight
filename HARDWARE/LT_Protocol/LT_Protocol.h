#ifndef LT_PROTOCOL_H_
#define LT_PROTOCOL_H_
#include "sys.h"
#include "stdio.h"
#include "usart.h"

#define	TSET			00														//����ģʽ
#define	MONOCHROME		01														//��ɫģʽ
#define	FULL_COLOR1		02														//ȫ��ģʽ1
#define	FULL_COLOR2		03														//ȫ��ģʽ2
#define	MULTI_SEG		04														//���ģʽ


/*��������*/
void LtCmdLightLed(u8 *inBuff);
void TestModLight(u8 value);
void MonochromeModLight(u8 *inBuff);
void FullColor1Light(u8 *inBuff);
void FullColor2Light(u8 *inBuff);
void MultiSegmentLight(u8 *inBuff);

#endif
