/**********************************************************************
[[直進道プログラム(GameTackle/Field/road.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../Gimmick/Wall.h"
#include "ResourceManager.h"
#include "road.h"
#include "../item.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldRoad(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldRoad,UpdateFieldRoad,DrawFieldRoad };	// 道独自の関数

/*=====================================================================
直線道当たり判定関数
=====================================================================*/
bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// 前座標が内側であれば外に出ないようにする

		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2,FIELDROAD_X / 2);
		return true;
	}

	return false;
}

/*=====================================================================
直線道更新関数
=====================================================================*/
void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
直線道描画関数
=====================================================================*/
void DrawFieldRoad(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);


	GetFieldShareMesh(FMESH_LONGFLAT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_LONGWALLLEFT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_LONGWALLRIGHT)->DrawSubset(0);

}

/*=====================================================================
直線道Awake関数
=====================================================================*/
void AwakeFieldRoad(FIELD_CHIP* pData)
{
	if (CheckRand(0.2f) == true)
	{// コインの設置
		const int numCoin = 20;													// コインの設置数
		const float itv_coin = FIELDCHIP_HEIGHT / (numCoin + 1);				// コイン設置間隔
		const float PosX = rand() % (int)FIELDROAD_X - (FIELDROAD_X / 2);		// コイン設置列

		for (int i = 0; i < numCoin; i++)
		{
			Vec3 pos(PosX, POSY_COIN, -FIELDCHIP_CENTER_Z + (i * itv_coin));		// 設置位置

			D3DXVec3TransformCoord(&pos, &pos, &pData->WldMat);			// 位置をワールド座標に変換

			SetItem(pData,pos, Vec3(0, 0, 0));							// コイン設置
		}

	}
	else if (CheckRand(0.5f) == true)
	{// GIMMICK:壁の設置
		int typeWall = rand() % MAX_GIMICKWALLQTE;
		SetFieldGimmick(pData, GTYPE_WALL, &Vec3(0, 0, 250.0f), typeWall);
	}
}

/*=====================================================================
直線道初期化関数
=====================================================================*/
void InitFieldRoad()
{
}

/*=====================================================================
直線道終了化関数
=====================================================================*/
void UninitFieldRoad()
{
}

/*=====================================================================
直線道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldRoadFunc()
{
	return &g_Func;
}