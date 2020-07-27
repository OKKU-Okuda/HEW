/**********************************************************************
[[フェーズ:GameTackle1プログラム(Phase_GameTackle1.cpp)]]
作者：奥田　真規

フェーズ：GameTackle1に関するプログラム
***********************************************************************/
#include"../Core/main.h"
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"
	
#include "Phase_GameTackle1.h"
#include "Phase_GameTackle2.h"
#include "Phase_Title.h"
#include "Phase_Result.h"

#include "../GameTackle/player.h"
#include "../GameTackle/enemy.h"
#include "../GameTackle/camera.h"
#include "../GameTackle/field.h"
#include "../GameTackle/shadow.h"
#include "../GameTackle/kiseki.h"
#include "../GameTackle/item.h"
#include "../GameTackle/player_control.h"
#include "../GameTackle/field_control.h"
#include "../GameTackle/effect.h"
#include "../GameTackle/UI.h"
#include "../GameTackle/pole.h"
#include "../GameTackle/ground_mesh.h"
#include "../GameTackle/countdown.h"

#include "../Result/player.h"

//---------------------------------------------------------------------
//マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC g_PhaseFunc = { InitGameTackle1,UninitGameTackle1,UpdateGameTackle1,DrawGameTackle1 };
static MySound g_bgmRunning;
/*=====================================================================
GameTackle1更新関数
=====================================================================*/
void UpdateGameTackle1()
{
	PrintDebugProc("タックル１フェーズ");

	// 次のフェーズに行く
//	if (GetKeyboardTrigger(DIK_RETURN))
	{	// タックル2
//		GoNextPhase(GetPhaseTitleFunc());
	}

	// プレイヤーの位置制御
	UpdatePlayerControl();

	// プレイヤーの更新処理
	UpdatePlayer();

	// ポールの更新処理
	UpdatePole();

	// 軌跡の更新
	UpdateKiseki();

	// 影の更新
	UpdateShadow();

	// エネミーの更新
	//GetTackleEnemyFunc()->Update();

	// フィールド制御の更新
	UpdateFieldControl();

	// フィールドの更新
	UpdateField();

	// アイテムの更新
	UpdateItem();

	// エフェクトの更新
	UpdateEffect();

	// UIの更新
	UpdateUI();

	// 地面の更新
	UpdateGround();

	//カウントダウンの更新
	UpdateCountdown();
}

/*=====================================================================
GameTackle1描画関数
=====================================================================*/
void DrawGameTackle1()
{
	// 地面の描画
	DrawGround();

	// フィールドの描画
	DrawField();

	// 影の描画
	DrawShadow();

	// モデルの描画処理
	DrawPlayer();

	//	ポールの描画処理
	//DrawPole();

	// アイテムの描画処理
	DrawItem();

	// 軌跡の描画
	DrawKiseki();

	// エネミーの描画
	//GetTackleEnemyFunc()->Draw();

	// エフェクトの描画
	DrawEffect();

	// UIの描画
	DrawUI();

	//カウントダウンの描画
	DrawCountdown();

}

/*=====================================================================
GameTackle1初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitGameTackle1(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------

		//影の初期化
		InitShadow();

		// プレイヤーの初期化
		InitPlayer();

		// ポールの初期化処理
		InitPole();

		//軌跡の初期化
		InitKiseki(0);

		// フィールドの初期化
		InitField();

		// アイテムの初期化
		InitItem();

		// エフェクトの初期化
		InitEffect(0);

		// UIの初期化
		InitUI(0);

		// 地面の初期化
		InitGround();

		// カウントダウンのリソース読み込み
		InitCountdown(0);

		// エネミーの読み込み
		//GetTackleEnemyFunc()->Init(true);

		// 音の読み込み
		g_bgmRunning = MySoundCreate("data/BGM/Running.wav");
		MySoundSetVolume(g_bgmRunning, 0.8f);
		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------


	// エネミーの初期化
	//GetTackleEnemyFunc()->Init(false);

	// アイテムのリセット
	ResetItem();

	// UIのリセット
	ResetUI();

	// プレイヤーのリセット
	ResetPlayerPos();

	// プレイヤーコントロールの書き換え処理
	ResetPlayerControl();

	// フィールドのリセット処理
	ResetField();

	//カウントダウンの初期化
	InitCountdown(1);

	// リザルト用プレイヤーの解除
	SetResultPlayer(false);

	// カメラ更新関数の設定
	GetCamera()->length = 30.0f;
	GetCamera()->rotXZ = 0.0f;
	SetCameraFunc(GameStartCam001Update);
}

/*=====================================================================
GameTackle1終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitGameTackle1(bool isEnd)
{
	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------

	// エネミーの終了化
	//GetTackleEnemyFunc()->Uninit(false);

	// 音の停止
	MySoundStop(g_bgmRunning);

	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	// 音の開放
	MySoundDelete(&g_bgmRunning);

	//カウントダウンの終了処理
	UninitCountdown();

	// プレイヤーの終了処理
	UninitPlayer();

	// ポールの終了処理
	UninitPole();

	// エネミーの開放
	//GetTackleEnemyFunc()->Uninit(false);

	// フィールドの終了化
	UninitField();

	// エフェクトの終了処理
	UninitEffect();

	// UIの終了処理
	UninitUI();

}

/*=====================================================================
GameTackle1ゲームエンド関数
=====================================================================*/
void GameTackle1Start()
{
	// 音の再生
	MySoundPlayEternal(g_bgmRunning);
	SetPlayerSE(PSE_START);
	DeleteField(SearchChipID(GetChipID(0, -1)));
	DeleteField(SearchChipID(GetChipID(0, -2)));

}

/*=====================================================================
GameTackle1ゲームエンド関数
=====================================================================*/
void GameTackle1End()
{
	SetPlayerSE(PSE_LOSE);
	EndFieldGimmick();
	SetPlayerMoveState(MSTATE_END);
	GoNextPhase(GetPhaseResultFunc());
}

/*=====================================================================
GameTackle1基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle1Func()
{
	return &g_PhaseFunc;
}