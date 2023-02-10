#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// 下面的宏定义暂时没有上
#define MY_I2C_SCL_PORT		GPIOB
#define MY_I2C_SDA_PORT		GPIOB
#define MY_I2C_SCL_PIN		GPIO_Pin_8
#define MY_I2C_SDA_PIN		GPIO_Pin_9
#define MY_I2C_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)x)
#define MY_I2C_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)x)
#define MY_I2C_R_SDA(x)		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define DELAY_US(y)			Delay_us(y)

void My_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);	// 保证初始化后SDA和SCL处于释放/空闲状态
}

void My_I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);
	Delay_us(10);	//放缓单片机IO口电平翻转速度，让MPU6050能跟得上
}

void My_I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
	Delay_us(10);	
}

uint8_t My_I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
	Delay_us(10);
	return BitValue;
}

void My_I2C_Start(void)
{
	My_I2C_W_SDA(1);
	My_I2C_W_SCL(1);
	My_I2C_W_SDA(0);	// SDA此下降沿瞬间将产生起始条件
	My_I2C_W_SCL(0);	// 拉低SCL便于后续时序单元的拼接
}

void My_I2C_Stop(void)
{
	//My_I2C_W_SCL(0);
	My_I2C_W_SDA(0);
	My_I2C_W_SCL(1);
	My_I2C_W_SDA(1);	// SDA此上升沿瞬间将产生终止条件
}

void My_I2C_SendByte(uint8_t Data)
{
	//static uint8_t i = 0;
	uint8_t i;
	//My_I2C_W_SCL(0);
	for(i = 0; i < 8; i++)
	{
		My_I2C_W_SDA(Data & (0x80 >> i));
		My_I2C_W_SCL(1);
		My_I2C_W_SCL(0);
	}
}

uint8_t My_I2C_ReceiveByte(void)
{
	uint8_t i,Data = 0x00;
	//My_I2C_W_SCL(0);
	My_I2C_W_SDA(1);	// 释放SDA控制权给从机
	for(i = 0; i < 8; i++)
	{
		My_I2C_W_SCL(1);								// 从机将在此SCL上升沿瞬间将数据放到SDA上
		if(My_I2C_R_SDA() == 1){Data |= (0x80 >> i);}	// 主机读取从机放在SDA上的数据
		My_I2C_W_SCL(0);
	}
	return Data;
}

void My_I2C_Host_ReceiveAsk(uint8_t AskBit)	// 发送应答：从机给主机发送数据，主机接收后的应答
{
	//My_I2C_W_SCL(0);
	My_I2C_W_SDA(AskBit);
	My_I2C_W_SCL(1);
	My_I2C_W_SCL(0);
}

uint8_t My_I2C_Slave_ReceiveAsk(void)		// 接收应答：从机接收主机发来的数据，接收后的应答
{
	uint8_t AskBit = 0x00;
	//My_I2C_W_SCL(0);
	My_I2C_W_SDA(1);						// 释放SDA控制权给从机
	My_I2C_W_SCL(1);						// 从机将在此SCL上升沿瞬间将数据放到SDA上
	AskBit = My_I2C_R_SDA();				// 主机读取从机放在SDA上的数据	//if(My_I2C_R_SDA() == 1){AskBit = 1;}
	My_I2C_W_SCL(0);
	return AskBit;
}
