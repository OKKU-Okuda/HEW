//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 奥田 真規　
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"

// フェーズ一覧
#include "../Phase/Phase_Title.h"
#include "../Phase/Phase_Result.h"
#include "../Phase/Phase_GameKick.h"
#include "../Phase/Phase_GameTackle1.h"
#include "../Phase/Phase_GameTackle2.h"
#include "../Phase/Phase_GameTackle3.h"

// テクスチャ読み込みでokか
#include "../GameTackle/Gimmick/Wall.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"RuggerMAN"			// ウインドウのキャプション名

#define PASS_WINDOWMODE		(false)			// 起動時に表示方法選択をパスするか(パス時、ウィンドウモードになる）
#define USE_TIMESTOPMODE	(true)			// 更新をパスして時間停止を行える機工を追加する(F2)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void InitGameResource();
void UninitGameResource();

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
static LPDIRECT3D9			g_pD3D = NULL;			// Direct3D オブジェクト
static LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
static PHASE_FUNC			g_Phase;				// メイン画面遷移データ
static HWND					g_hWnd;					// ウィンドウハンドル
static int					g_nCountFPS;			// FPSカウンタ
static DWORD				g_cntUpdate;			// 更新回数カウント回数

#if USE_TIMESTOPMODE
static bool					g_isTimeStop;			// 時間停止flag(更新関数をパスする)
#endif

//=============================================================================
// ゲームResource系統読み込み関数
//=============================================================================
void InitGameResource()
{
	// フェーズのリソース読み込み
	InitTitle(true);
	InitGameTackle1(true);
	InitGameTackle2(true);
	InitGameTackle3(true);
	InitGameKick(true);
	InitResult(true);

	// 最初のフェーズを指定
	g_Phase = *GetPhaseTitleFunc();
}

//=============================================================================
// ゲームResource系統開放関数
//=============================================================================
void UninitGameResource()
{
	// フェーズのリソース開放
	UninitResult(true);
	UninitGameKick(true);
	UninitGameTackle3(true);
	UninitGameTackle2(true);
	UninitGameTackle1(true);
	UninitTitle(true);
}

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	bool mode;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	g_hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

#if !PASS_WINDOWMODE // 表示方法を行うか否か

	int id = MessageBox(NULL, "テクスチャをキーボード用で適用する？", "(´・ω・)", MB_YESNOCANCEL | MB_ICONQUESTION);
	if (id == IDYES)
	{
		SetWallTextureForGamePad(false);
	}
	else
	{
		SetWallTextureForGamePad(true);
	}

	// 初期化処理(ウィンドウを作成してから行う)
	id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);

	switch (id)
	{
	case IDYES:		// YesならWindowモードで起動
		mode = true;
		break;
	case IDNO:		// Noならフルスクリーンモードで起動
		mode = false;
		break;
	case IDCANCEL:	// CANCELなら終了
	default:
		return -1;
		break;
	}

#else
	mode = true;
#endif

	if(FAILED(Init(hInstance, g_hWnd, mode)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// デプスバッファとして24bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;						// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	}


	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートパラメータの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// サウンドの初期化
	InitSound();

	// フェードの初期化
	InitFade();

	// デバッグの初期化
	InitDebugProc();

	// ゲーム素材の読み込み系
	InitGameResource();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// 一番最初のフェーズはmainで初期化
	g_Phase.Init(false);	

	// 乱数の初期化
	srand(timeGetTime());

#if USE_TIMESTOPMODE
	// 時間の停止初期化
	g_isTimeStop = false;	
#endif
	g_cntUpdate = 0ul;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// カメラの終了化
	UninitCamera();

	// ライトの終了化
	UninitLight();

	// 一番最初のフェーズはmainで初期化
	g_Phase.Uninit(false);

	// ゲーム内リソースの開放
	UninitGameResource();

	// デバッグの終了化
	UninitDebugProc();

	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
	UpdateInput();


#if USE_TIMESTOPMODE
	if (GetKeyboardTrigger(DIK_F2))
	{	// F2で反転
		g_isTimeStop = !g_isTimeStop;
	}

	if (g_isTimeStop == true)
	{	// 時間停止なら更新系スキップ
		return;
	}
#endif

	g_cntUpdate++;	// 更新関数カウントのインクリメント
	PrintDebugProc("%dFPS  %dtimes", g_nCountFPS, g_cntUpdate);

	// フェーズ更新処理
	g_Phase.Update();

	// カメラ更新処理
	UpdateCamera();

	// フェード処理の更新
	UpdateFade();

	// デバッグの更新
	UpdateDebugProc();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
#if 0
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(100, 100, 200, 100), 1.0f, 0);
#else
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 100), 1.0f, 0);
#endif
	// カメラ情報からマトリクス算出
	SetCamera();	

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		// フェーズ描画
		g_Phase.Draw();

		// フェード描画
		DrawFade();

		// デバッグ文字列描画
		DrawDebugProc();

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// ナニモシナイ...
//=============================================================================
void NoFunction()
{	// 何もしない関数
	return;
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// 現フェーズの関数群取得関数
//=============================================================================
PHASE_FUNC *GetPhase()
{
	return &g_Phase;
}

//=============================================================================
// ウィンドウハンドルの取得
//=============================================================================
HWND GetHandle()
{
	return g_hWnd;
}

//=============================================================================
// 更新関数使用回数の取得
//=============================================================================
DWORD GetCount()
{
	return g_cntUpdate;
}

/*=====================================================================
割合指定ランダム取得関数
戻り値：bool			true:割合でtrueにする
引数：　float rate			:0~1f　　trueを返す割合を指定
=====================================================================*/
bool CheckRand(float rate)
{
	int value = rand();

	int valmax = (int)(RAND_MAX * rate);

	if (abs(value) <= valmax)
	{// 許容範囲内であればtrue
		return true;
	}
	return false;
}

