#ifndef LT_PROTOCOL_H_
#define LT_PROTOCOL_H_
#include "sys.h"
#include "stdio.h"
#include "usart.h"

#define	MAX_LED_NUM		0xFF													//最大LED数量（影响全彩模式中自动清除上一次点亮的LED功能）

#define	TSET			00														//测试模式
#define	MONOCHROME		01														//单色模式
#define	FULL_COLOR1		02														//全彩模式1
#define	FULL_COLOR2		03														//全彩模式2
#define	MULTI_SEG		04														//多段模式

#define	LEN_RT			7+2														//回复命令的长度+0x0d0a
#define	SUCCESS			00														//执行成功
#define	FAILD			01														//执行失败	20201215暂未用到
#define	UNSUPPORT		02														//不支持的模式	
#define	XORERR			03														//异或校验失败
#define	OVERLENGTH		04														//指令长度超出
#define	NOPROGRAM		05														//预设程序不存在



/*函数声明*/
void LtCmdLightLed(u8 *inBuff);
void TestModLight(u8 *inBuff);
void MonochromeModLight(u8 *inBuff);
void FullColor1Light(u8 *inBuff);
void FullColor2Light(u8 *inBuff);
void MultiSegmentLight(u8 *inBuff);
void UartReturnCmd(u8 cmd, u8 Rt);

#endif
