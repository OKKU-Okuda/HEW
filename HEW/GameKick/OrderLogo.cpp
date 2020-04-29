/**********************************************************************
[[OrderLogoプログラム(OrderLogo.cpp)]]
	作者：奥田　真規

	OrderLogoに関するプログラム
***********************************************************************/
#include "../Core/input.h"
#include"../Core/main.h"
#include "OrderLogo.h"	

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

// 基本関数群
static void InitOrderLogo(bool isFirst);
static void UninitOrderLogo(bool isEnd);
static void UpdateOrderLogo();
static void DrawOrderLogo();

static void SetLogo5Vertex(VERTEX_2D *pVtx, Vec3* pBottomPos, Vec2*pBaseSize, float difference);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static OBJ_FUNC g_Func = { InitOrderLogo,UninitOrderLogo,UpdateOrderLogo,DrawOrderLogo };

static struct {
	Texture		pTex;
	VERTEX_2D	Vtx[5];
	WORD		Idx[8] = { 0,1,2,4,3,3,2,0 };
	Vec3		bottomPos;
	Vec2		baseSize;
	float		difference;
	float		rot;
}g_Kick;

/*=====================================================================
OrderLogo更新関数
=====================================================================*/
void UpdateOrderLogo()
{

	const float spd = 10.0f;

	g_Kick.rot += 0.1f;
	g_Kick.difference = sinf(g_Kick.rot)*100.0f;

	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_Kick.bottomPos.x += spd;
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		g_Kick.bottomPos.x -= spd;
	}

	if (GetKeyboardPress(DIK_UP))
	{
		g_Kick.bottomPos.y -= spd;
	}

	if (GetKeyboardPress(DIK_DOWN))
	{
		g_Kick.bottomPos.y += spd;
	}

	SetLogo5Vertex(g_Kick.Vtx, &g_Kick.bottomPos, &g_Kick.baseSize, g_Kick.difference);
}

/*=====================================================================
OrderLogo描画関数
=====================================================================*/
void DrawOrderLogo()
{
	D3DDEVICE;

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, g_Kick.pTex);

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, 5, 6, g_Kick.Idx, D3DFMT_INDEX16, g_Kick.Vtx, sizeof(VERTEX_2D));
	//pDevice->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, 5, 6, g_KickIdx, D3DFMT_INDEX16, g_KickVtx, sizeof(VERTEX_2D));

//	pDevice->DrawIndexedPrimitiveUP()
}

/*=====================================================================
OrderLogo初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitOrderLogo(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		D3DDEVICE;

		for (int i = 0; i < 5; i++)
		{
			Make2DVertex(&g_Kick.Vtx[i], &Vec3(0, 0, 0), &Vec2(0, 0));
		}

		Make2DVertex(&g_Kick.Vtx[0], &Vec3(SCREEN_CENTER_X-100, SCREEN_CENTER_Y-50, 0),	&Vec2(0, 0));		// 左上
		Make2DVertex(&g_Kick.Vtx[1], &Vec3(SCREEN_CENTER_X+100, SCREEN_CENTER_Y-50, 0), &Vec2(1, 0));		// 右上
		Make2DVertex(&g_Kick.Vtx[2], &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0), &Vec2(0.5f, 0.5f));			// 中央
		Make2DVertex(&g_Kick.Vtx[3], &Vec3(SCREEN_CENTER_X-100, SCREEN_CENTER_Y+50, 0), &Vec2(0, 1));		// 左下
		Make2DVertex(&g_Kick.Vtx[4], &Vec3(SCREEN_CENTER_X+100, SCREEN_CENTER_Y+50, 0), &Vec2(1, 1));		// 右下

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LastKick.png", &g_Kick.pTex);
		//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TEST.png", &g_KickTex);

		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// キックワーク系ステータス
	g_Kick.baseSize		= Vec2(300, 150);
	g_Kick.bottomPos	= Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0);
	g_Kick.difference	= 0;

}

/*=====================================================================
OrderLogo終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitOrderLogo(bool isEnd)
{
	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------





	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------




}

/*=====================================================================
OrderLogo基本関数群取得関数
=====================================================================*/
OBJ_FUNC* GetOrderLogoFunc()
{
	return &g_Func;
}

void SetLogo5Vertex(VERTEX_2D *pVtx, Vec3* pBottomPos, Vec2*pBaseSize, float difference)
{
	// 下底の頂点設置
	pVtx[3].vtx.y =
		pVtx[4].vtx.y = pBottomPos->y;

	pVtx[3].vtx.x = pBottomPos->x - pBaseSize->x;
	pVtx[4].vtx.x = pBottomPos->x + pBaseSize->x;

	// 上底の頂点設置
	pVtx[0].vtx.y =
		pVtx[1].vtx.y = pBottomPos->y - (pBaseSize->y * 2.0f);

	pVtx[0].vtx.x = pVtx[3].vtx.x + difference;
	pVtx[1].vtx.x = pVtx[4].vtx.x + difference;

	// 中央の頂点設置
	pVtx[2].vtx.y = pBottomPos->y - pBaseSize->y;

	pVtx[2].vtx.x = pBottomPos->x + (difference / 2.0f);

}