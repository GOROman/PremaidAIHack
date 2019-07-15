#pragma once

//
// 近藤科学 ICS 3.5 制御 (https://kondo-robot.com/faq/ics3-5-explain)
//

#include "Common.h"
#include "IServo.h"
#include <deque>

#define ICS_ID_MIN		(0)		// サーボID 最小値
#define ICS_ID_MAX		(31)	// サーボID	最大値
#define ICS_ID_COUNT	(32)	// 最大サーボ数

#define ICS_POS_CENTER	(7500)	// サーボ中央位置
#define ICS_POS_MIN		(3500)	// サーボ最小位置
#define ICS_POS_MAX		(11500)	// サーボ最大位置

#define ICS_STEP		(135.0f/4000.0f)	// サーボ1ステップあたりの角度

#define DEG2POS(deg)	((deg)/ICS_STEP + ICS_POS_CENTER)
#define POS2DEG(pos)	(((pos)-ICS_POS_CENTER)*ICS_STEP)


typedef unsigned char DATABYTE;


class CServo_ICS35 : public IServo {
public:
	enum {
		PARAMTYPE_STRETCH = 1,		// ストレッチデータ
		PARAMTYPE_SPEED = 2,		// スピードデータ
		PARAMTYPE_CURRENT = 3,		// 電流値
		PARAMTYPE_TEMPERATURE = 4,	// 温度値
	};

public:
	// コンストラクタ
	CServo_ICS35();

	// デストラクタ
	virtual ~CServo_ICS35();

	// シリアルポートをアタッチ
	virtual RESULT Attach(ISerialPort* serial);

	// 原点リセット
	virtual RESULT Reset(const int id);

	// 脱力させる
	virtual RESULT SetFree(const int id);

	// サーボを指定位置へ動かす
	virtual RESULT SetPosition(const int id, const int pos);

	// サーボの現在位置を取得
	virtual RESULT GetPosition(const int id, int* pos);

	// パラメータをセットする
	virtual RESULT SetParam(const int id, const int PARAMTYPE, const int param);

	// パラメータを取得する
	virtual RESULT GetParam(const int id, const int PARAMTYPE, int* param);

	// サーボ温度取得
	virtual RESULT GetTemp(const int id, float* temp);

	// サーボ電流取得
	virtual RESULT GetCurrent(const int id, float* current);

	// アップデート
	virtual RESULT Update();

protected:

	// 受信データをハンドリング
	size_t Proc(size_t count, int* data);

	// サーボから受信
	RESULT RecvData(size_t count);

	// サーボへコマンド送信し、結果を受信
	RESULT SendCommand(const DATABYTE byte1, const DATABYTE byte2, const DATABYTE byte3);

	// サーボへコマンド送信し、結果を受信
	RESULT ReadCommand(const DATABYTE byte1, const DATABYTE byte2, int* data);

private:
	ISerialPort* _serial;

	int _pos[ICS_ID_COUNT];

	std::deque<DATABYTE>	_buffer;
};

