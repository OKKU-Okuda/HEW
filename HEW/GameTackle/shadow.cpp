//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : 高橋翔輝
//
//=============================================================================
#include"../Core/main.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(25.0f)							// 影の幅
#define	SHADOW_SIZE_Z		(25.0f)							// 影の高さ

#define	MAX_SHADOW			(256)							// 影最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scl;		// 拡大縮小
	bool bUse;				// 使用しているかどうか
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuffShadow = NULL;		// 頂点バッファインターフェースへのポインタ

static SHADOW					g_aShadow[MAX_SHADOW];			// 影ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexShadow(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_SHADOW,				// ファイルの名前
								&g_pD3DTextureShadow);		// 読み込むメモリー

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	if(g_pD3DTextureShadow != NULL)
	{// テクスチャの開放
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow = NULL;
	}

	if(g_pD3DVtxBuffShadow != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffShadow->Release();
		g_pD3DVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// 減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(g_aShadow[nCntShadow].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aShadow[nCntShadow].scl.x, g_aShadow[nCntShadow].scl.y, g_aShadow[nCntShadow].scl.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aShadow[nCntShadow].pos.x,
													g_aShadow[nCntShadow].pos.y,
													g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld,
										&g_aShadow[nCntShadow].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをレンダリングパイプラインに設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureShadow);

			// ポリゴンの描画
		//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntShadow * 4), NUM_POLYGON);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int max_shadow = 1;		// MAX_SHADOW

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * max_shadow,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffShadow,					// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntShadow = 0; nCntShadow < max_shadow; nCntShadow++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[1].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[2].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
			pVtx[3].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ)
{
	//if (nIdxShadow < 0) return;	// 不正ならリターンする
								//{//頂点バッファの中身を埋める
	//	VERTEX_3D *pVtx;

	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//	pVtx += (nIdxShadow * 4);

	//	// 頂点座標の設定
	//	pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, 0.0f, fSizeZ / 2);
	//	pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, 0.0f, fSizeZ / 2);
	//	pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, 0.0f, -fSizeZ / 2);
	//	pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, 0.0f, -fSizeZ / 2);

	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffShadow->Unlock();
	//}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorShadow(int nIdxShadow, D3DXCOLOR col)
{
	//if (nIdxShadow < 0) return;	// 不正ならリターンする
								//{//頂点バッファの中身を埋める
	//	VERTEX_3D *pVtx;

	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//	pVtx += (nIdxShadow * 4);

	//	// 頂点座標の設定
	//	pVtx[0].diffuse =
	//	pVtx[1].diffuse =
	//	pVtx[2].diffuse =
	//	pVtx[3].diffuse = col;

	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffShadow->Unlock();
	//}
}

//=============================================================================
// 影の作成
//=============================================================================
int CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	int nIdxShadow = -1;

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].scl = scl;
			g_aShadow[nCntShadow].bUse = true;

			nIdxShadow = nCntShadow;
			break;
		}
	}

	return nIdxShadow;
}

//=============================================================================
// 影の破棄
//=============================================================================
void ReleaseShadow(int nIdxShadow)
{
	if(nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// 位置の設定
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	if (nIdxShadow < 0) return;	// 不正ならリターンする

	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].scl = scl;
}

