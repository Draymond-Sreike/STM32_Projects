#include "stm32f10x.h"                  // Device header

void LED_Init()
{
	/*
	 * 作用：使能外设时钟
	 * 注意：这个外设时钟针对GPIOA：RCC_APB2Periph_GPIOA
	 **/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//根据库函数调用需要，定义一个经过typedef过的结构体类型：GPIO_InitTypeDef 的变量
	//变量名为：GPIOA_InitStructure
	GPIO_InitTypeDef GPIOA_InitStructure;
	
	//设置这个结构体变量中的相应数据
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOA_InitStructure.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_2);
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//初始化GPIO
	//注意：1.该初始化针对GPIOA（在该函数的第1个变量中体现）
	//		2.该初始化函数需要填入上面定义的结构体变量的地址（在该函数的第2个变量中体现）
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	//以上初始化IO口的工作完成后，IO口会自动置于低电平状态
	
	//设置IO口为高电平，即熄灭LED灯
	GPIO_SetBits(GPIOA, (GPIO_Pin_1 |GPIO_Pin_2));
}

void ON_LED1()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void OFF_LED1()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
void ON_LED2()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void OFF_LED2()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

//用于改变LED1的引脚电平状态
void turn_LED1()
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);		
	}
}

//用于改变LED2的引脚电平状态
void turn_LED2()
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);		
	}
}

