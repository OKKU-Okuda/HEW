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

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
					/*コイン*/
UI		g_ui;

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
			&g_ui.pD3DTextureCoin[0]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FRAME_COIN,		// ファイルの名前
			&g_ui.pD3DTextureCoin[1]);	// 読み込むメモリー

				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_DISTANCE,			// ファイルの名前
			&g_ui.pD3DTextureDistance[0]);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FRAME_DISTANCE,		// ファイルの名前
			&g_ui.pD3DTextureDistance[1]);	// 読み込むメモリー

	}

	/*コイン*/
	{
		g_ui.posCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ui.rotCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// スコアの初期化
		g_ui.coin = 0;
		g_ui.coin_target = 10;

		g_ui.COIN_SIZE_X = 35.0f;
		g_ui.COIN_SIZE_Y = 50.0f;

		g_ui.COIN_PLACE = 1;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		// 頂点情報の作成
		MakeVertexCoin(pDevice);
	}

	/*距離*/
	{
		g_ui.posDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ui.rotDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// スコアの初期化
		g_ui.distance = 0;
		g_ui.distance_target = 10;

		g_ui.DISTANCE_SIZE_X = 35.0f;
		g_ui.DISTANCE_SIZE_Y = 50.0f;

		g_ui.DISTANCE_PLACE = 1;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		// 頂点情報の作成
		MakeVertexDistance(pDevice);

	}

	g_ui.add_timer = 0.0f;
	g_ui.add_timer_max = 60 * 1;

	g_ui.dec_timer = 0.0f;
	g_ui.dec_timer_max = 30 * 1;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
{
	/*コイン*/
	{
		if (g_ui.pD3DTextureCoin[0] != NULL)
		{// テクスチャの開放
			g_ui.pD3DTextureCoin[0]->Release();
			g_ui.pD3DTextureCoin[0] = NULL;
		}
		if (g_ui.pD3DTextureCoin[1] != NULL)
		{// テクスチャの開放
			g_ui.pD3DTextureCoin[1]->Release();
			g_ui.pD3DTextureCoin[1] = NULL;
		}

		if (g_ui.pD3DVtxBuffCoin != NULL)
		{// 頂点バッファの開放
			g_ui.pD3DVtxBuffCoin->Release();
			g_ui.pD3DVtxBuffCoin = NULL;
		}
	}

	/*距離*/
	{
		if (g_ui.pD3DTextureDistance[0] != NULL)
		{// テクスチャの開放
			g_ui.pD3DTextureDistance[0]->Release();
			g_ui.pD3DTextureDistance[0] = NULL;
		}
		if (g_ui.pD3DTextureDistance[1] != NULL)
		{// テクスチャの開放
			g_ui.pD3DTextureDistance[1]->Release();
			g_ui.pD3DTextureDistance[1] = NULL;
		}

		if (g_ui.pD3DVtxBuffDistance != NULL)
		{// 頂点バッファの開放
			g_ui.pD3DVtxBuffDistance->Release();
			g_ui.pD3DVtxBuffDistance = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUI(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*コイン*/
	for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++)
	{
		int number;

		number = (g_ui.coin % (int)(powf(10.0f, (float)(g_ui.COIN_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(g_ui.COIN_PLACE - nCntPlace - 1))));

		SetTextureCoin(nCntPlace, number);
	}

	/*桁*/
	if (g_ui.coin >= g_ui.coin_target)
	{
		g_ui.coin_target *= 10;	// 次の桁までのターゲットとする
		g_ui.COIN_PLACE++;

		g_ui.COIN_SIZE_X = 55.0f;
		g_ui.COIN_SIZE_Y = 70.0f;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		MakeVertexCoin(pDevice);

	}

	else if ((g_ui.coin == 15) || (g_ui.coin == 105) || (g_ui.coin == 1005) || (g_ui.coin == 10005) || (g_ui.coin == 100005) || (g_ui.coin == 1000005))
	{
		g_ui.COIN_SIZE_X = 35.0f;
		g_ui.COIN_SIZE_Y = 50.0f;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		MakeVertexCoin(pDevice);
	}

	/*距離*/
	for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++)
	{
		int number;

		number = (g_ui.distance % (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE - nCntPlace - 1))));

		SetTextureDistance(nCntPlace, number);
	}

	/*桁*/
	if (g_ui.distance >= g_ui.distance_target)
	{
		g_ui.distance_target *= 10;	// 次の桁までのターゲットとする
		g_ui.DISTANCE_PLACE++;

		g_ui.DISTANCE_SIZE_X = 55.0f;
		g_ui.DISTANCE_SIZE_Y = 70.0f;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		MakeVertexDistance(pDevice);

	}
	else if((g_ui.distance == 50) || (g_ui.distance == 150) || (g_ui.distance == 1050) || (g_ui.distance == 10050) || (g_ui.distance == 100050) || (g_ui.distance == 1000050))
	{
		g_ui.DISTANCE_SIZE_X = 35.0f;
		g_ui.DISTANCE_SIZE_Y = 50.0f;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		MakeVertexDistance(pDevice);
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
		pDevice->SetStreamSource(0, g_ui.pD3DVtxBuffCoin, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_ui.pD3DTextureCoin[0]);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// テクスチャの設定
		pDevice->SetTexture(0, g_ui.pD3DTextureCoin[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (g_ui.COIN_PLACE * 4), NUM_POLYGON);
	}

	/*距離*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_ui.pD3DVtxBuffDistance, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_ui.pD3DTextureDistance[0]);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// テクスチャの設定
		pDevice->SetTexture(0, g_ui.pD3DTextureDistance[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (g_ui.DISTANCE_PLACE * 4), NUM_POLYGON);
	}
}
//=============================================================================
// 頂点の作成	コイン
//=============================================================================
HRESULT MakeVertexCoin(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * g_ui.COIN_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_ui.pD3DVtxBuffCoin,								// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_ui.pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace)* g_ui.COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace) * (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) + g_ui.COIN_SIZE_X, COIN_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace)* g_ui.COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y + g_ui.COIN_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace) * (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) + g_ui.COIN_SIZE_X, COIN_POS_Y + g_ui.COIN_SIZE_Y, 0.0f);

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
			pVtx[1].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X)+15, COIN_ICON_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(COIN_ICON_POS_X - 15, COIN_ICON_POS_Y + 15, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X)+15, COIN_ICON_POS_Y + 15, 0.0f);

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
		g_ui.pD3DVtxBuffCoin->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成	距離
//=============================================================================
HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * g_ui.DISTANCE_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_ui.pD3DVtxBuffDistance,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_ui.pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace)* g_ui.DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y, 0.0f) + g_ui.posDistance;
			pVtx[1].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace) * (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) + g_ui.DISTANCE_SIZE_X, DISTANCE_POS_Y, 0.0f) + g_ui.posDistance;
			pVtx[2].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace)* g_ui.DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y + g_ui.DISTANCE_SIZE_Y, 0.0f) + g_ui.posDistance;
			pVtx[3].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace) * (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) + g_ui.DISTANCE_SIZE_X, DISTANCE_POS_Y + g_ui.DISTANCE_SIZE_Y, 0.0f) + g_ui.posDistance;

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
			pVtx[1].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)+15, METER_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(METER_POS_X - 15, METER_POS_Y + 25, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)+15, METER_POS_Y + 25, 0.0f);

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
		g_ui.pD3DVtxBuffDistance->Unlock();
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
	g_ui.pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_ui.pD3DVtxBuffCoin->Unlock();
}

//=============================================================================
// テクスチャ座標の設定		距離
//=============================================================================
void SetTextureDistance(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_ui.pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_ui.pD3DVtxBuffDistance->Unlock();
}

//=============================================================================
// コインの変更
//=============================================================================
void ChangeCoin(int value)
{
	g_ui.coin += value;

	if (g_ui.coin < 0)
	{
		g_ui.coin = 0;
	}
	else if (g_ui.coin >= (int)(powf(10.0f, (float)(g_ui.COIN_PLACE + 1))))
	{
		g_ui.coin = (int)(powf(10.0f, (float)(g_ui.COIN_PLACE + 1))) - 1;
	}
}

//=============================================================================
// 距離の変更
//=============================================================================
void ChangeDistance(int value)
{
	g_ui.distance += value;

	if (g_ui.distance < 0)
	{
		g_ui.distance = 0;
	}
	else if (g_ui.distance >= (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE + 1))))
	{
		g_ui.distance = (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE + 1))) - 1;
	}
}

UI *GetUI(void)
{
	return &g_ui;
}

void ResetUI(void)
{
	GetUI()->coin = 0;
	GetUI()->distance = 0;
}