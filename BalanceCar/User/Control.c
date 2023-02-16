#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "MPU6050.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "Encounter.h"

#define MIDDLE_PITCH 0

float pitch, roll, yaw;	// 当前的小车姿态角
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// 当前小车的角速度(gyroX:角速度方向为x轴)
int16_t motor_Left_Encounter, motor_Right_Encounter;
float motor_Left_Velocity, motor_Right_Velocity;
int Motor_Left_PWM,Motor_Right_PWM;                 

int Balance_PWM, Velocity_PWM;
float Balance_Kp=2000*0.6 ,Balance_Kd=5.1*0.6,Velocity_Kp=300,Velocity_Ki=80,Turn_Kp=4200,Turn_Kd=0;//PID参数（放大100倍）
int Balance_Loop(float pitch_current, float gyrox_current);
int Velocity_Loop(int16_t motor_Left_Encounter_current, int16_t motor_Right_Encounter_current);

void EXTI15_10_IRQHandler(void)			// 利用MPU6050的INT引脚的每5ms产生一次下降沿来触发中断函数的执行
{
	static unsigned int count_5ms = 0;		
	
	if(EXTI_GetITStatus(EXTI_Line12)!=0)// 获取中断标志位
	{
		count_5ms++;
		if(count_5ms==4) 					
		{
			Key_Inspect();			
			count_5ms=0;			
		}
		MPU6050_DMP_GetData(&pitch, &roll, &yaw, &accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);
		motor_Left_Encounter = EnCounter_Left_Get();
		motor_Right_Encounter = EnCounter_Right_Get();
		motor_Left_Velocity = motor_Left_Encounter * 29.9854758;	// 得到的是车轮的线速度,推导见函数内部注释
//		motor_Left_Velocity = Motor_Left_GetVelocity();		
		motor_Right_Velocity = motor_Right_Encounter * 29.9854758;
//		motor_Right_Velocity = Motor_Right_GetVelocity();
		
		Balance_PWM = Balance_Loop(pitch, gyroX);
		Velocity_PWM = Velocity_Loop(motor_Left_Encounter, motor_Right_Encounter);
		Motor_Left_PWM = Balance_PWM + Velocity_PWM;
		Motor_Right_PWM = Balance_PWM + Velocity_PWM;
		
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
	static float pitch_bias, gyrox_bias;
	static int Balance_PWM;
	pitch_bias = MIDDLE_PITCH - pitch_current;
	gyrox_bias = 0 - gyrox_current;
	Balance_PWM=-Balance_Kp*pitch_bias-gyrox_bias*Balance_Kd; //计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	
	return Balance_PWM;
}

int Velocity_Loop(int16_t motor_Left_Encounter_current, int16_t motor_Right_Encounter_current)
{
	static float encounter_Least, encounter_bia = 0.0f;
	static float encounter_Integral;
	
	encounter_Least = (motor_Left_Encounter_current + motor_Right_Encounter_current) - 0;
	encounter_bia *= 0.84;
	encounter_bia += encounter_Least*0.16;
	encounter_Integral += encounter_bia;
	if(encounter_Integral > 10000) 	encounter_Integral = 10000;
	if(encounter_Integral < -10000)	encounter_Integral = -10000;
	
	Velocity_PWM = encounter_bia*Velocity_Kp + encounter_Integral*(Velocity_Kp/200);
	return Velocity_PWM;
}
