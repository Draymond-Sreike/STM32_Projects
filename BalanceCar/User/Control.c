#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "MPU6050.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "Encounter.h"

#define MIDDLE_PITCH 0

float pitch, roll, yaw;	// ��ǰ��С����̬��
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;	// ��ǰС���Ľ��ٶ�(gyroX:���ٶȷ���Ϊx��)
int16_t motor_Left_Encounter, motor_Right_Encounter;
float motor_Left_Velocity, motor_Right_Velocity;
int Motor_Left_PWM,Motor_Right_PWM;                 

int Balance_PWM, Velocity_PWM;
float Balance_Kp=2000*0.6 ,Balance_Kd=5.1*0.6,Velocity_Kp=300,Velocity_Ki=80,Turn_Kp=4200,Turn_Kd=0;//PID�������Ŵ�100����
int Balance_Loop(float pitch_current, float gyrox_current);
int Velocity_Loop(int16_t motor_Left_Encounter_current, int16_t motor_Right_Encounter_current);

void EXTI15_10_IRQHandler(void)			// ����MPU6050��INT���ŵ�ÿ5ms����һ���½����������жϺ�����ִ��
{
	static unsigned int count_5ms = 0;		
	
	if(EXTI_GetITStatus(EXTI_Line12)!=0)// ��ȡ�жϱ�־λ
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
		motor_Left_Velocity = motor_Left_Encounter * 29.9854758;	// �õ����ǳ��ֵ����ٶ�,�Ƶ��������ڲ�ע��
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
	static float pitch_bias, gyrox_bias;
	static int Balance_PWM;
	pitch_bias = MIDDLE_PITCH - pitch_current;
	gyrox_bias = 0 - gyrox_current;
	Balance_PWM=-Balance_Kp*pitch_bias-gyrox_bias*Balance_Kd; //����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	
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
