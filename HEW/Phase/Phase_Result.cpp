/**********************************************************************
[[フェーズ:Resultプログラム(Phase_Result.cpp)]]
	作者：奥田　真規

	フェーズ：Resultに関するプログラム
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"
#include "../Phase/Phase_GameTackle1.h"


#include "Phase_Result.h"	
#include "Phase_Title.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC g_PhaseFunc = { InitResult,UninitResult,UpdateResult,DrawResult };

LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;			// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureNewGameLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffNewGameLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureExitLogo = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExitLogo = NULL;		// 頂点バッファインターフェースへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;				// テクスチャへのポインタ

static VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク

static ICON						g_Icon[ICON_CURSOR_MAX];			// アイコン

/*=====================================================================
Result更新関数
=====================================================================*/
void UpdateResult()
{
	PrintDebugProc("リザルトフェーズ");

	//// 次のフェーズに行く
	//if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
	//{	// タックル１
	//	GoNextPhase(GetPhaseTitleFunc());
	//}

	for (int i = 0; i < ICON_CURSOR_MAX; i++)
	{
		/*〇〇キーを押したらポーズ画面*/
		if (g_Icon[i].use == true)
		{
			if ((GetKeyboardTrigger(DIK_RETURN))/* || (IsButtonTriggered(0, BUTTON_C))*/)
			{

				/*TITLE*/
				if (g_Icon[i].pos.x == RESULT_TITLE_LOGO_POS_X)
				{
					GoNextPhase(GetPhaseTitleFunc());


					g_Icon[i].use = false;										//ラグビーアイコンを消す

					g_Icon->pos.x = ICON_MIN;									//ラグビーアイコンを初期位置に戻す

				}

				/*NEWGAME*/
				if (g_Icon[i].pos.x == RESULT_NEWGAME_LOGO_POS_X)
				{
					GoNextPhase(GetPhaseGameTackle1Func());



					g_Icon[i].use = false;										//ラグビーアイコンを消す

					g_Icon->pos.x = ICON_MIN;									//ラグビーアイコンを初期位置に戻す
				}

				/*EXIT*/
				if (g_Icon[i].pos.x == RESULT_EXIT_LOGO_POS_X)
				{
					DestroyWindow(GetHandle());
				}
			}

			/*左キー*/
			if (GetKeyboardTrigger(DIK_LEFT)/* || IsButtonTriggered(0, BUTTON_UP)*/)
			{

				if (g_Icon[i].pos.x >= ICON_MIN)
				{
					g_Icon[i].pos.x -= ICON_REN;
				}
				if (g_Icon[i].pos.x < ICON_MIN)
				{
					g_Icon[i].pos.x = ICON_MAX;
				}
			}

			/*右キー*/
			if (GetKeyboardTrigger(DIK_RIGHT)/* || IsButtonTriggered(0, BUTTON_DOWN)*/)
			{

				if (g_Icon[i].pos.x <= ICON_MAX)
				{
					g_Icon[i].pos.x += ICON_REN;
				}
				if (g_Icon[i].pos.x > ICON_MAX)
				{
					g_Icon[i].pos.x = ICON_MIN;
				}
			}
			//SetVertexIcon(i);
		}
	}

}

/*=====================================================================
Result描画関数
=====================================================================*/
void DrawResult()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*リザルト画面*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResult);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*TITLE*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResultTitleLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResultTitleLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*NEW GAME*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffNewGameLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureNewGameLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*EXIT*/
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffExitLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureExitLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}

/*=====================================================================
Result初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitResult(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
				TEXTURE_RESULT,				// ファイルの名前
				&g_pD3DTextureResult);		// 読み込むメモリー

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
				TEXTURE_NEWGAME_LOGO,		// ファイルの名前
				&g_pD3DTextureNewGameLogo);	// 読み込むメモリー

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
				TEXTURE_TITLE_LOGO,		// ファイルの名前
				&g_pD3DTextureResultTitleLogo);	// 読み込むメモリー

					// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
				TEXTURE_EXIT_LOGO,		// ファイルの名前
				&g_pD3DTextureExitLogo);	// 読み込むメモリー


		// 頂点情報の作成
		MakeVertexResult(pDevice);



		for (int i = 0; i < ICON_CURSOR_MAX; i++)
		{
			g_Icon[i].use = true;

			/*アイコンの位置*/
			g_Icon[i].pos = D3DXVECTOR3(90.0f, 520.0f, 0.0f);

			// 頂点情報の作成
			//MakeVertexIcon();
		}


		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------


}

/*=====================================================================
Result終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitResult(bool isEnd)
{
	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------

	if (g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if (g_pD3DVtxBuffResult != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if (g_pD3DTextureNewGameLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureNewGameLogo->Release();
		g_pD3DTextureNewGameLogo = NULL;
	}

	if (g_pD3DVtxBuffNewGameLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffNewGameLogo->Release();
		g_pD3DVtxBuffNewGameLogo = NULL;
	}

	if (g_pD3DTextureResultTitleLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultTitleLogo->Release();
		g_pD3DTextureResultTitleLogo = NULL;
	}

	if (g_pD3DVtxBuffResultTitleLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResultTitleLogo->Release();
		g_pD3DVtxBuffResultTitleLogo = NULL;
	}

	if (g_pD3DTextureExitLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureExitLogo->Release();
		g_pD3DTextureExitLogo = NULL;
	}

	if (g_pD3DVtxBuffExitLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffExitLogo->Release();
		g_pD3DVtxBuffExitLogo = NULL;
	}



	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------


}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffNewGameLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffNewGameLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X, RESULT_NEWGAME_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X + RESULT_NEWGAME_LOGO_WIDTH, RESULT_NEWGAME_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X, RESULT_NEWGAME_LOGO_POS_Y + RESULT_NEWGAME_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X + RESULT_NEWGAME_LOGO_WIDTH, RESULT_NEWGAME_LOGO_POS_Y + RESULT_NEWGAME_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffNewGameLogo->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResultTitleLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X, RESULT_TITLE_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X + RESULT_TITLE_LOGO_WIDTH, RESULT_TITLE_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X, RESULT_TITLE_LOGO_POS_Y + RESULT_TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X + RESULT_TITLE_LOGO_WIDTH, RESULT_TITLE_LOGO_POS_Y + RESULT_TITLE_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffResultTitleLogo->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffExitLogo,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExitLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X, RESULT_EXIT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X + RESULT_EXIT_LOGO_WIDTH, RESULT_EXIT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X, RESULT_EXIT_LOGO_POS_Y + RESULT_EXIT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X + RESULT_EXIT_LOGO_WIDTH, RESULT_EXIT_LOGO_POS_Y + RESULT_EXIT_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffExitLogo->Unlock();
	}


	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}
}

/*=====================================================================
Result基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc()
{
	return &g_PhaseFunc;
}