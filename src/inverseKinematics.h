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

	//���㵥Ԫ
	struct stMat
	{
	public: 
		float nx; float ny; float nz; float ox; float oy; float oz; float ax; float ay; float az; float px; float py; float pz;
		stMat(void)//��̬
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
		stMat(float x, float y, float z)//λ��+��̬
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
		svTheta(void) //��ʼ���Ƕȳ�ֵΪ0
		{
				t1 = t2 = t3 = t4 = t5 = t6 = 0;
		}
	};
private://�������
	struct _Matrix {
		float *arr;
	};
	void matrix_init(_Matrix *m);
	void matrix_free(_Matrix *m);
	float matrix_read(_Matrix *m, int i, int j);
	bool matrix_write(_Matrix *m, int i, int j, float val);
	bool matrix_multiply(_Matrix *A, _Matrix *B, _Matrix *C);
	void printMatrix(_Matrix *A);//��ӡ����
	void printSTMat(stMat*t);
public:
	svTheta currentTheta;//��ǰ����Ƕ�-����ֵ
	svTheta targetTheta;//����Ŀ��Ƕ�-����ֵ
	//�˶�ѧ���
	bool calcReverseKinematics(stMat* t, svTheta* v);//���ݵѿ�������ϵ�µ�λ�˺͵�ǰλ�˼����������ת�Ƕ�
	void getToolCurrentPosture(stMat* t);//��ȡ��ǰ��������ϵ�ڵѿ�������ϵ�е�λ��
	bool movetoTool(float, float, float);//ĩ�����Ź�������ϵ�ķ��򣬴ӵ�ǰλ���ƶ���ָ��λ��;�����ƶ�
	bool moveTool(stMat*);//ĩ�����Ź�������ϵ�ķ��򣬴ӵ�ǰ��̬�ƶ���ָ����̬(��������ϵ�µ���̬)������λ���ƶ�
	bool rotateToolZ(float angle);//���Ź�������ϵ��z����תָ���Ƕȣ���λΪ���ȣ�
	bool movetoCart(float, float, float);//ĩ�����ŵѿ�������ϵ�ķ��򣬴ӵ�ǰλ���ƶ���ָ��λ��;�����ƶ������100mm
	bool moveCart(stMat*);//���ŵѿ�������ϵ�ķ��򣬴ӵ�ǰ��̬�ƶ���ָ����̬������λ���ƶ�
	//�ֱ۸������
	void printPosture(void);//��ӡ��ǰ��̬
	//�ص�����
	typedef void(*moveFinish_cb_fun)(void);
	moveFinish_cb_fun pMoveFinish_cb_fun;//�ص�����
	void registMoveFinishCB(moveFinish_cb_fun);//�ƶ������ص�����
private:
	bool isVectorOrthogonal(stMat*t);//�ж������Ƿ�����
	bool isUnderRange(stMat*);//�����ƶ���Χ
	bool isEqual(float x1, float x2);//��floatֵ�Ƿ����
	void calcCurrentPosition(void);// ���ݵ�ǰ�Ƕȼ��㻡��ֵ
	bool calcCurrentAngle(svTheta*v, float *angle, int num);//���ݻ���ֵ����Ƕ�
	bool isMovebable(void);
};
