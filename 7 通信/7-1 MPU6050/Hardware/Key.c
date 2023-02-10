#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void key_Init()
{
	//以下该函数体部分的操作解释参考LED.c中的解析
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIOB_InitStructure;
	
	//设置IO口为上拉输入模式
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIOB_InitStructure.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_11);
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIOB_InitStructure);
}

uint8_t key_GetKeyNum()
{
	//变量keyFlag作为某一个按键按下标志
	//0表示按键未按下，1表示按键1按下，2表示按键2按下
	uint8_t keyFlag = 0;
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)	//如果按键1按下
	{
		
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		//以上为代码消除按键抖动的操作
		
		keyFlag = 1;
		//证明按键1按下
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)	//如果按键2按下
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		
		keyFlag = 2;
		//证明按键2按下
	}

	return keyFlag;
}
