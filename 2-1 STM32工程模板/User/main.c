#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC->APB2ENR = 0x00000010;	// 使能GPIOC时钟
	GPIOC->CRH = 0x00300000;	// 配置PC13模式
	//GPIOC->ODR = 0x00002000;	// 配置PC13输出（高电平）
	GPIOC->ODR = 0x00000000;	// 配置PC13输出（低电平）
	// 灯是低电平点亮的，所以13号口配0（给低电平）就亮，给1就灭
	// 所以ODR全0即亮灯，ODR为0x00002000即灭灯
	
	while(1)
	{
	
	}
}
