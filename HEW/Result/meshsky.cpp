//=============================================================================
//
// メッシュ空の処理 [meshsky.cpp]
// Author : 
//
//=============================================================================
#include "../Core/main.h"
#include "../Core/input.h"
#include "../Core/camera.h"
#include "meshsky.h"

#include "../GameTackle/player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/night.png"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_SKY		(5.0f)							// 移動速度
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.001f)				// 回転速度

#define	SKY_HEIGHT_RATE		(2.0f)		// 空ドームの高さ係数

#define	TEX_COUNT_LOOP		(1)			// テクスチャの繰り返し回数

#define	MAX_MESH_SKY		(1)			// 空の総数

#define PLAYERROT_RATE		(0.2f)		// プレイヤーロット基準でのロット補正計数
//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;		// インデックスバッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTop;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffTop;		// インデックスバッファへのポインタ

	D3DXMATRIX mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 pos;						// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rot;						// ポリゴンの回転角

	int nNumBlockH;							// ブロック数(水平)
	int nNumBlockV;							// ブロック数(垂直)
	int nNumVertex;							// 総頂点数	
	int nNumVertexIndex;					// 総インデックス数
	int nNumPolygon;						// 総ポリゴン数
	float fRadius;							// 半径
	int nTexType;							// テクスチャタイプ

	bool bReverse;							// 反転ON/OFF
} MESH_SKY;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSky = NULL;	// テクスチャ読み込み場所
MESH_SKY g_aMeshSky[2] = {};				// 空ワーク
int g_nNumMeshSky = 0;						// 空の数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockH, int nNumBlockV,
						float fRadius, float fRotY, int nTexType, bool bReverse)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_SKY *pMesh;

	if(g_nNumMeshSky >= MAX_MESH_SKY)
	{
		return E_FAIL;
	}

	pMesh = &g_aMeshSky[g_nNumMeshSky];
	g_nNumMeshSky++;

	pMesh->bReverse = bReverse;

	// ポリゴン表示位置の中心座標を設定
	pMesh->pos = pos;
	pMesh->rot = rot;
	pMesh->fRadius = fRadius;

	// テクスチャの読み込み
	if(nTexType == -1)
	{
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
									TEXTURE_FILENAME,	// ファイルの名前
									&g_pTextureSky);	// 読み込むメモリー
	}
	pMesh->nTexType = nTexType;

	// ブロック数の設定
	pMesh->nNumBlockH = nNumBlockH;
	pMesh->nNumBlockV = nNumBlockV;

	// 頂点数の設定
	pMesh->nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);

	// インデックス数の設定
	pMesh->nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);

	// ポリゴン数の設定
	pMesh->nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);
	
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pVtxBuff,					// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (pMesh->nNumBlockH + 1),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pVtxBuffTop,					// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pIdxBuff,					// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (pMesh->nNumBlockH + 2),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&pMesh->pIdxBuffTop,				// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / pMesh->nNumBlockH;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (pMesh->nNumBlockV + 1);
		const float WIDTH = 1.0f / pMesh->nNumBlockH;
		const float HEIGHT = 1.0f / pMesh->nNumBlockV;
		float fLengthXZ;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntV = 0; nCntV < (pMesh->nNumBlockV + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * pMesh->fRadius;

			for(int nCntH = 0; nCntH < (pMesh->nNumBlockH + 1); nCntH++, pVtx++)
			{
				// 頂点座標の設定
				pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				if(pMesh->bReverse)
				{
					pVtx->vtx.y = -sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_HEIGHT_RATE;
				}
				else
				{
					pVtx->vtx.y = sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_HEIGHT_RATE;
				}
				pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// 法線の設定
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラーの設定
				if(pMesh->bReverse)
				{
					pVtx->diffuse = D3DXCOLOR(0.0625f, 0.0625f, 0.375f, 0.5f);
				}
				else
				{
					pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				// テクスチャ座標の設定
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if(nCntV == pMesh->nNumBlockV)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// 頂点データをアンロックする
		pMesh->pVtxBuff->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pVtxBuffTop->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx->vtx.x = 0.0f;
		if(pMesh->bReverse)
		{
			pVtx->vtx.y = -sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_HEIGHT_RATE;
		}
		else
		{
			pVtx->vtx.y = sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_HEIGHT_RATE;
		}
		pVtx->vtx.z = 0.0f;

		// 法線の設定
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		if(pMesh->bReverse)
		{
			pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
		}
		else
		{
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標の設定
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius;
		for(int nCntH = 0; nCntH < pMesh->nNumBlockH; nCntH++, pVtx++)
		{
			// 頂点座標の設定
			pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			if(pMesh->bReverse)
			{
				pVtx->vtx.y = -sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_HEIGHT_RATE;
			}
			else
			{
				pVtx->vtx.y = sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_HEIGHT_RATE;
			}
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// 法線の設定
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			if(pMesh->bReverse)
			{
				pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
			}
			else
			{
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// テクスチャ座標の設定
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// 頂点データをアンロックする
		pMesh->pVtxBuffTop->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for(int nCntV = 0; nCntV < pMesh->nNumBlockV; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (pMesh->nNumBlockV - 1))
			{
				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1);
				pIdx++;
			}
		}

		// インデックスデータをアンロックする
		pMesh->pIdxBuff->Unlock();

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pIdxBuffTop->Lock(0, 0, (void**)&pIdx, 0);

		for(int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// インデックスデータをアンロックする
		pMesh->pIdxBuffTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshSky(void)
{
	MESH_SKY *pMesh;

	for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++)
	{
		pMesh = &g_aMeshSky[nCntSky];

		if(pMesh->pVtxBuff)
		{// 頂点バッファの開放
			pMesh->pVtxBuff->Release();
			pMesh->pVtxBuff = NULL;
		}

		if(pMesh->pIdxBuff)
		{// インデックスバッファの開放
			pMesh->pIdxBuff->Release();
			pMesh->pIdxBuff = NULL;
		}

		if(pMesh->pVtxBuffTop)
		{// 頂点バッファの開放
			pMesh->pVtxBuffTop->Release();
			pMesh->pVtxBuffTop = NULL;
		}

		if(pMesh->pIdxBuffTop)
		{// インデックスバッファの開放
			pMesh->pIdxBuffTop->Release();
			pMesh->pIdxBuffTop = NULL;
		}
	}

	if(g_pTextureSky)
	{// テクスチャの開放
		g_pTextureSky->Release();
		g_pTextureSky = NULL;
	}

	g_nNumMeshSky = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshSky(void)
{
	MESH_SKY *pMesh;

	for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++)
	{
		pMesh = &g_aMeshSky[nCntSky];

		pMesh->rot.y = GetPlayerRot()->y * PLAYERROT_RATE;

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	DWORD dwSettingLighting, dwSettingCullmode;
	MESH_SKY *pMesh;

	pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pMesh = &g_aMeshSky[0];
	for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++, pMesh++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y,
													pMesh->rot.x,
													pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x,
												pMesh->pos.y,
												pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		if(pMesh->bReverse)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// 表面をカリング
		}
		else
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, pMesh->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(pMesh->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		if(pMesh->nTexType == -1)
		{
			pDevice->SetTexture(0, g_pTextureSky);
		}
		else
		{
			pDevice->SetTexture(0, 0);
		}

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);


		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, pMesh->pVtxBuffTop, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(g_aMeshSky[nCntSky].pIdxBuffTop);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (pMesh->nNumBlockH + 2), 0, pMesh->nNumBlockH);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング

		pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

