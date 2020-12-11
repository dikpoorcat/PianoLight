#include "LT_Protocol.h"



/*******************************************************************************
名称：void LtCmdAnalysis(u8 *InBuff)
功能：LT指令解析
入参：无
出参：无
返回：无
*******************************************************************************/
void LtCmdAnalysis(u8 *InBuff)
{
	
}

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
			TestModLight(inBuff[5]);											//根据value字段参数执行
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
			//可以回复指令错误，不支持的模式
			break;
	}
}

/*******************************************************************************
名称：void TestModLight(u8 value)
功能：LT指令测试模式点亮LED。根据value字段参数执行。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void TestModLight(u8 value)
{
	switch(value)
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
			//可以回复指令错误，不支持的模式
			break;	
	}
}

/*******************************************************************************
名称：void MonochromeModLight(u8 *inBuff)
功能：LT指令单色模式点亮LED。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void MonochromeModLight(u8 *inBuff)
{
	
}

/*******************************************************************************
名称：void FullColor1Light(u8 *inBuff)
功能：LT指令全彩模式1点亮LED。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void FullColor1Light(u8 *inBuff)
{
	
}

/*******************************************************************************
名称：void FullColor2Light(u8 *inBuff)
功能：LT指令全彩模式2点亮LED。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void FullColor2Light(u8 *inBuff)
{
	//略
}

/*******************************************************************************
名称：void FullColor2Light(u8 *inBuff)
功能：LT指令多段模式点亮LED。
入参：u8 value
出参：无
返回：无
*******************************************************************************/
void MultiSegmentLight(u8 *inBuff)
{
	
}

