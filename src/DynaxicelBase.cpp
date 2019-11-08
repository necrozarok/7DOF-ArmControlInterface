#include "DynaxicelBase.h"

DynaxicelBase::DynaxicelBase()
{
	
}

int DynaxicelBase::checkDegree(int id, int degree)
{
	if (degree > max_degree[id - 1] || degree < min_degree[id - 1]) return 0;
	else return 1;
}

int DynaxicelBase::calculateUnit(int id) {
	return (max_value[id - 1] - min_value[id - 1]) / (max_degree[id - 1] - min_degree[id - 1]);
}

int DynaxicelBase::move(int id, int degree) {
	if (checkDegree(id, degree) == false) {
		log_->Add(logWARNING) << "The degree is wrong!" << endl;
		return 1;
	}
	else
	{
		dxl_goal_position[id - 1] = degree * calculateUnit(id) + min_value[id - 1];
		// Allocate goal position value into byte array
		param_goal_position[0] = DXL_LOBYTE(dxl_goal_position[id - 1]);
		param_goal_position[1] = DXL_HIBYTE(dxl_goal_position[id - 1]);

		dxl_addparam_result = group.addParam(id, param_goal_position);//already made initialize
		if (dxl_addparam_result != true)
		{
			log_->Add(logERROR) << " [ID:%03d] groupSyncWrite addparam failed" << endl;
			return 1;
		}
		return 0;
	}
}

int DynaxicelBase::autoMove(int id) {
	// Allocate goal position value into byte array
	param_goal_position[0] = DXL_LOBYTE(dxl_goal_position[id - 1]);
	param_goal_position[1] = DXL_HIBYTE(dxl_goal_position[id - 1]);

	dxl_addparam_result = group.addParam(id, param_goal_position);//already made initialize
	if (dxl_addparam_result != true)
	{
		log_->Add(logERROR) << " [ID:%03d] groupSyncWrite addparam failed" << endl;
		return 1;
	}
	return 0;
}

int DynaxicelBase::move2(int id, int value)
{
	//QMessageBox::about(NULL, "", "move2");
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_GOAL_POSITION, value, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//max_torque[id - 1] = torque;
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}


int DynaxicelBase::writeAll(void) {
	dxl_comm_result = group.txPacket();
	if (dxl_comm_result != COMM_SUCCESS) {
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	return 0;
}

void DynaxicelBase::clearAll() {
	group.clearParam();
}

int DynaxicelBase::checkPort(void) {
	if (portHandler->openPort())
	{
		//log_->Add(logINFO) << "Port Opened" << endl;
		return 0;
	}
	else
	{
		QMessageBox::about(NULL, "TIP", "openPort success");
		//log_->Add(logERROR) << " Failed to open the port!" << endl;
		return 1;
	}
}

int DynaxicelBase::setBaudRate(void) {
	if (portHandler->openPort())
	{
		log_->Add(logINFO) << "Baudrate Set" << endl;
		return 0;
	}
	else
	{
		log_->Add(logERROR) << " Failed to open the port!" << endl;
		return 1;
	}
}

int DynaxicelBase::setMaxTorque(int id, int torque) {
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MAX_TORQUE, torque, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		max_torque[id - 1] = torque;
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}

int DynaxicelBase::enableTorque(int id) {
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//printf("Dynamixel#%d has been successfully connected \n", id);
		return 0;
	}
}

UINT8 DynaxicelBase::ismoving(int id)
{
	UINT8 flag=0;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, id, ADDR_MX_IS_MOVING, &flag, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	//printf("Reading...");
	return flag;
}

uint16_t DynaxicelBase::getPresentCurrent(int id)
{
	uint16_t current = 0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_CURRENT, &current, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	//printf("Reading...");
	return current;
}

uint16_t DynaxicelBase::getPresentVelocity(int id)
{
	uint16_t velocity = 0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_PRESENT_VELOCITY, &velocity, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	//printf("Reading...");
	return velocity;
}

int DynaxicelBase::read(int id) {
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_PRESENT_POSITION, &dxl_present_position[id - 1], &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	//printf("Reading...");
	return dxl_present_position[id - 1];
}

void DynaxicelBase::print(void) {
	printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n",
		1, dxl_goal_position[0], dxl_present_position[0], 2, dxl_goal_position[1], dxl_present_position[1],
		3, dxl_goal_position[2], dxl_present_position[2], 4, dxl_goal_position[3], dxl_present_position[3]);
}

int * DynaxicelBase::GetGroupPos()
{
	return dxl_goal_position;
}

uint16_t * DynaxicelBase::GetPresentGroupPos()
{
	return dxl_present_position;
}

int DynaxicelBase::getGoal(int id) const {
	return dxl_goal_position[id - 1];
}

int DynaxicelBase::getPresent(int id) const {
	return dxl_present_position[id - 1];
}

int DynaxicelBase::getThreshold() const {
	return DXL_MOVING_STATUS_THRESHOLD;
}

int DynaxicelBase::getMaxTorque(int id) const {
	return max_torque[id - 1];
}

uint16_t DynaxicelBase::getCWLimit(int id) const
{
	return cw_pos_limit[id - 1];
}

uint16_t DynaxicelBase::getCCWLimit(int id) const
{
	return ccw_pos_limit[id - 1];
}

