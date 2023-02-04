#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include "Delay.h"
#include "Motor.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	
	uint8_t KeyNum = 0;
	int8_t Speed = 0; 
	OLED_ShowString(1, 1, "Hello World!");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Speed += 20;
			if(Speed > 100) {Speed = -100;}
		}
		Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(2, 1, Speed, 3);
	}
}
