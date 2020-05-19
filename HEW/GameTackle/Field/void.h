/**********************************************************************
[[直進道ヘッダー(GameTackle/Field/void.h)]]
	作者：奥田　真規

	GameTackle/Field/void.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------


/*=====================================================================
直線道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldVoidFunc();

// 関数群
void InitFieldVoid();
void UninitFieldVoid();
