#ifndef __CONTROL_H
#define __CONTROL_H

float Control_GetPitch_by_DMP(void);
int16_t Control_GetGyroX_by_DMP(void);
float Control_GetVelocity_LeftWheel_by_Encounter(void);
float Control_GetVelocity_RightWheel_by_Encounter(void);
int Control_GetPWM_Left(void);
int Control_GetPWM_Right(void);

#endif
