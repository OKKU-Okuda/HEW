/**********************************************************************
[[直進道ヘッダー(GameTackle/Field/road.h)]]
	作者：奥田　真規

	GameTackle/Field/road.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
直線道Awake関数
=====================================================================*/
void AwakeFieldRoad(FIELD_CHIP* pData);

/*=====================================================================
直線道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldRoadFunc();

// 関数群
void InitFieldRoad();
void UninitFieldRoad();
