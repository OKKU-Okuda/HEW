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
static MySound		g_Sound;
/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	// 1を押すたびに音が増える
	if (GetKeyboardTrigger(DIK_1))
	{
		MySound newsound = MySoundClone(g_Sound);
		MySoundPlayEternal(newsound);
	}

	// ピッチ変更
	if (GetKeyboardPress(DIK_UP))
	{
		float vol = MySoundGetPitch(g_Sound);

		vol += 0.01f;
		MySoundSetPitchAuto(g_Sound, vol);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		float vol = MySoundGetPitch(g_Sound);

		vol -= 0.01f;
		MySoundSetPitchAuto(g_Sound, vol);
	}


	// 次のフェーズに行く（いまはタイトル）
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
		g_Sound = MySoundCreate("data/BGM/bgm000.wav");

	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	MySoundPlayEternal(g_Sound);	// 永遠再生
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

	MySoundStop(g_Sound);	// 停止


	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	リソース開放処理
		//---------------------------------------------------------------------
		MySoundDeleteAuto(&g_Sound);// 増やしたものも一気に開放
	}

}

/*=====================================================================
Title基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}