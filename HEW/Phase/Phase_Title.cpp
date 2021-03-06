/**********************************************************************
[[フェーズ:Titleプログラム(Phase_Title.cpp)]]
	作者：奥田　真規

	フェーズ：Phase_Titleに関するプログラム
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "Phase_Title.h"

#include "../Title/camera.h"
#include "../Title/effect.h"				// タイトルエフェクト
#include "../Title/player.h"				// プレイヤー
#include "../Title/bottons.h"				// ボタン
#include "../Title/select.h"				// 選択処理
#include "../Title/logo.h"					// ロゴ
#include "../Title/control.h"				// コントロール
#include "../Title/config.h"				// コンフィグ
#ifdef _DEBUG
#include "Phase_GameKick.h"
#endif
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_soundBGM;

/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("タイトルフェーズ");


	// 後ろにある四角形エフェクトの更新
	GetEffectFunc()->Update();

	// 制御の更新
	GetControlFunc()->Update();

	// プレイヤーの更新
	GetPlayerFunc()->Update();

	// 選択の処理とそのエフェクトの更新
	GetSelectFunc()->Update();

	// ボタンの更新
	GetBottonsFunc()->Update();

	// タイトルロゴの更新
	GetLogoFunc()->Update();

	// コンフィグの描画
	UpdateConfig();

#ifdef NANANANA//_DEBUG
	PrintDebugProc("(ﾃﾞﾊﾞｯｸﾞ)K:キックゲームに直行");
	// キックに直行処理
	if (GetKeyboardTrigger(DIK_K))
	{
		GoNextPhase(GetPhaseGameKickFunc());
	}

#endif
}

/*=====================================================================
Title描画関数
=====================================================================*/
void DrawTitle()
{
	DWORD	d3drslightning;
	D3DDEVICE;

	// ライティングの設定
	pDevice->GetRenderState(D3DRS_LIGHTING, &d3drslightning);	
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// プレイヤーの描画
	GetPlayerFunc()->Draw();

	// 周囲に舞っているエフェクトの描画
	GetEffectFunc()->Draw();

	// 元のライティング状態に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);

	// ボタンの描画
	GetBottonsFunc()->Draw();

	// 選択エフェクトの描画処理
	GetSelectFunc()->Draw();

	// ロゴの描画
	GetLogoFunc()->Draw();

	// コンフィグの描画
	DrawConfig();
}

/*=====================================================================
Title初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitTitle(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------

		// BGM,SEの読み込み
		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");

		// エフェクトのリソース読み込み
		GetEffectFunc()->Init(true);

		// プレイヤーのリソース読み込み
		GetPlayerFunc()->Init(true);

		// ボタンのリソース読み込み
		GetBottonsFunc()->Init(true);

		// 選択エフェクトのリソース読み込み
		GetSelectFunc()->Init(true);

		// ロゴのリソース読み込み
		GetLogoFunc()->Init(true);

		// 制御のリソース読み込み
		GetControlFunc()->Init(true);

		// コンフィグのリソース読み込み
		InitConfig();
		return;
	}


	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------
	// カメラ関数の設定
	SetCameraFunc(UpdateTitleCameraFunc);
	ResetTitleCameraParameter();

	// プレイヤー初期化
	GetPlayerFunc()->Init(false);

	// ボタン初期化
	GetBottonsFunc()->Init(false);

	// 選択エフェクと初期化
	GetSelectFunc()->Init(false);

	// ロゴ初期化
	GetLogoFunc()->Init(false);

	// エフェクト初期化
	GetEffectFunc()->Init(false);

	// 制御初期化
	GetControlFunc()->Init(false);

	// コンフィグのリセット
	ResetConfig();

	MySoundPlayEternal(g_soundBGM);	// 永遠再生

	MySoundSetVolume(g_soundBGM, 0.5f);	// サウンドを5割
}

/*=====================================================================
Title終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitTitle(bool isEnd)
{

//---------------------------------------------------------------------
//	その他の終了処理
//---------------------------------------------------------------------

	MySoundStop(g_soundBGM);	// 停止

	// プレイヤー終了化
	GetPlayerFunc()->Uninit(false);

	// ボタン終了化
	GetBottonsFunc()->Uninit(false);

	// 選択エフェクと終了化
	GetSelectFunc()->Uninit(false);

	// ロゴ終了化
	GetLogoFunc()->Uninit(false);

	// エフェクト終了化
	GetEffectFunc()->Uninit(false);

	// 制御終了化
	GetControlFunc()->Uninit(false);


	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------


	// サウンドの開放
	MySoundDeleteAuto(&g_soundBGM);


	// プレイヤー開放
	GetPlayerFunc()->Uninit(true);

	// ボタン開放
	GetBottonsFunc()->Uninit(true);

	// 選択エフェクと開放
	GetSelectFunc()->Uninit(true);

	// ロゴ開放
	GetLogoFunc()->Uninit(true);

	// エフェクト開放
	GetEffectFunc()->Uninit(true);

	// 制御開放
	GetControlFunc()->Uninit(true);

	// コンフィグの開放
	UninitConfig();

}


/*=====================================================================
Title基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

