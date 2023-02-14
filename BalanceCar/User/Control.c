#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "MPU6050.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"

#define MIDDLE_PITCH 0

float pitch, roll, yaw;	// 当前的小车姿态角
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// 当前小车的角速度(gyroX:角速度方向为x轴)
float motor_Left_Velocity, motor_Right_Velocity;
int Motor_Left_PWM,Motor_Right_PWM;                 

int Balance_PWM;
float Balance_Kp=22500,Balance_Kd=108,Velocity_Kp=16000,Velocity_Ki=80,Turn_Kp=4200,Turn_Kd=0;//PID参数（放大100倍）
int Balance_Loop(float pitch_current, float gyrox_current);

void EXTI15_10_IRQHandler(void)			// 利用MPU6050的INT引脚的每5ms产生一次下降沿来触发中断函数的执行
{
	static unsigned int count_5ms = 0;		//?????????+1,???1ms+1

	if(EXTI_GetITStatus(EXTI_Line12)!=0)// 获取中断标志位
	{
		count_5ms++;
		if(count_5ms==4) 					//20ms?(?1s)???if??
		{
			Key_Inspect();			//???20ms??????
			count_5ms=0;			//????20ms
		}
		MPU6050_DMP_GetData(&pitch, &roll, &yaw, &accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);
		motor_Left_Velocity = Motor_Left_GetVelocity();		// 得到的是车轮的线速度
		motor_Right_Velocity = Motor_Right_GetVelocity();
		Balance_PWM = Balance_Loop(pitch, gyroX);
		
		Motor_Left_PWM = Balance_PWM;
		Motor_Right_PWM = Balance_PWM;
		
		PWM_Limit(&Motor_Left_PWM, 6900, -6900);
		PWM_Limit(&Motor_Right_PWM, 6900, -6900);
		
		Motor_Left_SetSpeed(Motor_Left_PWM);
		Motor_Right_SetSpeed(Motor_Right_PWM);

	}
	EXTI->PR = EXTI_Line12;  			// 清除中断标志位
}


float Control_GetPitch_by_DMP(void)
{
	return pitch;
}

int16_t Control_GetGyroX_by_DMP(void)
{
	return gyroX;
}

float Control_GetVelocity_LeftWheel_by_Encounter(void)
{
	return motor_Left_Velocity;
}

float Control_GetVelocity_RightWheel_by_Encounter(void)
{
	return motor_Right_Velocity;
}

int Control_GetPWM_Left(void)
{
	return Motor_Left_PWM;
}

int Control_GetPWM_Right(void)
{
	return Motor_Right_PWM;
}

int Balance_Loop(float pitch_current, float gyrox_current)
{
	float pitch_bias, gyrox_bias;
	int Balance_PWM;
	pitch_bias = MIDDLE_PITCH - pitch_current;
	gyrox_bias = 0 - gyrox_current;
	Balance_PWM=-Balance_Kp/100*pitch_bias-gyrox_bias*Balance_Kd/100; //计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	
	return Balance_PWM;

}

