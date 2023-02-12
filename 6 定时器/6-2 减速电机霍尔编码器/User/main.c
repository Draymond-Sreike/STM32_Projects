#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encounter.h"
#include "Motor.h"
#include "Key.h"
#include "Timer.h"

float LeftMotor_Count_5ms;
float RightMotor_Count_5ms;

float LeftMotor_Count_s;
float RightMotor_Count_s;

float LeftMotor_round_s;
float RightMotor_round_s;

float LeftMotor_rad_s;
float RightMotor_rad_s;

int main(void)
{
	OLED_Init();
	Encounter_Right_Init();
	Encounter_Left_Init();
	Motor_Init();
	Key_Init();
	Timer_3_Init();
	
	uint8_t KeyNum = 0;
	int8_t Speed = 0; 
	
	OLED_ShowString(1, 1, "Left:");
	OLED_ShowString(2, 1, "Right:");
	OLED_ShowString(1, 12, "rad/s");
	OLED_ShowString(2, 12, "rad/s");
	OLED_ShowString(3, 1, "PWM:");
	OLED_ShowString(3, 10, "%");

	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Speed += 20;
			if(Speed > 100) {Speed = -100;}
		}
		Motor_Left_SetSpeed(Speed);
		Motor_Right_SetSpeed(Speed);
		OLED_ShowSignedNum(3, 5, Speed, 3);
		
		OLED_ShowSignedNum(1,7,LeftMotor_rad_s,3);
		OLED_ShowSignedNum(2,7,RightMotor_rad_s,3);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001)//溢出中断
	{    
		LeftMotor_Count_5ms = EnCounter_Left_Get();
		RightMotor_Count_5ms = EnCounter_Right_Get();
	}
	
	LeftMotor_rad_s = LeftMotor_Count_5ms * 200 / 1560 * 360;
	RightMotor_rad_s = RightMotor_Count_5ms * 200 / 1560 * 360;
	
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
