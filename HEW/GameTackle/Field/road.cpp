/**********************************************************************
[[直進道プログラム(GameTackle/Field/road.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../player.h"
#include "road.h"
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

static Mesh g_meshFlat;			// 道の真ん中
static Texture g_texFlat;		// 真ん中のテクスチャ

static Mesh g_meshRightWall;	// 右の壁
static Mesh g_meshLeftWall;		// 左の壁


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

	if ((pPastPos->x < -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPos->x >= -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R) ||
		(pPastPos->x > (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R && pPos->x <= (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R))
	{	// 左or右側：外から中に入ろうとするとブロックされる処理
		pPos->x = pPastPos->x;

#ifdef _DEBUG
		PrintDebugProc("[debug:field_checkhit]：外からの侵入阻止");
#endif
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

	pDevice->SetTexture(0, g_texFlat);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	g_meshRightWall->DrawSubset(0);
	g_meshLeftWall->DrawSubset(0);
}

/*=====================================================================
直線道初期化関数
=====================================================================*/
void InitFieldRoad()
{
	D3DDEVICE;

	// 道の床部分作成
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	// 左右の壁作成
	g_meshRightWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));
	g_meshLeftWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texFlat);
}

/*=====================================================================
直線道終了化関数
=====================================================================*/
void UninitFieldRoad()
{
	// リソースの開放
	SAFE_RELEASE(g_meshFlat);
	SAFE_RELEASE(g_meshLeftWall);
	SAFE_RELEASE(g_meshRightWall);
	SAFE_RELEASE(g_texFlat);
}

/*=====================================================================
直線道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldRoadFunc()
{
	return &g_Func;
}