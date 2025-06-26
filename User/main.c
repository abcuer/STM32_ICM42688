#include "headfile.h"
int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
int16_t ax, ay, az, gx, gy, gz;

int main(void)
{
	/*模块初始化*/
	OLED_Init();
	ICM42688_Init();
	
	OLED_ShowString(1, 1, "id:");
	OLED_ShowHexNum(1, 6, ICM42688_GetID(), 4);
	while (1)
	{
//		ICM42688_GetAccelGyro(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
//		ax = ax_raw * icm42688_iic_acc_inv;  // 单位：mg
//		ay = ay_raw * icm42688_iic_acc_inv;
//		az = az_raw * icm42688_iic_acc_inv;
//		gx = gx_raw * icm42688_iic_gyro_inv;
//		gy = gy_raw * icm42688_iic_gyro_inv;
//		gz = gz_raw * icm42688_iic_gyro_inv;
//		OLED_ShowSignedNum(2, 1, ax, 5); 
//		OLED_ShowSignedNum(2, 7, ay, 5); 
//		OLED_ShowSignedNum(3, 1, az, 5); 
//		OLED_ShowSignedNum(3, 7, gx, 5); 
//		OLED_ShowSignedNum(4, 1, gy, 5); 
//		OLED_ShowSignedNum(4, 7, gz, 5); 
		update_orientation(ax,ay,az,gx,gy,gz,0.001f);
		OLED_ShowSignedNum(2, 1, yaw, 3); 
		OLED_ShowSignedNum(3, 1, pitch, 3); 
		OLED_ShowSignedNum(3, 7, roll, 3); 
		OLED_ShowSignedNum(4, 1, pitch_acc, 3); //加速度计直接测量较准确
		OLED_ShowSignedNum(4, 7, roll_acc, 3); //加速度计直接测量较准确
	}
}
