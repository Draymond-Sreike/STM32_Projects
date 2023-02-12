#include "stm32f10x.h"                  // Device header

void PWM_Init(void)	// TIM1的CH1和CH4输出PWM
{
	// 配置定时器
	//   时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//   选择内部时钟
	TIM_InternalClockConfig(TIM1);	// 程序默认就是选择内部时钟，这一句写于不写均可
	
	//   配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; 				// AAR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 200 -1;				// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	//   配置输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;								// CCR
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	// 配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能 这一句没有的话无法输出PWM（高级定时器要写，通用定时器不用）

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能（注释后效果不变）
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能（注释后效果不变）	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器（注释后效果不变）
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}

void PWM_SetCompare1(uint16_t Compare)	// 设置TIM1_CH1的输出比较寄存器
{
	
	TIM1->CCR1 = Compare;	
}

void PWM_SetCompare4(uint16_t Compare)	// 设置TIM1_CH4的输出比较寄存器
{
	
	TIM1->CCR4 = Compare;
}
