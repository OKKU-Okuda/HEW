/**********************************************************************
[[ジャンプ道プログラム(GameTackle/Field/jump.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../../Core/CheckHit.h"

#include "../field.h"
#include "ResourceManager.h"
#include "jump.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define JUMPVOID_ITV		(300.0f)		// 中央に設置する落とし穴の間隔

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static bool CheckHitFieldJump(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldJump(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldJump(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldJump,UpdateFieldJump,DrawFieldJump };	// 道独自の関数

// もでるを2つにして間を奈落にする(0手前、1奥)
static Mesh g_meshFlat[2];		// 道の真ん中

static Mesh g_meshRightWall[2];	// 右の壁
static Mesh g_meshLeftWall[2];	// 左の壁

/*=====================================================================
ジャンプ道当たり判定関数
=====================================================================*/
bool CheckHitFieldJump(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	if ((pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R) &&
		(pPastPos->z < (-JUMPVOID_ITV / 2) + PLAYER_FIELDSIZE_R || pPastPos->z > (JUMPVOID_ITV / 2) - PLAYER_FIELDSIZE_R))
	{	// (地面メッシュのある箇所のみ)前座標が内側であれば外に出ないようにする

		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	return false;
}

/*=====================================================================
ジャンプ道更新関数
=====================================================================*/
void UpdateFieldJump(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
ジャンプ道描画関数
=====================================================================*/
void DrawFieldJump(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	for (int i = 0; i < 2; i++)
	{
		g_meshFlat[i]->DrawSubset(0);

		g_meshRightWall[i]->DrawSubset(0);
		g_meshLeftWall[i]->DrawSubset(0);
	}
}

/*=====================================================================
ジャンプ道初期化関数
=====================================================================*/
void InitFieldJump()
{
	D3DDEVICE;

	const float sizeZ = (FIELDCHIP_HEIGHT - JUMPVOID_ITV) / 2;
	const float posZ = (JUMPVOID_ITV / 2) + (sizeZ / 2);

	// 道の床部分作成
	g_meshFlat[0] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ),
		&Vec3(0, 0, posZ));

	g_meshFlat[1] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ),
		&Vec3(0, 0, -posZ));

	// 左右の壁作成
	g_meshRightWall[0] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), posZ));
	g_meshLeftWall[0] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), posZ));

	g_meshRightWall[1] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));
	g_meshLeftWall[1] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

}

/*=====================================================================
ジャンプ道終了化関数
=====================================================================*/
void UninitFieldJump()
{
	// リソースの開放
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(g_meshFlat[i]);
		SAFE_RELEASE(g_meshLeftWall[i]);
		SAFE_RELEASE(g_meshRightWall[i]);
	}
}

/*=====================================================================
ジャンプ道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldJumpFunc()
{
	return &g_Func;
}