/**********************************************************************
[[プレイヤー行動（操作系)プログラム(player_control.cpp)]]
	作者：奥田　真規

	プレイヤーの操作面（回転面）に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "../Core/camera.h"
#include "camera.h"
#include "player.h"
#include "field.h"
#include "player_control.h"
#include "UI.h"
#include "countdown.h"

#include "../Phase/Phase_GameTackle1.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define CHANGE_METER(len)	(len / 40.0f)

#define	VALUE_MOVE			(10.0f)							// 移動量

#define ADDROT	(0.1f)				// 回転量
#define ADDPOS	(8.0f)				// 移動量
#define ADDXPOS		(3.0f)			// 横移動

#define MAX_SPD		(16.f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static void UpdatePlayerRotation();		// 回転チェック関数
static void UpdatePlayerTranslation();	// 位置チェック関数(QTE系処理は除く)

#ifdef _DEBUG
static void UpdatePlayerDebug();		// デバッグ移動関数
#endif
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static FIELD_DIRECTION		g_dirPlayer;		// プレイヤーのフィールド的方向

static bool					g_isRotation;		// プレイヤー回転中か
//static bool					g_isMoveAccept;		// 奥への移動権限があるか
static MOVE_STATE			g_stateMove;			// 移動状態
static float				g_lengthRun;			// 走りの総距離
static float				g_Spd;					// 奥に行くスピード
static float				g_HoriSpd;				// 横移動スピード

#ifdef _DEBUG
static bool g_isDebugControl = false;// (debug)操作に減速を付けて自由に操作できるようにする
static float g_spd = 0.0f;
#endif

/*=====================================================================
プレイヤー制御更新関数
=====================================================================*/
void UpdatePlayerControl()
{
	UpdatePlayerRotation();
	UpdatePlayerTranslation();
}

/*=====================================================================
プレイヤーリセット関数
=====================================================================*/
void ResetPlayerControl()
{
	g_dirPlayer = FDIRECTION_0ZP;
	g_isRotation = true;
	g_stateMove = MSTATE_READY;
	g_Spd = ADDPOS;
	g_lengthRun = 0.0f;
	g_HoriSpd = 0.0f;
#ifdef _DEBUG
	g_isDebugControl = false;// (debug)操作に減速を付けて自由に操作できるようにする
	g_spd = 0.0f;
#endif

}

/*=====================================================================
プレイヤー方向設定関数
=====================================================================*/
void SetPlayerDirection(FIELD_DIRECTION fdir)
{
	g_dirPlayer = fdir;
	g_isRotation = true;	// 回転flagを立てる
}

/*=====================================================================
プレイヤー方向取得関数
=====================================================================*/
FIELD_DIRECTION GetPlayerDirection()
{
	return g_dirPlayer;
}

/*=====================================================================
プレイヤー回転状態取得関数
=====================================================================*/
bool IsPlayerRotation()
{
	return g_isRotation;
}

/*=====================================================================
プレイヤー移動状態取得関数
=====================================================================*/
MOVE_STATE GetPlayerMoveState()
{
	return g_stateMove;
}

/*=====================================================================
プレイヤー移動状態設置関数
=====================================================================*/
void SetPlayerMoveState(MOVE_STATE mstate)
{
	g_stateMove = mstate;
}

/*=====================================================================
プレイヤー移動距離取得関数
=====================================================================*/
float GetPlayerRunMeter()
{
	return g_lengthRun;
}

/*=====================================================================
プレイヤー速度設定関数
=====================================================================*/
void SetPlayerSpd(float spd)
{
	if (spd > MAX_SPD)
	{
		g_Spd = MAX_SPD;
		return;
	}

	g_Spd = spd;
}

/*=====================================================================
プレイヤー速度取得関数
=====================================================================*/
float GetPlayerSpd()
{
	return g_Spd;
}

/*=====================================================================
cppない関数
=====================================================================*/