void DynaxicelBase::setGoal(int id, int pos) {
	dxl_goal_position[id - 1] = pos;
	//printf("[ID:%03d] Goal Changed:%03d\n", id, pos);
}

int DynaxicelBase::disableTorque(int id) {
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	printf("[ID:%03d] Torque disabled\n", id);
	return 0;
}

int DynaxicelBase::setPgain(int id, int pgain)
{
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_P_GAIN, pgain, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//printf("Dynamixel#%d has been successfully connected \n", id);
		return 0;
	}
}

int DynaxicelBase::setCW(int id, int cw)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_CW_POS_LIMIT, cw, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//QMessageBox::about(NULL, "", "set speed ");
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}

int DynaxicelBase::setCCW(int id, int ccw)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_CCW_POS_LIMIT, ccw, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//QMessageBox::about(NULL, "", "set speed ");
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}

void DynaxicelBase::closePort(void) {
	portHandler->closePort();
}

DynaxicelBase::~DynaxicelBase()
{
	if (portHandler != nullptr)
		portHandler->closePort();
		portHandler= nullptr;
	if (log_ != nullptr)
		log_ = nullptr;
}
void DynaxicelBase::Add_Log(logger* log)
{
	log_ = log;
}
//operation
int DynaxicelBase::loosenGrip() {
	if (setMaxTorque(1, 0) == 1) {
		return 1;
	};
	if (setMaxTorque(2, 50) == 1) {
		return 1;
	};
	if (setMaxTorque(3, 50) == 1) {
		return 1;
	};
	if (setMaxTorque(4, 50) == 1) {
		return 1;
	}
	log_->Add(logINFO) << "Loosening Grip" << endl;
	return 0;
}

int DynaxicelBase::tightenGrip() {
	if (setMaxTorque(1, getMaxTorque(1)) == 1) {
		return 1;
	};
	if (setMaxTorque(2, getMaxTorque(2)) == 1) {
		return 1;
	};
	if (setMaxTorque(3, getMaxTorque(3)) == 1) {
		return 1;
	};
	if (setMaxTorque(4, getMaxTorque(4)) == 1) {
		return 1;
	}
	log_->Add(logINFO) << "Tightening Grip" << endl;
	return 0;
}

int DynaxicelBase::setMaxSpeed()
{
	if (setSpeed(1, max_speed[0]) == 1) {
		return 1;
	};
	if (setSpeed(2, max_speed[1]) == 1) {
		return 1;
	};
	if (setSpeed(3, max_speed[2]) == 1) {
		return 1;
	};
	if (setSpeed(4, max_speed[3]) == 1) {
		return 1;
	}
	log_->Add(logINFO) << "set MaxSpeed" << endl;
	return 0;
}

int DynaxicelBase::setSpeed(int id, int speed)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MAX_SPEED, speed, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		//QMessageBox::about(NULL, "", "set speed ");
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}


void DynaxicelBase::newGoal() {
	log_->Add(logINFO) << "Reading in new goal" << endl;
	read(1);
	dxl_goal_position[0] = getPresent(1);
	//printf("[ID:%03d] GoalPos:%03d\n", 1, m_->getPresent(1));

	read(2);
	dxl_goal_position[1] = getPresent(2);
	//printf("[ID:%03d] GoalPos:%03d\n", 2, m_->getPresent(2));

	read(3);
	dxl_goal_position[2] = getPresent(3);
	//printf("[ID:%03d] GoalPos:%03d\n", 3, m_->getPresent(3));

	read(4);
	dxl_goal_position[3] = getPresent(4);
	//printf("[ID:%03d] GoalPos:%03d\n", 4, m_->getPresent(4));

}

void DynaxicelBase::AcceptPos(int i1, int i2, int i3, int i4)
{
	dxl_goal_position[0] = i1;
	dxl_goal_position[1] = i2;
	dxl_goal_position[2] = i3;
	dxl_goal_position[3] = i4;
}

int DynaxicelBase::run() {
	int thres = getThreshold();
	QTime time = QTime::currentTime(),time2;
	do
	{
		// Read goal position
		if (read(1) == 1) {
			log_->Add(logERROR) << "Read Failed" << endl;
			break;
		};
		if (read(2) == 1) {
			log_->Add(logERROR) << "Read Failed" << endl;
			break;
		};
		if (read(3) == 1) {
			log_->Add(logERROR) << "Read Failed" << endl;
			break;
		}
		if (read(4) == 1) {
			log_->Add(logERROR) << "Read Failed" << endl;
			break;
		}
		//print the status
		//print();
		//check if the motor has reached destination
		time2 = QTime::currentTime();
	} while ((abs(getGoal(1) - getPresent(1)) > thres) || (abs(getGoal(2) - getPresent(2)) > thres)\
		|| (abs(getGoal(3) - getPresent(3)) > thres) || (abs(getGoal(4) - getPresent(4)) > thres)||(abs(time.second()-time2.second())<10));
	return 0;
}

int DynaxicelBase::ReadPosLimit()
{
	for (int id = 1; id < 5; id++)
	{
		dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_CW_POS_LIMIT, &cw_pos_limit[id - 1], &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
			return 1;
		else if (dxl_error != 0)
			return 1;
		dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_CCW_POS_LIMIT, &ccw_pos_limit[id - 1], &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
			return 1;
		else if (dxl_error != 0)
			return 1;
	}
	return 0;
}
