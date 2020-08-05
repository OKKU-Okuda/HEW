/**********************************************************************
[[プレイヤー行動（操作系)ヘッダー(player_control.h)]]
	作者：奥田　真規


***********************************************************************/
#pragma once

#include "field.h"

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// 移動の状態遷移
enum MOVE_STATE {
	MSTATE_READY,		// 移動待機状態
	MSTATE_RUNNING,		// 移動状態
	MSTATE_END,			// 移動終了状態
};

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
プレイヤー方向設定関数
=====================================================================*/
void SetPlayerDirection(FIELD_DIRECTION fdir);

/*=====================================================================
プレイヤー方向取得関数
=====================================================================*/
FIELD_DIRECTION GetPlayerDirection();

/*=====================================================================
プレイヤー回転状態取得関数
=====================================================================*/
bool IsPlayerRotation();

/*=====================================================================
プレイヤー移動状態設置関数
=====================================================================*/
void SetPlayerMoveState(MOVE_STATE mstate);

/*=====================================================================
プレイヤー移動状態取得関数
=====================================================================*/
MOVE_STATE GetPlayerMoveState();

/*=====================================================================
プレイヤー移動距離取得関数
=====================================================================*/
float GetPlayerRunMeter();

/*=====================================================================
プレイヤー速度設定関数
=====================================================================*/
void SetPlayerSpd(float spd);

/*=====================================================================
プレイヤー速度取得関数
=====================================================================*/
float GetPlayerSpd();

// 基本関数群
void UpdatePlayerControl();
void ResetPlayerControl();
