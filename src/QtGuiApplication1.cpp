#include "QtGuiApplication1.h"
#pragma execution_character_set("utf-8")
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
	connect(ui.ToSetting, SIGNAL(clicked()), this, SLOT(ToSetting_Clicked()));
	connect(ui.ToTest, SIGNAL(clicked()), this, SLOT(ToTest_Clicked()));
	connect(ui.ToRecord, SIGNAL(clicked()), this, SLOT(ToRecord_Clicked()));
	connect(ui.ToCoordinate, SIGNAL(clicked()), this, SLOT(ToCoordinate_Clicked()));
	connect(ui.Exit, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(CurrentIndexChange(int)));
	//�˵���
	menu_operate = new QAction(QIcon(":/images/doc-open"), tr("&Open..."), this);
	menu_operate->setShortcuts(QKeySequence::Open);
	menu_operate->setStatusTip(tr("�����ļ������뱣��"));
	//connect(menu_operate, &QAction::triggered, this, QtGuiApplication1::open);
	QMenu *file = menuBar()->addMenu(tr("&����"));
	menuBar()->addMenu(tr("&����"));
	menuBar()->addMenu(tr("&����"));
	file->addAction(menu_operate);
	//this->setWindowFlags(Qt::FramelessWindowHint);//�����޲˵���
	ui.IDEdit->setValidator(new QIntValidator(0, 128, this));//ֻ������������
	
	//Maxon�ؼ�
	connect(ui.Disable, SIGNAL(clicked()),this, SLOT(Disable_Clicked()));
	connect(ui.Enable, SIGNAL(clicked()), this, SLOT(Enable_Clicked()));
	connect(ui.Move, SIGNAL(clicked()), this, SLOT(Move_Clicked()));
	connect(ui.Halt, SIGNAL(clicked()), this, SLOT(Halt_Clicked()));
	connect(ui.Setting, SIGNAL(clicked()), this, SLOT(Set_Clicked()));
	connect(ui.IDEdit, SIGNAL(editingFinished()), this, SLOT(ID_changed()));
	connect(ui.TargetEdit, SIGNAL(editingFinished()), this, SLOT(Pos_Send()));
	connect(ui.Relative, SIGNAL(clicked()), this, SLOT(Rel_Select()));
	connect(ui.Absolute, SIGNAL(clicked()), this, SLOT(Abs_Select()));
	connect(ui.Exit, SIGNAL(clicked()), this, SLOT(Exit_Clicked()));
	connect(ui.Node1MotionPara, SIGNAL(clicked()), this, SLOT(Node1MotionPara_Clicked()));
	connect(ui.Node2MotionPara, SIGNAL(clicked()), this, SLOT(Node2MotionPara_Clicked()));
	connect(ui.Node3MotionPara, SIGNAL(clicked()), this, SLOT(Node3MotionPara_Clicked()));
	//Maxon Teach
	connect(ui.Node1_dial, SIGNAL(sliderMoved(int)), this, SLOT(Node1DialMove(int)));
	connect(ui.Node2_dial, SIGNAL(sliderMoved(int)), this, SLOT(Node2DialMove(int)));
	connect(ui.Node3_dial, SIGNAL(sliderMoved(int)), this, SLOT(Node3DialMove(int)));
	connect(ui.Recurring_UrgencyHalt, SIGNAL(clicked()), this, SLOT(Recurring_UrgencyHalt_clicked()));
	//Velocity Mode
	connect(ui.SpeedMove1, SIGNAL(clicked()), this, SLOT(SpeedMove1_Clicked()));
	connect(ui.SpeedMove2, SIGNAL(clicked()), this, SLOT(SpeedMove2_Clicked()));
	connect(ui.SpeedMove3, SIGNAL(clicked()), this, SLOT(SpeedMove3_Clicked()));
	connect(ui.SpeedHalf1, SIGNAL(clicked()), this, SLOT(SpeedHalf1Clecked()));
	connect(ui.SpeedHalf2, SIGNAL(clicked()), this, SLOT(SpeedHalf2Clecked()));
	connect(ui.SpeedHalf3, SIGNAL(clicked()), this, SLOT(SpeedHalf3Clecked()));
	//dynamixel timer
	QTimer *timer1 = new QTimer(this);
	timer1->start(1000);
	QTimer *move_clicked_timer = new QTimer(this);
	connect(move_clicked_timer, SIGNAL(timeout()), this, SLOT(Move_clicked_Timer()));
	//�����߳�
	Running *readpos_running = new Running;
	readpos_running->moveToThread(&running_thread);
	connect(&running_thread, &QThread::finished, readpos_running, &QObject::deleteLater);
	connect(this, &QtGuiApplication1::MoveStart, readpos_running, &Running::ReadPosInRunning);
	connect(readpos_running, &Running::GetPresentSingnal, this, &QtGuiApplication1::Emit_Pre_Pos);
	connect(this, &QtGuiApplication1::Send_Pre_Pos, readpos_running, &Running::GetPresentPos);
	connect(readpos_running, &Running::PresentPos, this, &QtGuiApplication1::Show_Pos_In_Running);
	connect(readpos_running, &Running::Done, this, &QtGuiApplication1::Running_Done);
	connect(this, &QtGuiApplication1::MaxonRecurringDoneSignal, readpos_running, &Running::MaxonIsDone);
	//connect(this, &MoveStop, readpos_running, &Worker::doWork);
	running_thread.start();
	//dynamixel�ؼ�
	connect(timer1, SIGNAL(timeout()), this, SLOT(ComScanTimeout()));
	connect(ui.Connect, SIGNAL(clicked()), this, SLOT(Connect_clicked()));
	connect(ui.MoveButton, SIGNAL(clicked()), this, SLOT(movebutton()));
	connect(ui.ID1_dial, SIGNAL(sliderMoved(int)), this, SLOT(ID1_move(int)));
	connect(ui.ID2_dial, SIGNAL(sliderMoved(int)), this, SLOT(ID2_move(int)));
	connect(ui.ID3_dial, SIGNAL(sliderMoved(int)), this, SLOT(ID3_move(int)));
	connect(ui.ID4_dial, SIGNAL(sliderMoved(int)), this, SLOT(ID4_move(int)));
	connect(ui.GetPos, SIGNAL(clicked()), this, SLOT(GetPos_clicked()));
	connect(ui.Delete_Pos, SIGNAL(clicked()), this, SLOT(DeletePos_clicked()));
	connect(ui.Recurring, SIGNAL(clicked()), this, SLOT(Recurring_clicked()));
	connect(ui.LoopRecurring, SIGNAL(clicked()), this, SLOT(LoopRecurring_clicked()));
	connect(ui.Recurring_Halt, SIGNAL(clicked()), this, SLOT(Recurring_Halt_clicked()));
	connect(ui.enableTorque, SIGNAL(clicked()), this, SLOT(Enable_Torque_clicked()));
	connect(ui.disableTorque, SIGNAL(clicked()), this, SLOT(Disable_Torque_clicked()));
	connect(ui.ID1ParaWrite, SIGNAL(clicked()), this, SLOT(ID1_Para_Write_clicked()));
	connect(ui.ID2ParaWrite, SIGNAL(clicked()), this, SLOT(ID2_Para_Write_clicked()));
	connect(ui.ID3ParaWrite, SIGNAL(clicked()), this, SLOT(ID3_Para_Write_clicked()));
	connect(ui.ID4ParaWrite, SIGNAL(clicked()), this, SLOT(ID4_Para_Write_clicked()));
	connect(ui.ID1TorgueEnable, SIGNAL(clicked()), this, SLOT(ID1_Torgue_Enable_clicked()));
	connect(ui.ID2TorgueEnable, SIGNAL(clicked()), this, SLOT(ID2_Torgue_Enable_clicked()));
	connect(ui.ID3TorgueEnable, SIGNAL(clicked()), this, SLOT(ID3_Torgue_Enable_clicked()));
	connect(ui.ID4TorgueEnable, SIGNAL(clicked()), this, SLOT(ID4_Torgue_Enable_clicked()));
	connect(ui.ID1TorgueDisable, SIGNAL(clicked()), this, SLOT(ID1_Torgue_Disable_clicked()));
	connect(ui.ID2TorgueDisable, SIGNAL(clicked()), this, SLOT(ID2_Torgue_Disable_clicked()));
	connect(ui.ID3TorgueDisable, SIGNAL(clicked()), this, SLOT(ID3_Torgue_Disable_clicked()));
	connect(ui.ID4TorgueDisable, SIGNAL(clicked()), this, SLOT(ID4_Torgue_Disable_clicked()));
	connect(ui.ID1Move, SIGNAL(clicked()), this, SLOT(ID1_Move_clicked()));
	connect(ui.ID2Move, SIGNAL(clicked()), this, SLOT(ID2_Move_clicked()));
	connect(ui.ID3Move, SIGNAL(clicked()), this, SLOT(ID3_Move_clicked()));
	connect(ui.ID4Move, SIGNAL(clicked()), this, SLOT(ID4_Move_clicked()));
	//pos view
	//ui.PosView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.PosView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui.PosView->setRowCount(table_row);
	ui.PosView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");//ͨ����ʽ�����ñ�ͷ����ɫ
	//connect(ui.PosView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(getItem(QTreeWidgetItem*, int)));//��ȡѡ�еĵ�Ԫ������
}

