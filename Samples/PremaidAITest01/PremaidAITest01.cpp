
#define PORT_HV		_T("\\\\.\\COM1")		// DUAL USB ADAPTER HS
#define PORT_MV		_T("\\\\.\\COM2")		// USB Serial Port


#define BAUDRATE	1250000


#include <stdio.h>
#include <CPremaidAI.h>
#include "../../LibPremaidAI/Windows/CSerialPort_Windows.h"
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wsock32.lib")
	


static void mirroring_sample(CPremaidAI& ai)
{
	for (int i = 0; i < 1000; ++i) {
		int pos;
		ai.SetPosition(8, 0);
		pos = ai.GetPosition(8);	
		ai.SetPosition( 10, pos );

		ai.SetPosition(12, 0);
		pos = ai.GetPosition(12);
		ai.SetPosition(14, pos);

		ai.SetPosition(16, 0);
		pos = ai.GetPosition(16);
		ai.SetPosition(18, pos);
	}
}

void network_sample(CPremaidAI& ai) {
	
	
	WSAData wsaData;

	SOCKET sock;
	struct sockaddr_in addr;
	struct sockaddr_in send_addr;

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(12346);
	send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IPアドレスを格納したin_addr構造体

	bind(sock, (struct sockaddr*) & addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	int pos[25+1] = { 0 };
	int old[25+1] = { 0 };

#if 0
	ai.SetPositionAll(7500);

	for (int id = 1; id <= 25; id++) {
		ai.SetStrectch(id, 40);//24
	}
	for (int id = 1; id <= 25; id++) {
		ai.SetSpeed(id, 80);// 63 + 8 * j);
	}
#endif 

	WORD angle;

	while (1) {
#if 0
		static int table[11 + 1] = {
			0,

			2,
			4,
			6,
			8,
			10,

			12,
			14,
			16,
			18,
			20,

			22,
		};
#endif
#if 0
		for (int i = 1; i <= 11; i++) {
			int id = table[i];
			ai.SetPosition(id, 0);

			angle = ai.GetPosition(id);

			char buf[256];
			sprintf_s(buf, "%d=%d", id, angle);
			if (sendto(sock, buf, strlen(buf) + 1, 0, (struct sockaddr*) & send_addr, sizeof(send_addr)) < 0) {
				printf("error:%d\n", WSAGetLastError());
			}

			//		Sleep(10);
		}

		//		printf("ID:%2d %3d\n", id, ics.Pos[id]);
#endif
	int id = 1;

	char buf[4096];
	for (int j = 0; j < 1; ++j) {
		int n = recv(sock, buf, sizeof(buf), 0);
		if (n < 1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
			}
			else {
				printf("error : 0x%x\n", WSAGetLastError());
			}
			break;
		}
		else {
			int a = 0;
			sscanf_s(buf, "%d=%d\n", &id, &a);
			printf("ID:%2d = %5d\n", id, a);
			pos[id] = a;
		}
	}
	for (int i = 1; i <= 25; ++i) {

		if (pos[i] != old[i]) {
			old[i] = pos[i];
			
			ai.SetPosition(id, pos[i]);

		}
	}
	//Sleep(1);

	}
	closesocket(sock);

	WSACleanup();

}

void network_sample2() {

	ISerialPort* port = new CSerialPort_Windows;
	port->Connect(_T("COM10"), 115200, ISerialPort::NONE);

	WSAData wsaData;

	SOCKET sock;
	struct sockaddr_in addr;
	struct sockaddr_in send_addr;

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(12346);
	send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IPアドレスを格納したin_addr構造体

	bind(sock, (struct sockaddr*) & addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	int pos[25+1] = { 0 };
	int old[25+1] = { 0 };

#if 0
	ai.SetPositionAll(7500);

	for (int id = 1; id <= 25; id++) {
		ai.SetStrectch(id, 40);//24
	}
	for (int id = 1; id <= 25; id++) {
		ai.SetSpeed(id, 80);// 63 + 8 * j);
	}
#endif 

	WORD angle;

	while (1) {
#if 0
		static int table[11 + 1] = {
			0,

			2,
			4,
			6,
			8,
			10,

			12,
			14,
			16,
			18,
			20,

			22,
		};
#endif

		int id = 1;

		char buf[4096];
		for (int j = 0; j < 1; ++j) {
			int n = recv(sock, buf, sizeof(buf), 0);
			if (n < 1) {
				if (WSAGetLastError() == WSAEWOULDBLOCK) {
				}
				else {
					printf("error : 0x%x\n", WSAGetLastError());
				}
				break;
			}
			else {
				int a = 0;
				sscanf_s(buf, "%d=%d\n", &id, &a);
				printf("ID:%2d = %5d\n", id, a);
				pos[id] = a;
			}
		}
		for (int i = 1; i <= 25; ++i) {

			if (pos[i] != old[i]) {
				old[i] = pos[i];
				size_t written_size = 0;
				unsigned char buf[256] = { 0 };
				buf[0] = i;
				buf[1] = pos[i] >> 7;
				buf[2] = pos[i] & 0x7f;
				port->Write(buf, 3, &written_size);

			}
		}
		Sleep(1);

	}
	closesocket(sock);

	WSACleanup();

	delete port;

}

void test()
{
#if 0
	while (1)
	{
		size_t read_size = 0;
		port->Read((unsigned char*)buf, sizeof(buf), &read_size);
		if (read_size > 0) {
			printf("%d - %c\r\n", read_size, buf[0]);

		}
		printf(".");
		Sleep(100);
	}
#endif

}

int main()
{
	timeBeginPeriod(1);
	printf("Premaid AI Test 01\n");
//		network_sample2();
		
		
	CPremaidAI ai;
	
//test();


	// プリメイドAI ICSへ接続
	// 注.) Bluetooth 経由での接続ではありません
	ai.Connect(PORT_HV, PORT_MV, BAUDRATE);


	// ストレッチ設定
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetStrectch(i, 40);
	}

	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetPosition(i, 7500);
	}

	// サーボスピード設定	
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetSpeed(i, 60);
	}
	ai.SetPosition(24, 7800);

	for (int i = 0; i < 5; ++i) {
		ai.SetPosition(1, 7500);
		ai.SetPosition(2, 7500);
		ai.SetPosition(3, 7500-0);
		Sleep(200);
		ai.SetPosition(1, 7500);
		ai.SetPosition(2, 7500);
		ai.SetPosition(3, 7500);
		Sleep(200);

	}
	network_sample(ai);
//	mirroring_sample(ai);

#if 0
	// サーボ位置を動かす例
	for (int j = 0; j < 10; ++j) {
		printf(">>>>>%d\n", j);
		for (int i = 1; i <= SERVO_COUNT; ++i) {
			ai.SetPosition(i, 7500);
		}

		for (int i = 1; i <= SERVO_COUNT; ++i) {
			ai.SetPosition(i, 7700);
		}
	}
	// サーボ位置を読み取る
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		int pos = ai.GetPosition(i);
		printf("ServoPos: ID:%2d = %d\n", i, pos);
	}
	// いまのところ何もしない命令
	ai.Update();

	// サーボ温度を取得
	for (int i = 1; i <= 25; ++i) {
		printf("ID:%02d Temp:%f\n", i, ai.GetTemperature(i));
	}
#endif

	return 0;
}
