#ifndef __MY_I2C_H
#define __MY_I2C_H

void My_I2C_Init(void);
void My_I2C_W_SCL(uint8_t BitValue);
void My_I2C_W_SDA(uint8_t BitValue);
uint8_t My_I2C_R_SDA(void);
void My_I2C_Start(void);
void My_I2C_Stop(void);
void My_I2C_SendByte(uint8_t Data);
uint8_t My_I2C_ReceiveByte(void);
void My_I2C_Host_ReceiveAsk(uint8_t AskBit);
uint8_t My_I2C_Slave_ReceiveAsk(void);		

#endif
