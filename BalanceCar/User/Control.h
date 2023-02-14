#ifndef __CONTROL_H
#define __CONTROL_H

float Control_GetPitch_by_DMP(void);
int16_t Control_GetGyroX_by_DMP(void);
float Control_GetVelocity_LeftWheel_by_Encounter(void);
float Control_GetVelocity_RightWheel_by_Encounter(void);
int8_t Delay_5ms_by_MPU6050_INT_EXIT(unsigned int number_5ms);

#endif
