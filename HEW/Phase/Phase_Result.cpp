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

#include "../Result/meshsky.h"
#include "../Result/player.h"
#include "../Result/camera.h"
#include "../Result/score.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define ADD_RATE	(0.008f)

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
static float g_Rate = 0.f;		// 表示進捗

static MySound g_bgm = NULL;
/*=====================================================================
Result更新関数
=====================================================================*/
void UpdateResult()
{
	PrintDebugProc("リザルトフェーズ");

	if (GetFade() == FADE_NONE)
	{// fadeが終わったら進捗開始
		g_Rate += ADD_RATE;
		if (g_Rate > 1.f)
		{
			g_Rate = 1.f;

			if (GetKeyboardTrigger(DIK_RETURN))
			{
				GoNextPhase(GetPhaseTitleFunc());
			}

		}

	}

	UpdateResultPlayer();
	UpdateMeshSky();

	UpdateScore();
}

 
/*=====================================================================
Result描画関数
=====================================================================*/
void DrawResult()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 3D
	DrawMeshSky();
	DrawResultPlayer();

	// 2D
	DrawScore();
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

		g_bgm = MySoundCreate("data/BGM/Result.wav");

		InitMeshSky(Vec3(0, -220, -200), Vec3(0, 0, 0), 100, 100, 600.0f, 0.f);

		InitResultPlayer(0);

		InitScore(0);
		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------
	g_Rate = 0.f;

	MySoundPlayEternal(g_bgm);

	InitResultPlayer(1);

	ResetResultCam();

	InitScore(1);
	SetCameraFunc(ShowResultPlayerCam);		// 初期のカメラを設定
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

	MySoundStop(g_bgm);



	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	MySoundDelete(&g_bgm);

	UninitMeshSky();

	UninitResultPlayer();

	UninitScore();
}



/*=====================================================================
Result基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc()
{
	return &g_PhaseFunc;
}

/*=====================================================================
Result進捗取得関数
=====================================================================*/
float GetResultRate()
{
	return g_Rate;
}

#if 0
/*
/*〇〇キーを押したらポーズ画面*/
if (g_Icon.use == true)
{
	if ((GetKeyboardTrigger(DIK_RETURN))/* || (IsButtonTriggered(0, BUTTON_C))*/)
	{

		/*TITLE*/
		if (g_Icon.pos.x == RESULT_TITLE_LOGO_POS_X)
		{
			GoNextPhase(GetPhaseTitleFunc());


			g_Icon.use = false;										//ラグビーアイコンを消す

			g_Icon.pos.x = ICON_MIN;									//ラグビーアイコンを初期位置に戻す

		}

		/*NEWGAME*/
		if (g_Icon.pos.x == RESULT_NEWGAME_LOGO_POS_X)
		{
			GoNextPhase(GetPhaseGameTackle1Func());



			g_Icon.use = false;										//ラグビーアイコンを消す

			g_Icon.pos.x = ICON_MIN;									//ラグビーアイコンを初期位置に戻す
		}

		/*EXIT*/
		if (g_Icon.pos.x == RESULT_EXIT_LOGO_POS_X)
		{
			DestroyWindow(GetHandle());
		}
	}

	/*左キー*/
	if (GetKeyboardTrigger(DIK_LEFT)/* || IsButtonTriggered(0, BUTTON_UP)*/)
	{

		if (g_Icon.pos.x >= ICON_MIN)
		{
			g_Icon.pos.x -= ICON_REN;
		}
		if (g_Icon.pos.x < ICON_MIN)
		{
			g_Icon.pos.x = ICON_MAX;
		}
	}

	/*右キー*/
	if (GetKeyboardTrigger(DIK_RIGHT)/* || IsButtonTriggered(0, BUTTON_DOWN)*/)
	{

		if (g_Icon.pos.x <= ICON_MAX)
		{
			g_Icon.pos.x += ICON_REN;
		}
		if (g_Icon.pos.x > ICON_MAX)
		{
			g_Icon.pos.x = ICON_MIN;
		}
	}
	SetVertexResultIcon();

}
*/
#endif
