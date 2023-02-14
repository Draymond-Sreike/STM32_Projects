#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "MPU6050.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"

#define MIDDLE_PITCH 0

float pitch, roll, yaw;	// ��ǰ��С����̬��
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// ��ǰС���Ľ��ٶ�(gyroX:���ٶȷ���Ϊx��)
float motor_Left_Velocity, motor_Right_Velocity;
int Motor_Left_PWM,Motor_Right_PWM;                 

int Balance_PWM;
float Balance_Kp=22500,Balance_Kd=108,Velocity_Kp=16000,Velocity_Ki=80,Turn_Kp=4200,Turn_Kd=0;//PID�������Ŵ�100����
int Balance_Loop(float pitch_current, float gyrox_current);

void EXTI15_10_IRQHandler(void)			// ����MPU6050��INT���ŵ�ÿ5ms����һ���½����������жϺ�����ִ��
{
	static unsigned int count_5ms = 0;		//?????????+1,???1ms+1

	if(EXTI_GetITStatus(EXTI_Line12)!=0)// ��ȡ�жϱ�־λ
	{
		count_5ms++;
		if(count_5ms==4) 					//20ms?(?1s)???if??
		{
			Key_Inspect();			//???20ms??????
			count_5ms=0;			//????20ms
		}
		MPU6050_DMP_GetData(&pitch, &roll, &yaw, &accX, &accY, &accZ, &gyroY, &gyroX, &gyroZ);
		motor_Left_Velocity = Motor_Left_GetVelocity();		// �õ����ǳ��ֵ����ٶ�
		motor_Right_Velocity = Motor_Right_GetVelocity();
		Balance_PWM = Balance_Loop(pitch, gyroX);
		
		Motor_Left_PWM = Balance_PWM;
		Motor_Right_PWM = Balance_PWM;
		
		PWM_Limit(&Motor_Left_PWM, 6900, -6900);
		PWM_Limit(&Motor_Right_PWM, 6900, -6900);
		
		Motor_Left_SetSpeed(Motor_Left_PWM);
		Motor_Right_SetSpeed(Motor_Right_PWM);

	}
	EXTI->PR = EXTI_Line12;  			// ����жϱ�־λ
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
	Balance_PWM=-Balance_Kp/100*pitch_bias-gyrox_bias*Balance_Kd/100; //����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	
	return Balance_PWM;

}

