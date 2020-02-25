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
static PHASE_FUNC g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };

/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	if (GetKeyboardTrigger(DIK_0))
		GoNextPhase(GetPhaseTitleFunc());
}

/*=====================================================================
Title描画関数
=====================================================================*/
void DrawTitle()
{

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

	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------


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
	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	リソース開放処理
		//---------------------------------------------------------------------

	}

	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------


}

/*=====================================================================
Title基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}