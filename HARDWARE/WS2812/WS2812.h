#ifndef WS2812_H_
#define WS2812_H_

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"
#include "wdg.h"
#include "string.h"
#include "global.h"

//更换数据输出引脚时只需修改WS2812_init()和以下几行
#define LED0_H 	(GPIOA->BSRR = 0x00000040) 		//PA6置1	快速地对GPIOA的位6进行翻转	LED0控制引脚
#define LED0_L 	(GPIOA->BSRR = 0x00400000)		//PA6置0	快速地对GPIOA的位6进行翻转	LED0控制引脚
#define LED1_H 	(GPIOA->BSRR = 0x00000080) 		//PA7置1	快速地对GPIOA的位7进行翻转	LED1控制引脚
#define LED1_L 	(GPIOA->BSRR = 0x00800000)		//PA7置0	快速地对GPIOA的位7进行翻转	LED1控制引脚
//#define LED0_H 	(GPIOA->BSRR = 0x00000004) 		//PA2置1	快速地对GPIOA的位2进行翻转	LED0控制引脚
//#define LED0_L 	(GPIOA->BSRR = 0x00040000)		//PA2置0	快速地对GPIOA的位2进行翻转	LED0控制引脚
//#define LED1_H 	(GPIOA->BSRR = 0x00000008) 		//PA3置1	快速地对GPIOA的位3进行翻转	LED1控制引脚
//#define LED1_L 	(GPIOA->BSRR = 0x00080000)		//PA3置0	快速地对GPIOA的位3进行翻转	LED1控制引脚
#define LED0	0
#define LED1	1


extern u8 RGB_use0[32][3];
extern u8 RGB_use1[32][3];

extern u8 RGB_red0[25][3];			//顺序为RGB 

extern u8 RGB_luanxie[25][3];		//顺序为RGB
extern u8 RGB_luanxie1[25][3];		//顺序为RGB
extern u8 RGB_black[25][3];		//顺序为RGB 
extern u8 RGB_red[25][3];			//顺序为RGB 
extern u8 RGB_red1[25][3];			//顺序为RGB 
extern u8 RGB_red2[25][3];			//顺序为RGB 
extern u8 RGB_green[25][3];			//顺序为RGB 
//extern u8 RGB_red1[25][3];			//顺序为RGB 
extern u8 RGB_green2[25][3];			//顺序为RGB 
extern u8 RGB_blue[25][3];			//顺序为RGB 
//extern u8 RGB_red1[25][3];			//顺序为RGB 
extern u8 RGB_blue2[25][3];			//顺序为RGB 

extern u8 RGB_zanting1[25][3];			//顺序为RGB 
extern u8 RGB_zanting2[25][3];			//顺序为RGB 



void CreateArray(u8 R,u8 G,u8 B,u32 Mark,u8 Num,u8 * Array);
void Jianmang(u8 R0,u8 G0,u8 B0,u8 R1,u8 G1,u8 B1,u8 Num,u16 time);
void Liushui(u8 R0,u8 G0,u8 B0,u8 R1,u8 G1,u8 B1,u8 Num,u16 time);
void Jianruo(u8 * Array,u8 Num,u16 time);
void Jianruo2(u8 * Array_A,u8 * Array_B,u8 Num,u16 time);
void Huxi(u8 * Array,u8 Num,u16 time);
void Huxi2(u8 * Array_A,u8 * Array_B,u8 Num,u16 time);
void increase(u8 * Array,u8 * Array_n,u16 len,u8 * Array_0);
void reduct(u8 * Array,u8 * Array_n,u8 len);
void WS2812_init(void);
void LED1_Send_8bits(u8 date);
void LED0_Send_8bits(u8 date);
void LED0_Send_pixel(u8 * Array);
void LED1_Send_pixel(u8 * Array);
void Light_LED(u8 ID,u8 * Array,u8 Num);
void Res_WS2812(void);



//---------------------------------------------------------------------------------

#define LED0_CODE1 \
{\
	LED0_L;\
	/*__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");*/\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");/*0.7us*/\
	date=date<<1;/*高位在前*/\
	LED0_H;\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");/*0.6us*/\
}

#define LED0_CODE0 \
{\
	LED0_L;\
	/*__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");*/\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");/*0.35us*/\
	date=date<<1;/*高位在前*/\
	LED0_H;\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");/*0.8us*/\
}

#define LED1_CODE1 \
{\
	LED1_L;\
	/*__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");*/\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");/*0.7us*/\
	date=date<<1;/*高位在前*/\
	LED1_H;\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");/*0.6us*/\
}

#define LED1_CODE0 \
{\
	LED1_L;\
	/*__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");*/\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");/*0.35us*/\
	date=date<<1;/*高位在前*/\
	LED1_H;\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");\
	__ASM("nop");__ASM("nop");__ASM("nop");__ASM("nop");/*0.8us*/\
}

#endif
