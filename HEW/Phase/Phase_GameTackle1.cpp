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

	// プレイヤーの更新
	GetPlayerFuncX()->Update();

	// エネミーの更新
	GetTackleEnemyFunc()->Update();
}

/*=====================================================================
GameTackle1描画関数
=====================================================================*/
void DrawGameTackle1()
{
	// プレイヤーの描画
	GetPlayerFuncX()->Draw();

	// エネミーの描画
	GetTackleEnemyFunc()->Draw();
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

		// プレイヤーの読み込み
		GetPlayerFuncX()->Init(true);

		// エネミーの読み込み
		GetTackleEnemyFunc()->Init(true);

		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// プレイヤーの初期化
	GetPlayerFuncX()->Init(false);

	// エネミーの初期化
	GetTackleEnemyFunc()->Init(false);

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

	// プレイヤーの終了化
	GetPlayerFuncX()->Uninit(false);

	// エネミーの終了化
	GetTackleEnemyFunc()->Uninit(false);


	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	// プレイヤーの開放
	GetPlayerFuncX()->Uninit(true);



	// エネミーの開放
	GetTackleEnemyFunc()->Uninit(false);
}

/*=====================================================================
GameTackle1基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle1Func()
{
	return &g_PhaseFunc;
}