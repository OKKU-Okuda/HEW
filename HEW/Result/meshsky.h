//=============================================================================
//
// メッシュ空の処理 [meshsky.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ,
						float fRadius, float fRotY, int nTexType = -1, bool bReverse = false);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);

