#include "inverseKinematics.h"

inverseKinematics::inverseKinematics(logger * log)
{
	log_ = log;
	//bServoOn = false;
	//armStatus = SERVO_OFF;
	//currentSpeed = 5;//默认当前速度为5
	//for (int i = 0; i < 6; i++)
	//{
	//	axisRg[i].minus = -90;
	//	axisRg[i].bias = 0;
	//	axisRg[i].maxium = 90;
	//	axisRg[i].current = 0;
	//	axisRg[i].target = axisRg[i].current;
	//	axisRg[i].wait = axisRg[i].current;
	//}
	//axisRg[0].i2c_addr = 1;
	//axisRg[1].i2c_addr = 1;
	//axisRg[2].i2c_addr = 1;
	//axisRg[3].i2c_addr = 1;
	//axisRg[4].i2c_addr = 1;
	//axisRg[5].i2c_addr = 1;
	//setChannel(0, 1, 2, 3, 4, 5);
	//pMoveFinish_cb_fun = NULL;
}

inverseKinematics::~inverseKinematics()
{
}

void inverseKinematics::InputPose(float px, float py, float pz, float phi)
{
	_px = px;
	_py = py;
	_pz = pz;
	_phi = phi;
	z1 = _pz - l1 - l0;
	r1 = _px;
}

float inverseKinematics::deg(float rad)
{
	return rad * 180 / 3.1416;
}

float inverseKinematics::getTheta(int id)
{
	return theta[id - 1];
}

void inverseKinematics::cal1()
{
	theta[0] = atan2(_py, _px);
	theta[0] = deg(theta[0]);
	log_->Add(logDEBUG) << "Value of theta1 is " << theta[0] << endl;
}

void inverseKinematics::cal2()
{
	float beta = deg(atan2(z1, r1));
	float psi = deg(acos((pow(z1, 2) + pow(r1, 2) + pow(l2, 2) - pow(l3, 2)) / (2 * sqrt(pow(r1, 2) + pow(z1, 2))*l2)));


	if (theta[2] >= 0) {
		theta[1] = 90 - (90 - beta - psi);
	}
	else if (theta[2] < 0) {
		theta[1] = 90 - (90 - beta + psi);
	}
	//log_->Add(logDEBUG) << "Value of psi is " << psi << endl;
	//log_->Add(logDEBUG) << "Value of beta is " << beta << endl;
	log_->Add(logDEBUG) << "Value of theta2 is " << theta[1] << endl;
}

void inverseKinematics::cal3()
{
	theta[2] = acos((pow(z1, 2) + pow(r1, 2) - pow(l2, 2) - pow(l3, 2)) / (2 * l2*l3));
	theta[2] = deg(theta[2]);
	//log_->Add(logDEBUG) << "Value of z1 is " << z1 << endl;
	//log_->Add(logDEBUG) << "Value of r1 is " << r1 << endl;
	log_->Add(logDEBUG) << "Value of theta3 is " << theta[2] << endl;
}

void inverseKinematics::cal4()
{
	theta[3] = _phi - theta[1] - theta[2] + 90;
	log_->Add(logDEBUG) << "Value of theta4 is " << theta[3] << endl;
}

void inverseKinematics::calAll()
{
	cal1();
	cal3();
	cal2();
	cal4();
}

void inverseKinematics::matrix_init(_Matrix * m)
{
	m->arr = (float *)malloc(4 * 4 * sizeof(float));
	memset(m->arr, 0x00, 4 * 4 * sizeof(float));
}

void inverseKinematics::matrix_free(_Matrix * m)
{
	free(m->arr);
}

float inverseKinematics::matrix_read(_Matrix * m, int i, int j)
{
	if (i >= 4 || j >= 4)
	{
		return -31415;
	}
	return *(m->arr + i * 4 + j);
}

bool inverseKinematics::matrix_write(_Matrix * m, int i, int j, float val)
{
	if (i >= 4 || j >= 4)
	{
		return false;
	}
	*(m->arr + i * 4 + j) = val;
	return true;
}

