#include "position.h"
Position::Position(logger * log)
{
	log_ = log;
}

Position::Position(logger * log, DWORD errorCode)
{
	log_ = log;
	ErrorCode = errorCode;
}

Position::Position(logger *log , HANDLE handle, DWORD errorCode)
{
	log_ = log;
	Handle = handle;
	ErrorCode = errorCode;
}

Position::~Position()
{
	if (log_ != nullptr)
		log_ = nullptr;
}

bool Position::setAcceleration(WORD node, DWORD value)
{
	if (value >= 10001||value<=-10001)
	{
		log_->Add(logWARNING) << "Node "<<node<< "setAcceleration Error!\n" << endl;
		return false;
	}
	switch (node) {
	case M1_Node:
		Node1_Acceleration = value;
		break; 
	case M2_Node:
		Node2_Acceleration = value;
		break;
	case M3_Node:
		Node3_Acceleration = value;
		break;
	default: 
		log_->Add(logWARNING) << "Node " << node << "is out of node range in setAcceleration!\n" << endl;
		return false;
	}
	return true;
}

bool Position::setDeceleration(WORD node, DWORD value)
{
	if (value >= 10001 || value <= -10001)
	{
		log_->Add(logWARNING) << "Node " << node << "setDeceleration Error!\n" << endl;
		return false;
	}
	switch (node) {
	case M1_Node:
		Node1_Deceleration = value;
		break;
	case M2_Node:
		Node2_Deceleration = value;
		break;
	case M3_Node:
		Node3_Deceleration = value;
		break;
	default:
		log_->Add(logWARNING) << "Node " << node << "is out of node range in setDeceleration!\n" << endl;
		return false;
	}
	return true;
}

bool Position::setVelocity(WORD node, DWORD value)
{
	if (value >= 10001 || value <= -10001)
	{
		log_->Add(logWARNING) << "Node " << node << "setVelocity Error!\n" << endl;
		return false;
	}
	switch (node) {
	case M1_Node:
		Node1_Velocity = value;
		break;
	case M2_Node:
		Node2_Velocity = value;
		break;
	case M3_Node:
		Node3_Velocity = value;
		break;
	default:
		log_->Add(logWARNING) << "Node " << node << "is out of node range in setVelocity!\n" << endl;
		return false;
	}
	return true;
}

bool Position::openPort()
{
	if (Handle)
	{
		if (Handle) VCS_CloseDevice(Handle, &ErrorCode);
		Handle = 0;
	}
	Handle = VCS_OpenDeviceDlg(&ErrorCode);
	if (Handle) 
	{
		log_->Add(logINFO) << "Port Open Error\n" << endl;
		return 0;
	}
	return 1;
}

bool Position::setBaudRate()
{
	return VCS_SetProtocolStackSettings(Handle, Baudrate, Timeout, &ErrorCode);
}

void Position::closePort()
{
	if (Handle) VCS_CloseDevice(Handle, &ErrorCode);
	Handle = 0;
}

long Position::getPresent(WORD node)
{
	long prepos;
	VCS_GetPositionIs(Handle, node, &prepos, &ErrorCode);
	return prepos;
}

bool Position::init()
{
	
	//Clear Error History
	if (!VCS_ClearFault(Handle, M1_Node, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node"<< M1_Node<<"ClearFault Error!\n" << endl;
		return false;
	}
	if (!VCS_ClearFault(Handle, M2_Node, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M2_Node << " ClearFault Error!\n" << endl;
		return false;
	}
	if (!VCS_ClearFault(Handle, M3_Node, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M3_Node << " ClearFault Error!\n" << endl;
		return false;
	}
	//Write Profile Position Mode
	if (VCS_SetOperationMode(Handle, M1_Node, OMD_PROFILE_POSITION_MODE, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M1_Node << " SetOperationMode Error!\n" << endl;
		return false;
	}
	if (VCS_SetOperationMode(Handle, M2_Node, OMD_PROFILE_POSITION_MODE, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M2_Node << " SetOperationMode Error!\n" << endl;
		return false;
	}
	if (VCS_SetOperationMode(Handle, M3_Node, OMD_PROFILE_POSITION_MODE, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M3_Node << " SetOperationMode Error!\n" << endl;
		return false;
	}
	//Write Profile Position Objects
	if (VCS_SetPositionProfile(Handle, M1_Node, Node1_Velocity, Node1_Acceleration, Node1_Deceleration, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M1_Node << " SetPositionProfile Error!\n" << endl;
		return false;
	}
	if (VCS_SetPositionProfile(Handle, M2_Node, Node2_Velocity, Node2_Acceleration, Node2_Deceleration, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M2_Node << " SetPositionProfile Error!\n" << endl;
		return false;
	}
	if (VCS_SetPositionProfile(Handle, M3_Node, Node3_Velocity, Node3_Acceleration, Node3_Deceleration, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M3_Node << " SetPositionProfile Error!\n" << endl;
		return false;
	}
	//Read Actual Position
	if (VCS_GetPositionIs(Handle, M1_Node, &Node1_StartPos, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M1_Node << " GetPositionIs Error!\n" << endl;
		return false;
	}
	if (VCS_GetPositionIs(Handle, M2_Node, &Node2_StartPos, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M2_Node << " GetPositionIs Error!\n" << endl;
		return false;
	}
	if (VCS_GetPositionIs(Handle, M3_Node, &Node3_StartPos, &ErrorCode))
	{
		log_->Add(logWARNING) << "Node " << M3_Node << " GetPositionIs Error!\n" << endl;
		return false;
	}
	return true;
}

WORD Position::calculateUnit(WORD node)
{
	return (max_qc[node - 1] - min_qc[node - 1]) / (max_degree[node - 1] - min_degree[node - 1]);;
}

bool Position::Overstep(WORD node, int degree)
{
	if (degree > max_degree[node - 1] || degree < min_degree[node - 1]) return 0;
	else return 1;
}

bool Position::Rotate(WORD node, int degree)
{
	if (Overstep(node, degree) == 1) {
		log_->Add(logWARNING) <<"Node "<<node<< "Error in Rotate\n" << endl;
		return 0;
	}
	else
	{
		if (VCS_GetPositionIs(Handle, node, &Node1_StartPos, &ErrorCode))
		{
			if (!VCS_MoveToPosition(Handle, node, degree*calculateUnit(node), Radio, Immediately, &ErrorCode))
			{
				//ShowErrorInformation(ErrorCode);
				return 0;
			}
		}
		return 1;
	}
}

bool Position::On(WORD node)
{
	BOOL oFault = FALSE;
	if (!VCS_GetFaultState(Handle, node, &oFault, &ErrorCode))
	{
		//ShowErrorInformation(ErrorCode);
		return 0;
	}
	if (oFault)
	{
		if (!VCS_ClearFault(Handle, node, &ErrorCode))
		{
			//ShowErrorInformation(ErrorCode);
			return 0;
		}
	}

	if (!VCS_SetEnableState(Handle, node, &ErrorCode))
	{
		//ShowErrorInformation(ErrorCode);
		return 0;
	}
	return 1;
}

bool Position::Off(WORD node)
{
	//UpdateNodeId();
	if (!VCS_SetDisableState(Handle,node,&ErrorCode))
	{
		//ShowErrorInformation(ErrorCode);
		return 0;
	}
	return 1;
}
