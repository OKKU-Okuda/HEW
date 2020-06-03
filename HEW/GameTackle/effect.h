//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "../Core/main.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType = EFFECTTYPE_BULLET_PLAYER);

#endif
