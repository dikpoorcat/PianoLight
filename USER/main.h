#ifndef MAIN_H_
#define MAIN_H_

#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "WS2812.h"
#include "wdg.h" 
#include "string.h"
#include "adc.h"
#include "LT_Protocol.h"
#include "global.h"



void GuZhang(void);
void ZanTing(void);
void DaiJi(void);
void ZhengChang(void);
void CongDian(void);
u8 XOR_check(u8 *buffer, u8 num);

#endif
