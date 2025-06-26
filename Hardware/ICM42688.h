#ifndef _ICM42688_h
#define _ICM42688_h

#include "MyI2C.h"
#include "ICM42688_Reg.h"

void ICM42688_Init(void);
void ICM42688_WriteReg(uint8_t reg, uint8_t data);
uint8_t ICM42688_ReadReg(uint8_t reg);
void ICM42688_Write_Data(uint8_t addr, uint8_t reg, uint8_t dat);
bool ICM42688_CheckID(void);
void Set_LowpassFilter_Range_ICM42688_IIC(enum icm42688_afs afs,
                                          enum icm42688_aodr aodr,
                                          enum icm42688_gfs gfs,
                                          enum icm42688_godr godr);
uint8_t ICM42688_GetID(void);
int16_t ICM42688_Read16(uint8_t regH);
void ICM42688_GetAccelGyro(int16_t* ax, int16_t* ay, int16_t* az,
                           int16_t* gx, int16_t* gy, int16_t* gz);
void update_orientation(int16_t gx, int16_t gy, int16_t gz, int16_t ax, int16_t ay, int16_t az, float dt);

#endif
