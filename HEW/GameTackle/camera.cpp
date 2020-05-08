/**********************************************************************
[[Tackleカメラプログラム(GameTackle/camera.cpp)]]
	作者：奥田　真規

	タックルのカメラに関するプログラム
***********************************************************************/
#include "../Core/input.h"
#include "../Core/camera.h"
#include"../Core/main.h"
#include "../Core/debugproc.h"

#include "camera.h"	
#include "player.h"
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
static Vec3 OffsetCamPos(0, 0, -80.0f);

void TackleCameraUpdate(CAMERA* pCam)
{
	const float add = 2.0f;

	PLAYER *player = GetPlayer();

	// キー操作でのカメラ移動
	if (GetKeyboardPress(DIK_DOWN))
	{
		OffsetCamPos.z -= add;
	}

	if (GetKeyboardPress(DIK_UP))
	{
		OffsetCamPos.z += add;
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		OffsetCamPos.x -= add;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		OffsetCamPos.x += add;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		OffsetCamPos.y += add;
	}

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		OffsetCamPos.y -= add;
	}

	pCam->at = player[0].pos + Vec3(0, 20.0f, 0);
	pCam->pos = pCam->at + OffsetCamPos;
 
}