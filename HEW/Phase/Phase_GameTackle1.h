/**********************************************************************
[[Phase_GameTackle1ヘッダー(Phase_GameTackle1.h)]]
	作者：GameTackle1GameTackle1？？

	Phase_GameTackle1.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
GameTackle1ゲームエンド関数
=====================================================================*/
void GameTackle1Start();

/*=====================================================================
GameTackle1ゲームエンド関数
=====================================================================*/
void GameTackle1End();

/*=====================================================================
GameTackle1基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle1Func();

// 基本関数群
void InitGameTackle1(bool isFirst);
void UninitGameTackle1(bool isEnd);
void UpdateGameTackle1();
void DrawGameTackle1();