bool inverseKinematics::matrix_multiply(_Matrix * A, _Matrix * B, _Matrix * C)
{
	int i = 0;
	int j = 0;
	int k = 0;
	float temp = 0;

	//运算
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			temp = 0;
			for (k = 0; k < 4; k++)
			{
				temp += matrix_read(A, i, k) * matrix_read(B, k, j);
			}
			matrix_write(C, i, j, temp);
		}
	}
	return true;
}

void inverseKinematics::printMatrix(_Matrix * A)
{
	int i = 0;
	int j = 0;
	char m[12];
	char temp[128];
	//运算
	for (i = 0; i < 4; i++)
	{
		temp[0] = 0x00;
		for (j = 0; j < 4; j++)
		{
			//dtostrf(matrix_read(A, i, j), 4, 3, m);
			strcat(temp, "\t");
			strcat(temp, m);
		}
		//print_debug("%s\r\n", temp);
	}
}

void inverseKinematics::printSTMat(stMat * t)
{
	char temp[256];
	char m[12];
	/*dtostrf(t->nx, 4, 3, m); sprintf(temp, "%s", m);
	dtostrf(t->ox, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->ax, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->px, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);

	dtostrf(t->ny, 4, 3, m); strcat(temp, "\r\n"); strcat(temp, m);
	dtostrf(t->oy, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->ay, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->py, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);

	dtostrf(t->nz, 4, 3, m); strcat(temp, "\r\n"); strcat(temp, m);
	dtostrf(t->oz, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->az, 4, 3, m); strcat(temp, "\t"); strcat(temp, m);
	dtostrf(t->pz, 4, 3, m); strcat(temp, "\t"); strcat(temp, m); strcat(temp, "\r\n0\t0\t0\t1\t\r\n");*/
	//print_debug(temp);
}

void inverseKinematics::registMoveFinishCB(moveFinish_cb_fun)
{
	//(*)p(1)-带参数
	
}

bool inverseKinematics::isVectorOrthogonal(stMat *t)
{
  //t->nx,t->ny, t->nz的平方和为1
  //t->n t->o t->a  n*o n*a o*a 内积均为0则正交
	if (fabs((t->nx) * (t->nx) + (t->ny) * (t->ny) + (t->nz) * (t->nz) - 1) > MINUS) return false;
	if (fabs((t->ox) * (t->ox) + (t->oy) * (t->oy) + (t->oz) * (t->oz) - 1) > MINUS) return false;
	if (fabs((t->ax) * (t->ax) + (t->ay) * (t->ay) + (t->az) * (t->az) - 1) > MINUS) return false;
	if (fabs((t->nx) * (t->ox) + (t->ny) * (t->oy) + (t->nz) * (t->oz)) > MINUS) return false;
	if (fabs((t->nx) * (t->ax) + (t->ny) * (t->ay) + (t->nz) * (t->az)) > MINUS) return false;
	if (fabs((t->ax) * (t->ox) + (t->ay) * (t->oy) + (t->az) * (t->oz)) > MINUS) return false;
	return true;
}

bool inverseKinematics::isUnderRange(stMat *)
{
	//t->px, t->py, t->pz
	return true;
}

bool inverseKinematics::isEqual(float x1, float x2)
{
	if (fabs(x1 - x2) <= MINUS)
		return true;
	else
		return false;
}

void inverseKinematics::calcCurrentPosition(void)
{
	//currentTheta.t1 = axisRg[0].current * 0.017453;//PI / 180.0;
	//currentTheta.t2 = axisRg[1].current * 0.017453;
	//currentTheta.t3 = axisRg[2].current * 0.017453;
	//currentTheta.t4 = axisRg[3].current * 0.017453;
	//currentTheta.t5 = axisRg[4].current * 0.017453;
	//currentTheta.t6 = axisRg[5].current * 0.017453;
}

