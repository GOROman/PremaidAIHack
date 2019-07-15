#pragma once

#include "Common.h"
#include "ISerialPort.h"

#define IServo_API_VER (100)

// Servo Motor Interface
class IServo {
public:
	virtual ~IServo() {}

	// シリアルポートをアタッチ
	virtual RESULT Attach(ISerialPort* serial) = 0;

	// 原点リセット
	virtual RESULT Reset(const int id) = 0;

	// 脱力させる
	virtual RESULT SetFree(const int id) = 0;

	// サーボを指定位置へ動かす
	virtual RESULT SetPosition(const int id, const int pos) = 0;

	// サーボの現在位置を取得
	virtual RESULT GetPosition(const int id, int* pos) = 0;

	// パラメータをセットする
	virtual RESULT SetParam(const int id, const int PARAMTYPE, const int param) = 0;

	// パラメータを取得する
	virtual RESULT GetParam(const int id, const int PARAMTYPE, int* param) = 0;

	// サーボ温度取得
	virtual RESULT GetTemp(const int id, float* temp) = 0;

	// サーボ電流取得
	virtual RESULT GetCurrent(const int id, float* current) = 0;

	// アップデート
	virtual RESULT Update() = 0;
};
