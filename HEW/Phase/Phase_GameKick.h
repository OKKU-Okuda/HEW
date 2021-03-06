/**********************************************************************
[[Phase_GameKickヘッダー(Phase_GameKick.h)]]
	作者：GameKickGameKick？？

	Phase_GameKick.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
GameKick基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseGameKickFunc();

// 基本関数群
void InitGameKick(bool isFirst);
void UninitGameKick(bool isEnd);
void UpdateGameKick();
void DrawGameKick();