bool inverseKinematics::calcCurrentAngle(svTheta * v, float * angle, int num)
{
	if (num != 6) return false;
	angle[0] = v->t1 * 57.295780;//180.0 / PI
	angle[1] = v->t2 * 57.295780;
	angle[2] = v->t3 * 57.295780;
	angle[3] = v->t4 * 57.295780;
	angle[4] = v->t5 * 57.295780;
	angle[5] = v->t6 * 57.295780;
	return true;
}
bool inverseKinematics::isMovebable(void)
{
	//if (!bServoOn) return false;
	return true;
}
/*
  机器人逆向运动学计算 ReverseKinematics calculation
  intput : stMat *t
  output : svTheta *v
  输入：笛卡尔坐标系中表示的末端期望姿态
  输出：六轴旋转角度，若计算失败返回false，角度全为0
  当输入值超过移动范围、计算值超过允许角度时，计算异常时，返回false
*/
bool inverseKinematics::calcReverseKinematics(stMat * t, svTheta * v)
{
	int i, k;
	v->t1 = v->t2 = v->t3 = v->t4 = v->t5 = v->t6 = 0;
	//更新当前值
	calcCurrentPosition();
	//检查输入是否合法
	//判断t中旋转向量是否正交
	if (!isVectorOrthogonal(t))
	{
		//print_debug("isVectorOrthogonal Failed!\r\n");
		return false;
	}
	//是否超出移动范围
	if (!isUnderRange(t))
	{
		//print_debug("isUnderRange Failed!\r\n");
		return false;
	}
	//to be continued
	//provide to our customer only.
	//service@doit.am for more information
	return true;
}

void inverseKinematics::getToolCurrentPosture(stMat * t)
{
	struct _Matrix A1;
	struct _Matrix A2;
	struct _Matrix A;
	matrix_init(&A1);
	matrix_init(&A2);
	matrix_init(&A);
	calcCurrentPosition();
	//A1
	//float st = sin(currentTheta.t1);
	//float ct = cos(currentTheta.t1);
	//matrix_write(&A1, 0, 0, ct); matrix_write(&A1, 0, 2, -st); matrix_write(&A1, 0, 3, ARM_A1 * ct);
	//matrix_write(&A1, 1, 0, st); matrix_write(&A1, 1, 2, ct); matrix_write(&A1, 1, 3, ARM_A1 * st);
	//matrix_write(&A1, 2, 1, -1); matrix_write(&A1, 2, 3, ARM_D1);
	//matrix_write(&A1, 3, 3, 1);

	////print_debug("A1\r\n");
	////printMatrix(&A1);

	//st = sin(currentTheta.t2);
	//ct = cos(currentTheta.t2);
	//matrix_write(&A2, 0, 0, st); matrix_write(&A2, 0, 1, ct); matrix_write(&A2, 0, 3, ARM_A2 * st);
	//matrix_write(&A2, 1, 0, -ct); matrix_write(&A2, 1, 1, st); matrix_write(&A2, 1, 3, -ARM_A2 * ct);
	//matrix_write(&A2, 2, 2, 1);
	//matrix_write(&A2, 3, 3, 1);
	//matrix_multiply(&A1, &A2, &A);//A=A1*A2

	////print_debug("A1*A2\r\n");
	////printMatrix(&A);

	//st = sin(currentTheta.t3);
	//ct = cos(currentTheta.t3);
	//matrix_init(&A1);
	//matrix_write(&A1, 0, 0, ct); matrix_write(&A1, 0, 2, -st); matrix_write(&A1, 0, 3, ARM_A3 * ct - ARM_D3 * st);
	//matrix_write(&A1, 1, 0, st); matrix_write(&A1, 1, 2, ct); matrix_write(&A1, 1, 3, ARM_A3 * st + ARM_D3 * ct);
	//matrix_write(&A1, 2, 1, -1);
	//matrix_write(&A1, 3, 3, 1);
	//matrix_multiply(&A, &A1, &A2);

	////print_debug("A1*A2*A3\r\n");
	////printMatrix(&A2);

	//st = sin(currentTheta.t4);
	//ct = cos(currentTheta.t4);
	//matrix_init(&A1);
	//matrix_write(&A1, 0, 0, ct); matrix_write(&A1, 0, 2, st);
	//matrix_write(&A1, 1, 0, st); matrix_write(&A1, 1, 2, -ct);
	//matrix_write(&A1, 2, 1, 1);
	//matrix_write(&A1, 3, 3, 1);
	//matrix_multiply(&A2, &A1, &A);

	////print_debug("A1*A2*A3*A4\r\n");
	////printMatrix(&A);

	//st = sin(currentTheta.t5);
	//ct = cos(currentTheta.t5);
	//matrix_init(&A1);
	//matrix_write(&A1, 0, 0, ct); matrix_write(&A1, 0, 2, -st);
	//matrix_write(&A1, 1, 0, st); matrix_write(&A1, 1, 2, ct);
	//matrix_write(&A1, 2, 1, -1);
	//matrix_write(&A1, 3, 3, 1);
	//matrix_multiply(&A, &A1, &A2);//A=A*A5

	////print_debug("A1*A2*A3*A4*A5\r\n");
	////printMatrix(&A2);

	//st = sin(currentTheta.t6);
	//ct = cos(currentTheta.t6);
	//matrix_init(&A1);
	//matrix_write(&A1, 0, 0, ct); matrix_write(&A1, 0, 1, -st);
	//matrix_write(&A1, 1, 0, st); matrix_write(&A1, 1, 1, ct);
	//matrix_write(&A1, 2, 2, 1); matrix_write(&A1, 2, 3, ARM_D6);
	//matrix_write(&A1, 3, 3, 1);
	//matrix_multiply(&A2, &A1, &A);//A=A*A6

	//print_debug("A1*A2*A3*A4*A5*A6\r\n");
	//printMatrix(&A);

	t->nx = matrix_read(&A, 0, 0);
	t->ny = matrix_read(&A, 1, 0);
	t->nz = matrix_read(&A, 2, 0);
	t->ox = matrix_read(&A, 0, 1);
	t->oy = matrix_read(&A, 1, 1);
	t->oz = matrix_read(&A, 2, 1);
	t->ax = matrix_read(&A, 0, 2);
	t->ay = matrix_read(&A, 1, 2);
	t->az = matrix_read(&A, 2, 2);
	t->px = matrix_read(&A, 0, 3);
	t->py = matrix_read(&A, 1, 3);
	t->pz = matrix_read(&A, 2, 3);

	matrix_free(&A1);
	matrix_free(&A2);
	matrix_free(&A);
}

