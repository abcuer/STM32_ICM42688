#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);			/* 初始化MPU6050 SCL、SDA */
void MyI2C_Start(void); 		/* 开启I2C通信 */
void MyI2C_Stop(void);			/* 关闭I2C通信 */
void MyI2C_SendByte(uint8_t Byte);			/* 发送8位字节数据 */
uint8_t MyI2C_ReceiveByte(void);				/* 接收8位字节数据 */
void MyI2C_SendAck(uint8_t AckBit);			/* 接收方发送应答信号来确认接收到信号 */ 
uint8_t MyI2C_ReceiveAck(void);					/* 收取来自接收方的应答 */

#endif
