//=============================================================================
//
// UI処理 [UI.cpp]
// Author : 福島　凜
//
//=============================================================================
#include"../Core/main.h"

#include "UI.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCoin(LPDIRECT3DDEVICE9 pDevice);
void SetTextureCoin(int idx, int number);

HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice);
void SetTextureDistance(int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
					/*コイン*/
LPDIRECT3DTEXTURE9		g_pD3DTextureCoin[2] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffCoin = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posCoin;						// 位置
D3DXVECTOR3				g_rotCoin;						// 向き

int						g_coin;							// コイン

					/*距離*/
LPDIRECT3DTEXTURE9		g_pD3DTextureDistance[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffDistance = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posDistance;					// 位置
D3DXVECTOR3				g_rotDistance;					// 向き

int						g_distance;						// 距離

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_COIN,			// ファイルの名前
			&g_pD3DTextureCoin[0]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FRAME_COIN,		// ファイルの名前
			&g_pD3DTextureCoin[1]);	// 読み込むメモリー

				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_DISTANCE,			// ファイルの名前
			&g_pD3DTextureDistance[0]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FRAME_DISTANCE,		// ファイルの名前
			&g_pD3DTextureDistance[1]);	// 読み込むメモリー

	}

					/*コイン*/
	{
		g_posCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_rotCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// スコアの初期化
		g_coin = 0;

		// 頂点情報の作成
		MakeVertexCoin(pDevice);
	}

					/*距離*/
	{
		g_posDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_rotDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// スコアの初期化
		g_distance = 0;

		// 頂点情報の作成
		MakeVertexDistance(pDevice);

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
{
					/*コイン*/
	{
		if (g_pD3DTextureCoin[0] != NULL)
		{// テクスチャの開放
			g_pD3DTextureCoin[0]->Release();
			g_pD3DTextureCoin[0] = NULL;
		}
		if (g_pD3DTextureCoin[1] != NULL)
		{// テクスチャの開放
			g_pD3DTextureCoin[1]->Release();
			g_pD3DTextureCoin[1] = NULL;
		}

		if (g_pD3DVtxBuffCoin != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffCoin->Release();
			g_pD3DVtxBuffCoin = NULL;
		}
	}

					/*距離*/
	{
		if (g_pD3DTextureDistance[0] != NULL)
		{// テクスチャの開放
			g_pD3DTextureDistance[0]->Release();
			g_pD3DTextureDistance[0] = NULL;
		}
		if (g_pD3DTextureDistance[1] != NULL)
		{// テクスチャの開放
			g_pD3DTextureDistance[1]->Release();
			g_pD3DTextureDistance[1] = NULL;
		}

		if (g_pD3DVtxBuffDistance != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffDistance->Release();
			g_pD3DVtxBuffDistance = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUI(void)
{
					/*コイン*/
	{
		for (int nCntPlace = 0; nCntPlace < COIN_PLACE; nCntPlace++)
		{
			int number;

			number = (g_coin % (int)(powf(10.0f, (float)(COIN_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(COIN_PLACE - nCntPlace - 1))));

			SetTextureCoin(nCntPlace, number);
		}
	}
					/*距離*/
	{
		for (int nCntPlace = 0; nCntPlace < DISTANCE_PLACE; nCntPlace++)
		{
			int number;

			number = (g_distance % (int)(powf(10.0f, (float)(DISTANCE_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(DISTANCE_PLACE - nCntPlace - 1))));
			SetTextureDistance(nCntPlace, number);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

					/*コイン*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffCoin, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureCoin[0]);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < COIN_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureCoin[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (COIN_PLACE * 4), NUM_POLYGON);
	}
					/*距離*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffDistance, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureDistance[0]);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < DISTANCE_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureDistance[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (DISTANCE_PLACE * 4), NUM_POLYGON);
	}
}
//=============================================================================
// 頂点の作成	コイン
//=============================================================================
HRESULT MakeVertexCoin(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * COIN_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffCoin,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < COIN_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(COIN_POS_X + nCntPlace * COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(COIN_POS_X + nCntPlace * (COIN_INTERVAL_X + COIN_SIZE_X) + COIN_SIZE_X, COIN_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(COIN_POS_X + nCntPlace * COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y + COIN_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(COIN_POS_X + nCntPlace * (COIN_INTERVAL_X + COIN_SIZE_X) + COIN_SIZE_X, COIN_POS_Y + COIN_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(COIN_ICON_POS_X - 15, COIN_ICON_POS_Y - 15, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X) * COIN_PLACE + 15, COIN_ICON_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(COIN_ICON_POS_X - 15, COIN_ICON_POS_Y + 15, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X) * COIN_PLACE + 15, COIN_ICON_POS_Y + 15, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffCoin->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成	距離
//=============================================================================
HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * DISTANCE_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffDistance,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < DISTANCE_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(DISTANCE_POS_X + nCntPlace * DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(DISTANCE_POS_X + nCntPlace * (DISTANCE_INTERVAL_X + DISTANCE_SIZE_X) + DISTANCE_SIZE_X, DISTANCE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(DISTANCE_POS_X + nCntPlace * DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y + DISTANCE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(DISTANCE_POS_X + nCntPlace * (DISTANCE_INTERVAL_X + DISTANCE_SIZE_X) + DISTANCE_SIZE_X, DISTANCE_POS_Y + DISTANCE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(METER_POS_X - 15, METER_POS_Y - 15, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)* DISTANCE_PLACE + 15, METER_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(METER_POS_X - 15, METER_POS_Y + 25, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)* DISTANCE_PLACE + 15, METER_POS_Y + 25, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffDistance->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定		コイン
//=============================================================================
void SetTextureCoin(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffCoin->Unlock();
}

//=============================================================================
// テクスチャ座標の設定		距離
//=============================================================================
void SetTextureDistance(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffDistance->Unlock();
}

//=============================================================================
// コインの変更
//=============================================================================
void ChangeCoin(int value)
{
	g_coin += value;

	if (g_coin < 0)
	{
		g_coin = 0;
	}
	else if (g_coin >= (int)(powf(10.0f, (float)(COIN_PLACE + 1))))
	{
		g_coin = (int)(powf(10.0f, (float)(COIN_PLACE + 1))) - 1;
	}
}

//=============================================================================
// 距離の変更
//=============================================================================
void ChangeDistance(int value)
{
	g_distance += value;
	if (g_distance < 0)
	{
		g_distance = 0;
	}
	else if (g_distance >= (int)(powf(10.0f, (float)(DISTANCE_PLACE + 1))))
	{
		g_distance = (int)(powf(10.0f, (float)(DISTANCE_PLACE + 1))) - 1;
	}
}
