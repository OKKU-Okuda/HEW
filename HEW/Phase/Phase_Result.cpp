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

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC g_PhaseFunc = { InitResult,UninitResult,UpdateResult,DrawResult };

/*=====================================================================
Result更新関数
=====================================================================*/
void UpdateResult()
{
	PrintDebugProc("リザルトフェーズ");

	// 次のフェーズに行く
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// タックル１
		GoNextPhase(GetPhaseTitleFunc());
	}

}

/*=====================================================================
Result描画関数
=====================================================================*/
void DrawResult()
{

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




	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	リソース開放処理
		//---------------------------------------------------------------------

	}

}

/*=====================================================================
Result基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc()
{
	return &g_PhaseFunc;
}