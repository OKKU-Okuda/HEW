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
#include "field.h"
#include "pole.h"	
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
LPDIRECT3DTEXTURE9	g_pD3DTexturePole[POLETYPE_MAX];	// テクスチャ読み込み場所
LPD3DXMESH			g_pMeshPole[POLETYPE_MAX];			// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		g_pD3DXMatBuffPole[POLETYPE_MAX];	// メッシュのマテリアル情報を格納
DWORD				g_aNumMatPole[POLETYPE_MAX];		// 属性情報の総数

D3DXMATRIX			g_mtxWorldPole;						// ワールドマトリックス

POLE				g_aPole[MAX_POLE];					// アイテムワーク

const char *c_aFileNamePole[POLETYPE_MAX] =
{
	"data/MODEL/pole.x",		// コイン
};

int g_PolePoint;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPole(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPoleType = 0; nCntPoleType < POLETYPE_MAX; nCntPoleType++)
	{
		g_pD3DTexturePole[nCntPoleType] = NULL;
		g_pMeshPole[nCntPoleType] = NULL;
		g_pD3DXMatBuffPole[nCntPoleType] = NULL;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(c_aFileNamePole[nCntPoleType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffPole[nCntPoleType],
			NULL,
			&g_aNumMatPole[nCntPoleType],
			&g_pMeshPole[nCntPoleType])))
		{
			return E_FAIL;
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
			TEXTURE_FILENAME,						// ファイルの名前
			&g_pD3DTextureModelPole[nCntPoleType]);	// 読み込むメモリー
#endif
	}

	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		g_aPole[nCntPole].pos = D3DXVECTOR3(500.0f, 10.0f, 600.0f + (nCntPole * 30));
		g_aPole[nCntPole].scl = D3DXVECTOR3(1.f, 1.f, 1.f);
		g_aPole[nCntPole].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPole[nCntPole].fRadius = 0.0f;
		g_aPole[nCntPole].nIdxShadow = -1;
		g_aPole[nCntPole].nType = POLETYPE_COIN;
		g_aPole[nCntPole].bUse = false;
		g_aPole[nCntPole].bHit = false;
	}

	g_PolePoint = 0;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPole(void)
{
	for (int nCntPoleType = 0; nCntPoleType < POLETYPE_MAX; nCntPoleType++)
	{
		if (g_pD3DTexturePole[nCntPoleType] != NULL)
		{// テクスチャの開放
			g_pD3DTexturePole[nCntPoleType]->Release();
			g_pD3DTexturePole[nCntPoleType] = NULL;
		}

		if (g_pMeshPole[nCntPoleType] != NULL)
		{// メッシュの開放
			g_pMeshPole[nCntPoleType]->Release();
			g_pMeshPole[nCntPoleType] = NULL;
		}

		if (g_pD3DXMatBuffPole[nCntPoleType] != NULL)
		{// マテリアルの開放
			g_pD3DXMatBuffPole[nCntPoleType]->Release();
			g_pD3DXMatBuffPole[nCntPoleType] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePole(void)
{
	for (int nCntPole = 0; nCntPole < POLETYPE_MAX; nCntPole++)
	{
		g_aPole[nCntPole].pos = *GetPlayerPos();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPole(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;

	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		if (g_aPole[nCntPole].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldPole);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aPole[nCntPole].scl.x, g_aPole[nCntPole].scl.y, g_aPole[nCntPole].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPole[nCntPole].rot.y, g_aPole[nCntPole].rot.x, g_aPole[nCntPole].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aPole[nCntPole].pos.x, g_aPole[nCntPole].pos.y, g_aPole[nCntPole].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPole);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffPole[g_aPole[nCntPole].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatPole[g_aPole[nCntPole].nType]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTexturePole[g_aPole[nCntPole].nType]);

				// 描画
				g_pMeshPole[g_aPole[nCntPole].nType]->DrawSubset(nCntMat);
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

	if (GetKeyboardPress(DIK_R))
	{
		InitPole();
	}
}

//=============================================================================
// アイテムの設定
//=============================================================================
void SetPole(FIELD_CHIP* pParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		if (!g_aPole[nCntPole].bUse)
		{
			g_aPole[nCntPole].pos = pos;
			g_aPole[nCntPole].rot = rot;
			g_aPole[nCntPole].fRadius = POLE_RADIUS;
			g_aPole[nCntPole].nType = nType;
			g_aPole[nCntPole].bUse = true;

			break;
		}
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void DeletePole(int nIdxPole)
{
	if (nIdxPole >= 0 && nIdxPole < MAX_POLE)
	{
		g_aPole[nIdxPole].bUse = false;
	}
}

//=============================================================================
// item配列のゼロクリア
//=============================================================================
void ResetPole()
{
	ZeroMemory(g_aPole, sizeof(g_aPole));
}

//=============================================================================
// アイテムの取得
//=============================================================================
POLE *GetPole(void)
{
	return &g_aPole[0];
}
