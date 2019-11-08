#pragma once
#include <math.h>
#include "log.h"
#include "depend.h"
#include "DynaxicelBase.h"
class inverseKinematics
{
private:
	//dynamixel
	const float l0 = 50;
	const float l1 = 65;
	const float l2 = 185;
	const float l3 = 115;
	const float l4 = 80;
	const float alpha2 = -90;
	float theta[4] = { 0 ,0, 0, 0 };
	float _px, _py, _pz, _phi;
	float z1, r1;
	logger *log_;
public:
	inverseKinematics(logger *log);
	~inverseKinematics();
	//dynamixel
	void InputPose(float px, float py, float pz, float phi);
	float deg(float rad);//radian to degrees
	float getTheta(int id);
	void cal1();//1st calculation
	void cal2();//3rd calculation
	void cal3();//2nd calculation
	void cal4();//4th calculation
	void calAll();

	//计算单元
	struct stMat
	{
	public: 
		float nx; float ny; float nz; float ox; float oy; float oz; float ax; float ay; float az; float px; float py; float pz;
		stMat(void)//姿态
		{
				nx = 1;
				ny = 0;
				nz = 0;
				ox = 0;
				oy = 1;
				oz = 0;
				ax = 0;
				ay = 0;
				az = 1;
				px = 0;
				py = 0;
				pz = 0;
		}
		stMat(float x, float y, float z)//位置+姿态
		{
				nx = 1;
				ny = 0;
				nz = 0;
				ox = 0;
				oy = 1;
				oz = 0;
				ax = 0;
				ay = 0;
				az = 1;
				px = x;
				py = y;
				pz = z;
		}
	};
	struct svTheta 
	{
	public: 
		float t1; float t2; float t3; float t4; float t5; float t6;
		svTheta(void) //初始化角度初值为0
		{
				t1 = t2 = t3 = t4 = t5 = t6 = 0;
		}
	};
private://矩阵相关
	struct _Matrix {
		float *arr;
	};
	void matrix_init(_Matrix *m);
	void matrix_free(_Matrix *m);
	float matrix_read(_Matrix *m, int i, int j);
	bool matrix_write(_Matrix *m, int i, int j, float val);
	bool matrix_multiply(_Matrix *A, _Matrix *B, _Matrix *C);
	void printMatrix(_Matrix *A);//打印矩阵
	void printSTMat(stMat*t);
public:
	svTheta currentTheta;//当前各轴角度-弧度值
	svTheta targetTheta;//各轴目标角度-弧度值
	//运动学相关
	bool calcReverseKinematics(stMat* t, svTheta* v);//根据笛卡尔坐标系下的位姿和当前位姿计算各轴拟旋转角度
	void getToolCurrentPosture(stMat* t);//获取当前工具坐标系在笛卡尔坐标系中的位姿
	bool movetoTool(float, float, float);//末端沿着工具坐标系的方向，从当前位置移动到指定位置;增量移动
	bool moveTool(stMat*);//末端沿着工具坐标系的方向，从当前姿态移动到指定姿态(工具坐标系下的姿态)；绝对位姿移动
	bool rotateToolZ(float angle);//沿着工具坐标系的z轴旋转指定角度（单位为弧度）
	bool movetoCart(float, float, float);//末端沿着笛卡尔坐标系的方向，从当前位置移动到指定位置;增量移动，最多100mm
	bool moveCart(stMat*);//沿着笛卡尔坐标系的方向，从当前姿态移动到指定姿态；绝对位姿移动
	//手臂各轴相关
	void printPosture(void);//打印当前姿态
	//回调函数
	typedef void(*moveFinish_cb_fun)(void);
	moveFinish_cb_fun pMoveFinish_cb_fun;//回调函数
	void registMoveFinishCB(moveFinish_cb_fun);//移动结束回调函数
private:
	bool isVectorOrthogonal(stMat*t);//判断向量是否正交
	bool isUnderRange(stMat*);//超出移动范围
	bool isEqual(float x1, float x2);//两float值是否相等
	void calcCurrentPosition(void);// 根据当前角度计算弧度值
	bool calcCurrentAngle(svTheta*v, float *angle, int num);//根据弧度值计算角度
	bool isMovebable(void);
};