void UpdatePlayerRotation()
{
	float rotY = (D3DX_PI / 2)*(int)g_dirPlayer + D3DX_PI;		// いるべきｙ軸回転
	float rotYfrom = GetPlayerRot()->y;					// 回転開始角度
	float rotVec = rotY - rotYfrom;						// 回転方向

	if (g_isRotation == false)
	{// 回転flagがfalseの場合はこの処理を飛ばす
		return;
	}

	// 回転量の差がある場合はプレイヤーのロット基準で目的回転値を求める
	if (rotVec > D3DX_PI)
	{
		rotY -= D3DX_PI * 2;
		rotVec = rotY - rotYfrom;
	}
	else if (rotVec < -D3DX_PI)
	{
		rotY += D3DX_PI * 2;
		rotVec = rotY - rotYfrom;
	}

	// 回転値が異なる場合の処理
	if (GetPlayerRot()->y != rotY)
	{

		float addrot = ADDROT;

		if (fabsf(rotVec) < ADDROT)
		{// 回転量が指定回転以上回ってしまう場合は調整(回転flagもfalseに）
			g_isRotation = false;

			GetPlayerRot()->y = rotY;
		}
		else
		{
			g_isRotation = true;

			if (rotVec < 0)
			{// マイナス回転であれば加算量の符号を反転する
				addrot *= -1;
			}

			GetPlayerRot()->y += addrot;
		}

		// 回転の整合処理
		if (GetPlayerRot()->y > D3DX_PI * 2)
		{
			GetPlayerRot()->y -= D3DX_PI * 2;
		}
		else if (GetPlayerRot()->y < 0)
		{
			GetPlayerRot()->y += D3DX_PI * 2;
		}

		return;
	}
	
	g_isRotation = false;
}

void UpdatePlayerTranslation()
{
	int countdown_flag = GetCountdownFlag();

	if (g_isRotation == true)
	{// 回転中は移動処理しない
		return;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F6))
	{// コントロールモード反転
		g_isRotation = true;

		if (g_isDebugControl == true)
		{// 通常に戻す
			g_isDebugControl = false;
			SetCameraFunc(TackleCameraUpdate);
		}
		else
		{// デバッグ化
			g_isDebugControl = true;
			SetCameraFunc(DebugCameraUpdate);
		}
	}

	if (g_isDebugControl == true)
	{
		PrintDebugProc("[debug_player]F6:デバックコントロールを無効にする(現在有効)");
		PrintDebugProc("[debug_player]↑↓←→キー:十字移動");

		UpdatePlayerDebug();
		return;
	}

	PrintDebugProc("[debug_player]F6:デバックコントロールを有効にする(現在無効)");
	PrintDebugProc("[debug_player]Aキー、Dキー:横移動");

#endif

	// 横移動処理
	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
	{
		g_HoriSpd = -ADDXPOS;
	}

	if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
	{
		g_HoriSpd = ADDXPOS;
	}

	g_HoriSpd *= 0.7f;

	*GetPlayerPos() += GetFieldVector(AddFieldDirection(g_dirPlayer, 1)) * g_HoriSpd;

	// 奥移動移動
	if (g_stateMove == MSTATE_RUNNING)
	{
		*GetPlayerPos() += GetFieldVector(g_dirPlayer) * g_Spd;
		g_lengthRun += CHANGE_METER(g_Spd);							// 総距離計算
#ifdef _DEBUG
		PrintDebugProc(">>>>移動距離 %f M 速度%f<<<<", g_lengthRun, g_Spd);		// デバッグ
#endif // _DEBUG
		ChangeDistance((int)g_lengthRun);
	}
	else if (g_stateMove==MSTATE_READY && countdown_flag==2)
	{// 開幕はWで移動開始
		g_stateMove = MSTATE_RUNNING;
		GameTackle1Start();
	}
}

#ifdef _DEBUG
void UpdatePlayerDebug()
{

	//移動処理
	int		dir = 0;	// ０：向きは変えない

	if (GetKeyboardPress(DIK_LEFT))
	{
		dir |= 8;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		dir |= 4;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_spd = VALUE_MOVE;
	}

	// 入力されたキーに合わせて向きを決める
	float roty = 0.0f;
	switch (dir)
	{
	case 1:	// 下向き
		roty = 0.0f;
		break;

	case 2:	// 上向き
		roty = D3DX_PI;
		break;

	case 4:	// 右向き
		roty = -D3DX_PI / 2;
		break;

	case 8:	// 左向き
		roty = D3DX_PI / 2;
		break;

	case 5:	// 右下向き
		roty = -D3DX_PI / 4;
		break;

	case 6:	// 右上向き
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// 左下向き
		roty = D3DX_PI / 4;
		break;

	case 10: // 左上向き
		roty = D3DX_PI / 4 * 3;
		break;

	case 0:
		roty = GetPlayerRot()->y;
		break;

	default:
		break;


	}

	GetPlayerRot()->y = roty;


	g_spd *= 0.9f;// 減速

	//移動処理[[ここでﾌﾟﾚｲﾔｰ側のポジション確定]]
	GetPlayerPos()->x -= sinf(GetPlayerRot()->y) * g_spd;
	GetPlayerPos()->z -= cosf(GetPlayerRot()->y) * g_spd;

}
#endif