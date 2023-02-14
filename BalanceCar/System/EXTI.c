#include "stm32f10x.h"                  // Device header

void EXTI_for_MPU6050_INT_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   	//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//使能GPIO端口时钟
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	          	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			      	//根据设定参数初始化GPIO
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 					   	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//4级抢占,4级响应。
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
