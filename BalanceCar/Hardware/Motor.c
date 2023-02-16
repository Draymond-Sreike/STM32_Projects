#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Encounter.h"
#include "IOI2C.h"

#define PI 3.14159265
#define WHEEL_DIAMETER 67.0 // 车轮直径

void Motor_Init(void)
{
	// 控制电机正反转的引脚
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

//#define PI 3.14159265							//PI圆周率
//#define Control_Frequency  200.0	//编码器读取频率
//#define Diameter_67  67.0 				//轮子直径67mm 
//#define EncoderMultiples   4.0 		//编码器倍频数
//#define Encoder_precision  13.0 	//编码器精度 13线
//#define Reduction_Ratio  30.0			//减速比30
//#define Perimeter  210.4867 			//周长，单位mm
//void Get_Velocity_Form_Encoder(int encoder_left,int encoder_right)
//{ 	
//	float Rotation_Speed_L,Rotation_Speed_R;						//电机转速  转速=编码器读数（5ms每次）*读取频率/倍频数/减速比/编码器精度
//	Rotation_Speed_L = encoder_left*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
//	Velocity_Left = Rotation_Speed_L*PI*Diameter_67;		//求出编码器速度=转速*周长
//	Rotation_Speed_R = encoder_right*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
//	Velocity_Right = Rotation_Speed_R*PI*Diameter_67;		//求出编码器速度=转速*周长
//}

//#define PI 3.14159265				//PI圆周率
//#define Control_Frequency  200.0	//编码器读取频率
//#define Diameter_67  67.0 		//轮子直径67mm 
//#define EncoderMultiples   4.0 	//编码器倍频数
//#define Encoder_precision  13.0 	//编码器精度 13线
//#define Reduction_Ratio  30.0		//减速比30
//#define Perimeter  210.486708 	//周长，单位mm
//编码器转一圈产生的（编码器）边沿数 = 13.0*4.0*1 = 52.0
//车轮转一圈产生的（编码器）边沿数 = 30.0 * 52.0 = 1560.0
//车轮转速 = 每秒编码器计数/车轮转一圈的编码器计数
//		   = 每5ms编码器计数*200.0/车轮转一圈的编码器计数
//		   = EnCounter_xxxxx_Get()*200.0/1560.0
// 		   = EnCounter_xxxxx_Get()*0.12820513
//车轮速度 = 车轮转速*车轮周长 = EnCounter_xxxxx_Get()*0.12820513*210.486708
//							   = EnCounter_xxxxx_Get()*29.9854758
float Motor_Left_GetVelocity(void)
{ 	
	
	static float velocity = 0.0f;	//车轮速度
	//车轮转速=编码器读数（5ms每次）*读取频率/倍频数/减速比/编码器精度
	//车轮速度=车轮转速*车轮周长
	velocity = EnCounter_Left_Get() * 29.9854758;
	return velocity;
}

float Motor_Right_GetVelocity(void)
{ 	
	static float Speed = 0.0f;
	//车轮转速=编码器读数（5ms每次）*读取频率/倍频数/减速比/编码器精度
	//车轮速度=车轮转速*车轮周长
	Speed = EnCounter_Right_Get() * 26.9584758;
	return Speed;
}