bool inverseKinematics::movetoTool(float, float, float)
{
	//if (fabs(x) > 100 || fabs(y) > 100 || fabs(z) > 100)
	//{
	//	print_debug("Input out of range!");
	//	return false;
	//}

	//stMat t;
	//svTheta v;
	//getToolCurrentPosture(&t);
	//print_debug("Current posture\r\n");
	//printSTMat(&t);
	////A=A1*A2
	//struct _Matrix A1;
	//struct _Matrix A2;
	//struct _Matrix A;
	//matrix_init(&A1);
	//matrix_init(&A2);
	//matrix_init(&A);
	//matrix_write(&A1, 0, 0, t.nx); matrix_write(&A1, 0, 1, t.ox); matrix_write(&A1, 0, 2, t.ax); matrix_write(&A1, 0, 3, t.px);
	//matrix_write(&A1, 1, 0, t.ny); matrix_write(&A1, 1, 1, t.oy); matrix_write(&A1, 1, 2, t.ay); matrix_write(&A1, 1, 3, t.py);
	//matrix_write(&A1, 2, 0, t.nz); matrix_write(&A1, 2, 1, t.oz); matrix_write(&A1, 2, 2, t.az); matrix_write(&A1, 2, 3, t.pz);
	//matrix_write(&A1, 3, 3, 1);
	//matrix_write(&A2, 0, 3, x); matrix_write(&A2, 1, 3, y); matrix_write(&A2, 2, 3, z);
	//matrix_write(&A2, 0, 0, 1); matrix_write(&A2, 1, 1, 1); matrix_write(&A2, 2, 2, 1); matrix_write(&A2, 3, 3, 1);
	//matrix_multiply(&A1, &A2, &A);//A=A1*A2
	//t.nx = matrix_read(&A, 0, 0); t.ny = matrix_read(&A, 1, 0); t.nz = matrix_read(&A, 2, 0);
	//t.ox = matrix_read(&A, 0, 1); t.oy = matrix_read(&A, 1, 1); t.oz = matrix_read(&A, 2, 1);
	//t.ax = matrix_read(&A, 0, 2); t.ay = matrix_read(&A, 1, 2); t.az = matrix_read(&A, 2, 2);
	//t.px = matrix_read(&A, 0, 3); t.py = matrix_read(&A, 1, 3); t.pz = matrix_read(&A, 2, 3);
	//matrix_free(&A1); matrix_free(&A2); matrix_free(&A);

	//print_debug("Ready to posture\r\n");
	//printSTMat(&t);
	//if (calcReverseKinematics(&t, &v) == false)
	//{
	//	print_debug("calcReverseKinematics failed!");
	//	return false;
	//}
	//float a[6] = { 0, 0, 0, 0, 0, 0 };
	//calcCurrentAngle(&v, a, 6);
	//print_debug("Target angle:(%s", printFloat(a[0]));
	//print_debug(",%s", printFloat(a[1]));
	//print_debug(",%s", printFloat(a[2]));
	//print_debug(",%s", printFloat(a[3]));
	//print_debug(",%s", printFloat(a[41]));
	//print_debug(",%s)\r\n", printFloat(a[5]));
	//if (setTargetPosition(a, 6))
	//{
	//	print_debug("movetoTool calc ok!\r\n");
	//	beginMove();
	//	return true;
	//}
	//else
	//{
	//	print_debug("movetoTool calc not ok!\r\n");
	//	return false;
	//}
	return true;
}

