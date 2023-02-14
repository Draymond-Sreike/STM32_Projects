#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void PWM_Limit(int* IN,int max,int min);
int Control_GetPWM_Left(void);
int Control_GetPWM_Right(void);

#endif
