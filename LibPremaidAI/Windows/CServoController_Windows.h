#pragma once

#ifdef _WIN32

#include "Common.h"
#include <windows.h>

#include "ISerialPort.h"
#include "IServo.h"
#include "IServoController.h"

class CServoController_Windows : public IServoController
{
	bool				_isConnected;

	ISerialPort* _serial;
	IServo*		 _ics_hv;
	IServo*		 _ics_mv;
public:
	static const int MV_SERVO_COUNT = 11;
	static const int HV_SERVO_COUNT = 14;
	static const int SERVO_COUNT = MV_SERVO_COUNT + HV_SERVO_COUNT;


	CServoController_Windows();
	
	virtual ~CServoController_Windows();

	void Connect(const wchar_t* port, const int baudrate);

	void Disconnect();

	float GetTemp(const int id);

	float GetCurrent(const int id);

	void SetPosition(const int id, const int pos);

	void SetServoPosAll(const int pos);

	int GetServoPos(const int id);

	// ストレッチの設定
	void SetStrectch(const int id, const int stretch);

	// サーボスピードの設定
	void SetSpeed(const int id, const int speed);

	void Update();
};

#endif // _WIN32
