#ifndef _Mahony_h
#define _Mahony_h

float inv_sqrt(float x);
void MahonyFilter_6Axis(float q[4], 
	                       float gx, float gy, float gz,
												 float ax, float ay, float az);
void trans_quaternionToEulerAngle(float* yaw, float* pitch, float* roll, float q[4]);

#endif
