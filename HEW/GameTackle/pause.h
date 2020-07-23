//=============================================================================
//
// ポーズ画面処理 [pause.h]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_
#include"../Core/main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(int type);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
bool GetPauseUse(void);

#endif
