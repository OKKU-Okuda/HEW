/**********************************************************************
[[直進道プログラム(GameTackle/Field/road.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../field.h"
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
static bool CheckHitFieldRoad(FIELD_CHIP* pData);
static void UpdateFieldRoad(FIELD_CHIP* pData);
static void DrawFieldRoad(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldRoad,UpdateFieldRoad,DrawFieldRoad };	// 道独自の関数

static Mesh g_meshFlat;			// 道の真ん中
static Mesh g_meshRightWall;	// 右の壁
static Mesh g_meshLeftWall;		// 左の壁

/*=====================================================================
●●関数
	●●する関数
	戻り値 : void
	引数 : void
=====================================================================*/
void InitFieldRoad()
{

	// 道の床部分作成
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	// 左右の壁作成
	g_meshRightWall= Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(FIELDROAD_X / 2, 0, 0));
	g_meshLeftWall = Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(-FIELDROAD_X / 2, 0, 0));

}

bool CheckHitFieldRoad(FIELD_CHIP* pData)
{
	
}
// ワールド行列の逆行列に
void UpdateFieldRoad(FIELD_CHIP* pData)
{

}

void DrawFieldRoad(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, NULL);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);
	g_meshRightWall->DrawSubset(0);
	g_meshLeftWall->DrawSubset(0);
}