#include "headfile.h"
int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
//int16_t ax, ay, az, gx, gy, gz;
float quat[4] = {1.0f, 0.0f, 0.0f, 0.0f};  

int main(void)
{
	/*模块初始化*/
	OLED_Init();
	ICM42688_Init();
	
	OLED_ShowString(1, 1, "id:");
	OLED_ShowHexNum(1, 6, ICM42688_GetID(), 4);
	while (1)
	{
		ICM42688_GetAccelGyro(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
		float ax = ax_raw / 2048.0f;   // 假设加速度量程 ±16g
		float ay = ay_raw / 2048.0f;
		float az = az_raw / 2048.0f;

		float gx = gx_raw / 16.4f;     // 假设陀螺仪量程 ±2000dps
		float gy = gy_raw / 16.4f;
		float gz = gz_raw / 16.4f;
		 // 3. 姿态融合更新四元数
//		MahonyFilter_6Axis(quat, gx_raw, gy_raw, gz_raw, ax_raw, ay_raw, az_raw);
		MahonyFilter_6Axis(quat, gx, gy, gz, ax, ay, az);
		// 4. 四元数转欧拉角
		trans_quaternionToEulerAngle(&yaw, &pitch, &roll, quat);
		OLED_ShowSignedNum(2, 1, yaw, 3); 
		OLED_ShowSignedNum(3, 1, pitch, 3); 
		OLED_ShowSignedNum(3, 7, roll, 3); 
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
//		update_orientation(ax,ay,az,gx,gy,gz,0.001f);
//		OLED_ShowSignedNum(2, 1, yaw, 3); 
//		OLED_ShowSignedNum(3, 1, pitch, 3); 
//		OLED_ShowSignedNum(3, 7, roll, 3); 
//		OLED_ShowSignedNum(4, 1, pitch_acc, 3); //加速度计直接测量较准确
//		OLED_ShowSignedNum(4, 7, roll_acc, 3); //加速度计直接测量较准确
	}
}
