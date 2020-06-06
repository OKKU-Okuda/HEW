/**********************************************************************
[[プレイヤー行動（操作系)プログラム(player_control.cpp)]]
	作者：奥田　真規

	プレイヤーの操作面（回転面）に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "player.h"
#include "field.h"
#include "player_control.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define	VALUE_MOVE			(5.0f)							// 移動量

#define ADDROT	(0.1f)				// 回転量
#define ADDPOS	(2.0f)				// 移動量
#define ADDXPOS		(3.0f)			// 横移動
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
	if (g_isRotation == true)
	{// 回転中は移動処理しない
		return;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F6))
	{// コントロールモード反転
		g_isDebugControl = !g_isDebugControl;
		g_isRotation = true;
	}

	if (g_isDebugControl == true)
	{
		PrintDebugProc("[debug_player]F6:デバックコントロールを無効にする(現在有効)");
		
		UpdatePlayerDebug();
		return;
	}

	PrintDebugProc("[debug_player]F6:デバックコントロールを有効にする(現在無効)");
	
#endif

	// 奥移動移動
	*GetPlayerPos() += GetFieldVector(g_dirPlayer) * ADDPOS;

	// 横移動処理
	if (GetKeyboardPress(DIK_A))
	{
		*GetPlayerPos() += GetFieldVector(AddFieldDirection(g_dirPlayer, -1)) * ADDXPOS;
	}

	if (GetKeyboardPress(DIK_D))
	{
		*GetPlayerPos() += GetFieldVector(AddFieldDirection(g_dirPlayer, 1)) * ADDXPOS;
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