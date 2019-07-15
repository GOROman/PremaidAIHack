#pragma once
#include "Common.h"

#define ISerialPort_API_VER (100)


// Serial Port Interface
class  ISerialPort {
public:
	enum PARITY {
		NONE = 0,
		ODD = 1,
		EVEN = 2,
	};

public:
	virtual ~ISerialPort() {}

	// シリアルポートへ接続
	virtual RESULT Connect(const wchar_t* portname, const int baudrate, const PARITY parity) = 0;

	// シリアルポートから切断
	virtual RESULT Disconnect() = 0;

	// シリアルポートから読み込み
	virtual RESULT Read(unsigned char* buffer, const size_t size, size_t* read_size) = 0;

	// シリアルポートへ書き込み
	virtual RESULT Write(const unsigned char* buffer, const size_t size, size_t* written_size) = 0;

	// 指定時間(ms)待機
	virtual RESULT Sleep(const int ms) = 0;
};
