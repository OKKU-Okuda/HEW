/**********************************************************************
[[フィールド系リソース簡易管理プログラム(GameTackle/Field/ResourceManager.cpp)]]
	作者：奥田　真規

	フィールド間をまたいで使用するResourceを簡易的に管理するプログラム
***********************************************************************/
#include "../../Core/main.h"

#include "../field.h"
#include "ResourceManager.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

static void CreateFieldTexture();		// テクスチャの読み込み
static void CreateFieldMesh();			// メッシュの読み込み

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

Texture g_texPool[MAX_FIELDSHARETEXTURE];
Mesh	g_meshPool[MAX_FIELDSHAREMESH];

/*=====================================================================
テクスチャ読み込み関数
=====================================================================*/
void CreateFieldTexture()
{
	D3DDEVICE;

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field01.png", &g_texPool[FTEX_NONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_wall.png", &g_texPool[FTEX_WALL]);

}

/*=====================================================================
メッシュ読み込み関数
=====================================================================*/
void CreateFieldMesh()
{
	g_meshPool[FMESH_LONGFLAT]		= Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT), &Vec3(0, 0, 0));

	g_meshPool[FMESH_LONGWALLRIGHT] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	g_meshPool[FMESH_LONGWALLLEFT] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	// 分岐フィールド系のメッシュ
	{
		const float sizeZ = (FIELDCHIP_HEIGHT - FIELDROAD_X) / 2;
		const float posZ = FIELDCHIP_CENTER_Z - (sizeZ / 2);

		const float size_flatX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2);
		const float pos_flatX = size_flatX / 2 + (FIELDROAD_X / 2);

		const float size_WallX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2) - ROADWALL_SIZEX;
		const float pos_WallX = pos_flatX + (ROADWALL_SIZEX / 2);

		g_meshPool[FMESH_LEFTFRONTWALL] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
			&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

		g_meshPool[FMESH_RIGHTFRONTWALL] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
			&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));



		g_meshPool[FMESH_LEFTCENTERWALL] = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
			&Vec3(-pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));

		g_meshPool[FMESH_RIGHTCENTERWALL] = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
			&Vec3(pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));


		g_meshPool[FMESH_FRONTFLAT] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ + FIELDROAD_X), &Vec3(0, 0, -posZ + (FIELDROAD_X / 2)));


		g_meshPool[FMESH_LEFTFLAT] = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(-pos_flatX, 0, 0));
		g_meshPool[FMESH_RIGHTFLAT] = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(pos_flatX, 0, 0));
	}

}

/*=====================================================================
フィールドリソース読み込み関数
=====================================================================*/
void InitFieldResource()
{
	// 念のためゼロクリア
	ZeroMemory(g_texPool, sizeof(g_texPool));
	ZeroMemory(g_meshPool, sizeof(g_meshPool));

	CreateFieldTexture();
	CreateFieldMesh();
}

/*=====================================================================
フィールドリソース開放関数
=====================================================================*/
void UninitFieldResource()
{
	for (int i = 0; i < MAX_FIELDSHARETEXTURE; i++)
	{// テクスチャの開放
		SAFE_RELEASE(g_texPool[i]);
	}

	for (int i = 0; i < MAX_FIELDSHAREMESH; i++)
	{// メッシュの開放
		SAFE_RELEASE(g_meshPool[i]);
	}
}

/*=====================================================================
フィールド共通テクスチャ取得関数
=====================================================================*/
Texture GetFieldShareTexture(FIELD_SHARETEXTURE type)
{
	return g_texPool[type];
}

/*=====================================================================
フィールド共通メッシュ取得関数
=====================================================================*/
Mesh GetFieldShareMesh(FIELD_SHAREMESH type)
{
	return g_meshPool[type];
}
