#include "stm32f10x.h"                  // Device header

void Timer_3_Init(void)	// Timer3的定时中断似乎会与Delay函数冲突，所以不要用。
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; 				// AAR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 3600 -1;				// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM3->SR&=~(1<<0);	// 清除因为TImeBaseInit函数执行所置的中断标志位
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}

//void TIM3_IRQHandler(void)
//{
//	if(TIM3->SR&0X0001)//溢出中断
//	{    
//			
//		
//	}
//	
//	TIM3->SR&=~(1<<0);//清除中断标志位 	    
//}
