//=============================================================================
//
// 影処理 [shadow.h]
// Author : 高橋翔輝
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
void ReleaseShadow(int nIdxShadow);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);

#endif
