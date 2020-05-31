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

#ifdef _DEBUG
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texPool[FTEX_NONE]);
#else
	g_texPool[FTEX_NONE] = NULL;
#endif

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
