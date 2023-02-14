#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg_Byte(uint8_t RegAddress, uint8_t Data);	
uint8_t MPU6050_ReadReg_Byte(uint8_t RegAddress);
void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
					 int16_t* Gyro1, int16_t* Gyro2, int16_t* Gyro3);
void MPU6050_DMP_Init(void);
void MPU6050_DMP_GetData(float *pitch, float *roll, float *yaw, 
						  short* accX, short* accY, short* accZ,
						  short* gyro1, short* gyro2, short* gyro3);

#endif
