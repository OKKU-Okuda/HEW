//=============================================================================
//
// ポーズ画面処理 [pause.cpp]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#include "pause.h"
#include"../Core/input.h"
#include"../Core/sound.h"
#include"../Core/fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PAUSE			"data/TEXTURE/background.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_PAUSE_TUDUKERU	"data/TEXTURE/ゲームを続ける.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_PAUSE_YAMERU	"data/TEXTURE/ゲームをやめる.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_GAMEPAD			"data/TEXTURE/tutrial03.png"		// 読み込むテクスチャファイル名
#define	PAUSE_TUDUKERU_POS_X	(400)								// タイトルロゴの位置(X座標)
#define	PAUSE_TUDUKERU_POS_Y	(500)								// タイトルロゴの位置(Y座標)
#define	PAUSE_TUDUKERU_WIDTH	(600)								// タイトルロゴの幅
#define	PAUSE_TUDUKERU_HEIGHT	(130)								// タイトルロゴの高さ
#define	YAMERU_POS_X			(400)								// スタートボタンの位置(X座標)
#define	YAMERU_POS_Y			(600)								// スタートボタンの位置(Y座標)
#define	YAMERU_WIDTH			(600)								// スタートボタンの幅
#define	YAMERU_HEIGHT			(130)								// スタートボタンの高さ
#define	COUNT_APPERA_YAMERU		(60)								// スタートボタン出現までの時間
#define	INTERVAL_DISP_YAMERU	(60)								// スタートボタン点滅の時間
#define	COUNT_WAIT_DEMO			(60 * 5)							// デモまでの待ち時間
#define	GAMEPAD_POS_X			(350)								// スタートボタンの位置(X座標)
#define	GAMEPAD_POS_Y			(100)								// スタートボタンの位置(Y座標)
#define	GAMEPAD_WIDTH			(450)								// スタートボタンの幅
#define	GAMEPAD_HEIGHT			(330)								// スタートボタンの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);
void SetColorHajimeru(bool key);
void SetColorYameru(bool key);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePause = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPause = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTudukeru = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTudukeru = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureYameru = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffYameru = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureGamepad = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffGamepad = NULL;	// 頂点バッファインターフェースへのポインタ
bool                    g_pause_use = false;
bool                    g_pause_key = false;
static MySound			g_seSelection;					// 選択音
static MySound			g_seDicidion;					// 決定音

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_pause_use = false;
	g_pause_key = false;

	// 頂点情報の作成
	MakeVertexPause(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_PAUSE,				// ファイルの名前
			&g_pD3DTexturePause);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_PAUSE_TUDUKERU,			// ファイルの名前
			&g_pD3DTextureTudukeru);	// 読み込むメモリー


		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_PAUSE_YAMERU,			// ファイルの名前
			&g_pD3DTextureYameru);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_GAMEPAD,			// ファイルの名前
			&g_pD3DTextureGamepad);		// 読み込むメモリー
	}

	g_seSelection= MySoundCreate("data/SE/WallDown.wav");
	g_seDicidion = MySoundCreate("data/SE/WallDown.wav");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{


	if(g_pD3DTexturePause != NULL)
	{// テクスチャの開放
		g_pD3DTexturePause->Release();
		g_pD3DTexturePause = NULL;
	}

	if(g_pD3DVtxBuffPause != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffPause->Release();
		g_pD3DVtxBuffPause = NULL;
	}

	if(g_pD3DTextureTudukeru != NULL)
	{// テクスチャの開放
		g_pD3DTextureTudukeru->Release();
		g_pD3DTextureTudukeru = NULL;
	}

	if(g_pD3DVtxBuffTudukeru != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTudukeru->Release();
		g_pD3DVtxBuffTudukeru = NULL;
	}

	if(g_pD3DTextureYameru != NULL)
	{// テクスチャの開放
		g_pD3DTextureYameru->Release();
		g_pD3DTextureYameru = NULL;
	}

	if(g_pD3DVtxBuffYameru != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffYameru->Release();
		g_pD3DVtxBuffYameru = NULL;
	}

	if (g_pD3DTextureGamepad != NULL)
	{// テクスチャの開放
		g_pD3DTextureGamepad->Release();
		g_pD3DTextureGamepad = NULL;
	}

	if (g_pD3DVtxBuffGamepad != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffGamepad->Release();
		g_pD3DVtxBuffGamepad = NULL;
	}

	MySoundDelete(&g_seSelection);
	MySoundDelete(&g_seDicidion);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
#if 0
	if(g_nCountAppearYameru >= COUNT_APPERA_YAMERU)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_PAUSE);
		}
	}
