//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#include "../Core/camera.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT						"data/TEXTURE/explosion000.png"	// 読み込むテクスチャファイル名
#define	BULLET_SIZE_X						(50.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y						(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_EFFECT					(2.0f)							// 移動速度
#define	MAX_EFFECT							(128)							// ビルボード最大数
#define EFFECT_TEXTURE_PATTERN_DIVIDE_X		(8)                             // テクスチャX分割数
#define EFFECT_TEXTURE_PATTERN_DIVIDE_Y		(1)                             // テクスチャY分割数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nCounter;			// カウンター
	int nPattern;			// パターンNo.
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);
void SetTextureEffect(int nIdxEffect, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldEffect;			// ワールドマトリックス

EFFECT				g_aEffect[MAX_EFFECT];	// エフェクトワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EFFECT,			// ファイルの名前
			&g_pD3DTextureEffect);	// 読み込むメモリー
	}
	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fSizeX = BULLET_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = BULLET_SIZE_Y;
		g_aEffect[nCntEffect].nCounter = 0;
		g_aEffect[nCntEffect].nPattern = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if(g_pD3DTextureEffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if(g_pD3DVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	//D3DXVECTOR3 *rotCamera = GetRotCamera();


	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].bUse)
		{
			//nCounter-
			g_aEffect[nCntEffect].nCounter--;

			//4フレームごとに
			if((g_aEffect[nCntEffect].nCounter % 4) == 0)
			{
				//新しいテクスチャ
				g_aEffect[nCntEffect].nPattern++;
				if(g_aEffect[nCntEffect].nPattern >= 6)
				{//8枚目のテクスチャになったら
					g_aEffect[nCntEffect].bUse = false;//falseになる
				}
				else
				{
					// テクスチャ座標の設定
					SetTextureEffect(nCntEffect, g_aEffect[nCntEffect].nPattern);//新しいパターンのテクスチャ
				}
			}

			// 頂点座標の設定 1フレームごと大きくなる
			g_aEffect[nCntEffect].fSizeX += 0.50f;
			g_aEffect[nCntEffect].fSizeY += 0.50f;
			SetVertexEffect(nCntEffect, g_aEffect[nCntEffect].fSizeX, g_aEffect[nCntEffect].fSizeY);

			//透過処理
			if(g_aEffect[nCntEffect].nPattern >= 2)
			{
				// 頂点カラーの設定
				g_aEffect[nCntEffect].col.a -= 0.02f;
				if(g_aEffect[nCntEffect].col.a < 0.0f)
				{
					g_aEffect[nCntEffect].col.a = 0.0f;
				}
				SetColorEffect(nCntEffect, g_aEffect[nCntEffect].col);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z比較なし

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// ビューマトリックスを取得
			mtxView = *GetMtxView();

			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffEffect,					// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEffect(int nIdxEffect, int nPattern)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// テクスチャ座標の設定
		int x = nPattern % EFFECT_TEXTURE_PATTERN_DIVIDE_X;
		int y = nPattern / EFFECT_TEXTURE_PATTERN_DIVIDE_X;
		float sizeX = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_X;
		float sizeY = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_Y;

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	int nIdxEffect = -1;

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nCounter = 0;
			g_aEffect[nCntEffect].nPattern = 0;
			g_aEffect[nCntEffect].bUse = true;

			// 頂点座標の設定
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			SetColorEffect(nCntEffect, g_aEffect[nCntEffect].col);

			// テクスチャ座標の設定
			SetTextureEffect(nCntEffect, 0);

			nIdxEffect = nCntEffect;

			break;
		}
	}

	return nIdxEffect;
}
