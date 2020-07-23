/**********************************************************************
[[右分岐道道プログラム(GameTackle/Field/turnR.cpp)]]
	作者：奥田　真規

	右分岐道道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../../Phase/Phase_GameTackle1.h"
#include "../field.h"
#include "ResourceManager.h"
#include "../field_control.h"
#include "../player_control.h"
#include "turnR.h"
#include "../UI.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define RANGE_INPUTLR	(FIELDCHIP_WIDTH*1.25f)		// 中心までのinput読み込みエリア

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// 
enum TURNLR_QTETYPE {
	QTE_NOINPUT = 0,
	QTE_LEFT,
	QTE_RIGHT
};

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static bool CheckHitFieldTurnR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldTurnR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldTurnR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldTurnR,UpdateFieldTurnR,DrawFieldTurnR };	// 道独自の関数
static TURNLR_QTETYPE g_QTEState = QTE_NOINPUT;

static Mesh g_meshWallBack = NULL;

//static Mesh g_meshWallLeftFront = NULL;
///static Mesh g_meshWallRightFront = NULL;
//static Mesh g_meshWallLeftCenter = NULL;
//static Mesh g_meshWallRightCenter = NULL;


//static Mesh g_meshFlatFront = NULL;
//static Mesh g_meshFlatLeft = NULL;
//static Mesh g_meshFlatRight = NULL;


/*=====================================================================
左右分岐道道当たり判定関数
=====================================================================*/
bool CheckHitFieldTurnR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	// 位置の状態列挙
	enum POSITION_STATE {
		PSTATE_VOID = 0x0,				// 落ちろ
		PSTATE_INVERTICAL = 0x01,		// 縦エリアの中にいる
		PSTATE_INHORIZONTAL = 0x02,		// 横エリアの中にいる
		PSTATE_INCROSSAREA = PSTATE_INVERTICAL | PSTATE_INHORIZONTAL,		// 縦と横のエリアに重なってるとこにいる
	};

	DWORD PosState = PSTATE_VOID;		// 位置の状態

	// エリア状態の判別処理
	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2)+ PLAYER_FIELDSIZE_R &&
		pPastPos->z < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// 縦エリアにいる
		PosState |= PSTATE_INVERTICAL;
	}

	if (pPastPos->z > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->z < (FIELDROAD_X / 2)+PLAYER_FIELDSIZE_R&&
		pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R)
	{	// 横エリアにいる
		PosState |= PSTATE_INHORIZONTAL;
	}


	// 状態別処理
	if (PosState == PSTATE_INVERTICAL)
	{	
		PrintDebugProc("tate");
		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	else if (PosState == PSTATE_INHORIZONTAL)
	{
		PrintDebugProc("yoko");
		SAFE_NUMBER(pPos->z, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	else if (PosState == PSTATE_INCROSSAREA)
	{
		PrintDebugProc("cross");
		if (pPos->z > (FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R)
		{	// クロスの場合は壁より奥に進もうとするとゲームオーバー
			GameTackle1End();
			//pPos->z = FIELDROAD_X / 2- PLAYER_FIELDSIZE_R;
		}

		return true;
	}

	return false;
}

/*=====================================================================
左右分岐道道更新関数
=====================================================================*/
void UpdateFieldTurnR(FIELD_CHIP* pData, Vec3* pPos)
{
	const float maxIptPosZ = -FIELDROAD_X / 2 - PLAYER_FIELDSIZE_R;		// 入力受付のｚ最大値
	const float minIptPosZ = maxIptPosZ - RANGE_INPUTLR;				//　同上最小値

	const float maxRotPosZ = FIELDROAD_X / 5;						// 回転のｚ最大値
	const float minRotPosZ = -FIELDROAD_X / 5;						// 同上最小値


	if (pPos->z <= maxIptPosZ && pPos->z >= minIptPosZ && g_QTEState == QTE_NOINPUT)
	{// 未入力状態で左右キーを押すとCURVE予約
#ifdef _DEBUG
		PrintDebugProc("[debug_TurnL]Qキー、Eキー:CURVE");
#endif
		SetUIGuideActive(UD_LEFT, false);
		SetUIGuideActive(UD_RIGHT, true);

		if (GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(0, BUTTON_Y))
		{
			g_QTEState = QTE_LEFT;
			SetUIGuideActive(UD_LEFT, true);
			SetUIGuideActive(UD_RIGHT, false);
			PlayUIGuideSelect();
		}
		else if (GetKeyboardTrigger(DIK_E) || IsButtonTriggered(0, BUTTON_Z))
		{
			g_QTEState = QTE_RIGHT;
			PlayUIGuideSelect();
		}
	}
	else if (g_QTEState != QTE_NOINPUT && pPos->z <= maxRotPosZ && pPos->z >= minRotPosZ)
	{// 回転エリアに到着すると予約通りの回転を行う
		FIELD_CHIP*	pDelChip = NULL;			// CURVEとは反対側にある道
		CHIP_ID id_start;// 続く道のid

		if (g_QTEState == QTE_RIGHT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), 1));

			// 反対側の道を消す処理
			pDelChip = SearchChipID(AddFieldID(pData->ID, GetFieldIDVector(AddFieldDirection(GetPlayerDirection(), 2))));
			if (pDelChip != NULL)
			{
				DeleteField(pDelChip);
			}


			id_start = AddFieldID(pData->ID, GetFieldIDVector(GetPlayerDirection()));
			SpawnField(id_start);
		}
		else if (g_QTEState == QTE_LEFT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), -1));
		}

		g_QTEState = QTE_NOINPUT;

		SetUIGuideActive(UD_LEFT, false);
		SetUIGuideActive(UD_RIGHT, false);

	}
}

/*=====================================================================
左右分岐道道描画関数
=====================================================================*/
void DrawFieldTurnR(FIELD_CHIP* pData)
{
	D3DDEVICE;


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	GetFieldShareMesh(FMESH_FRONTFLAT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_RIGHTFLAT)->DrawSubset(0);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_WALL));

	GetFieldShareMesh(FMESH_LEFTFRONTWALL)->DrawSubset(0);
	GetFieldShareMesh(FMESH_RIGHTFRONTWALL)->DrawSubset(0);

	g_meshWallBack->DrawSubset(0);

	GetFieldShareMesh(FMESH_RIGHTCENTERWALL)->DrawSubset(0);


}

/*=====================================================================
左右分岐道道初期化関数
=====================================================================*/
void InitFieldTurnR()
{
	// 
	g_meshWallBack = Create3DBoxMesh(&Vec3((FIELDROAD_X/2) + (FIELDCHIP_WIDTH / 2) + (ROADWALL_SIZEX/2), ROADWALL_SIZEY, ROADWALL_SIZEX),
		&Vec3((FIELDCHIP_WIDTH / 4)-(FIELDROAD_X/4)-(ROADWALL_SIZEX/2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), (FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2)));

}

/*=====================================================================
左右分岐道道終了化関数
=====================================================================*/
void UninitFieldTurnR()
{
}

/*=====================================================================
左右分岐道道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldTurnRFunc()
{
	return &g_Func;
}