bool inverseKinematics::moveTool(stMat *)
{
	//将t转化为笛卡尔空间的姿态矩阵
  //calcReverseKinematics计算各轴目标角度
  //开始运动
	//print_debug("moveTool2:%d\r\n", t->nx);
	return true;
}

bool inverseKinematics::rotateToolZ(float angle)
{
	return true;
}

bool inverseKinematics::movetoCart(float, float, float)
{
	//获取当前姿态getToolCurrentPosture，确定目标姿态
  //calcReverseKinematics计算各轴目标角度
  //开始运动
	//if (fabs(x) > 100 || fabs(y) > 100 || fabs(z) > 100)
	//{
	//	//print_debug("Input out of range!");
	//	return false;
	//}
	stMat t;
	svTheta v;
	getToolCurrentPosture(&t);
	//print_debug("Current posture\r\n");
	printSTMat(&t);

	/*t.px = t.px + x;
	t.py = t.py + y;
	t.pz = t.pz + z;*/
	//print_debug("Ready to posture\r\n");
	printSTMat(&t);

	if (calcReverseKinematics(&t, &v) == false)
	{
		//print_debug("calcReverseKinematics failed!");
		return false;
	}
	float a[6] = { 0, 0, 0, 0, 0, 0 };
	calcCurrentAngle(&v, a, 6);
	/*print_debug("Target angle:(%s", printFloat(a[0]));
	print_debug(",%s", printFloat(a[1]));
	print_debug(",%s", printFloat(a[2]));
	print_debug(",%s", printFloat(a[3]));
	print_debug(",%s", printFloat(a[41]));
	print_debug(",%s)\r\n", printFloat(a[5]));*/
	//if (setTargetPosition(a, 6))
	//{
	//	//print_debug("movetoCart calc ok!\r\n");
	//	beginMove();
	//	return true;
	//}
	//else
	//{
	//	//print_debug("movetoCart calc not ok!\r\n");
	//	return false;
	//}
}

bool inverseKinematics::moveCart(stMat *)
{
	//calcReverseKinematics计算各轴目标角度
 //开始运动
	return false;
}

void inverseKinematics::printPosture(void)
{
	stMat t;
	getToolCurrentPosture(&t);
	//print_debug("Current posture\r\n");
	printSTMat(&t);
}
