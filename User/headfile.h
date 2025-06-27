#ifndef _headfile_h
#define _headfile_h

#include "stm32f10x.h"                  // Device header

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#include "delay.h"
#include "sys.h"

#include "OLED.h"
#include "ICM42688.h"
#include "Mahony.h"

extern float icm42688_iic_acc_inv;
extern float icm42688_iic_gyro_inv;

extern float pitch;
extern float yaw;
extern float roll;
extern float pitch_acc;
extern float roll_acc;

#endif
