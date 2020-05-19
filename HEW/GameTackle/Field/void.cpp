/**********************************************************************
[[直進道プログラム(GameTackle/Field/void.cpp)]]
	作者：奥田　真規

	直進道に関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../player.h"
#include "void.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define FIELDVOID_SIZEY		(1000.0f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static bool CheckHitFieldVoid(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldVoid(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldVoid(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldVoid,UpdateFieldVoid,DrawFieldVoid };	// 道独自の関数

#ifdef _DEBUG		
static Mesh g_meshBorder;		// 道の境界線がわかるメッシュ(デバッグのみ）
static Texture g_texBorder;		// 境界テクスチャ(デバッグのみ）
#endif
/*=====================================================================
直線道当たり判定関数
=====================================================================*/
bool CheckHitFieldVoid(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
#ifdef _DEBUG
	PrintDebugProc("[debug:field_checkhit]：奈落の為無条件で落とします");

#endif
	return false;

}

/*=====================================================================
直線道更新関数
=====================================================================*/
void UpdateFieldVoid(FIELD_CHIP* pData, Vec3* pPos)
{


}

/*=====================================================================
直線道描画関数
=====================================================================*/
void DrawFieldVoid(FIELD_CHIP* pData)
{
#ifdef _DEBUG		
	D3DDEVICE;
	pDevice->SetTexture(0, g_texBorder);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshBorder->DrawSubset(0);
#endif
}

/*=====================================================================
直線道初期化関数
=====================================================================*/
void InitFieldVoid()
{
#ifdef _DEBUG
	D3DDEVICE;

	// 道の床部分作成
	g_meshBorder = Create3DBoxMesh(&Vec3(FIELDCHIP_WIDTH, FIELDVOID_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/yellow_halfalpha.png", &g_texBorder);
#endif
}

/*=====================================================================
直線道終了化関数
=====================================================================*/
void UninitFieldVoid()
{
#ifdef _DEBUG
	// リソースの開放
	SAFE_RELEASE(g_meshBorder);
	SAFE_RELEASE(g_texBorder);
#endif
}

/*=====================================================================
直線道独自関数アドレス取得関数
=====================================================================*/
FIELD_OBJFUNC* GetFieldVoidFunc()
{
	return &g_Func;
}