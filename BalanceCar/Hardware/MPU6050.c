#include "stm32f10x.h"                  // Device header
#include "My_I2C.h"
#include "MPU6050_Reg.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "IOI2C.h"
#define MPU6050_ADDRESS		0xD0		// 这个0xD0包含了写标志位

#define DEFAULT_MPU_HZ  (200)
#define q30  1073741824.0f

short gyro[3], accel[3], sensors;

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
//	IIC_Init();
//	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_YGYRO);
//    IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS_2000);
//    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS_2);
//    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1);
//    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, 1);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 1);

	My_I2C_Init();
	MPU6050_WriteReg_Byte(MPU6050_PWR_MGMT_1, 0x80);
	
	MPU6050_WriteReg_Byte(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg_Byte(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg_Byte(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg_Byte(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg_Byte(0X37,0X80);	//INT

	MPU6050_WriteReg_Byte(MPU6050_GYRO_CONFIG, 0x18);	// 陀螺仪选择最大量程±2000°/s (32767/2000≈16.4)
	MPU6050_WriteReg_Byte(MPU6050_ACCEL_CONFIG, 0x00);	// 加速度计选择±2g
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg_Byte(MPU6050_WHO_AM_I);
}

//void MPU6050_GetData(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
//					 int16_t* Gyro1, int16_t* Gyro2, int16_t* Gyro3)
//{// Gyro1: 角速度方向为Y轴方向，即绕X轴旋转
// // Gyro2: 角速度方向为X轴方向，即绕Y轴旋转
// // Gyro3: 绕Z轴旋转
//	uint8_t Data_H, Data_L;
//	
//	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_XOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_XOUT_L);
//	*AccX = (Data_H << 8) | Data_L;
//	
//	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_YOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_YOUT_L);
//	*AccY = (Data_H << 8) | Data_L;

//	Data_H = MPU6050_ReadReg_Byte(MPU6050_ACCEL_ZOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_ACCEL_ZOUT_L);
//	*AccZ = (Data_H << 8) | Data_L;
//	
//	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_XOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_XOUT_L);
//	*Gyro1 = (Data_H << 8) | Data_L;
//	
//	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_YOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_YOUT_L);
//	*Gyro2 = (Data_H << 8) | Data_L;

//	Data_H = MPU6050_ReadReg_Byte(MPU6050_GYRO_ZOUT_H);
//	Data_L = MPU6050_ReadReg_Byte(MPU6050_GYRO_ZOUT_L);
//	*Gyro3 = (Data_H << 8) | Data_L;
//}

// 用在DMP初始化的、官方提供的函数
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

// 用在DMP初始化的、官方提供的变量
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

// 用在DMP初始化的、官方提供的函数
static  unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

// 用在DMP初始化的、官方提供的函数
static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		//printf("setting bias succesfully ......\r\n");
    }
}

void MPU6050_DMP_Init(void)
{
//	
//	// 以下参考官方的函数进行简化配置（没有直接看官方的函数，看的是WHEELTEC写的，它们应该是看官方函数写的...）
//	mpu_init();
	if(!mpu_init())
	{
		mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
		mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
		mpu_set_sample_rate(DEFAULT_MPU_HZ);
		dmp_load_motion_driver_firmware();
		dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
		dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
			  DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			  DMP_FEATURE_GYRO_CAL);
		dmp_set_fifo_rate(DEFAULT_MPU_HZ);
		run_self_test();
		mpu_set_dmp_state(1);
	}
}

void MPU6050_DMP_GetData(float *pitch, float *roll, float *yaw, 
						  short* accX, short* accY, short* accZ,
						  short* gyro1, short* gyro2, short* gyro3)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	unsigned char more;
	long quat[4];
	
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);	// 获取DMP数据
	
	if (sensors & INV_WXYZ_QUAT)
	{	// 四元数
		q0=quat[0] / q30;
		q1=quat[1] / q30;
		q2=quat[2] / q30;
		q3=quat[3] / q30; 	
		
		*pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; 										// 俯仰角
		*roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; 		// 横滚角
		*yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;	// 航向角			
		*accX = accel[0];
		*accY = accel[1];
		*accZ = accel[2];		
		*gyro1 = gyro[0];
		*gyro2 = gyro[1];
		*gyro3 = gyro[2];
	}
}