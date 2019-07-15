#pragma once

#include <tchar.h>
#include <stdio.h>
#include <assert.h>

#define _USE_MATH_DEFINES	// M_PIを使うため
#include <math.h>

#include <cstddef>

#define DEG2RAD(deg) (((deg)/360)*2*M_PI)
#define RAD2DEG(rad) (((rad)/2/M_PI)*360)

// リザルト
enum RESULT {
	RESULT_OK = 0,						// エラー無
	RESULT_ERROR = -1,					// エラー発生

	RESULT_NOTIMPLEMENTED	= -999999,	// 未実装API
};
