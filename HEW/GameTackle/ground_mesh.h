/**********************************************************************
[[地面生成ヘッダー(ground_mesh.h)]]
	作者：奥田　真規

	ground_mesh.cppに関するヘッダー
***********************************************************************/
#pragma once

#define GROUNDSIZE				(FIELDCHIP_WIDTH * 4.f)


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// 基本関数群
void UpdateGround();
void DrawGround();
void InitGround();
