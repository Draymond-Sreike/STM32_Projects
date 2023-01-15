#include "stm32f10x.h"                  // Device header

int main(void)
{
	// 配置寄存器点灯
//	RCC->APB2ENR = 0x00000010;	// 使能GPIOC时钟
//	GPIOC->CRH = 0x00300000;	// 配置PC13模式
//	//GPIOC->ODR = 0x00002000;	// 配置PC13输出（高电平）
//	GPIOC->ODR = 0x00000000;	// 配置PC13输出（低电平）
//	// 灯是低电平点亮的，所以13号口配0（给低电平）就亮，给1就灭
//	// 所以ODR全0即亮灯，ODR为0x00002000即灭灯
	
	// 库函数点灯
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	while(1)
	{
	
	}
}
