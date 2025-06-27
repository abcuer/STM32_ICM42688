#include "headfile.h"
// 角度单位统一用度
#define ALPHA 0.98f
#define RAD2DEG 57.2957795131f
// 静态变量用于单位换算
float icm42688_iic_acc_inv = 1.0f;
float icm42688_iic_gyro_inv = 1.0f;
float pitch = 0, roll = 0, yaw = 0;
float pitch_acc = 0, roll_acc = 0;
// 互补滤波结构体

void ICM42688_WriteReg(uint8_t reg, uint8_t data)
{
    MySPI_Start();                        // 拉低 CS 开始通信
    MySPI_SwapByte(reg & 0x7F);          // 寄存器地址（最高位为0，表示写）
    MySPI_SwapByte(data);                // 要写入的数据
    MySPI_Stop();                         // 拉高 CS 结束通信
}

uint8_t ICM42688_ReadReg(uint8_t reg)
{
    uint8_t data;

    MySPI_Start();
    MySPI_SwapByte(reg | 0x80); // 最高位=1 表示读操作
    data = MySPI_SwapByte(0xFF); // 发送dummy byte读取
    MySPI_Stop();

    return data;
}

void ICM42688_Init(void)
{
	MySPI_Init();
	
    ICM42688_WriteReg(ICM42688_REG_BANK_SEL, 0x00);        // 切换到Bank0
    ICM42688_WriteReg(ICM42688_PWR_MGMT0, 0x0F);            // 加速度和陀螺仪打开
    ICM42688_WriteReg(ICM42688_GYRO_CONFIG0, 0x03);         // 1000Hz，2000dps
    ICM42688_WriteReg(ICM42688_ACCEL_CONFIG0, 0x03);        // 1000Hz，16g
}

uint8_t ICM42688_GetID(void)
{
    ICM42688_WriteReg(ICM42688_REG_BANK_SEL, 0x00);  // 确保处于 Bank0
    return ICM42688_ReadReg(ICM42688_WHO_AM_I);      // 读取 WHO_AM_I（0x75）
}

int16_t ICM42688_Read16(uint8_t regH)
{
    uint8_t high = ICM42688_ReadReg(regH);
    uint8_t low  = ICM42688_ReadReg(regH + 1);
    return (int16_t)((high << 8) | low);
}

void ICM42688_GetAccelGyro(int16_t* ax, int16_t* ay, int16_t* az,
                           int16_t* gx, int16_t* gy, int16_t* gz)
{
	*ax = ICM42688_Read16(ICM42688_ACCEL_DATA_X1);
	*ay = ICM42688_Read16(ICM42688_ACCEL_DATA_Y1);
	*az = ICM42688_Read16(ICM42688_ACCEL_DATA_Z1);

	*gx = ICM42688_Read16(ICM42688_GYRO_DATA_X1);
	*gy = ICM42688_Read16(ICM42688_GYRO_DATA_Y1);
	*gz = ICM42688_Read16(ICM42688_GYRO_DATA_Z1);
}

void update_orientation(int16_t gx, int16_t gy, int16_t gz, int16_t ax, int16_t ay, int16_t az, float dt)
{
	ICM42688_GetAccelGyro(&ax, &ay, &az, &gx, &gy, &gz);

    // 1. 计算加速度计估计的角度
    pitch_acc = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD2DEG;
    roll_acc = atan2f(ay, az) * RAD2DEG;
	float gx_dps = gx * icm42688_iic_gyro_inv;  // 转成度/秒
	
    // 2. 陀螺仪积分
//    pitch += gx * dt;
	pitch += gx_dps * dt;
    roll  += gy * dt;
    yaw   += gz * dt;

    // 3. 互补滤波融合，加速度计修正陀螺仪漂移
    pitch = pitch * ALPHA + pitch_acc * (1 - ALPHA);
    roll  = roll  * ALPHA + roll_acc  * (1 - ALPHA);
}
