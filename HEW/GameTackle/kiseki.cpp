//=============================================================================
//
// 剣の軌跡処理 [kiseki.cpp]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"
#include "kiseki.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_KISEKI						"data/TEXTURE/kiseki02.png"		// 読み込むテクスチャファイル名
#define	BULLET_SIZE_X						(50.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y						(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_KISEKI					(2.0f)							// 移動速度
#define	MAX_KISEKI							(5000)							// ビルボード最大数
#define KISEKI_TEXTURE_PATTERN_DIVIDE_X		(30)								// テクスチャX分割数
#define KISEKI_TEXTURE_PATTERN_DIVIDE_Y		(1)								// テクスチャY分割数

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
	int nType;				// 種類
	bool bUse;				// 使用しているかどうか
} KISEKI;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexKiseki(LPDIRECT3DDEVICE9 pDevice);
void SetVertexKiseki(int nIdxKiseki, D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 senntann1, D3DXVECTOR3 sentann2);
void SetColorKiseki(int nIdxKiseki, D3DXCOLOR col);
void SetTextureKiseki(int nIdxKiseki, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureKiseki = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffKiseki = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldKiseki;			// ワールドマトリックス

KISEKI				g_aKiseki[MAX_KISEKI];	// 爆発ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitKiseki(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexKiseki(pDevice);
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_KISEKI,			// ファイルの名前
			&g_pD3DTextureKiseki);	// 読み込むメモリー
	}

	for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		g_aKiseki[nCntKiseki].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKiseki[nCntKiseki].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKiseki[nCntKiseki].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aKiseki[nCntKiseki].fSizeX = BULLET_SIZE_X;
		g_aKiseki[nCntKiseki].fSizeY = BULLET_SIZE_Y;
		g_aKiseki[nCntKiseki].nCounter = 0;
		g_aKiseki[nCntKiseki].nPattern = 0;
		g_aKiseki[nCntKiseki].nType = KISEKITYPE_BULLET_PLAYER;
		g_aKiseki[nCntKiseki].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKiseki(void)
{
	if(g_pD3DTextureKiseki != NULL)
	{// テクスチャの開放
		g_pD3DTextureKiseki->Release();
		g_pD3DTextureKiseki = NULL;
	}

	if(g_pD3DVtxBuffKiseki != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffKiseki->Release();
		g_pD3DVtxBuffKiseki = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKiseki(void)
{

	for (int i = 0; i < MAX_KISEKI; i++)
	{
		if (g_aKiseki[i].bUse)
		{

			//透過処理
			g_aKiseki[i].col.a -= 0.07f;
			if(g_aKiseki[i].col.a < 0.0f)
			{
				//a値0以下になったら0
				g_aKiseki[i].col.a = 0.0f;

				//falseになる
				g_aKiseki[i].bUse = false;
			}
			SetColorKiseki(i, g_aKiseki[i].col);

		}


	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawKiseki(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z比較なし

	for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		if(g_aKiseki[nCntKiseki].bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldKiseki);

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aKiseki[nCntKiseki].scale.x, g_aKiseki[nCntKiseki].scale.y, g_aKiseki[nCntKiseki].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldKiseki, &g_mtxWorldKiseki, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aKiseki[nCntKiseki].pos.x, g_aKiseki[nCntKiseki].pos.y, g_aKiseki[nCntKiseki].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldKiseki, &g_mtxWorldKiseki, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldKiseki);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffKiseki, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureKiseki);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntKiseki * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexKiseki(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_KISEKI,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffKiseki,					// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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
		g_pD3DVtxBuffKiseki->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexKiseki(int nIdxKiseki, D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].vtx = nemoto2-nemoto1;
		pVtx[2].vtx = sentan1-nemoto1;
		pVtx[3].vtx = sentan2-nemoto1;


		// 頂点データをアンロックする
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorKiseki(int nIdxKiseki, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureKiseki(int nIdxKiseki, int nPattern)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);


		// テクスチャ座標の設定
		int x = nPattern % KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		int y = nPattern / KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		float sizeX = 1.0f / KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		float sizeY = 1.0f / KISEKI_TEXTURE_PATTERN_DIVIDE_Y;


		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetKiseki(D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2, int nType,int col_type)
{
	int nIdxKiseki = -1;
	for (int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		if (!g_aKiseki[nCntKiseki].bUse)
		{
			g_aKiseki[nCntKiseki].pos = nemoto1;
			g_aKiseki[nCntKiseki].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aKiseki[nCntKiseki].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aKiseki[nCntKiseki].nCounter = 0;
			g_aKiseki[nCntKiseki].nPattern = 0;
			g_aKiseki[nCntKiseki].nType = nType;
			g_aKiseki[nCntKiseki].bUse = true;

			// 頂点座標の設定
			SetVertexKiseki(nCntKiseki, nemoto1, nemoto2, sentan1, sentan2);

			if (col_type == 1)
			{
				g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

			}
			else
			{
				g_aKiseki[nCntKiseki].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.5f);
			}

			SetColorKiseki(nCntKiseki, g_aKiseki[nCntKiseki].col);

			// テクスチャ座標の設定
			SetTextureKiseki(nCntKiseki, g_aKiseki[nCntKiseki].nType);

			nIdxKiseki = nCntKiseki;

			break;

		}
	}



	return nIdxKiseki;
}



