#include "depend.h"
//#pragma execution_character_set("utf-8")
BOOL Dependency::OpenDevice()
{
	HANDLE hNewKeyHandle;
	//Close Previous Device
	if (m_KeyHandle)
	{
		if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
		m_KeyHandle = 0;
	}
	hNewKeyHandle = VCS_OpenDeviceDlg(&m_ulErrorCode);
	if (hNewKeyHandle)
	{
		m_KeyHandle = hNewKeyHandle;

		//Clear Error History
		if (VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
		{
			//Read Operation Mode
			if (VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode))
			{
				//Read Position Profile Objects
				if (VCS_GetPositionProfile(m_KeyHandle, m_usNodeId, &m_ulProfileVelocity, &m_ulProfileAcceleration, &m_ulProfileDeceleration, &m_ulErrorCode))
				{
					//Write Profile Position Mode
					if (VCS_SetOperationMode(m_KeyHandle, m_usNodeId, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
					{
						//Write Profile Position Objects
						if (VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, 100, 1000, 1000, &m_ulErrorCode))
						{
							//Read Actual Position
							if (VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lStartPosition, &m_ulErrorCode))
							{
								return true;
							}
						}
					}
				}
			}
		}
		ShowErrorInformation(m_ulErrorCode);
	}
	else
	{
		QMessageBox::about(NULL, "Information", "Can't open device!");
	}

	return FALSE;
}

BOOL Dependency::RestoreEPOS()
{
	//Restore Settings
	if (m_oInitialisation)
	{
		VCS_SetDisableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode);
		VCS_SetOperationMode(m_KeyHandle, m_usNodeId, m_bMode, &m_ulErrorCode);
		VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode);
	}
	//Stop Updating
	StopTimer();
	//Close Device
	if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
	return TRUE;
}

