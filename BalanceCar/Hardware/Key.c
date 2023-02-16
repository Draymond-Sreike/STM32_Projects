#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Control.h"

unsigned char keynum=0;
unsigned char last_state=0,now_state=0;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//uint8_t Key_GetKeyNum(void)
//{
//	uint8_t KeyNum = 0;
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
//	{
//		Delay_ms(20);
//		for(;GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0;);
//		Delay_ms(20);
//		KeyNum = 1;
//	}
//	return KeyNum;
//}

// ����Delay�������inv_mpu.c�ļ���mpu_reset_fifo�����е�һ�δ��뷢�����ţ�����Delay����ִ��ʱ�������̿�������������������Delayɨ�谴��
// �����ö�ʱ��ɨ�谴�����ο�51�Ĺ����ļ���9-2 �����ʱ��ɨ�谴����

unsigned char Key_GetKeyNum(void)
{			
	unsigned char temp=0;				
	temp=keynum;						
	keynum=0;							
	return temp;
}

unsigned char Key_Num(void)			
{
	unsigned char num=0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){num=1;}
//	if(P3_0==0){num=2;}
//	if(P3_2==0){num=3;}
//	if(P3_3==0){num=4;}
	return num;
}

void Key_Inspect(void)
{
	last_state=now_state;
	now_state=Key_Num();				
	if(last_state==1&&now_state==0)		
	{
		keynum=1;
	}
	if(last_state==2&&now_state==0)
	{
		keynum=2;
	}
	if(last_state==3&&now_state==0)
	{
		keynum=3;
	}
	if(last_state==4&&now_state==0)
	{
		keynum=4;
	}
}

