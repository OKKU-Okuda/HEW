/**********************************************************************
[[地面生成プログラム(ground_mesh.cpp)]]
	作者：奥田　真規

	プレイヤーが乗らないフィールド生成に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "field.h"
#include "ground_mesh.h"
#include "player.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define GROUNDSIZE				(FIELDCHIP_WIDTH * 3.5)

#define GROUND_POSY				(-75.f)
//#define FOR(i,exit)				for(int (i) = 0; (i) < (exit); (i)++)
#define RATE_POS				(0.0002f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
//static void CreateGroundIndexBuffer();			// 地面インデックス生成関数
static void ChangeTexturePos();

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static VtxBuff	g_Vtx;
static Matrix	g_Mat;		// 行列
static Texture	g_Tex;

/*=====================================================================
地面更新関数
=====================================================================*/
void UpdateGround()
{
	Vec3 pos_ground = *GetPlayerPos();
	pos_ground.y = GROUND_POSY;
	GetMatrix(&g_Mat, &pos_ground, &Vec3(D3DX_PI / 2, 0, 0));


	// 位置でテクスチャの変更
	ChangeTexturePos();

}

/*=====================================================================
地面描画関数
=====================================================================*/
void DrawGround()
{
	Draw3DVertexBuffer(g_Tex, g_Vtx, &g_Mat);
}

/*=====================================================================
地面初期化関数
=====================================================================*/
void InitGround()
{
	D3DDEVICE;
	g_Vtx = Create3DPolygon(&Vec2(GROUNDSIZE, GROUNDSIZE));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field005.jpg", &g_Tex);

}


/*=====================================================================
地面テクスチャ座標変更関数
=====================================================================*/
void ChangeTexturePos()
{
	VERTEX_3D* pVtx = NULL;
	Vec2	  TexPos;

	TexPos.x = GetPlayerPos()->x * RATE_POS;
	TexPos.y = -GetPlayerPos()->z * RATE_POS;

	g_Vtx->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex.x =
		pVtx[2].tex.x = TexPos.x - 0.5f;
	pVtx[1].tex.x =
		pVtx[3].tex.x = TexPos.x + 0.5f;

	pVtx[0].tex.y =
		pVtx[1].tex.y = TexPos.y - 0.5f;
	pVtx[2].tex.y =
		pVtx[3].tex.y = TexPos.y + 0.5f;

	g_Vtx->Unlock();
}

#if 0
void CreateGroundIndexBuffer()
{

	D3DDEVICE;
	IndexPtr	pIdx = NULL;
	DWORD		idx_array = 0u;

	// オブジェクトのインデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(INDEX_TYPE) * NUM_GROUNDINDEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_IdxGroundMesh,					// インデックスバッファインターフェースへのポインタ
		NULL);								// NULLに設定


	g_IdxGroundMesh->Lock(0, 0,(void**)&pIdx, 0);

	for (int i = 0; i < NUM_GROUNDVERTEX - 1; i++)
	{
		if (i != 0)
		{
			pIdx[idx_array++] = i;
		}

		for (int j = i; j <= (NUM_GROUNDVERTEX - 1) * 10 + i + 1;)
		{
			pIdx[idx_array++] = j;


			if (j % 10 == i % 10)
			{
				j++;
			}
			else
			{
				j += 10;
			}
		}

		if (i != NUM_GROUNDVERTEX - 2)
		{
			pIdx[idx_array++] = (NUM_GROUNDVERTEX - 1) * 10 + i + 1;
		}
	}

	g_IdxGroundMesh->Unlock();
}
#endif