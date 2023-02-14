#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Key.h"
#include "Control.h"
#include "EXTI.h"

//int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// 当前小车的角速度(GyroX:角速度方向为x轴)
float Motor_Left_Velocity, Motor_Right_Velocity;
//float pitch = 0.0f, roll = 0.0f, yaw = 0.0f;

void test_main(void);
void MPU6050_GetData_Test(void);
void MPU6050_DMP_GetData_Test(void);
void Motor_GetSpeed_Test(void);
void Remp_test(void);

int main(void)
{	
	OLED_Init();
	
//	MPU6050_GetData_Test();
//	MPU6050_DMP_GetData_Test();
//	Motor_GetSpeed_Test();
//	Remp_test();
	test_main();
}

void test_main(void)
{
	uint8_t KeyNum = 0;
	int16_t Speed = 0; 
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	Motor_Init();
	Key_Init();
	EXTI_for_MPU6050_INT_Init();
	

	OLED_ShowString(1, 1, "P:");
	OLED_ShowString(1, 7, "PWML:");
	OLED_ShowString(2, 10, "R:");
//	OLED_ShowString(1, 16, "%");
//	OLED_ShowString(2, 16, "%");
	OLED_ShowString(2, 1, "GX:");
	OLED_ShowString(3, 1, "WL:");
	OLED_ShowString(4, 1, "WR:");
	OLED_ShowString(3, 9, "mm/s");
	OLED_ShowString(4, 9, "mm/s");

	
	while(1)
	{
		KeyNum = Key_GetKeyNum();
		if(KeyNum == 1)
		{
			Speed += 20;
			if(Speed > 100) {Speed = -100;}
		}
		Motor_Left_SetSpeed(Speed*7200/100);
		Motor_Right_SetSpeed(Speed*7200/100);
		OLED_ShowSignedNum(1, 12, Control_GetPWM_Left(), 4);
		OLED_ShowSignedNum(2, 12, Control_GetPWM_Right(), 4);
		OLED_ShowSignedNum(1, 3, (int8_t)Control_GetPitch_by_DMP(), 2);
		OLED_ShowSignedNum(2, 4, (int16_t)Control_GetGyroX_by_DMP(), 4);
		OLED_ShowSignedNum(3, 4, (int16_t)Control_GetVelocity_LeftWheel_by_Encounter(), 4);
		OLED_ShowSignedNum(4, 4, (int16_t)Control_GetVelocity_RightWheel_by_Encounter(), 4);
	}	
}

void MPU6050_GetData_Test(void)
{
	static int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// 当前小车的角速度(GyroX:角速度方向为x轴)
	//uint8_t MPU6050_ID;
	MPU6050_Init();
	MPU6050_DMP_Init();
	OLED_ShowString(1, 4, "Acce");
	OLED_ShowString(1, 12, "Gyro");
	
	OLED_ShowString(2, 1, "X:");
	OLED_ShowString(3, 1, "Y:");
	OLED_ShowString(4, 1, "Z:");
	while(1)
	{
		//MPU6050_GetData(&accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);
		
//		MPU6050_DMP_GetData(&pitch, &roll, &yaw, &accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);

		OLED_ShowSignedNum(2, 3, accX, 5);
		OLED_ShowSignedNum(2, 11, gyroX, 5);
		OLED_ShowSignedNum(3, 3, accY, 5);
		OLED_ShowSignedNum(3, 11, gyroY, 5);
		OLED_ShowSignedNum(4, 3, accZ, 5);
		OLED_ShowSignedNum(4, 11, gyroZ, 5);		
	}	
}

void MPU6050_DMP_GetData_Test(void)
{

	MPU6050_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	MPU6050_DMP_Init();
	OLED_ShowString(1, 4, "Angle");
	
	OLED_ShowString(2, 1, "P:");
	OLED_ShowString(3, 1, "R:");
	OLED_ShowString(4, 1, "Y:");

	
	while(1)
	{

//		MPU6050_DMP_GetData(&pitch, &roll, &yaw, &accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);
//		
//		OLED_ShowSignedNum(2, 3, (short)(pitch), 5);
//		OLED_ShowSignedNum(3, 3, (short)(roll), 5);
//		OLED_ShowSignedNum(4, 3, (short)(yaw), 5);	
		

		
//		OLED_ShowSignedNum(2, 3, accX, 5);
//		OLED_ShowSignedNum(2, 11, gyroX, 5);
//		OLED_ShowSignedNum(3, 3, accY, 5);
//		OLED_ShowSignedNum(3, 11, gyroY, 5);
//		OLED_ShowSignedNum(4, 3, accZ, 5);
//		OLED_ShowSignedNum(4, 11, gyroZ, 5);				
	}
}

void Motor_GetSpeed_Test(void)
{
	Motor_Init();
	Key_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();

	//Timer_3_Init();
	EXTI_for_MPU6050_INT_Init();

	uint8_t KeyNum = 0;
	int16_t Speed = 0; 
	
	OLED_ShowString(1, 1, "Left:");
	OLED_ShowString(2, 1, "Right:");
	OLED_ShowString(1, 12, "mm/s");
	OLED_ShowString(2, 12, "mm/s");
	OLED_ShowString(3, 1, "PWM:");
	OLED_ShowString(3, 10, "%");

	while(1)
	{
		KeyNum = Key_GetKeyNum();
		if(KeyNum == 1)
		{
			Speed += 20;
			if(Speed > 100) {Speed = -100;}
		}

		Motor_Left_SetSpeed(Speed*7200/100);
		Motor_Right_SetSpeed(Speed*7200/100);
		OLED_ShowSignedNum(3, 5, Speed, 3);
		OLED_ShowSignedNum(1,7,(int16_t)Control_GetVelocity_LeftWheel_by_Encounter(),4);
		OLED_ShowSignedNum(2,7,(int16_t)Control_GetVelocity_RightWheel_by_Encounter(),4);

	}
}

//void Remp_test(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

//	// ??GPIO
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_SetBits(GPIOA, GPIO_Pin_15);
//	GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//}

//void TIM3_IRQHandler(void)
//{
//	if(TIM3->SR&0X0001)// 判断中断标志位
//	{    
//		Motor_Left_Velocity = Motor_Left_GetVelocity();
//		Motor_Right_Velocity = Motor_Right_GetVelocity();
//	}
//	
//	
//	TIM3->SR&=~(1<<0);// 清除中断标志位 	    
//}

//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line12)!=0)// 获取中断标志位
//	{    
//		Motor_Left_Velocity = Motor_Left_GetVelocity();
//		Motor_Right_Velocity = Motor_Right_GetVelocity();
//	}

//	EXTI->PR = EXTI_Line12;  			// 清除中断标志位
//}
