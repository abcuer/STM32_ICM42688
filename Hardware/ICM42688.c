#include "headfile.h"

#define ALPHA 0.98f
#define RAD2DEG 57.2957795131f
// 静态变量用于单位换算
float icm42688_iic_acc_inv = 1.0f;
float icm42688_iic_gyro_inv = 1.0f;
static uint8_t ICM42688_GYROSCOPE_DEV_ADDR = 0x00;
float pitch = 0, roll = 0, yaw = 0;
float pitch_acc = 0, roll_acc = 0;

void ICM42688_WriteReg(uint8_t reg, uint8_t data)
{
    MyI2C_Start();
    MyI2C_SendByte(ICM42688_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(reg);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(data);
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

uint8_t ICM42688_ReadReg(uint8_t reg)
{
    uint8_t data;
    MyI2C_Start();
    MyI2C_SendByte(ICM42688_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(reg);
    MyI2C_ReceiveAck();
    
    MyI2C_Start();
    MyI2C_SendByte(ICM42688_ADDRESS | 0x01);
    MyI2C_ReceiveAck();
    data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    MyI2C_Stop();
    
    return data;
}

// 寄存器：addr 是设备地址，reg 是寄存器地址，dat 是写入数据
void ICM42688_Write_Data(uint8_t addr, uint8_t reg, uint8_t dat)
{
    MyI2C_Start();
    MyI2C_SendByte(addr << 1);        // 写模式
    if (MyI2C_ReceiveAck()) goto stop;

    MyI2C_SendByte(reg);              // 寄存器地址
    if (MyI2C_ReceiveAck()) goto stop;

    MyI2C_SendByte(dat);              // 数据
    MyI2C_ReceiveAck();

stop:
    MyI2C_Stop();
}

// 1. 设备ID检查与地址配置
bool ICM42688_CheckID(void)
{
    uint8_t id = ICM42688_GetID();
    if (id == 0x47)
    {
        ICM42688_GYROSCOPE_DEV_ADDR = 0x69;
        return true;
    }
    else
    {
        OLED_ShowString(1, 1, "ICM ERR");
		return false;
	}
}

// 2. 主配置函数
void Set_LowpassFilter_Range_ICM42688_IIC(enum icm42688_afs afs,
                                          enum icm42688_aodr aodr,
                                          enum icm42688_gfs gfs,
                                          enum icm42688_godr godr)
{
    // 确保ID检查通过
    if (!ICM42688_CheckID()) return;

    // -------- 加速度计配置 --------
    uint8_t accel_cfg = ((uint8_t)afs << 5) | ((uint8_t)aodr & 0x1F);
    ICM42688_Write_Data(ICM42688_GYROSCOPE_DEV_ADDR, ICM42688_ACCEL_CONFIG0, accel_cfg);

    // -------- 陀螺仪配置 --------
    uint8_t gyro_cfg = ((uint8_t)gfs << 5) | ((uint8_t)godr & 0x1F);
    ICM42688_Write_Data(ICM42688_GYROSCOPE_DEV_ADDR, ICM42688_GYRO_CONFIG0, gyro_cfg);

    // -------- 单位转换系数 --------
    switch (afs)
    {
        case ICM42688_AFS_2G:    icm42688_iic_acc_inv = 2000.0f / 32768.0f; break;
        case ICM42688_AFS_4G:    icm42688_iic_acc_inv = 4000.0f / 32768.0f; break;
        case ICM42688_AFS_8G:    icm42688_iic_acc_inv = 8000.0f / 32768.0f; break;
        case ICM42688_AFS_16G:   icm42688_iic_acc_inv = 16000.0f / 32768.0f; break;
        default:                 icm42688_iic_acc_inv = 1.0f; break;
    }

    switch (gfs)
    {
        case ICM42688_GFS_15_625DPS:  icm42688_iic_gyro_inv = 15.625f / 32768.0f; break;
        case ICM42688_GFS_31_25DPS:   icm42688_iic_gyro_inv = 31.25f / 32768.0f; break;
        case ICM42688_GFS_62_5DPS:    icm42688_iic_gyro_inv = 62.5f / 32768.0f; break;
        case ICM42688_GFS_125DPS:     icm42688_iic_gyro_inv = 125.0f / 32768.0f; break;
        case ICM42688_GFS_250DPS:     icm42688_iic_gyro_inv = 250.0f / 32768.0f; break;
        case ICM42688_GFS_500DPS:     icm42688_iic_gyro_inv = 500.0f / 32768.0f; break;
        case ICM42688_GFS_1000DPS:    icm42688_iic_gyro_inv = 1000.0f / 32768.0f; break;
        case ICM42688_GFS_2000DPS:    icm42688_iic_gyro_inv = 2000.0f / 32768.0f; break;
        default:                      icm42688_iic_gyro_inv = 1.0f; break;
    }
}

void ICM42688_Init(void)
{
    MyI2C_Init();

    // 选择Bank 0
    ICM42688_WriteReg(0x76, 0x00);       // 选择Bank0（非常关键）
    ICM42688_WriteReg(0x4E, 0x0F);       // PWR_MGMT0，启动加速度计和陀螺仪
    ICM42688_WriteReg(0x4F, 0x06);       // GYRO_CONFIG0：1kHz，±2000dps
    ICM42688_WriteReg(0x50, 0x06);       // ACCEL_CONFIG0：1kHz，±16g
	
//	Set_LowpassFilter_Range_ICM42688_IIC(
//		ICM42688_AFS_4G,
//		ICM42688_AODR_1000HZ,
//		ICM42688_GFS_500DPS,
//		ICM42688_GODR_1000HZ
//	);
	
	Set_LowpassFilter_Range_ICM42688_IIC(
		ICM42688_AFS_16G,
		ICM42688_AODR_1000HZ,
		ICM42688_GFS_2000DPS,
		ICM42688_GODR_1000HZ
	);

}

uint8_t ICM42688_GetID(void)
{
    return ICM42688_ReadReg(0x75);  // WHO_AM_I 地址为 0x75
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
    *ax = ICM42688_Read16(0x1F); // ACCEL_X1
    *ay = ICM42688_Read16(0x21); // ACCEL_Y1
    *az = ICM42688_Read16(0x23); // ACCEL_Z1

    *gx = ICM42688_Read16(0x25); // GYRO_X1
    *gy = ICM42688_Read16(0x27); // GYRO_Y1
    *gz = ICM42688_Read16(0x29); // GYRO_Z1
}



// 输入参数：
// dt: 时间间隔（秒）
// gx, gy, gz: 陀螺仪角速度（度/秒）
// ax, ay, az: 加速度计数据（单位g）

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

