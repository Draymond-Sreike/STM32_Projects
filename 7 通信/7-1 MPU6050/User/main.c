#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

#include <stdio.h>
int main()
{
	int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
	uint8_t MPU6050_ID;
	
	OLED_Init();
	MPU6050_Init();
	
	MPU6050_ID = MPU6050_GetID();
	OLED_ShowString(1, 4, "Acce");
	OLED_ShowString(1, 12, "Gyro");
	
	printf("1");
	
	OLED_ShowString(2, 1, "X:");
	OLED_ShowString(3, 1, "Y:");
	OLED_ShowString(4, 1, "Z:");
	while(1)
	{
		MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
		
		OLED_ShowSignedNum(2, 3, AccX, 5);
		OLED_ShowSignedNum(2, 11, GyroX, 5);
		OLED_ShowSignedNum(3, 3, AccY, 5);
		OLED_ShowSignedNum(3, 11, GyroY, 5);
		OLED_ShowSignedNum(4, 3, AccZ, 5);
		OLED_ShowSignedNum(4, 11, GyroZ, 5);		
	}
}
