/**********************************************************************
[[Tackleカメラプログラム(GameTackle/camera.cpp)]]
	作者：奥田　真規

	タックルのカメラに関するプログラム
***********************************************************************/
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "camera.h"	
#include "player.h"
#include "countdown.h"
#include "field.h"
#include "field_control.h"
#include "ground_mesh.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define CAMERA_ATOFFSET	(30)	// 注視点のプレイヤー座標からの3次元オフセット(y以外はいじるとやばい）
#define CAMERA_RANGE	(150.0f)		// プレイヤーからのレンジ
#define CAMERA_OFFSETY	(70.0f)		// 注視点からの視点オフセットｙ

#define CAMERA_DEBUGOFFSET		(Vec3(0, 55.0f, -180.0f))		// デバッグ用カメラの3次元オフセット
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
//static Vec3 OffsetCamPos(0, 55.0f, -180.0f);

/*=====================================================================
スタートカメラ1番目関数
=====================================================================*/
void GameStartCam001Update(CAMERA* pCam)
{
	Vec3 StartPos(PLAYER_POSX, 20, -500);
	Vec3 EndPos(PLAYER_POSX, 20, GetLatestFieldPosition().z);

	int sec;
	float per;

	GetCountdownTime(&sec, &per);
	if (sec == 2)
	{// 次のカメラワークにするかを判断
		GameStartCam002Update(pCam);
		SetCameraFunc(GameStartCam002Update);

		return;
	}

	//perPow = per * per;

	if (EndPos.z > GROUNDSIZE )
	{
		EndPos.z = GROUNDSIZE;
	}

	pCam->at = StartPos + ((EndPos - StartPos)* per);

	pCam->pos = pCam->at + Vec3(0, 300, -100);

	pCam->up = Vec3(0, 0, 1);
	//分岐の切れ目が見えちゃうので修正
}

/*=====================================================================
スタートカメラ2番目関数
=====================================================================*/
void GameStartCam002Update(CAMERA* pCam)
{
	Vec3 StartPos(PLAYER_POSX +50, 40, PLAYER_POSZ + 50);
	Vec3 EndPos(PLAYER_POSX+50, 40, PLAYER_POSZ -50);

	int sec;
	float per;

	GetCountdownTime(&sec, &per);
	if (sec == 1)
	{// 次のカメラワークにするかを判断
		TackleCameraUpdate(pCam);
		SetCameraFunc(TackleCameraUpdate);

		return;
	}

	pCam->at = *GetPlayerPos() + Vec3(0, 20.f, 0);

	pCam->pos = StartPos + ((EndPos - StartPos)* per);

}

/*=====================================================================
メインカメラ関数
=====================================================================*/
void TackleCameraUpdate(CAMERA* pCam)
{	
	Vec3 at = *GetPlayerPos();

	at.y = CAMERA_ATOFFSET;
	pCam->at = at;
	//pCam->pos = *GetPlayerPos() + OffsetCamPos;
	// カメラの注視点と視点を主人公に追従させる
	pCam->pos.x = sinf(GetPlayerRot()->y) * CAMERA_RANGE + pCam->at.x ; //+ OffsetCamPos.x;
	pCam->pos.z = cosf(GetPlayerRot()->y) * CAMERA_RANGE + pCam->at.z ; //+ OffsetCamPos.z;
	pCam->pos.y = pCam->at.y + CAMERA_OFFSETY;
	pCam->up = Vec3(0, 1, 0);

}

/*=====================================================================
デバッグカメラ関数(固定）
=====================================================================*/
void DebugCameraUpdate(CAMERA* pCam)
{
	pCam->at = *GetPlayerPos();
	pCam->pos = *GetPlayerPos() + CAMERA_DEBUGOFFSET;
}