void QtGuiApplication1::Disable_Clicked()
{
	emit DisableSignal();
	return;
}

QtGuiApplication1::~QtGuiApplication1()
{
	running_thread.exit();
	running_thread.wait(10 * 1000);
	if (control != nullptr)
		control=nullptr;
	if (timer1 != nullptr)
	{
		delete timer1;
		timer1 = nullptr;
	}
	if (move_clicked_timer != nullptr)
	{
		delete move_clicked_timer;
		move_clicked_timer = nullptr;
	}
}

void QtGuiApplication1::Dy_ComPort()
{
	ui.comboBoxComPort->clear();
	m_serialPortName.clear();
	QList<QSerialPortInfo> m_list = QSerialPortInfo::availablePorts();
	for (int i = 0; i < m_list.count(); i++)
	{
		m_serialPortName << m_list.at(i).portName();
		ui.comboBoxComPort->addItem(m_list.at(i).portName());
	}
	return;
}

void QtGuiApplication1::init(DynaxicelBase * ControlObject)
{
	control = ControlObject;
}

void QtGuiApplication1::ToSetting_Clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void QtGuiApplication1::ToTest_Clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void QtGuiApplication1::ToRecord_Clicked()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void QtGuiApplication1::ToCoordinate_Clicked()
{
	ui.stackedWidget->setCurrentIndex(3);
}

void QtGuiApplication1::Enable_Clicked()
{
	emit EnableSignal();
	return;
}

void QtGuiApplication1::Move_Clicked()
{
	emit MoveSignal();
	return;
}

void QtGuiApplication1::Exit_Clicked()
{
	emit ExitSignal();
	return;
}

void QtGuiApplication1::Halt_Clicked()
{
	emit HaltSignal();
	return;
}

void QtGuiApplication1::Set_Clicked()
{
	emit SetSignal();
	return;
}

void QtGuiApplication1::ID_changed()
{
	QString id_str = ui.IDEdit->text();
	int id = id_str.toInt();
	//�жϽ���ֵ�Ƿ�䶯
	if (id == pre_id)
		return;
	else
		pre_id = id;
	if(id<=128&& id>0)
	emit IDSignal(id);
	else
	QMessageBox::about(NULL, "Information","ID vale 1~128");
	return;
}

void QtGuiApplication1::Pos_Send()
{
	QString pos_str = ui.TargetEdit->text();
	int pos = pos_str.toInt();
	//�жϽ���ֵ�Ƿ�䶯
	if (pos == pre_pos)
		return;
	else
		pre_pos = pos;
	emit PosSignal(pos);
}

void QtGuiApplication1::Rel_Select()
{
	//QMessageBox::about(NULL, "Information", "emit RelativeSignal");
	emit RelativeSignal();
	return;
}

void QtGuiApplication1::Abs_Select()
{
	//QMessageBox::about(NULL, "Information", "emit AbsoluteSignal");
	emit AbsoluteSignal();
	return;
}

void QtGuiApplication1::Enable_Status(bool enable)
{
		ui.Setting->setEnabled(!enable);
		ui.Enable->setEnabled(!enable);
		ui.Disable->setEnabled(enable);
		ui.Move->setEnabled(enable);
		ui.Halt->setEnabled(enable);	
}

