
#include <stdio.h>
#include <CPremaidAI.h>

#define PORT		_T("COM3")
#define BAUDRATE	1250000

int main()
{
	printf("Premaid AI Test 01\n");

	CPremaidAI ai;

	// プリメイドAI ICSへ接続
	// 注.) Bluetooth 経由での接続ではありません
	ai.Connect(PORT, BAUDRATE);

	// サーボスピード設定
	for (int i = 1; i <= SERVO_COUNT; ++i) {
		ai.SetSpeed(i, 40);
	}

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

	return 0;
}
