#pragma once

#ifdef LIBPREMAIDAI_DLL_EXPORT
#else
#define LIBPREMAIDAI_DLL_EXPORT  __declspec(dllimport)
#endif

#include <Common.h>

#include <ISerialPort.h>
#include <IServo.h>

enum  {
	SHOULDER_PITCH_R = 1,	// 肩ピッチR
	SHOULDER_PITCH_L = 3,	// 肩ピッチL
};
/*
1	肩ピッチR
1	頭ピッチ
2	肩ピッチL
2	頭ヨー
3	ヒップヨーR
3	頭萌
4	ヒップヨーL
4	肩ロールR
5	ヒップロールR
5	肩ロールL
6	ヒップロールL
6	上腕ヨーR
7	腿ピッチR
7	上腕ヨーL
8	腿ピッチL
8	肘ピッチR
9	膝ピッチR
9	肘ピッチL
10	膝ピッチL
10	手首ヨーR
11	足首ピッチR
11	手首ヨーL
12	足首ピッチL
13	足首ロールR
14	足首ロールL
*/

#define ICS_COUNT		(2)

enum {
	TYPE_HV = 0,
	TYPE_MV = 1,
};

#define MV_SERVO_ID_COUNT  (11)	// MVサーボID数
#define HV_SERVO_ID_COUNT  (14)	// HVサーボID数

#define SERVO_COUNT     (MV_SERVO_ID_COUNT + HV_SERVO_ID_COUNT)

class LIBPREMAIDAI_DLL_EXPORT CPremaidAI {
	bool	_isConnected;

	ISerialPort* _serial;
	IServo* _ics[ICS_COUNT];

public:
	CPremaidAI();

	virtual ~CPremaidAI();

	ISerialPort* GetSerialPort() { return _serial;  }

//	IServo* GetServo(const int type) {
//	}

	// 接続する
	virtual void Connect(const wchar_t* port, const int baudrate);

	// 切断する
	virtual void Disconnect();

	// サーボ位置を設定
	virtual void SetPosition(const int id, const int pos);

	// 角度(度)を指定してサーボ位置を設定
	virtual void SetRotation(const int id, const float deg);

	// すべてのサーボ位置を一括指定
	virtual void SetPositionAll(const int pos);

	// サーボ位置取得
	virtual int GetPosition(const int id) ;

	// ストレッチ設定
	virtual void SetStrectch(const int id, const int stretch) ;

	// サーボスピード設定
	virtual void SetSpeed(const int id, const int speed) ;

	// サーボ温度を得る
	virtual float GetTemperature(const int id);

	// サーボ電流値を得る
	virtual float GetCurrent(const int id);

	// 更新
	virtual void Update();
		

};
