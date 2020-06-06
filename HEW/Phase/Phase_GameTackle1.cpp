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

#include "../GameTackle/player.h"
#include "../GameTackle/enemy.h"
#include "../GameTackle/camera.h"
#include "../GameTackle/field.h"
#include "../GameTackle/shadow.h"
#include "../GameTackle/kiseki.h"
#include "../GameTackle/item.h"

#include "../GameTackle/player_control.h"

#include "../GameTackle/effect.h"
#include "../GameTackle/UI.h"


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
static PHASE_FUNC g_PhaseFunc = { InitGameTackle1,UninitGameTackle1,UpdateGameTackle1,DrawGameTackle1 };

/*=====================================================================
GameTackle1更新関数
=====================================================================*/
void UpdateGameTackle1()
{
	PrintDebugProc("タックル１フェーズ");

	// 次のフェーズに行く
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// タックル2
		GoNextPhase(GetPhaseGameTackle2Func());
	}

	// プレイヤーの位置制御
	UpdatePlayerControl();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 軌跡の更新
	UpdateKiseki();

	// 影の更新
	UpdateShadow();

	// エネミーの更新
	GetTackleEnemyFunc()->Update();

	// フィールドの更新
	UpdateField();

	// アイテムの更新
	UpdateItem();

	// エフェクトの更新
	UpdateEffect();

	// UIの更新
	UpdateUI();
}

/*=====================================================================
GameTackle1描画関数
=====================================================================*/
void DrawGameTackle1()
{
	// フィールドの描画
	DrawField();


	// 影の描画
	DrawShadow();

	// モデルの描画処理
	DrawPlayer();

	// アイテムの描画処理
	DrawItem();

	// 軌跡の描画
	DrawKiseki();

	// エネミーの描画
	GetTackleEnemyFunc()->Draw();

	// エフェクトの描画
	DrawEffect();

	// UIの描画
	DrawUI();

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


		// エネミーの読み込み
		GetTackleEnemyFunc()->Init(true);

		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------


	// エネミーの初期化
	GetTackleEnemyFunc()->Init(false);

	// フィールドのリセット処理
	ResetField();

	// プレイヤーコントロールの書き換え処理
	ResetPlayerControl();

	// カメラ更新関数の設定
	GetCamera()->length = 30.0f;
	GetCamera()->rotXZ = 0.0f;
	SetCameraFunc(TackleCameraUpdate);
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
	GetTackleEnemyFunc()->Uninit(false);


	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	// プレイヤーの終了処理
	UninitPlayer();

	// エネミーの開放
	GetTackleEnemyFunc()->Uninit(false);

	// フィールドの終了化
	UninitField();

	// エフェクトの終了処理
	UninitEffect();

	// UIの終了処理
	UninitUI();

}

/*=====================================================================
GameTackle1基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle1Func()
{
	return &g_PhaseFunc;
}