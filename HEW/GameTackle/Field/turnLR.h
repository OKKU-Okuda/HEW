/**********************************************************************
[[左右分岐道道ヘッダー(GameTackle/Field/turnLR.h)]]
	作者：奥田　真規

	GameTackle/Field/turnLR.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------


/*=====================================================================
左右分岐道道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldTurnLRFunc();

// 関数群
void InitFieldTurnLR();
void UninitFieldTurnLR();
