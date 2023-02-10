#include "stm32f10x.h"                  // Device header
#include "My_I2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0		// 这个0xD0包含了写标志位

void MPU6050_WriteReg_Byte(uint8_t RegAddress, uint8_t Data)	// 指定寄存器写
{
	My_I2C_Start();
	My_I2C_SendByte(MPU6050_ADDRESS);
	My_I2C_Slave_ReceiveAsk();
	My_I2C_SendByte(RegAddress);
	My_I2C_Slave_ReceiveAsk();
	My_I2C_SendByte(Data);
	My_I2C_Slave_ReceiveAsk();
	My_I2C_Stop();
}

uint8_t MPU6050_ReadReg_Byte(uint8_t RegAddress)
{
	uint8_t Data;
	
	My_I2C_Start();
	My_I2C_SendByte(MPU6050_ADDRESS);			// 易错，不是在这里将写标志更换为读标志，这里仍然要用写标志			
	My_I2C_Slave_ReceiveAsk();
	My_I2C_SendByte(RegAddress);
	My_I2C_Slave_ReceiveAsk();
	
	My_I2C_Start();
	My_I2C_SendByte(MPU6050_ADDRESS | 0x01);	//	将写标志更换为读标志
	My_I2C_Slave_ReceiveAsk();
	Data = My_I2C_ReceiveByte();
	My_I2C_Host_ReceiveAsk(1);
	My_I2C_Stop();
	
	return Data;
}



void MPU6050_Init(void)
{
	My_I2C_Init();
	
	MPU6050_WriteReg_Byte(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg_Byte(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg_Byte(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg_Byte(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg_Byte(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg_Byte(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg_Byte(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
					 int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	uint8_t Data_H, Data_L;
	
	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_XOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_XOUT_L);
	*AccX = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_YOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_YOUT_L);
	*AccY = (Data_H << 8) | Data_L;

	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_ZOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_XOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_XOUT_L);
	*GyroX = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_YOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_YOUT_L);
	*GyroY = (Data_H << 8) | Data_L;

	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_ZOUT_H);
	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (Data_H << 8) | Data_L;
}
