#include "headfile.h"

int16_t ax,ay,az,gx,gy,gz;

int main(void)
{
	/*模块初始化*/
	OLED_Init();
	ICM42688_Init();

	OLED_ShowHexNum(1, 1, ICM42688_GetID(), 4);
	
	while (1)
	{
		update_orientation(ax,ay,az,gx,gy,gz,0.001f);
		OLED_ShowSignedNum(2, 1, yaw, 3); 
		OLED_ShowSignedNum(3, 1, pitch_acc, 3); //加速度计直接测量较准确
		OLED_ShowSignedNum(4, 1, roll_acc, 3); //加速度计直接测量较准确
//		OLED_ShowSignedNum(2, 1, ax, 4); 
//		OLED_ShowSignedNum(2, 7, ay, 4); 
//		OLED_ShowSignedNum(3, 1, az, 4); 
//		OLED_ShowSignedNum(3, 7, gx, 4); 
//		OLED_ShowSignedNum(4, 1, gy, 4); 
//		OLED_ShowSignedNum(4, 7, gz, 4); 
	}
}