BOOL Dependency::UpdateStatus()
{
	//QMessageBox::about(NULL, "Information", "UpdateStatus");
	BOOL oEnable = TRUE;
	BOOL oResult = m_oUpdateActive;
	if (m_oRadio == 0)
	{
		emit RadioSignal(0);
		//m_Move.SetWindowText("&Move Relative");
	}
	else
	{
		emit RadioSignal(1);
		//m_Move.SetWindowText("&Move Absolute");
	}

	if (oResult)
	{
		oResult = VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode);
		if (oResult)
		{
			switch (m_bMode)
			{
			case -6: m_strActiveMode = "Step/Direction Mode"; break;
			case -5: m_strActiveMode = "Master Encoder Mode"; break;
			case -3: m_strActiveMode = "Current Mode"; break;
			case -2: m_strActiveMode = "Velocity Mode"; break;
			case -1: m_strActiveMode = "Position Mode"; break;
			case 1: m_strActiveMode = "Profile Position Mode"; break;
			case 3: m_strActiveMode = "Profile Velocity Mode"; break;
			case 6: m_strActiveMode = "Homing Mode"; break;
			case 7: m_strActiveMode = "Interpolated Position Mode"; break;
			default: m_strActiveMode = "Unknown Mode";
			}
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			m_strActiveMode = "Unknown Mode";
		}
	}
	else
	{
		m_strActiveMode = "Unknown Mode";
	}
	emit OperateModeSignal(m_strActiveMode);
	if (oResult)
	{
		oResult = VCS_GetEnableState(m_KeyHandle, m_usNodeId, &oEnable, &m_ulErrorCode);//oEnable 1: Device enabled 0: Device not enabled
		if (oResult)
		{
			emit EnableSignal(oEnable);
			//QMessageBox::about(NULL, "Information", "can recive enable state");
			/*m_DeviceSettings.EnableWindow(!oEnable);
			m_Enable.EnableWindow(!oEnable);
			m_Disable.EnableWindow(oEnable);
			m_Move.EnableWindow(oEnable);
			m_Halt.EnableWindow(oEnable);*/
		}
		else
		{
			QMessageBox::about(NULL, "Information", "cant recive enable state");
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			emit EnableSignal(!oEnable);
			/*m_DeviceSettings.EnableWindow(oEnable);
			m_Enable.EnableWindow(oEnable);
			m_Disable.EnableWindow(!oEnable);
			m_Move.EnableWindow(!oEnable);
			m_Halt.EnableWindow(!oEnable);*/
		}
	}
	else
	{
		emit UnconnectedSignal();
		/*m_DeviceSettings.EnableWindow(oEnable);
		m_Enable.EnableWindow(!oEnable);
		m_Disable.EnableWindow(!oEnable);
		m_Move.EnableWindow(!oEnable);
		m_Halt.EnableWindow(!oEnable);*/
	}
	if (oResult)
	{
		oResult = VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lActualValue, &m_ulErrorCode);
		if (!oResult)
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			m_lActualValue = 0;
			m_lStartPosition = 0;
		}
	}
	else
	{
		m_lActualValue = 0;
		m_lStartPosition = 0;
	}
	emit ActualPosSignal(m_lActualValue);
	//if (m_hWnd) UpdateData(false);
	//速度模式参数监测
	if (m_SpeedMode1)
	{
		VCS_GetVelocityIs(m_KeyHandle, M1_Node, &m_lActuaSpeed1, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M1_Node, &m_lActuaCurrent1, &m_ulErrorCode);
		emit SpeedSignal1(m_lStartSpeed1, m_lActuaSpeed1, m_lActuaCurrent1);
		if (m_lActuaCurrent1 > m_ThresholdCurrent1)
		{
			if (!VCS_HaltVelocityMovement(m_KeyHandle, M1_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			if (!VCS_SetDisableState(m_KeyHandle, M1_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			m_SpeedMode1 = FALSE;
		}

	}
	if (m_SpeedMode2)
	{
		VCS_GetVelocityIs(m_KeyHandle, M2_Node, &m_lActuaSpeed2, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M2_Node, &m_lActuaCurrent2, &m_ulErrorCode);
		emit SpeedSignal2(m_lStartSpeed2, m_lActuaSpeed2, m_lActuaCurrent2);
		if (m_lActuaCurrent2 > m_ThresholdCurrent2)
		{
			if (!VCS_HaltVelocityMovement(m_KeyHandle, M2_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			if (!VCS_SetDisableState(m_KeyHandle, M2_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			m_SpeedMode2 = FALSE;
		}

	}
	if (m_SpeedMode3)
	{
		VCS_GetVelocityIs(m_KeyHandle, M3_Node, &m_lActuaSpeed3, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M3_Node, &m_lActuaCurrent3, &m_ulErrorCode);
		emit SpeedSignal3(m_lStartSpeed3, m_lActuaSpeed3, m_lActuaCurrent3);
		if (m_lActuaCurrent3 > m_ThresholdCurrent3)
		{
			if (!VCS_HaltVelocityMovement(m_KeyHandle, M3_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			if (!VCS_SetDisableState(m_KeyHandle, M3_Node, &m_ulErrorCode))
			{
				ShowErrorInformation(m_ulErrorCode);
			}
			m_SpeedMode3 = FALSE;
		}

	}
	if (m_TeachMode)
	{
		VCS_GetVelocityIs(m_KeyHandle, M1_Node, &m_lActuaSpeed1, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M1_Node, &m_lActuaCurrent1, &m_ulErrorCode);

		VCS_GetVelocityIs(m_KeyHandle, M2_Node, &m_lActuaSpeed2, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M2_Node, &m_lActuaCurrent2, &m_ulErrorCode);

		VCS_GetVelocityIs(m_KeyHandle, M3_Node, &m_lActuaSpeed3, &m_ulErrorCode);
		VCS_GetCurrentIsAveraged(m_KeyHandle, M3_Node, &m_lActuaCurrent3, &m_ulErrorCode);

		emit TeachModeDateSignal(m_lActuaSpeed1, m_lActuaCurrent1, m_lActuaSpeed2, m_lActuaCurrent2, m_lActuaSpeed3, m_lActuaCurrent3);
	}
	if (m_Recurring)
	{
		VCS_GetPositionIs(m_KeyHandle, M1_Node, &m_lActualValue1, &m_ulErrorCode);
		VCS_GetPositionIs(m_KeyHandle, M2_Node, &m_lActualValue2, &m_ulErrorCode);
		VCS_GetPositionIs(m_KeyHandle, M3_Node, &m_lActualValue3, &m_ulErrorCode);
		if (abs(m_lActualValue1- m_lTargetPosition1)<2000&& abs(m_lActualValue2 - m_lTargetPosition2) < 2000 && abs(m_lActualValue3 - m_lTargetPosition3) < 2000 )
		{
			m_Recurring = FALSE;
			emit RecurringDoneSignal();
		}
	}
	return oResult;
}

void Dependency::SpeedModeStart1( long speed, short threshold_current)
{
	m_SpeedMode1 = TRUE;
	m_ThresholdCurrent1 = threshold_current;
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, M1_Node, &m_ulErrorCode))
	{
		//Read Operation Mode
		if (VCS_GetOperationMode(m_KeyHandle, M1_Node, &m_bMode, &m_ulErrorCode))
		{
			//Read v Profile Objects
			if (VCS_GetVelocityProfile(m_KeyHandle, M1_Node, &m_ulProfileAcceleration1, &m_ulProfileDeceleration1, &m_ulErrorCode))
			{
				//Write Profile v Mode
				if (VCS_SetOperationMode(m_KeyHandle, M1_Node, OMD_PROFILE_VELOCITY_MODE, &m_ulErrorCode))
				{
					//Write Profile v Objects
					if (VCS_SetVelocityProfile(m_KeyHandle, M1_Node, m_ulProfileAcceleration1, m_ulProfileDeceleration1, &m_ulErrorCode))
					{
						//Read Actual v
						if (VCS_GetVelocityIs(m_KeyHandle, M1_Node, &m_lStartSpeed1, &m_ulErrorCode))
						{
							BOOL oFault = FALSE;
							if (!VCS_GetFaultState(m_KeyHandle, M1_Node, &oFault, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
								return;
							}
							if (oFault)
							{
								if (!VCS_ClearFault(m_KeyHandle, M1_Node, &m_ulErrorCode))
								{
									ShowErrorInformation(m_ulErrorCode);
									return;
								}
							}
							if (!VCS_SetEnableState(m_KeyHandle, M1_Node, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							if (!VCS_MoveWithVelocity(m_KeyHandle, M1_Node, speed, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							UpdateStatus();
							return;
						}
					}
				}
			}
		}
	}
}

void Dependency::SpeedModeStart2(long speed, short threshold_current)
{
	m_SpeedMode2 = TRUE;
	m_ThresholdCurrent2 = threshold_current;
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, M2_Node, &m_ulErrorCode))
	{
		//Read Operation Mode
		if (VCS_GetOperationMode(m_KeyHandle, M2_Node, &m_bMode, &m_ulErrorCode))
		{
			//Read v Profile Objects
			if (VCS_GetVelocityProfile(m_KeyHandle, M2_Node, &m_ulProfileAcceleration2, &m_ulProfileDeceleration2, &m_ulErrorCode))
			{
				//Write Profile v Mode
				if (VCS_SetOperationMode(m_KeyHandle, M2_Node, OMD_PROFILE_VELOCITY_MODE, &m_ulErrorCode))
				{
					//Write Profile v Objects
					if (VCS_SetVelocityProfile(m_KeyHandle, M2_Node, m_ulProfileAcceleration2, m_ulProfileDeceleration2, &m_ulErrorCode))
					{
						//Read Actual v
						if (VCS_GetVelocityIs(m_KeyHandle, M2_Node, &m_lStartSpeed2, &m_ulErrorCode))
						{
							BOOL oFault = FALSE;
							if (!VCS_GetFaultState(m_KeyHandle, M2_Node, &oFault, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
								return;
							}
							if (oFault)
							{
								if (!VCS_ClearFault(m_KeyHandle, M2_Node, &m_ulErrorCode))
								{
									ShowErrorInformation(m_ulErrorCode);
									return;
								}
							}
							if (!VCS_SetEnableState(m_KeyHandle, M2_Node, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							if (!VCS_MoveWithVelocity(m_KeyHandle, M2_Node, speed, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							UpdateStatus();
							return;
						}
					}
				}
			}
		}
	}
}

void Dependency::SpeedModeStart3(long speed, short threshold_current)
{
	m_SpeedMode3 = TRUE;
	m_ThresholdCurrent3 = threshold_current;
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, M3_Node, &m_ulErrorCode))
	{
		//Read Operation Mode
		if (VCS_GetOperationMode(m_KeyHandle, M3_Node, &m_bMode, &m_ulErrorCode))
		{
			//Read v Profile Objects
			if (VCS_GetVelocityProfile(m_KeyHandle, M3_Node, &m_ulProfileAcceleration3, &m_ulProfileDeceleration3, &m_ulErrorCode))
			{
				//Write Profile v Mode
				if (VCS_SetOperationMode(m_KeyHandle, M3_Node, OMD_PROFILE_VELOCITY_MODE, &m_ulErrorCode))
				{
					//Write Profile v Objects
					if (VCS_SetVelocityProfile(m_KeyHandle, M3_Node, m_ulProfileAcceleration3, m_ulProfileDeceleration3, &m_ulErrorCode))
					{
						//Read Actual v
						if (VCS_GetVelocityIs(m_KeyHandle, M3_Node, &m_lStartSpeed3, &m_ulErrorCode))
						{
							BOOL oFault = FALSE;
							if (!VCS_GetFaultState(m_KeyHandle, M3_Node, &oFault, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
								return;
							}
							if (oFault)
							{
								if (!VCS_ClearFault(m_KeyHandle, M3_Node, &m_ulErrorCode))
								{
									ShowErrorInformation(m_ulErrorCode);
									return;
								}
							}
							if (!VCS_SetEnableState(m_KeyHandle, M3_Node, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							if (!VCS_MoveWithVelocity(m_KeyHandle, M3_Node, speed, &m_ulErrorCode))
							{
								ShowErrorInformation(m_ulErrorCode);
							}
							UpdateStatus();
							return;
						}
					}
				}
			}
		}
	}
}

void Dependency::SpeedModeHalt1()
{
	if (!VCS_HaltVelocityMovement(m_KeyHandle, M1_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}

	if (!VCS_SetDisableState(m_KeyHandle, M1_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	m_SpeedMode1 = FALSE;
}

void Dependency::SpeedModeHalt2()
{
	if (!VCS_HaltVelocityMovement(m_KeyHandle, M2_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}

	if (!VCS_SetDisableState(m_KeyHandle, M2_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	m_SpeedMode2 = FALSE;
}

void Dependency::SpeedModeHalt3()
{
	if (!VCS_HaltVelocityMovement(m_KeyHandle, M3_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}

	if (!VCS_SetDisableState(m_KeyHandle, M3_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	m_SpeedMode3 = FALSE;
}

void Dependency::MotionPara1(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity1 = ProfileVelocity;
	m_ulProfileAcceleration1 = ProfileAcceleration;
	m_ulProfileDeceleration1 = ProfileDeceleration;
	m_Amaxvelocity1 = Amaxvelocity;
	m_AmaxAcceleration1 = AmaxAcceleration;
	m_AmaxFollowingError1 = AmaxFollowingError;
	VCS_SetMaxFollowingError(m_KeyHandle, M1_Node, m_AmaxFollowingError1, &m_ulErrorCode);
	VCS_SetMaxProfileVelocity(m_KeyHandle, M1_Node, m_Amaxvelocity1, &m_ulErrorCode);
	VCS_SetMaxAcceleration(m_KeyHandle, M1_Node, m_AmaxAcceleration1, &m_ulErrorCode);
	emit MotionParaSettingSuccessSignal1();
}

void Dependency::MotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity2 = ProfileVelocity;
	m_ulProfileAcceleration2 = ProfileAcceleration;
	m_ulProfileDeceleration2 = ProfileDeceleration;
	m_Amaxvelocity2 = Amaxvelocity;
	m_AmaxAcceleration2 = AmaxAcceleration;
	m_AmaxFollowingError2 = AmaxFollowingError;
	VCS_SetMaxFollowingError(m_KeyHandle, M2_Node, m_AmaxFollowingError2, &m_ulErrorCode);
	VCS_SetMaxProfileVelocity(m_KeyHandle, M2_Node, m_Amaxvelocity2, &m_ulErrorCode);
	VCS_SetMaxAcceleration(m_KeyHandle, M2_Node, m_AmaxAcceleration2, &m_ulErrorCode);
	emit MotionParaSettingSuccessSignal2();
}

void Dependency::MotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity3 = ProfileVelocity;
	m_ulProfileAcceleration3 = ProfileAcceleration;
	m_ulProfileDeceleration3 = ProfileDeceleration;
	m_Amaxvelocity3 = Amaxvelocity;
	m_AmaxAcceleration3 = AmaxAcceleration;
	m_AmaxFollowingError3 = AmaxFollowingError;
	VCS_SetMaxFollowingError(m_KeyHandle, M3_Node, m_AmaxFollowingError3, &m_ulErrorCode);
	VCS_SetMaxProfileVelocity(m_KeyHandle, M3_Node, m_Amaxvelocity3, &m_ulErrorCode);
	VCS_SetMaxAcceleration(m_KeyHandle, M3_Node, m_AmaxAcceleration3, &m_ulErrorCode);
	emit MotionParaSettingSuccessSignal3();
}

void Dependency::Node1Move(DWORD pos)
{
	
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, 1, &m_ulErrorCode))
	{
		//Write Profile Position Mode
		if (VCS_SetOperationMode(m_KeyHandle, 1, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
		{
			//Write Profile Position Objects
			if (VCS_SetPositionProfile(m_KeyHandle, 1, m_ulProfileVelocity1, m_ulProfileAcceleration1, m_ulProfileDeceleration1, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetPositionIs(m_KeyHandle, 1, &m_lStartPosition, &m_ulErrorCode))
				{
					BOOL oFault = FALSE;
					if (!VCS_GetFaultState(m_KeyHandle, 1, &oFault, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
						return;
					}
					if (oFault)
					{
						if (!VCS_ClearFault(m_KeyHandle, 1, &m_ulErrorCode))
						{
							ShowErrorInformation(m_ulErrorCode);
							return;
						}
					}
					if (!VCS_SetEnableState(m_KeyHandle, 1, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					//QMessageBox::about(NULL, "", QString::number(pos));
					if (!VCS_MoveToPosition(m_KeyHandle, 1, pos, 1, m_oImmediately, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					UpdateStatus();
					return;
				}
			}
		}
	}
	ShowErrorInformation(m_ulErrorCode);
}

void Dependency::Node2Move(DWORD pos)
{
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, 2, &m_ulErrorCode))
	{
		//Write Profile Position Mode
		if (VCS_SetOperationMode(m_KeyHandle, 2, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
		{
			//Write Profile Position Objects
			if (VCS_SetPositionProfile(m_KeyHandle, 2, m_ulProfileVelocity2, m_ulProfileAcceleration2, m_ulProfileDeceleration2, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetPositionIs(m_KeyHandle, 2, &m_lStartPosition, &m_ulErrorCode))
				{
					BOOL oFault = FALSE;
					if (!VCS_GetFaultState(m_KeyHandle, 2, &oFault, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
						return;
					}
					if (oFault)
					{
						if (!VCS_ClearFault(m_KeyHandle, 2, &m_ulErrorCode))
						{
							ShowErrorInformation(m_ulErrorCode);
							return;
						}
					}
					if (!VCS_SetEnableState(m_KeyHandle, 2, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					//QMessageBox::about(NULL, "", QString::number(pos));
					if (!VCS_MoveToPosition(m_KeyHandle, 2, pos, 1, m_oImmediately, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					UpdateStatus();
					return;
				}
			}
		}
	}
	ShowErrorInformation(m_ulErrorCode);
}

void Dependency::Node3Move(DWORD pos)
{
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, 3, &m_ulErrorCode))
	{
		//Write Profile Position Mode
		if (VCS_SetOperationMode(m_KeyHandle, 3, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
		{
			//Write Profile Position Objects
			if (VCS_SetPositionProfile(m_KeyHandle, 3, m_ulProfileVelocity3, m_ulProfileAcceleration3, m_ulProfileDeceleration3, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetPositionIs(m_KeyHandle, 3, &m_lStartPosition, &m_ulErrorCode))
				{
					BOOL oFault = FALSE;
					if (!VCS_GetFaultState(m_KeyHandle, 3, &oFault, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
						return;
					}
					if (oFault)
					{
						if (!VCS_ClearFault(m_KeyHandle, 3, &m_ulErrorCode))
						{
							ShowErrorInformation(m_ulErrorCode);
							return;
						}
					}
					if (!VCS_SetEnableState(m_KeyHandle, 3, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					//QMessageBox::about(NULL, "", QString::number(pos));
					if (!VCS_MoveToPosition(m_KeyHandle, 3, pos, 1, m_oImmediately, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					UpdateStatus();
					return;
				}
			}
		}
	}
	ShowErrorInformation(m_ulErrorCode);
}

void Dependency::InTeachPage()
{
	m_TeachMode = TRUE;
}

void Dependency::NoInTeachPage()
{
	m_TeachMode = FALSE;
}

void Dependency::TeachReadPos()
{
	long node1_pos=0, node2_pos=0, node3_pos=0;
	VCS_GetPositionIs(m_KeyHandle, M1_Node, &node1_pos, &m_ulErrorCode);
	VCS_GetPositionIs(m_KeyHandle, M2_Node, &node2_pos, &m_ulErrorCode);
	VCS_GetPositionIs(m_KeyHandle, M3_Node, &node3_pos, &m_ulErrorCode);
	emit TeachModePosSignal(node1_pos, node2_pos, node3_pos);
}

void Dependency::TeachRecurring(long node1_pos, long node2_pos, long node3_pos)
{
	m_Recurring = TRUE;
	m_lTargetPosition1 = node1_pos;
	m_lTargetPosition2 = node2_pos;
	m_lTargetPosition3 = node3_pos;
	Node1Move(node1_pos);
	Node2Move(node2_pos);
	Node3Move(node3_pos);
}

void Dependency::TeachHalt()
{
	if (!VCS_HaltPositionMovement(m_KeyHandle, M1_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	if (!VCS_HaltPositionMovement(m_KeyHandle, M2_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
	if (!VCS_HaltPositionMovement(m_KeyHandle, M3_Node, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

BOOL Dependency::ShowErrorInformation(DWORD p_ulErrorCode)
{
	char* pStrErrorInfo;
	QString strDescription;

	if ((pStrErrorInfo = (char*)malloc(100)) == NULL)
	{
		QMessageBox::about(NULL, "Information", "Not enough memory to allocate buffer for error information string!");
		return FALSE;
	}

	if (VCS_GetErrorInfo(p_ulErrorCode, pStrErrorInfo, 100))
	{
		strDescription = pStrErrorInfo;
		QMessageBox::about(NULL, "Information", strDescription);
		free(pStrErrorInfo);
		return TRUE;
	}
	else
	{
		free(pStrErrorInfo);
		QMessageBox::about(NULL, "Information", "Error information can't be read!");
		return FALSE;
	}
}

void Dependency::StopTimer()
{
	if(timer!=NULL)
	if (timer->isActive()) {
		timer->stop();
	}
	m_oUpdateActive = FALSE;
	UpdateStatus();
}

void Dependency::UpdateNodeIdString()
{
	const size_t size(3);
	char strNodeId[size];
	_itoa(m_usNodeId, strNodeId, 10);
	m_strNodeId = strNodeId;
}

void Dependency::Disable()
{
	//QMessageBox::about(NULL, "TIP", "recive disable signal");
	UpdateNodeIdString();
	if (!VCS_SetDisableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

void Dependency::Enable()
{
	//QMessageBox::about(NULL, "TIP", "recive enable signal");
	BOOL oFault = FALSE;
	UpdateNodeIdString();
	if (!VCS_GetFaultState(m_KeyHandle, m_usNodeId, &oFault, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
		return;
	}
	if (oFault)
	{
		if (!VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
		{
			ShowErrorInformation(m_ulErrorCode);
			return;
		}
	}
	if (!VCS_SetEnableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

void Dependency::Move()
{
	//QMessageBox::about(NULL, "TIP", "recive move signal");
	UpdateNodeIdString();
	if (VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lStartPosition, &m_ulErrorCode))
	{
		emit StartPosSignal(m_lStartPosition);
		if (!VCS_MoveToPosition(m_KeyHandle, m_usNodeId, m_lTargetPosition, m_oRadio, m_oImmediately, &m_ulErrorCode))
		{
			ShowErrorInformation(m_ulErrorCode);
		}
	}
	UpdateStatus();
}

void Dependency::Halt()
{
	//QMessageBox::about(NULL, "TIP", "recive halt signal");
	UpdateNodeIdString();
	if (!VCS_HaltPositionMovement(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

void Dependency::Setting()
{
	//QMessageBox::about(NULL, "TIP", "recive setting signal");
	UpdateNodeIdString();
	if (OpenDevice())
	{
		m_oInitialisation = TRUE;
		timer->start(100);
		m_oUpdateActive = TRUE;
	}
	else
	{
		m_oInitialisation = FALSE;
		StopTimer();
	}
}

void Dependency::IDupdate(WORD id)
{
	QMessageBox::about(NULL, "Information", "ID recive");
	m_usNodeId = id;
}

void Dependency::Posupdate(long pos)
{
	QMessageBox::about(NULL, "Information", "Pos recive");
	m_lTargetPosition = pos;
}

void Dependency::RadioRelative()
{
	QMessageBox::about(NULL, "Information", "relative recive");
	m_oRadio = 0;
}

void Dependency::RadioAbsolute()
{
	QMessageBox::about(NULL, "Information", "absolute recive");
	m_oRadio = 1;
}

Dependency::Dependency()
{
	//Settings
	m_oImmediately = TRUE;
	m_oUpdateActive = FALSE;
	m_usNodeId = 1;
	//OpenDevice
	if (OpenDevice())
	{
		m_oInitialisation = TRUE;
		timer = new QTimer(this);
		timer->start(1000);
		connect(timer, SIGNAL(timeout()), this, SLOT(UpdateStatus()));
		m_oUpdateActive = TRUE;
		QMessageBox::about(NULL, "Information", QString::fromLocal8Bit("已开启定时器"));
	}
	else
	{
		m_oInitialisation = FALSE;
		//m_Enable.EnableWindow(false/ture);
	}
}

Dependency::~Dependency()
{
	RestoreEPOS();
	if(timer!=nullptr)
	delete timer;
	timer = nullptr;
}
