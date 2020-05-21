//=============================================================================
//
// コイン処理 [item.cpp]
// Author : 早野太智
//
//=============================================================================
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"
#include "../Core/CheckHit.h"
#include "item.h"	
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureItem[ITEMTYPE_MAX];	// テクスチャ読み込み場所
LPD3DXMESH			g_pMeshItem[ITEMTYPE_MAX];			// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		g_pD3DXMatBuffItem[ITEMTYPE_MAX];	// メッシュのマテリアル情報を格納
DWORD				g_aNumMatItem[ITEMTYPE_MAX];		// 属性情報の総数

D3DXMATRIX			g_mtxWorldItem;						// ワールドマトリックス

ITEM				g_aItem[MAX_ITEM];					// アイテムワーク

const char *c_aFileNameItem[ITEMTYPE_MAX] =
{
	"data/MODEL/item000.x",		// コイン
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(c_aFileNameItem[nCntItemType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffItem[nCntItemType],
			NULL,
			&g_aNumMatItem[nCntItemType],
			&g_pMeshItem[nCntItemType])))
		{
			return E_FAIL;
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
			TEXTURE_FILENAME,						// ファイルの名前
			&g_pD3DTextureModelItem[nCntItemType]);	// 読み込むメモリー
#endif
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].firstpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].endpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fRadius = 0.0f;
		g_aItem[nCntItem].nIdxShadow = -1;
		g_aItem[nCntItem].nType = ITEMTYPE_COIN;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].bHit = false;
		g_aItem[nCntItem].time = 0.0f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		if (g_pD3DTextureItem[nCntItemType] != NULL)
		{// テクスチャの開放
			g_pD3DTextureItem[nCntItemType]->Release();
			g_pD3DTextureItem[nCntItemType] = NULL;
		}

		if (g_pMeshItem[nCntItemType] != NULL)
		{// メッシュの開放
			g_pMeshItem[nCntItemType]->Release();
			g_pMeshItem[nCntItemType] = NULL;
		}

		if (g_pD3DXMatBuffItem[nCntItemType] != NULL)
		{// マテリアルの開放
			g_pD3DXMatBuffItem[nCntItemType]->Release();
			g_pD3DXMatBuffItem[nCntItemType] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].bHit = CheckHitBB(*GetPlayerPos(), g_aItem[nCntItem].pos, D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z), D3DXVECTOR3(ITEM_SIZE_X, ITEM_SIZE_Y, ITEM_SIZE_Z));

			if (g_aItem[nCntItem].bHit == true)
			{
				g_aItem[nCntItem].time += ADD_ITEM_TIME;

				if (g_aItem[nCntItem].time > 1.0f)
				{
					g_aItem[nCntItem].time = 1.0f;
				}


				CalcScreenToWorld(&g_aItem[nCntItem].endpos, ITEM_UI_POS_X, ITEM_UI_POS_Y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, GetMtxView(), GetMtxProjection());

				BezierCurve(&g_aItem[nCntItem].pos, g_aItem[nCntItem].time, &g_aItem[nCntItem].firstpos, &g_aItem[nCntItem].pos, &g_aItem[nCntItem].pos, &g_aItem[nCntItem].endpos);
			}
			else
			{
				g_aItem[nCntItem].firstpos = g_aItem[nCntItem].pos;
			}


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[g_aItem[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[g_aItem[nCntItem].nType]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);

				// 描画
				g_pMeshItem[g_aItem[nCntItem].nType]->DrawSubset(nCntMat);
			}
		}
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// アイテムの設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].fRadius = ITEM_RADIUS;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;

			break;
		}
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if (nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		g_aItem[nIdxItem].bUse = false;
	}
}

//=============================================================================
// アイテムの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

/*=====================================================================
BezierCurve関数
	ベジェ曲線の処理する関数
	戻り値 : D3DXVECTOR3*
	引数 : ( D3DXVECTOR3* , float , D3DXVECTOR3* , D3DXVECTOR3* , D3DXVECTOR3* , D3DXVECTOR3*)
=====================================================================*/
D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_out,		// 戻り値
	float t,				// 0から1までの時間
	D3DXVECTOR3* p_start,	// ベジェ曲線の始点
	D3DXVECTOR3* p_second,	// ベジェ曲線の第1制御点
	D3DXVECTOR3* p_third,	// ベジェ曲線の第2制御点
	D3DXVECTOR3* p_end)		// ベジェ曲線の終点
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

/*=====================================================================
CalcScreenToWorld関数
	スクリーン座標をワールド座標に変換する関数
	戻り値 : D3DXVECTOR3*
	引数 : 	(D3DXVECTOR3* , float , float , float , int , int , D3DXMATRIX* , D3DXMATRIX* )
=====================================================================*/
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,	// 戻り値
	float Sx,			// スクリーンX座標
	float Sy,			// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0〜1）
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

	VP._11 = Screen_w / 2.0f;
	VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f;
	VP._42 = Screen_h / 2.0f;

	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(p_out, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return p_out;
}