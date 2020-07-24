//=============================================================================
//
// スタート画面処理 [countdown.h]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include"../Core/main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCountdown(int type);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
int GetCountdownFlag(void);

//=============================================================================
// カウント時間取得関数[カウント状態を考慮しない]
//=============================================================================
void GetCountdownTime(int* pRemainSec, float* pRemainRatePerSec);

#endif
