/**********************************************************************
[[フィールド系リソース簡易管理ヘッダー(GameTackle/Field/ResourceManager.h)]]
	作者：奥田　真規

	GameTackle/Field/ResourceManager.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------




enum FIELD_SHARETEXTURE {
	FTEX_NONE,					// 何もないテクスチャ(debug時はデバック用テクスチャが読み込まれる)
	FTEX_WALL,					// 壁のテクスチャ
	MAX_FIELDSHARETEXTURE		/*最大数取得*/
};

enum FIELD_SHAREMESH {
	FMESH_LONGFLAT,				// チャンク長の地面
	FMESH_LONGWALLLEFT,			// チャンク長の左壁
	FMESH_LONGWALLRIGHT,		// チャンク長の右壁

	FMESH_FRONTFLAT,			// 半分長の地面
	FMESH_LEFTFLAT,				// 半分長左半分の横型地面
	FMESH_RIGHTFLAT,			// 半分長右半分の横型地面
		
	FMESH_LEFTFRONTWALL,		// 手前の左半分壁
	FMESH_RIGHTFRONTWALL,		// 手前の右半分壁
	FMESH_LEFTCENTERWALL,		// 奥の左半分横型壁
	FMESH_RIGHTCENTERWALL,		// 奥の右半分横型壁

	MAX_FIELDSHAREMESH			/*最大数取得*/
};

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
フィールド共通テクスチャ取得関数
=====================================================================*/
Texture GetFieldShareTexture(FIELD_SHARETEXTURE type);

/*=====================================================================
フィールド共通メッシュ取得関数
=====================================================================*/
Mesh GetFieldShareMesh(FIELD_SHAREMESH type);


// 基本関数群
void InitFieldResource();
void UninitFieldResource();
