#include "./include/controller.h"
#include "./include/motor.h"

#define AFA 60
#define L   120;

typedef struct
{
	float v1;
	float v2;
	float v3;
}ActThreeVell;

ActThreeVell ThreeWheelVellControl(float Vx, float Vy, float angularVell)
{
	//theta是机器人坐标系的x轴正方向与场地坐标系y轴正方向的夹角(最终theta所带值为与陀螺仪偏航角有关的一个变量)
	theta = 0;

	vell.v1 = (float)(-cos((AFA + theta) / 180.0f*3.1415926f) * Vx - sin((theta + AFA) / 180.0f*3.1415926f) * Vy + L * angularVell);

	vell.v2 = (float)(cos(theta / 180.0f*3.1415926f) * Vx + sin(theta /180.0f*3.1415926f) * Vy      + L * angularVell);

	vell.v3 = (float)(-cos((AFA - theta) / 180.0f * 3.1415926f) * Vx + sin((AFA - theta) / 180.0f*3.1415926f) * Vy + L * angularVell);

	return vell;
}




