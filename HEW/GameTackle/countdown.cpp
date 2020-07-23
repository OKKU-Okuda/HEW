//=============================================================================
//
// スタート画面処理 [countdown.cpp]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#include "countdown.h"
#include"../Core/sound.h"
#include"../Core/fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_COUNTDOWN		"data/TEXTURE/NUM.png"			// 読み込むテクスチャファイル名
#define	TEXTURE_SHUTSUJIN		"data/TEXTURE/KickOff.png"			// 読み込むテクスチャファイル名
#define	COUNTDOWN_POS_X			(550)								// スタートロゴの位置(X座標)
#define	COUNTDOWN_POS_Y			(200)								// スタートロゴの位置(Y座標)
#define	COUNTDOWN_WIDTH			(200)								// スタートロゴの幅
#define	COUNTDOWN_HEIGHT		(200)								// スタートロゴの高さ
#define	SHUTSUJIN_POS_X			(350)								// スタートボタンの位置(X座標)
#define	SHUTSUJIN_POS_Y			(80)								// スタートボタンの位置(Y座標)
#define	SHUTSUJIN_WIDTH			(800)								// スタートボタンの幅
#define	SHUTSUJIN_HEIGHT		(400)								// スタートボタンの高さ
#define	COUNT_APPERA_COUNTDOWN	(30)								// スタートボタン出現までの時間
#define	INTERVAL_DISP_COUNTDOWN	(60)								// スタートボタン点滅の時間
#define	COUNT_WAIT_DEMO		(60 * 5)								// デモまでの待ち時間
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X (10)						// テクスチャパターンX
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y (1)						// テクスチャパターンY

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCountdown(LPDIRECT3DDEVICE9 pDevice);
void SetColorCountdown(void);
void SetTextureCountdown(int nPattern);
void SetVertexCountdown(float fSizeX, float fSizeY);
void SetVertexShutsujin(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCountdown = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffCountdown = NULL;		// 頂点バッファインターフェースへのポインタ
static LPDIRECT3DTEXTURE9		g_pD3DTextureShutsujin = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffShutsujin = NULL;		// 頂点バッファインターフェースへのポインタ
static int						g_nCountAppearCountdown = 0;		// テクスチャ表示までのインターバル
static float					g_fAlphaLogo = 0.0f;				// テクスチャのa値
static float					g_ExpantionLogo = 0.0f;				// テクスチャの拡大値
static float					g_Shutsujin_pos_x = 0.0f;			// テクスチャの拡大値
static int						g_CountdownPattern = 3;				// テクスチャパターン数
static int						countdown_flag = 0;					// 0;カウントダウン 1;出陣 2戦闘
static MySound					g_seCountdown;						// カウントダウン音
static MySound					g_seKickoff;						// キックオフ音
static bool						g_sound;							// キックオフ音

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCountdown(int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//初期化処理
	g_nCountAppearCountdown = 0;
	g_fAlphaLogo = 0.0f;
	g_ExpantionLogo = 0.0f;
	g_CountdownPattern = 3;
	countdown_flag = 0;
	g_Shutsujin_pos_x = 1000.0f;

	// 頂点情報の作成
	MakeVertexCountdown(pDevice);

	SetTextureCountdown(g_CountdownPattern);

	if (type == 0)
	{

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_COUNTDOWN,								// ファイルの名前
			&g_pD3DTextureCountdown);						// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_SHUTSUJIN,							// ファイルの名前
			&g_pD3DTextureShutsujin);					// 読み込むメモリー

		g_seCountdown = MySoundCreate("data/SE/countdown.wav");

	}


	g_sound = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCountdown(void)
{

	if(g_pD3DTextureCountdown != NULL)
	{// テクスチャの開放
		g_pD3DTextureCountdown->Release();
		g_pD3DTextureCountdown = NULL;
	}

	if(g_pD3DVtxBuffCountdown != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffCountdown->Release();
		g_pD3DVtxBuffCountdown = NULL;
	}

	if(g_pD3DTextureShutsujin != NULL)
	{// テクスチャの開放
		g_pD3DTextureShutsujin->Release();
		g_pD3DTextureShutsujin = NULL;
	}

	if(g_pD3DVtxBuffShutsujin != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffShutsujin->Release();
		g_pD3DVtxBuffShutsujin = NULL;
	}

	MySoundDelete(&g_seCountdown);
	MySoundDelete(&g_seKickoff);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCountdown(void)
{
	if (GetFade() == 0)
	{
		if (g_sound == false)
		{
			MySoundSetVolume(g_seCountdown, 3.0f);
			MySoundPlayOnce(g_seCountdown);
			g_sound = true;
		}

		//カウントダウンの更新
		if (countdown_flag == 0)
		{
			// BGM停止
			//MySoundStopAll();

			//透明なら
			if (g_fAlphaLogo < 1.0f)
			{
				//徐々に実体化
				g_fAlphaLogo += 0.017f;

				//拡大
				g_ExpantionLogo += 1.0f;

				//実体化した時処理
				if (g_fAlphaLogo >= 1.0f)
				{
					g_fAlphaLogo = 1.0f;

					g_nCountAppearCountdown = 0;



				}

				SetColorCountdown();

				SetVertexCountdown(g_ExpantionLogo, g_ExpantionLogo);
			}
			else
			{
				//インターバル
				g_nCountAppearCountdown++;

				//インターバル終わったら
				if (g_nCountAppearCountdown > COUNT_APPERA_COUNTDOWN)
				{
					g_fAlphaLogo = 0.0f;

					g_CountdownPattern--;

					//拡大値も初期化
					g_ExpantionLogo = 0;

					//次のテクスチャ
					SetTextureCountdown(g_CountdownPattern);

					SetColorCountdown();

					if (g_CountdownPattern == 0)
					{
						countdown_flag = 1;
						g_nCountAppearCountdown = 0;
					}

				}
			}
		}


		if (countdown_flag == 1)
		{
			if (g_Shutsujin_pos_x > SHUTSUJIN_POS_X)
			{
				//移動
				g_Shutsujin_pos_x -= 50.0f;
			}
			else if (g_Shutsujin_pos_x <= SHUTSUJIN_POS_X)
			{
				g_nCountAppearCountdown++;

			}

			if (g_nCountAppearCountdown > COUNT_APPERA_COUNTDOWN)
			{
				//スタート
				countdown_flag = 2;

				//MySoundSetVolume(g_seKickoff, 0.5f);
				//MySoundPlayOnce(g_seKickoff);

			}

			SetVertexShutsujin();


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (countdown_flag == 0)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffCountdown, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureCountdown);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
	if (countdown_flag == 1)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffShutsujin, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureShutsujin);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

	
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCountdown(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffCountdown,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(COUNTDOWN_POS_X+ COUNTDOWN_WIDTH, COUNTDOWN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y+ COUNTDOWN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(COUNTDOWN_POS_X+ COUNTDOWN_WIDTH, COUNTDOWN_POS_Y+ COUNTDOWN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.3f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.3f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffCountdown->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffShutsujin,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffShutsujin->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffShutsujin->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorCountdown(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// 頂点データをアンロックする
		g_pD3DVtxBuffCountdown->Unlock();
	}

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexCountdown(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(COUNTDOWN_POS_X - fSizeX, COUNTDOWN_POS_Y - fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(COUNTDOWN_POS_X + COUNTDOWN_WIDTH + fSizeX, COUNTDOWN_POS_Y - fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(COUNTDOWN_POS_X - fSizeX, COUNTDOWN_POS_Y + COUNTDOWN_HEIGHT + fSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(COUNTDOWN_POS_X + COUNTDOWN_WIDTH + fSizeX, COUNTDOWN_POS_Y + COUNTDOWN_HEIGHT + fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffCountdown->Unlock();
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexShutsujin(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffShutsujin->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffShutsujin->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureCountdown(int nPattern)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		int x = nPattern % COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		int y = nPattern / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		float sizeX = 1.0f / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		float sizeY = 1.0f / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y;

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffCountdown->Unlock();
	}
}

//=============================================================================
// スタート取得関数
//=============================================================================
int GetCountdownFlag(void)
{
	return countdown_flag;
}


