#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_Left_SetSpeed(int16_t Speed);
void Motor_Right_SetSpeed(int16_t Speed);
float Motor_Left_GetVelocity(void);
float Motor_Right_GetVelocity(void);

#endif
