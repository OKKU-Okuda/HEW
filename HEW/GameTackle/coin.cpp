//=============================================================================
//
// コイン処理 [coin.cpp]
// Author : 早野太智
//
//=============================================================================
#include"../Core/main.h"
#include "coin.h"	
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// ベジェ曲線の処理
//=============================================================================
D3DXVECTOR3 *BezierCurve( D3DXVECTOR3* p_out, float t, D3DXVECTOR2* p_start, D3DXVECTOR2* p_second, D3DXVECTOR2* p_third, D3DXVECTOR2* p_end)
{
	float tp = 1 - t;
	float a, b, c, d;

	// ３次のバーンステイン多項式
	a = t * t * t;
	b = 3 * t * t * tp;
	c = 3 * t * tp * tp;
	d = tp * tp * tp;

	p_out->x = (a * p_end->x) + (b * p_third->x) + (c * p_second->x) + (d * p_start->x);
	p_out->y = (a * p_end->y) + (b * p_third->y) + (c * p_second->y) + (d * p_start->y);
	p_out->z = 0;

	return p_out;
}

//=============================================================================
// スクリーン座標をワールド座標に変換
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,			// スクリーンX座標
	float Sy,			// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0～1）
	int Screen_w,		// スクリーンの横幅
	int Screen_h,		// スクリーンの縦幅
	D3DXMATRIX* View,	// ビューマトリックス
	D3DXMATRIX* Prj		// プロジェクションマトリックス
) {
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(p_out, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return p_out;
}