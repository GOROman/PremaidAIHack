#pragma once

#ifdef _WIN32

#include <windows.h>

#include "Common.h"
#include "ISerialPort.h"

// シリアルポート Windows API 実装
class CSerialPort_Windows : public ISerialPort
{
	HANDLE	_hPort;
	bool	_isConnected;

protected:
	// シリアルポートのパラメータ設定
	void SetSerialParams(const int baudrate, const int parity) {
		BOOL result = FALSE;
		
		DCB dcb;
		result = ::GetCommState(_hPort, &dcb);
		assert(result == TRUE);

		// パリティを設定する
		switch (parity) {
			case ISerialPort::PARITY::NONE: dcb.Parity = 0;				break;
			case ISerialPort::PARITY::ODD: 	dcb.Parity = ODDPARITY;		break;
			case ISerialPort::PARITY::EVEN: dcb.Parity = EVENPARITY;	break;
			default:
				assert(0 && "Unknown Pariry.");
		}

		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.fOutxCtsFlow = FALSE;       // 　CTSハードウェアフロー制御：CTS制御を使用しない場合はFLASEを指定
											//   　　　　　　CTS 制御をする場合はTRUEを指定してCTS信号を監視します。                     
		dcb.fOutxDsrFlow = FALSE;       //  DSRハードウェアフロー制御：使用しない場合はFALSEを指定
		dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS制御：　RTS制御をしない場合はRTS_CONTROL_DISABLEを指定

		dcb.fBinary = TRUE;
		dcb.fErrorChar = TRUE;             // パリティエラー発生時のキャラクタ（ErrorChar）置換：　なし→FLALSE
		dcb.ErrorChar = (BYTE)0xff;

		// ソフトウェアフロー制御
		dcb.fOutX = FALSE;                 // 送信時XON/OFF制御の有無：　なし→FLALSE  
		dcb.fInX = FALSE;                   // 受信時XON/XOFF制御の有無：なし→FALSE  

		result = ::SetCommState(_hPort, &dcb);
		assert(result == TRUE);
	}

	// パージ
	void Purge() {
		BOOL result = ::PurgeComm(_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		assert(result == TRUE);
	}

public:
	CSerialPort_Windows() : _isConnected(false), _hPort(NULL) {
	}

	virtual ~CSerialPort_Windows() {
		Disconnect();
	}

	// シリアルポートへ接続
	virtual RESULT Connect(const wchar_t* portname, const int baudrate, const PARITY parity)
	{
		if (_isConnected == true) return RESULT_ERROR;

		_hPort = ::CreateFile(
			portname,
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);

//		assert(_hPort != INVALID_HANDLE_VALUE);

		if (_hPort == INVALID_HANDLE_VALUE) {
			printf("ERROR:Could not open %ws!\n", portname);
			return RESULT_ERROR;
		}

		// シリアルパラメータ設定
		SetSerialParams(baudrate, parity);

		// バッファのパージ
		Purge();

		_isConnected = true;

		return RESULT_OK;
	}

	// シリアルポートから切断
	virtual RESULT Disconnect()
	{
		if (_isConnected == false) return RESULT_ERROR;

		BOOL result = ::CloseHandle(_hPort);
		assert(result == TRUE);

		_hPort = NULL;
		_isConnected = false;
		return RESULT_OK;
	}

	// シリアルポートから読み込み
	virtual RESULT Read(unsigned char* buffer, const size_t size, size_t* read_size)
	{
		if (_isConnected == false) return RESULT_ERROR;
		BOOL result = FALSE;
#if 1

//		Sleep(1);
		DWORD errors;
		COMSTAT stat;
		result = ClearCommError(_hPort, &errors, &stat);
		//CE_TXFULL
		if (errors != 0) {
			if (errors & CE_TXFULL) { printf("CE_TXFULL\n"); }
			if (errors & CE_OVERRUN) { printf("CE_OVERRUN\n"); }
			if (errors & CE_FRAME) { printf("CE_FRAME\n"); }
			if (errors & CE_IOE) { printf("CE_IOE\n"); }
			if (errors & CE_PTO) { printf("CE_PTO\n"); }
			if (errors & CE_RXPARITY) { printf("CE_RXPARITY\n"); }


			DWORD err = GetLastError();
			printf("★Error:Serial Port Error:%08x(%d)\n", errors, err);
		}
		//		assert(errors == 0);
		assert(result == TRUE);
//		Sleep(1);

		DWORD len = stat.cbInQue; // 受信したメッセージ長
		if (len > 0) {
			printf("<<READ[%d=%d]:", (int)size, len);
			result = ::ReadFile(_hPort, buffer, len, (LPDWORD)read_size, NULL);
			assert(len == *read_size);
			for (int j = 0; j < *read_size; j++) {
				printf("%02x ", buffer[j]);
			}
			printf("\n");
			assert(result == TRUE);
		}
#else
		Sleep(10);
		printf("<<READ :");
		result = ::ReadFile(_hPort, buffer, size, (LPDWORD)read_size, NULL);
		for (int j = 0; j < *read_size; j++) {
			printf("%02x ", buffer[j]);
		}
		printf("\n");

#endif
	return (result == TRUE) ? RESULT_OK : RESULT_ERROR;
	}

	// シリアルポートへ書き込み
	virtual RESULT Write(const unsigned char* buffer, const size_t size, size_t* written_size)
	{
		
		if (_isConnected == false) return RESULT_ERROR;
		BOOL result = FALSE;
		for (int i = 0; i < 10; i++) {
			printf(">>WRITE[%02d]:",i);
			for (int j = 0; j < size; j++) {
				printf("%02x ", buffer[j]);
			}
			printf("\n");
			result = ::WriteFile(_hPort, buffer, (DWORD)size, (LPDWORD)written_size, NULL);
			assert(result == TRUE);

#if 0
			DWORD errors;
			COMSTAT stat;
			result = ClearCommError(_hPort, &errors, &stat);
			//CE_TXFULL
			if (errors != 0) {

				DWORD err = GetLastError();
				printf("  ★Error:Serial Port Error:%08x(%d)\n", errors, err);
				printf("  =====RETRY!==== \n");
				DWORD len = stat.cbInQue; // 受信したメッセージ長
				if (len > 0) {
					unsigned char buf[1024];
					DWORD read_size;
					result = ::ReadFile(_hPort, buf, len, &read_size, NULL);
					for (int j = 0; j < read_size; j++) {
						printf("[%02x] ", buf[j]);
					}
					assert(result == TRUE);
					printf("\n");
				}
//				Purge();
				Sleep(1);
				break;
//				continue;
			}
			else {
				break;
			}
#else
			break;
#endif

		}
		return (result == TRUE) ? RESULT_OK : RESULT_ERROR;
	}

	// 指定時間(ms)待機
	virtual RESULT Sleep(const int ms) {
		::Sleep(ms);
		return RESULT_OK;
	}

};

#endif // _WIN32
