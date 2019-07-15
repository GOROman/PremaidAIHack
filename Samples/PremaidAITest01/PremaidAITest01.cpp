
#include <stdio.h>
#include <CPremaidAI.h>

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wsock32.lib")


#define PORT		_T("COM3")
#define BAUDRATE	1250000

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


	ai.SetPositionAll(7500);

	for (int id = 1; id <= 25; id++) {
		ai.SetStrectch(id, 24);//24
	}
	for (int id = 1; id <= 25; id++) {
		ai.SetSpeed(id, 100);// 63 + 8 * j);
	}

	WORD angle;
	while (1) {
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



		char buf[1024];
		int n = recv(sock, buf, sizeof(buf), 0);
		if (n < 1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
			}
			else {
				printf("error : 0x%x\n", WSAGetLastError());
			}
		}
		else {
			//			memcpy(buf, &angle, sizeof(buf));
			int a = 0;
			sscanf_s(buf, "%d=%d\n", &id, &a);
			printf("ID:%2d = %5d\n", id, a);
			ai.SetPosition(id, a);
		}


	}
	closesocket(sock);

	WSACleanup();

}

int main()
{
	timeBeginPeriod(1);
	printf("Premaid AI Test 01\n");

	CPremaidAI ai;

	// プリメイドAI ICSへ接続
	// 注.) Bluetooth 経由での接続ではありません
	ai.Connect(PORT, BAUDRATE);

	// サーボスピード設定
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetSpeed(i, 90);
	}
	// ストレッチ設定
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetStrectch(i, 40);
	}
	ai.SetPositionAll(0);

	network_sample(ai);

//	mirroring_sample(ai);

#if 0
	sample(ai);
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