#endif

	//ポーズボタンおしたら
	if (GetKeyboardTrigger(DIK_P)|| IsButtonTriggered(0, BUTTON_M))
	{
		//今ONならOFF OFFならON
		g_pause_use =! g_pause_use;
	}
	//ポーズONなら
	if (g_pause_use == true)
	{
		//キー入力あると
		if (GetKeyboardTrigger(DIK_DOWN)|| GetKeyboardTrigger(DIK_UP)
			|| IsButtonTriggered(0, BUTTON_UP)|| IsButtonTriggered(0, BUTTON_DOWN))
		{
			g_pause_key = !g_pause_key;
			// SE再生
			MySoundSetVolume(g_seSelection, 0.5f);
			MySoundPlayOnce(g_seSelection);
		}

		if (GetKeyboardTrigger(DIK_RETURN)|| IsButtonTriggered(0, BUTTON_C))
		{
			if (g_pause_key == true)
			{
				//SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_TITLE);

				// SE再生
				MySoundSetVolume(g_seSelection, 0.5f);
				MySoundPlayOnce(g_seSelection);


			}

			else if (g_pause_key == false)
			{
				g_pause_use = !g_pause_use;
			}

		}

		SetColorHajimeru(g_pause_key);
		SetColorYameru(g_pause_key);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (g_pause_use == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffPause, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePause);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffGamepad, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureGamepad);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffTudukeru, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTudukeru);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffYameru, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureYameru);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}


}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffPause,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffPause->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffTudukeru,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTudukeru->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X, PAUSE_TUDUKERU_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X + PAUSE_TUDUKERU_WIDTH, PAUSE_TUDUKERU_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X, PAUSE_TUDUKERU_POS_Y + PAUSE_TUDUKERU_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X + PAUSE_TUDUKERU_WIDTH, PAUSE_TUDUKERU_POS_Y + PAUSE_TUDUKERU_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pD3DVtxBuffTudukeru->Unlock();
	}


	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffYameru,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffYameru->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(YAMERU_POS_X, YAMERU_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(YAMERU_POS_X + YAMERU_WIDTH, YAMERU_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(YAMERU_POS_X, YAMERU_POS_Y + YAMERU_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(YAMERU_POS_X + YAMERU_WIDTH, YAMERU_POS_Y + YAMERU_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pD3DVtxBuffYameru->Unlock();
	}


	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffGamepad,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffGamepad->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(GAMEPAD_POS_X, GAMEPAD_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(GAMEPAD_POS_X + GAMEPAD_WIDTH, GAMEPAD_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(GAMEPAD_POS_X, GAMEPAD_POS_Y + GAMEPAD_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(GAMEPAD_POS_X + GAMEPAD_WIDTH, GAMEPAD_POS_Y + GAMEPAD_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pD3DVtxBuffGamepad->Unlock();
	}



	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorHajimeru(bool key)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTudukeru->Lock(0, 0, (void**)&pVtx, 0);

		if (key == false)
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}


		// 頂点データをアンロックする
		g_pD3DVtxBuffTudukeru->Unlock();
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorYameru(bool key)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffYameru->Lock(0, 0, (void**)&pVtx, 0);

		if (key == true)
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}


		// 頂点データをアンロックする
		g_pD3DVtxBuffYameru->Unlock();
	}

}

//=============================================================================
// ポーズ取得関数
//=============================================================================
bool GetPauseUse(void)
{
	return g_pause_use;
}




