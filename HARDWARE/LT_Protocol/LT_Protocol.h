#ifndef LT_PROTOCOL_H_
#define LT_PROTOCOL_H_
#include "sys.h"
#include "stdio.h"
#include "usart.h"

#define	TSET			00														//测试模式
#define	MONOCHROME		01														//单色模式
#define	FULL_COLOR1		02														//全彩模式1
#define	FULL_COLOR2		03														//全彩模式2
#define	MULTI_SEG		04														//多段模式


/*函数声明*/
void LtCmdLightLed(u8 *inBuff);
void TestModLight(u8 value);
void MonochromeModLight(u8 *inBuff);
void FullColor1Light(u8 *inBuff);
void FullColor2Light(u8 *inBuff);
void MultiSegmentLight(u8 *inBuff);

#endif
