/**********************************************************************
[[右崖道プログラム(GameTackle/Field/cliffR.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "ResourceManager.h"
#include "cliffR.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffR,UpdateFieldCliffR,DrawFieldCliffR };	// 道独自の関数

static Mesh g_meshFlat;			// 道の真ん中


/*=====================================================================
右崖道当たり判定関数
=====================================================================*/
bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 6) + PLAYER_FIELDSIZE_R)
	{	// 前座標が内側であれば外に出ないようにする(右側は半分奈落の為出る）

		if (pPos->x < -FIELDROAD_X / 2)
		{
			pPos->x = -FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
右崖道更新関数
=====================================================================*/
void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
右崖道描画関数
=====================================================================*/
void DrawFieldCliffR(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	GetFieldShareMesh(FMESH_LONGWALLLEFT)->DrawSubset(0);
}

/*=====================================================================
右崖道初期化関数
=====================================================================*/
void InitFieldCliffR()
{
	D3DDEVICE;

	// 道の床部分作成
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 3, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(-(FIELDROAD_X / 2 - FIELDROAD_X / 6), 0, 0));
}

/*=====================================================================
右崖道終了化関数
=====================================================================*/
void UninitFieldCliffR()
{
	// リソースの開放
	SAFE_RELEASE(g_meshFlat);
}

/*=====================================================================
右崖道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffRFunc()
{
	return &g_Func;
}