void QtGuiApplication1::Unconnected_Status()
{
	ui.Setting->setEnabled(1);
	ui.Enable->setEnabled(0);
	ui.Disable->setEnabled(0);
	ui.Move->setEnabled(0);
	ui.Halt->setEnabled(0);
}

void QtGuiApplication1::Move_Type(bool type)
{
	if (!type)
		ui.Move->setText("Move Relative");
	else
		ui.Move->setText("Move Absolute");
}

void QtGuiApplication1::Get_StartPos(long pos)
{
	QMessageBox::about(NULL, "tips", "Get_StartPos ");
	QString pos_str;
	pos_str=QString::number(pos, 10);
	ui.StartEdit->setText(pos_str);
}

void QtGuiApplication1::Get_ActualPos(long pos)
{
	//QMessageBox::about(NULL, "tips", "Get_ActualPos ");
	QString pos_str;
	pos_str=QString::number(pos, 10);
	ui.ActualEdit->setText(pos_str);
}

void QtGuiApplication1::Get_OperateMode(QString mod)
{
	ui.label_2->setText(mod);
}

void QtGuiApplication1::Node1MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui.mspeed1->text() == "")
		velo = 200;
	else
		velo = ui.mspeed1->text().toInt();
	//�������ٶ�
	if (ui.macceleration1->text() == "")
		acce = 1000;
	else
		acce = ui.macceleration1->text().toInt();
	//��ֹ���ٶ�
	if (ui.mdeceleration1->text() == "")
		dece = 1000;
	else
		dece = ui.mdeceleration1->text().toInt();
	//��������ٶ�
	if (ui.mmspeed1->text() == "")
		max_velo = 2000;
	else
		max_velo = ui.mmspeed1->text().toInt();
	//���������ٶ�
	if (ui.mmacceleration1->text() == "")
		max_acce = 10000;
	else
		max_acce = ui.mmacceleration1->text().toInt();
	//���������
	if (ui.mmFollowingError1->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui.mmFollowingError1->text().toInt();
	emit SendMotionPara1(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void QtGuiApplication1::Node2MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui.mspeed2->text() == "")
		velo = 200;
	else
		velo = ui.mspeed2->text().toInt();
	//�������ٶ�
	if (ui.macceleration2->text() == "")
		acce = 1000;
	else
		acce = ui.macceleration2->text().toInt();
	//��ֹ���ٶ�
	if (ui.mdeceleration2->text() == "")
		dece = 1000;
	else
		dece = ui.mdeceleration2->text().toInt();
	//��������ٶ�
	if (ui.mmspeed2->text() == "")
		max_velo = 2000;
	else
		max_velo = ui.mmspeed2->text().toInt();
	//���������ٶ�
	if (ui.mmacceleration2->text() == "")
		max_acce = 10000;
	else
		max_acce = ui.mmacceleration2->text().toInt();
	//���������
	if (ui.mmFollowingError2->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui.mmFollowingError2->text().toInt();
	emit SendMotionPara2(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void QtGuiApplication1::Node3MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui.mspeed3->text() == "")
		velo = 200;
	else
		velo = ui.mspeed3->text().toInt();
	//�������ٶ�
	if (ui.macceleration3->text() == "")
		acce = 1000;
	else
		acce = ui.macceleration3->text().toInt();
	//��ֹ���ٶ�
	if (ui.mdeceleration3->text() == "")
		dece = 1000;
	else
		dece = ui.mdeceleration3->text().toInt();
	//��������ٶ�
	if (ui.mmspeed3->text() == "")
		max_velo = 2000;
	else
		max_velo = ui.mmspeed3->text().toInt();
	//���������ٶ�
	if (ui.mmacceleration3->text() == "")
		max_acce = 10000;
	else
		max_acce = ui.mmacceleration3->text().toInt();
	//���������
	if (ui.mmFollowingError3->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui.mmFollowingError3->text().toInt();
	emit SendMotionPara3(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void QtGuiApplication1::SpeedMove1_Clicked()
{
	QString Speed = ui.SpeedNode1->text(), Current=ui.CurrentThreshold1->text();
	if (Speed == "" || Current == "") {
		QMessageBox::about(NULL, "", "�벹ȫ�����ٶȺ͵�����ֵ");
		return;
	}
	int speed_int = Speed.toInt(),current_int= Current.toInt();
	//�жϽ���ֵ�Ƿ�䶯
	if (speed_int <= -99999999 || speed_int >= 99999999 || current_int <= 0 || current_int >= 99999999)
	{
		QMessageBox::about(NULL, "", "��������Чֵ");
		return;
	}
	emit StartVelocityMode1(speed_int,current_int);
}

void QtGuiApplication1::SpeedMove2_Clicked()
{
	QString Speed = ui.SpeedNode2->text(), Current = ui.CurrentThreshold2->text();
	if (Speed == "" || Current == "") {
		QMessageBox::about(NULL, "", "�벹ȫ�����ٶȺ͵�����ֵ");
		return;
	}
	int speed_int = Speed.toInt(), current_int = Current.toInt();
	//�жϽ���ֵ�Ƿ�䶯
	if (speed_int <= -99999999 || speed_int >= 99999999 || current_int <= 0 || current_int >= 99999999)
	{
		QMessageBox::about(NULL, "", "��������Чֵ");
		return;
	}
	emit StartVelocityMode2(speed_int, current_int);
}

void QtGuiApplication1::SpeedMove3_Clicked()
{
	QString Speed = ui.SpeedNode3->text(), Current = ui.CurrentThreshold3->text();
	if (Speed == "" || Current == "") {
		QMessageBox::about(NULL, "", "�벹ȫ�����ٶȺ͵�����ֵ");
		return;
	}
	int speed_int = Speed.toInt(), current_int = Current.toInt();
	//�жϽ���ֵ�Ƿ�䶯
	if (speed_int <= -99999999 || speed_int >= 99999999 || current_int <= 0 || current_int >= 99999999)
	{
		QMessageBox::about(NULL, "", "��������Чֵ");
		return;
	}
	emit StartVelocityMode3(speed_int, current_int);
}

void QtGuiApplication1::VelocityData1(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1)
{
	ui.SpeedBegin1->setText(QString::number(m_lStartSpeed1, 10));
	ui.SpeedActual1->setText(QString::number(m_lActuaSpeed1, 10));
	ui.CurrentActual1->setText(QString::number(m_lActuaCurrent1, 10));
	ui.SpeedTorgue1->setText(QString::number((int)(9.55f * 18 * m_lActuaCurrent1/ m_lActuaSpeed1), 10));
}

void QtGuiApplication1::VelocityData2(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1)
{
	ui.SpeedBegin2->setText(QString::number(m_lStartSpeed1, 10));
	ui.SpeedActual2->setText(QString::number(m_lActuaSpeed1, 10));
	ui.CurrentActual2->setText(QString::number(m_lActuaCurrent1, 10));
	ui.SpeedTorgue2->setText(QString::number((int)(9.55f * 18 * m_lActuaCurrent1 / m_lActuaSpeed1), 10));
}

void QtGuiApplication1::VelocityData3(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1)
{
	ui.SpeedBegin3->setText(QString::number(m_lStartSpeed1, 10));
	ui.SpeedActual3->setText(QString::number(m_lActuaSpeed1, 10));
	ui.CurrentActual3->setText(QString::number(m_lActuaCurrent1, 10));
	ui.SpeedTorgue3->setText(QString::number((int)(9.55f * 18 * m_lActuaCurrent1 / m_lActuaSpeed1), 10));
}

void QtGuiApplication1::SpeedHalf1Clecked()
{
	emit SpeedHalt1Signal();
}

void QtGuiApplication1::SpeedHalf2Clecked()
{
	emit SpeedHalt1Signa2();
}

void QtGuiApplication1::SpeedHalf3Clecked()
{
	SpeedHalt1Signa3();
}

void QtGuiApplication1::MotionParaSettingSuccess1()
{
	QMessageBox::about(NULL, "", "Maxon Node 1 �������óɹ����ٶȣ��������ٶȣ�ֹͣ���ٶȽ����˶�Mode�����д�롣");
}

void QtGuiApplication1::MotionParaSettingSuccess2()
{
	QMessageBox::about(NULL, "", "Maxon Node 2 �������óɹ����ٶȣ��������ٶȣ�ֹͣ���ٶȽ����˶�Mode�����д�롣");
}

void QtGuiApplication1::MotionParaSettingSuccess3()
{
	QMessageBox::about(NULL, "", "Maxon Node 3 �������óɹ����ٶȣ��������ٶȣ�ֹͣ���ٶȽ����˶�Mode�����д�롣");
}

void QtGuiApplication1::Node1DialMove(int pos)
{
	emit Node1MoveSignal(pos);
}

void QtGuiApplication1::Node2DialMove(int pos)
{
	emit Node2MoveSignal(pos);
}

void QtGuiApplication1::Node3DialMove(int pos)
{
	emit Node3MoveSignal(pos);
}

void QtGuiApplication1::TeachModeDate(long speed1, short current1, long speed2, short current2, long speed3, short current3)
{
	ui.TeachInterfaceCurrentNode1->setText(QString::number(speed1, 10));
	ui.TeachInterfaceVelocityNode1->setText(QString::number(current1, 10));
	ui.TeachInterfaceTorgueNode1->setText(QString::number((int)(40/22*80*9.55f * 18 * current1 / speed1), 10));

	ui.TeachInterfaceCurrentNode2->setText(QString::number(speed2, 10));
	ui.TeachInterfaceVelocityNode2->setText(QString::number(current2, 10));
	ui.TeachInterfaceTorgueNode2->setText(QString::number((int)(2*80 * 9.55f * 18 * current2 / speed2), 10));

	ui.TeachInterfaceCurrentNode3->setText(QString::number(speed3, 10));
	ui.TeachInterfaceVelocityNode3->setText(QString::number(current3, 10));
	ui.TeachInterfaceTorgueNode3->setText(QString::number((int)(100 * 9.55f * 18 * current3 / speed3), 10));

	int id1_current = (int)4.5f*abs(control->getPresentCurrent(1) - 2048);
	int id1_velocity = control->getPresentVelocity(1) > 1024 ? (int)0.11f*(control->getPresentVelocity(1) - 1024) : -(int)0.11f*control->getPresentVelocity(1);
	ui.TeachInterfaceCurrentID1->setText(QString::number(id1_current, 10));
	ui.TeachInterfaceVelocityID1->setText(QString::number(id1_velocity, 10));
	ui.TeachInterfaceTorgueID1->setText(QString::number(control->getMaxTorque(1)));

	int id2_current = (int)4.5f*abs(control->getPresentCurrent(2) - 2048);
	int id2_velocity = control->getPresentVelocity(2) > 1024 ? (int)0.11f*(control->getPresentVelocity(2) - 1024) : -(int)0.11f*control->getPresentVelocity(2);
	ui.TeachInterfaceCurrentID2->setText(QString::number(id2_current, 10));
	ui.TeachInterfaceVelocityID2->setText(QString::number(id2_velocity, 10));
	ui.TeachInterfaceTorgueID2->setText(QString::number(control->getMaxTorque(2)));

	int id3_current = (int)4.5f*abs(control->getPresentCurrent(3) - 2048);
	int id3_velocity = control->getPresentVelocity(3) > 1024 ? (int)0.11f*(control->getPresentVelocity(3) - 1024) : -(int)0.11f*control->getPresentVelocity(3);
	ui.TeachInterfaceCurrentID3->setText(QString::number(id3_current, 10));
	ui.TeachInterfaceVelocityID3->setText(QString::number(id3_velocity, 10));
	ui.TeachInterfaceTorgueID3->setText(QString::number(control->getMaxTorque(3)));

	int id4_current = (int)4.5f*abs(control->getPresentCurrent(4) - 2048);
	int id4_velocity = control->getPresentVelocity(4) > 1024 ? (int)0.11f*(control->getPresentVelocity(4) - 1024) : -(int)0.11f*control->getPresentVelocity(4);
	ui.TeachInterfaceCurrentID4->setText(QString::number(id4_current, 10));
	ui.TeachInterfaceVelocityID4->setText(QString::number(id4_velocity, 10));
	ui.TeachInterfaceTorgueID4->setText(QString::number(control->getMaxTorque(4)));
}

void QtGuiApplication1::RecurringDone()
{
	emit MaxonRecurringDoneSignal();
}


void QtGuiApplication1::Connect_clicked()
{
	if (ui.Connect->text() == QString("Connect"))
	{
		//QStringתchar*
		QString  str = ui.comboBoxComPort->currentText();
		QByteArray ba = str.toLatin1();
		if (control->portHandler != nullptr)
			control->portHandler->closePort();
		//dynamixel initialization
		control->portHandler = dynamixel::PortHandler::getPortHandler(ba.data());
		control->packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
		control->group = dynamixel::GroupSyncWrite(control->portHandler, control->packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION);
		//�򿪴���
		if (control->portHandler->openPort())
		{
			QMessageBox::about(NULL, "TIP", "Connect Success!");

			//log_->Add(logINFO) << "Port Opened" << endl;
			ui.Connect->setText(QString("Disconnect"));
			////����ʹ��
			//ui->sendButton->setEnabled(true);
			Dy_Initialize();//��ʼ���ٶȣ������Լ���ťȡֵ����
		}
		else
		{
			QMessageBox::about(NULL, "TIP", "Connect Error!");
			return;
			//log_->Add(logERROR) << " Failed to open the port!" << endl;
		}
	}
	else//disconnect
	{
		if (control->portHandler != nullptr)
			control->portHandler->closePort();
		ui.Connect->setText(QString("Connect"));
	}
}

void QtGuiApplication1::ID1_move(int value)
{
	if (control->move2(1, value))
	{
		QString s = QString("%1").arg(value);
		QMessageBox::about(NULL, "failed  move to", s);
	}
}

void QtGuiApplication1::ID2_move(int value)
{
	if (control->move2(2, value))
	{
		QString s = QString("%1").arg(value);
		QMessageBox::about(NULL, "failed  move to", s);
	}
}

void QtGuiApplication1::ID3_move(int value)
{
	if (control->move2(3, value))
	{
		QString s = QString("%1").arg(value);
		QMessageBox::about(NULL, "failed  move to", s);
	}
}

void QtGuiApplication1::ID4_move(int value)
{
	if (control->move2(4, value))
	{
		QString s = QString("%1").arg(value);
		QMessageBox::about(NULL, "failed  move to", s);
	}
}

void QtGuiApplication1::movebutton()
{
	//qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	int RandomValue = qrand() % 4000;
	if (!control->move2(1, RandomValue))
		QMessageBox::about(NULL, "TIP", QString::number(RandomValue));
}

bool QtGuiApplication1::Dy_Initialize()
{
	//��ʼ���ٶ�
	control->setMaxSpeed();
	//��ʼ������
	control->tightenGrip();
	//������ťȡֵ
	ui.ID1_dial->setMinimum(control->getCWLimit(1));
	ui.ID1_dial->setMaximum(control->getCCWLimit(1));

	ui.ID2_dial->setMinimum(control->getCWLimit(2));
	ui.ID2_dial->setMaximum(control->getCCWLimit(2));

	ui.ID3_dial->setMinimum(control->getCWLimit(3));
	ui.ID3_dial->setMaximum(control->getCCWLimit(3));

	ui.ID4_dial->setMinimum(control->getCWLimit(4));
	ui.ID4_dial->setMaximum(control->getCCWLimit(4));
	return true;
}

void QtGuiApplication1::GetPos_clicked()
{
	ui.PosView->setRowCount(++table_row);
	emit TeachReadPosSignal();
	control->read(1);
	control->read(2);
	control->read(3);
	control->read(4);
	ui.PosView->setItem(table_row-1, 3, new QTableWidgetItem(QString(QString::number((int)control->getPresent(1)))));
	ui.PosView->setItem(table_row-1, 4, new QTableWidgetItem(QString(QString::number((int)control->getPresent(2)))));
	ui.PosView->setItem(table_row-1, 5, new QTableWidgetItem(QString(QString::number((int)control->getPresent(3)))));
	ui.PosView->setItem(table_row-1, 6, new QTableWidgetItem(QString(QString::number((int)control->getPresent(4)))));
}

void QtGuiApplication1::DeletePos_clicked()
{
	ui.PosView->setRowCount(--table_row);
}

void QtGuiApplication1::Recurring_clicked()
{
	ui.Recurring->setEnabled(0);
	ui.GetPos->setEnabled(0);
	ui.Delete_Pos->setEnabled(0);
	ui.LoopRecurring->setEnabled(0);
	row_count = ui.PosView->rowCount();
	column_count = ui.PosView->columnCount();
	current_row = 0;
	//maxon ����
	emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
	//dynamixel ����
	control->enableTorque(1);
	control->enableTorque(2);
	control->enableTorque(3);
	control->enableTorque(4);
	for (int j = 3; j < column_count; j++)
	{
		int pos= ui.PosView->item(current_row, j)->text().toInt();
		control->setGoal(j-2, pos);
		control->autoMove(j -2);
		//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
	}
	control->writeAll();
	//control->run();
	emit MoveStart(control->GetGroupPos(),control->getThreshold());//����Ŀ��λ�ø�running�߳�
	current_row += 1;
	//for (int i = 0; i < ui.PosView->rowCount(); i++)
	//{
	//	for (int j = 0; j < ui.PosView->columnCount(); j++)
	//	{
	//		int pos= ui.PosView->item(i, j)->text().toInt();
	//		control->setGoal(j+1, pos);
	//		control->autoMove(j + 1);
	//		//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
	//	}
	//	control->writeAll();
	//	control->run();
	//	//emit RunningSignal(control->GetGroupPos());//����Ŀ��λ�ø�running�߳�
	//	control->clearAll();
	//}
	//ui.GetPos->setEnabled(1);
	//ui.Delete_Pos->setEnabled(1);
}

void QtGuiApplication1::LoopRecurring_clicked()
{
	ui.Recurring->setEnabled(0);
	ui.GetPos->setEnabled(0);
	ui.Delete_Pos->setEnabled(0);
	ui.Recurring->setEnabled(0);

	isLoopRecurring = TRUE;
	row_count = ui.PosView->rowCount();
	column_count = ui.PosView->columnCount();
	current_row = 0;
	//maxon ����
	emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
	//dynamixel ����
	control->enableTorque(1);
	control->enableTorque(2);
	control->enableTorque(3);
	control->enableTorque(4);
	for (int j = 3; j < column_count; j++)
	{
		int pos = ui.PosView->item(current_row, j)->text().toInt();
		control->setGoal(j - 2, pos);
		control->autoMove(j - 2);
		//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
	}
	control->writeAll();
	//control->run();
	emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
	current_row += 1;
}


void QtGuiApplication1::Recurring_Halt_clicked()
{
	current_row = row_count + 1;
	if (isLoopRecurring)
	{
		isLoopRecurring = FALSE;
	}
	ui.Recurring->setEnabled(1);
	ui.LoopRecurring->setEnabled(1);
	ui.GetPos->setEnabled(1);
	ui.Delete_Pos->setEnabled(1);
}

void QtGuiApplication1::Recurring_UrgencyHalt_clicked()
{
	emit TeachHaltSignal();
	
	current_row = row_count + 1;
	if (isLoopRecurring)
	{
		isLoopRecurring = FALSE;
	}
	
	ui.Recurring->setEnabled(1);
	ui.LoopRecurring->setEnabled(1);
	ui.GetPos->setEnabled(1);
	ui.Delete_Pos->setEnabled(1);
}

void QtGuiApplication1::Teach_Recieve_Pos(long node1_pos, long node2_pos, long node3_pos)
{
	ui.PosView->setItem(table_row - 1, 0, new QTableWidgetItem(QString(QString::number(node1_pos))));
	ui.PosView->setItem(table_row - 1, 1, new QTableWidgetItem(QString(QString::number(node2_pos))));
	ui.PosView->setItem(table_row - 1, 2, new QTableWidgetItem(QString(QString::number(node3_pos))));
}

void QtGuiApplication1::Emit_Pre_Pos()
{
	control->read(1);
	control->read(2);
	control->read(3);
	control->read(4);
	emit Send_Pre_Pos(control->GetPresentGroupPos());
}

void QtGuiApplication1::Enable_Torque_clicked()
{
	control->enableTorque(1);
	control->enableTorque(2);
	control->enableTorque(3);
	control->enableTorque(4);
}

void QtGuiApplication1::Disable_Torque_clicked()
{
	control->disableTorque(1);
	control->disableTorque(2);
	control->disableTorque(3);
	control->disableTorque(4);
}

void QtGuiApplication1::ID1_Para_Write_clicked()
{
	int cw, ccw, velocity, torque,p_gain;
	//cw��λ
	if (ui.ID1CW->text() == "")
		cw = 50;
	else
	{
		cw = ui.ID1CW->text().toInt();
		if (cw < 0 || cw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCW(1, cw);
	}
	//ccw��λ
	if (ui.ID1CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui.ID1CCW->text().toInt();
		if (ccw < 0 || ccw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCCW(1,ccw);
	}
	//�ٶ�
	if (ui.ID1Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui.ID1Velocity->text().toInt();
		if (velocity < 0 || velocity>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setSpeed(1, velocity);
	}
	//ת��
	if (ui.ID1Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui.ID1Torque->text().toInt();
		if (torque < 0 || torque>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setMaxTorque(1, torque);
	}
	//P����
	if (ui.ID1Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui.ID1Pgain->text().toInt();
		if (p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128)
			control->setPgain(1, p_gain);
		else
			QMessageBox::about(NULL, "ȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
	}
	QMessageBox::about(NULL,"","ID1 ����д��ɹ�");
}

void QtGuiApplication1::ID2_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui.ID2CW->text() == "")
		cw = 50;
	else
	{
		cw = ui.ID2CW->text().toInt();
		if (cw < 0 || cw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCW(2, cw);
	}
	//ccw��λ
	if (ui.ID2CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui.ID2CCW->text().toInt();
		if (ccw < 0 || ccw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCCW(2, ccw);
	}
	//�ٶ�
	if (ui.ID2Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui.ID2Velocity->text().toInt();
		if (velocity < 0 || velocity>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setSpeed(2, velocity);
	}
	//ת��
	if (ui.ID2Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui.ID2Torque->text().toInt();
		if (torque < 0 || torque>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setMaxTorque(2, torque);
	}
	//P����
	if (ui.ID2Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui.ID2Pgain->text().toInt();
		if (p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128)
			control->setPgain(2, p_gain);
		else
			QMessageBox::about(NULL, "ȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
	}
	QMessageBox::about(NULL, "", "ID2 ����д��ɹ�");
}

void QtGuiApplication1::ID3_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui.ID3CW->text() == "")
		cw = 50;
	else
	{
		cw = ui.ID3CW->text().toInt();
		if (cw < 0 || cw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCW(3, cw);
	}
	//ccw��λ
	if (ui.ID3CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui.ID3CCW->text().toInt();
		if (ccw < 0 || ccw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCCW(3, ccw);
	}
	//�ٶ�
	if (ui.ID3Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui.ID3Velocity->text().toInt();
		if (velocity < 0 || velocity>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setSpeed(3, velocity);
	}
	//ת��
	if (ui.ID3Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui.ID3Torque->text().toInt();
		if (torque < 0 || torque>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setMaxTorque(3, torque);
	}
	//P����
	if (ui.ID3Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui.ID3Pgain->text().toInt();
		if (p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128)
			control->setPgain(3, p_gain);
		else
			QMessageBox::about(NULL, "ȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
	}
	QMessageBox::about(NULL, "", "ID3 ����д��ɹ�");
}

void QtGuiApplication1::ID4_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui.ID4CW->text() == "")
		cw = 50;
	else
	{
		cw = ui.ID4CW->text().toInt();
		if (cw < 0 || cw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCW(4, cw);
	}
	//ccw��λ
	if (ui.ID4CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui.ID4CCW->text().toInt();
		if (ccw < 0 || ccw>4095)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~4095��");
		else
			control->setCCW(4, ccw);
	}
	//�ٶ�
	if (ui.ID4Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui.ID4Velocity->text().toInt();
		if (velocity < 0 || velocity>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setSpeed(4, velocity);
	}
	//ת��
	if (ui.ID4Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui.ID4Torque->text().toInt();
		if (torque < 0 || torque>1023)
			QMessageBox::about(NULL, "ȡֵ��Χ����", "������0~1023��");
		else
			control->setMaxTorque(4, torque);
	}
	//P����
	if (ui.ID4Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui.ID4Pgain->text().toInt();
		if (p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128)
			control->setPgain(4, p_gain);
		else
			QMessageBox::about(NULL, "ȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
	}
	QMessageBox::about(NULL, "", "ID4 ����д��ɹ�");
}

void QtGuiApplication1::ID1_Torgue_Enable_clicked()
{
	control->enableTorque(1);
}

void QtGuiApplication1::ID2_Torgue_Enable_clicked()
{
	control->enableTorque(2);
}

void QtGuiApplication1::ID3_Torgue_Enable_clicked()
{
	control->enableTorque(3);
}

void QtGuiApplication1::ID4_Torgue_Enable_clicked()
{
	control->enableTorque(4);
}

void QtGuiApplication1::ID1_Torgue_Disable_clicked()
{
	control->disableTorque(1);
}

void QtGuiApplication1::ID2_Torgue_Disable_clicked()
{
	control->disableTorque(2);
}

void QtGuiApplication1::ID3_Torgue_Disable_clicked()
{
	control->disableTorque(3);
}

void QtGuiApplication1::ID4_Torgue_Disable_clicked()
{
	control->disableTorque(4);
}

void QtGuiApplication1::ID1_Move_clicked()
{
	if (ui.ID1Move->text() == QString("�ƶ�"))
	{
		ui.ID1Move->setText(QString("ֹͣ"));
		if (ui.ID1PosMode->isChecked())
		{
			int Pos = ui.ID1PosValue->text().toInt();
			if (Pos < 0 || Pos>4095)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~4095");
				ui.ID1Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->move2(1, Pos);
				//if(!move_clicked_timer->isActive())
				//move_clicked_timer->start(1000);
				return;
			}
		}
		else//Velocity Mode
		{
			int speed = ui.ID1PosValue->text().toInt();
			if (speed < 0 || speed>1023)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~1023");
				ui.ID1Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->setCW(1, 0);
				control->setCCW(1,0);
				control->setSpeed(1, speed);
				return;
			}
		}

	}
	else//ֹͣ
	{
		ui.ID1Move->setText(QString("�ƶ�"));
		if (ui.ID1PosMode->isChecked())
		{
			int pre_pos = control->read(1);
			control->move2(1, pre_pos);
			return;
		}
		else
		{
			control->setCW(1, control->getCWLimit(1));
			control->setCCW(1, control->getCCWLimit(1));
			return;
		}
	}
}

void QtGuiApplication1::ID2_Move_clicked()
{
	if (ui.ID2Move->text() == QString("�ƶ�"))
	{
		ui.ID2Move->setText(QString("ֹͣ"));
		if (ui.ID2PosMode->isChecked())
		{
			int Pos = ui.ID2PosValue->text().toInt();
			if (Pos < 0 || Pos>4095)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~4095");
				ui.ID2Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->move2(2, Pos);
				//if (!move_clicked_timer->isActive())
					//move_clicked_timer->start(1000);
					return;
			}
		}
		else//Velocity Mode
		{
			int speed = ui.ID2PosValue->text().toInt();
			if (speed < 0 || speed>1023)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~1023");
				ui.ID2Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->setCW(2, 0);
				control->setCCW(2, 0);
				control->setSpeed(2, speed);
				return;
			}
		}

	}
	else//ֹͣ
	{
		ui.ID2Move->setText(QString("�ƶ�"));
		if (ui.ID2PosMode->isChecked())
		{
			int pre_pos = control->read(2);
			control->move2(2, pre_pos);
			return;
		}
		else
		{
			control->setCW(2, control->getCWLimit(2));
			control->setCCW(2, control->getCCWLimit(2));
			return;
		}
	}
}

void QtGuiApplication1::ID3_Move_clicked()
{
	if (ui.ID3Move->text() == QString("�ƶ�"))
	{
		ui.ID3Move->setText(QString("ֹͣ"));
		if (ui.ID3PosMode->isChecked())
		{
			int Pos = ui.ID3PosValue->text().toInt();
			if (Pos < 0 || Pos>4095)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~4095");
				ui.ID3Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->move2(3, Pos);
				//if (!move_clicked_timer->isActive())
					//move_clicked_timer->start(1000);
					return;
			}
		}
		else//Velocity Mode
		{
			int speed = ui.ID3PosValue->text().toInt();
			if (speed < 0 || speed>1023)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~1023");
				ui.ID3Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->setCW(3, 0);
				control->setCCW(3, 0);
				control->setSpeed(3, speed);
				return;
			}
		}

	}
	else//ֹͣ
	{
		ui.ID3Move->setText(QString("�ƶ�"));
		if (ui.ID3PosMode->isChecked())
		{
			int pre_pos = control->read(3);
			control->move2(3, pre_pos);
			return;
		}
		else
		{
			control->setCW(3, control->getCWLimit(3));
			control->setCCW(3, control->getCCWLimit(3));
			return;
		}
	}
}

void QtGuiApplication1::ID4_Move_clicked()
{
	if (ui.ID4Move->text() == QString("�ƶ�"))
	{
		ui.ID4Move->setText(QString("ֹͣ"));
		if (ui.ID4PosMode->isChecked())
		{
			int Pos = ui.ID4PosValue->text().toInt();
			if (Pos < 0 || Pos>4095)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~4095");
				ui.ID4Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->move2(4, Pos);
				//if (!move_clicked_timer->isActive())
					//move_clicked_timer->start(1000);
					return;
			}
		}
		else//Velocity Mode
		{
			int speed = ui.ID4PosValue->text().toInt();
			if (speed < 0 || speed>1023)
			{
				QMessageBox::about(NULL, "Value Range Error", "λ��ֵ���뷶Χ0~1023");
				ui.ID4Move->setText(QString("�ƶ�"));
				return;
			}
			else
			{
				control->setCW(4, 0);
				control->setCCW(4, 0);
				control->setSpeed(4, speed);
				return;
			}
		}

	}
	else//ֹͣ
	{
		ui.ID4Move->setText(QString("�ƶ�"));
		if (ui.ID4PosMode->isChecked())
		{
			int pre_pos = control->read(4);
			control->move2(4, pre_pos);
			return;
		}
		else
		{
			control->setCW(4, control->getCWLimit(4));
			control->setCCW(4, control->getCCWLimit(4));
			return;
		}
	}
}

void QtGuiApplication1::Move_clicked_Timer()
{
	if (control->ismoving(1) || control->ismoving(2) || control->ismoving(3) || control->ismoving(4))
	{
		return;
	}
	else
	{
		move_clicked_timer->stop();
		ui.ID1Move->setText(QString("�ƶ�"));
		ui.ID2Move->setText(QString("�ƶ�"));
		ui.ID3Move->setText(QString("�ƶ�"));
		ui.ID4Move->setText(QString("�ƶ�"));
	}
}

void QtGuiApplication1::Show_Pos_In_Running(uint16_t * pos)
{
	//QMessageBox::about(NULL,"","statusbar");
	//QStatusBar* bar = statusBar(); //��ȡ״̬��
	//QLabel* first_statusLabel = new QLabel; //�½���ǩ
	//first_statusLabel->setMinimumSize(150, 20); //���ñ�ǩ��С�ߴ�
	//first_statusLabel->setFrameShape(QFrame::WinPanel); //���ñ�ǩ��״
	//first_statusLabel->setFrameShadow(QFrame::Sunken); //���ñ�ǩ��Ӱ

	//QLabel* second_statusLabel = new QLabel;
	//second_statusLabel->setMinimumSize(150, 20);
	//second_statusLabel->setFrameShape(QFrame::WinPanel);
	//second_statusLabel->setFrameShadow(QFrame::Sunken);

	//QLabel* third_statusLabel = new QLabel;
	//QLabel* four_statusLabel = new QLabel;

	////��ʾһ����Ϣ��
	//bar->addWidget(first_statusLabel);
	//bar->addWidget(second_statusLabel);

	////��ʾ������Ϣ��
	//bar->addPermanentWidget(third_statusLabel);
	//bar->insertPermanentWidget(3, four_statusLabel);

	//first_statusLabel->setText(tr("status1")); //��ʼ������
	//second_statusLabel->setText(tr("status2"));
	//third_statusLabel->setText("yong jiu xin xi");
	//four_statusLabel->setText("four_statusLabel");

	//�����ʾ��ʱ��Ϣ
	//QString pos_message = "ID1_Pos:"+QString::number(control->getPresent(1))+"	ID2_Pos:"+ QString::number(control->getPresent(2))+"	ID3_Pos:"+ QString::number(control->getPresent(3))+"	ID4_Pos:"+ QString::number(control->getPresent(4));
	QString pos_message = "ID1_Pos:"+QString::number(pos[0])+"	ID2_Pos:"+ QString::number(pos[1])+"	ID3_Pos:"+ QString::number(pos[2])+"	ID4_Pos:"+ QString::number(pos[3]);
	statusBar()->showMessage(pos_message, 3000);//3s��ر�
	emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
}

void QtGuiApplication1::Running_Done()
{
	//QMessageBox::about(NULL, "", "Running_Done");
	control->clearAll();
	if (isLoopRecurring)
	{
		static BOOL isinverse = false;
		if (!isinverse)
		{
			if (current_row + 1 <= row_count)
			{
				//maxon ����
				emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
				//dynamixel����
				for (int j = 3; j < column_count; j++)
				{
					int pos = ui.PosView->item(current_row, j)->text().toInt();
					control->setGoal(j - 2, pos);
					control->autoMove(j - 2);
					//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
				}
				control->writeAll();
				//control->run();
				emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
				current_row += 1;
				return;
			}
			else
			{
				isinverse = true;
				current_row -= 2;
				//maxon ����
				emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
				//dynamixel����
				for (int j = 3; j < column_count; j++)
				{
					int pos = ui.PosView->item(current_row, j)->text().toInt();
					control->setGoal(j - 2, pos);
					control->autoMove(j - 2);
					//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
				}
				control->writeAll();
				//control->run();
				emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
				current_row -= 1;
				return;
			}
			//else
			//{
			//	//isinverse = true;
			//	current_row=0;
			//	//maxon ����
			//	emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
			//	//dynamixel����
			//	for (int j = 3; j < column_count; j++)
			//	{
			//		int pos = ui.PosView->item(current_row, j)->text().toInt();
			//		control->setGoal(j - 2, pos);
			//		control->autoMove(j - 2);
			//		//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
			//	}
			//	control->writeAll();
			//	//control->run();
			//	emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
			//	current_row += 1;
			//}
		}
		else
		{
			if (current_row  >= 0)
			{
				//maxon ����
				emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
				//dynamixel����
				for (int j = 3; j < column_count; j++)
				{
					int pos = ui.PosView->item(current_row, j)->text().toInt();
					control->setGoal(j - 2, pos);
					control->autoMove(j - 2);
					//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
				}
				control->writeAll();
				//control->run();
				emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
				current_row -= 1;
				return;
			}
			else
			{
				isinverse = false;
				current_row += 2;
				//maxon ����
				emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
				//dynamixel����
				for (int j = 3; j < column_count; j++)
				{
					int pos = ui.PosView->item(current_row, j)->text().toInt();
					control->setGoal(j - 2, pos);
					control->autoMove(j - 2);
					//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
				}
				control->writeAll();
				//control->run();
				emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
				current_row += 1;
				return;
			}
		}
		return;
	}
	if (current_row+1 <= row_count)
	{
		//maxon ����
		emit TeachRecurringSignal(ui.PosView->item(current_row, 0)->text().toInt(), ui.PosView->item(current_row, 1)->text().toInt(), ui.PosView->item(current_row, 2)->text().toInt());
		//dynamixel����
		for (int j = 3; j < column_count; j++)
		{
			int pos = ui.PosView->item(current_row, j)->text().toInt();
			control->setGoal(j-2, pos);
			control->autoMove(j -2);
			//QMessageBox::about(NULL,"", ui.PosView->item(i, j)->text());
		}
		control->writeAll();
		//control->run();
		emit MoveStart(control->GetGroupPos(), control->getThreshold());//����Ŀ��λ�ø�running�߳�
		current_row += 1;
		return;
	}
	else
	{
		//������ذ�ť
		ui.Recurring->setEnabled(1);
		ui.LoopRecurring->setEnabled(1);
		ui.GetPos->setEnabled(1);
		ui.Delete_Pos->setEnabled(1);
		return;
	}
}

void QtGuiApplication1::ComScanTimeout()
{
	//��⴮���Ƿ�仯��ˢ�´����б�
	QStringList m_serialPortName2;
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		m_serialPortName2 << info.portName();
	}
	if (m_serialPortName2 != m_serialPortName)
	{
		Dy_ComPort();
	}
		
	return;
}

void QtGuiApplication1::CurrentIndexChange(int index)
{
	if (index == 2)
	{
		emit InTeachPageSignal();
		QMessageBox::about(NULL, "Value Range Error", "����ʾ�̽���");
	}
	else
	{
		emit NoInTeachPageSignal();
	}
	

}