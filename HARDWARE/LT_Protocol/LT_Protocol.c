#include "LT_Protocol.h"



/*******************************************************************************
���ƣ�void LtCmdAnalysis(u8 *InBuff)
���ܣ�LTָ�����
��Σ���
���Σ���
���أ���
*******************************************************************************/
void LtCmdAnalysis(u8 *InBuff)
{
	
}

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
			TestModLight(inBuff[5]);											//����value�ֶβ���ִ��
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
			//���Իظ�ָ����󣬲�֧�ֵ�ģʽ
			break;
	}
}

/*******************************************************************************
���ƣ�void TestModLight(u8 value)
���ܣ�LTָ�����ģʽ����LED������value�ֶβ���ִ�С�
��Σ�u8 value
���Σ���
���أ���
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
			//���Իظ�ָ����󣬲�֧�ֵ�ģʽ
			break;	
	}
}

/*******************************************************************************
���ƣ�void MonochromeModLight(u8 *inBuff)
���ܣ�LTָ�ɫģʽ����LED��
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void MonochromeModLight(u8 *inBuff)
{
	
}

/*******************************************************************************
���ƣ�void FullColor1Light(u8 *inBuff)
���ܣ�LTָ��ȫ��ģʽ1����LED��
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void FullColor1Light(u8 *inBuff)
{
	
}

/*******************************************************************************
���ƣ�void FullColor2Light(u8 *inBuff)
���ܣ�LTָ��ȫ��ģʽ2����LED��
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void FullColor2Light(u8 *inBuff)
{
	//��
}

/*******************************************************************************
���ƣ�void FullColor2Light(u8 *inBuff)
���ܣ�LTָ����ģʽ����LED��
��Σ�u8 value
���Σ���
���أ���
*******************************************************************************/
void MultiSegmentLight(u8 *inBuff)
{
	
}

