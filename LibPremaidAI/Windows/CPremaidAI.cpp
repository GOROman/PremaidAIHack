#pragma once

#define LIBPREMAIDAI_DLL_EXPORT __declspec(dllexport)


#include <Common.h>

#include "CPremaidAI.h"

#include "CSerialPort_Windows.h"
#include "CServo_ICS35.h"


static const int BONEID2SERVOIDTABLE[] =
{
	0,	// 0: N/A
	1,	// 1: HV SHOULDER_PITCH_R
	1,	// 2: MV HEAD PITCH
	2,	// 3: HV_SHOULDER_PITCH_L
	2,	// 4: MV HEAD YAW
	3,	// 5: HV HIP_YAW_R
	3,	// 6: MV 
	4,	// 7: HV HIP_YAW_R
	4,	// 8: MV 
	5,	// 9: HV
	5,	//10: MV
	6,	//11: HV
	6,	//12: MV
	7,	//13: HV
	7,	//14: MV
	8,	//15: HV KNEE_PITCH_R
	8,	//16: MV
	9,	//17: HV KNEE_PITCH_L
	9,	//18: MV
	10,	//19: HV
	10,	//20: MV
	11,	//21: HV
	11,	//22: MV
	12,	//23: HV
	13,	//24: HV
	14,	//25: HV
};

static const int BONEFLIPTABLE[] =
{
	 0,	// 0: N/A
	 0,	// 1: HV SHOULDER_PITCH_R
	 0,	// 2: MV HEAD PITCH
	 1,	// 3: HV_SHOULDER_PITCH_L
	 0,	// 4: MV HEAD YAW
	 0,	// 5: HV HIP_YAW_R
	 0,	// 6: MV HEAD_ROLL 
	 1,	// 7: HV HIP_YAW_L
	 0,	// 8: MV R
	 0,	// 9: HV R
	 1,	//10: MV L
	 1,	//11: HV L
	 0,	//12: MV R
	 0,	//13: HV R
	 1,	//14: MV L
	 1,	//15: HV L
	 0,	//16: MV R
	 0,	//17: HV R
	 1,	//18: MV L
	 1,	//19: HV L
	 0,	//20: MV R
	 0,	//21: HV R
	 1,	//22: MV L
	 1,	//23: HV L
	 0,	//24: HV R
	 1,	//25: HV L
};

static const int BONEMOTORTYPE[] =
{
	 0,	// 0: N/A
	 0,	// 1: HV SHOULDER_PITCH_R
	 1,	// 2: MV HEAD PITCH
	 0,	// 3: HV_SHOULDER_PITCH_L
	 1,	// 4: MV HEAD YAW
	 0,	// 5: HV HIP_YAW_R
	 1,	// 6: MV HEAD_ROLL 
	 0,	// 7: HV HIP_YAW_L
	 1,	// 8: MV R
	 0,	// 9: HV R
	 1,	//10: MV L
	 0,	//11: HV L
	 1,	//12: MV R
	 0,	//13: HV R
	 1,	//14: MV L
	 0,	//15: HV L
	 1,	//16: MV R
	 0,	//17: HV R
	 1,	//18: MV L
	 0,	//19: HV L
	 1,	//20: MV R
	 0,	//21: HV R
	 1,	//22: MV L
	 0,	//23: HV L
	 0,	//24: HV R
	 0,	//25: HV L
};





CPremaidAI::CPremaidAI() : _isConnected(false)
{
	// シリアルポートを生成
	_serial[TYPE_HV] = new CSerialPort_Windows();
	_serial[TYPE_MV] = new CSerialPort_Windows();

	// サーボコントローラを生成
	_ics[TYPE_HV] = new CServo_ICS35();
	_ics[TYPE_MV] = new CServo_ICS35();
}

// デストラクタ
CPremaidAI::~CPremaidAI()
{
	Disconnect();
	delete _ics[TYPE_MV];
	delete _ics[TYPE_HV];

	delete _serial[TYPE_MV];
	delete _serial[TYPE_HV];
}

// 指定関節のサーボ位置を設定
void CPremaidAI::SetPosition(const int id, const int pos) {

	assert(id > 0);
	assert(id <= 25);


	int new_pos = pos;
	if (new_pos == 0) {

	}
	else {
		if (BONEFLIPTABLE[id] == 1) {
//			new_pos = (pos - 7500) * -1 + 7500;
		}
	}
	_ics[BONEMOTORTYPE[id]]->SetPosition(BONEID2SERVOIDTABLE[id], new_pos);
	printf("SetPosition(%2d, %5d) ICS:%d ServoID:%2d Flip:%d Pos:%d->%d\n", id, pos, BONEMOTORTYPE[id], BONEID2SERVOIDTABLE[id], BONEFLIPTABLE[id], pos, new_pos);
}

// 角度を指定してサーボ位置を設定
void CPremaidAI::SetRotation(const int id, const float deg) {

	float pos = DEG2POS(deg);
	SetPosition(id, (int)pos);
}


// プリメイドAI ICSへ接続
void CPremaidAI::Connect(const wchar_t* port_hv, const wchar_t* port_mv, const int baudrate)
{
	int result = 0;
	result = _serial[TYPE_HV]->Connect(port_hv, baudrate, ISerialPort::EVEN);
	result = _serial[TYPE_MV]->Connect(port_mv, baudrate, ISerialPort::EVEN);

	if (result == RESULT_OK)
	{
		_ics[TYPE_HV]->Attach(_serial[TYPE_HV]);
		_ics[TYPE_MV]->Attach(_serial[TYPE_MV]);

		_isConnected = true;
	}
}

// 切断
void CPremaidAI::Disconnect()
{
	if (_isConnected == false) return;

//	_serial[TYPE_HV]->Disconnect();

	_isConnected = false;
}

// サーボ温度取得
float CPremaidAI::GetTemperature(const int id) {
	float temp = 0;
	_ics[BONEMOTORTYPE[id]]->GetTemp(BONEID2SERVOIDTABLE[id], &temp);
	return temp;
}

// サーボ電流値取得
float CPremaidAI::GetCurrent(const int id) {
	float cur = 0;
	_ics[BONEMOTORTYPE[id]]->GetCurrent(BONEID2SERVOIDTABLE[id], &cur);
	return cur;
}

void CPremaidAI::SetPositionAll(const int pos) {
	for (int id = 1; id <= 25; ++id) {
		SetPosition(id, pos);
	}
}

// サーボ位置の取得
int CPremaidAI::GetPosition(const int id) {
	int pos = 0;
	_ics[BONEMOTORTYPE[id]]->GetPosition(BONEID2SERVOIDTABLE[id], &pos);
	return pos;
}
// ストレッチの設定
void CPremaidAI::SetStrectch(const int id, const int stretch) {
	printf("SetStretch(%d,%d)\n", id, stretch);
	_ics[BONEMOTORTYPE[id]]->SetParam(BONEID2SERVOIDTABLE[id], CServo_ICS35::PARAMTYPE_STRETCH, stretch);
}

// サーボスピードの設定
void CPremaidAI::SetSpeed(const int id, const int speed) {
	printf("SetSpeed(%d,%d)\n", id, speed);
	_ics[BONEMOTORTYPE[id]]->SetParam(BONEID2SERVOIDTABLE[id], CServo_ICS35::PARAMTYPE_SPEED, speed);
}

void CPremaidAI::Update()
{
	_ics[TYPE_HV]->Update();
	_ics[TYPE_MV]->Update();
}