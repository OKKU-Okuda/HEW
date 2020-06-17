/**********************************************************************
[[地面生成プログラム(ground_mesh.cpp)]]
	作者：奥田　真規

	プレイヤーが乗らないフィールド生成に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "ground_mesh.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define INDEX_TYPE				WORD
#define NUM_GROUNDVERTEX		(50)

#define NUM_GROUNDINDEX			(2 * (NUM_GROUNDVERTEX * NUM_GROUNDVERTEX - NUM_GROUNDVERTEX + 2))

#define NUM_GROUNDAROUND		(2)
#define POOL_GROUND				((NUM_GROUNDAROUND*2)*(NUM_GROUNDAROUND*2))
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

typedef INDEX_TYPE* IndexPtr;
typedef VERTEX_3D* VertexPtr;

typedef struct {
	VtxBuff		Vtx;
	Matrix		WldMat;
}GROUND_MESH;


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static void CreateGroundIndexBuffer();			// 地面インデックス生成関数


//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static IdxBuff g_IdxGroundMesh;
static GROUND_MESH g_Ground[POOL_GROUND];



void CreateGroundVertexBuffer()
{

}

/*=====================================================================
インデックス設定プログラム
=====================================================================*/
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