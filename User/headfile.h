#ifndef _headfile_h
#define _headfile_h

#include "stm32f10x.h"                  // Device header

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "uart.h"
#include "MYSPI.h"

#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "ICM42688.h"

extern float pitch;
extern float yaw;
extern float roll;
extern float pitch_acc;
extern float roll_acc;

#endif
