#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Encounter.h"
#include "IOI2C.h"

#define PI 3.14159265
#define WHEEL_DIAMETER 67.0 // ����ֱ��

void Motor_Init(void)
{
	// ���Ƶ������ת������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	PWM_Init();
	Encounter_Left_Init();
	Encounter_Right_Init();
}

void Motor_Left_SetSpeed(int16_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		PWM_SetCompare1(Speed);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		PWM_SetCompare1(-Speed);
	}
}

void Motor_Right_SetSpeed(int16_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare4(Speed);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM_SetCompare4(-Speed);
	}
}

//#define PI 3.14159265							//PIԲ����
//#define Control_Frequency  200.0	//��������ȡƵ��
//#define Diameter_67  67.0 				//����ֱ��67mm 
//#define EncoderMultiples   4.0 		//��������Ƶ��
//#define Encoder_precision  13.0 	//���������� 13��
//#define Reduction_Ratio  30.0			//���ٱ�30
//#define Perimeter  210.4867 			//�ܳ�����λmm
//void Get_Velocity_Form_Encoder(int encoder_left,int encoder_right)
//{ 	
//	float Rotation_Speed_L,Rotation_Speed_R;						//���ת��  ת��=������������5msÿ�Σ�*��ȡƵ��/��Ƶ��/���ٱ�/����������
//	Rotation_Speed_L = encoder_left*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
//	Velocity_Left = Rotation_Speed_L*PI*Diameter_67;		//����������ٶ�=ת��*�ܳ�
//	Rotation_Speed_R = encoder_right*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
//	Velocity_Right = Rotation_Speed_R*PI*Diameter_67;		//����������ٶ�=ת��*�ܳ�
//}

//#define PI 3.14159265				//PIԲ����
//#define Control_Frequency  200.0	//��������ȡƵ��
//#define Diameter_67  67.0 		//����ֱ��67mm 
//#define EncoderMultiples   4.0 	//��������Ƶ��
//#define Encoder_precision  13.0 	//���������� 13��
//#define Reduction_Ratio  30.0		//���ٱ�30
//#define Perimeter  210.486708 	//�ܳ�����λmm
//������תһȦ�����ģ��������������� = 13.0*4.0*1 = 52.0
//����תһȦ�����ģ��������������� = 30.0 * 52.0 = 1560.0
//����ת�� = ÿ�����������/����תһȦ�ı���������
//		   = ÿ5ms����������*200.0/����תһȦ�ı���������
//		   = EnCounter_xxxxx_Get()*200.0/1560.0
// 		   = EnCounter_xxxxx_Get()*0.12820513
//�����ٶ� = ����ת��*�����ܳ� = EnCounter_xxxxx_Get()*0.12820513*210.486708
//							   = EnCounter_xxxxx_Get()*29.9854758
float Motor_Left_GetVelocity(void)
{ 	
	
	static float velocity = 0.0f;	//�����ٶ�
	//����ת��=������������5msÿ�Σ�*��ȡƵ��/��Ƶ��/���ٱ�/����������
	//�����ٶ�=����ת��*�����ܳ�
	velocity = EnCounter_Left_Get() * 29.9854758;
	return velocity;
}

float Motor_Right_GetVelocity(void)
{ 	
	static float Speed = 0.0f;
	//����ת��=������������5msÿ�Σ�*��ȡƵ��/��Ƶ��/���ٱ�/����������
	//�����ٶ�=����ת��*�����ܳ�
	Speed = EnCounter_Right_Get() * 26.9584758;
	return Speed;
}
