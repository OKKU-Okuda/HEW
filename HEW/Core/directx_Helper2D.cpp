/**********************************************************************
[[DirectX2Dヘルププログラム(directxHelper_2D.cpp)]]
	作者：奥田　真規

	DirectXのプログラム簡略化(2D)に関するプログラム
***********************************************************************/
#include "directx_Helper2D.h"
#include <tchar.h>
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

/*=====================================================================
頂点バッファ描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VtxBuff pVtx:頂点バッファ
=====================================================================*/
void Draw2DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx)
{
	D3DDEVICE(pDevice);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, pTex);
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_2D));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	return;
}

/*=====================================================================
頂点描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VERTEX_2D* pVtx　:頂点
=====================================================================*/
void Draw2DVertex(LPDIRECT3DTEXTURE9 pTex, VERTEX_2D* pVtx)
{
	D3DDEVICE(pDevice);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, pTex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, pVtx, sizeof(VERTEX_2D));
	return;

}

/*=====================================================================
普遍的に頂点設置関数(ノーマル)
	戻り値：void
	引数：
	int num					=0:obj全体にテクスチャを張るように座標を設定する
							=1:しない
	VERTEX_2D *vtx_data		:頂点でーた（４つの限定）
	 D3DXVECTOR3 *Pos,		:中心座標
	 D3DXVECTOR2 *Size		:中心からのサイズ
=====================================================================*/
void MakeNormal2DVertex(int num, VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size)
{
	// 反射職の設定
	Set2DVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

	// パースペクティブコレクト
	(vtx_data)->rhw =
		(vtx_data + 1)->rhw =
		(vtx_data + 2)->rhw =
		(vtx_data + 3)->rhw = 1.0f;

	// 頂点座標
	(vtx_data)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 1)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 2)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y + Size->y, 0.0f);
	(vtx_data + 3)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y + Size->y, 0.0f);

	// 第一引数で０指定があれば
	if (num == 0)
	{
		// テクスチャ座標
		(vtx_data)->tex = D3DXVECTOR2(0.0f, 0.0f);
		(vtx_data + 1)->tex = D3DXVECTOR2(1.0f, 0.0f);
		(vtx_data + 2)->tex = D3DXVECTOR2(0.0f, 1.0f);
		(vtx_data + 3)->tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

/*=====================================================================
普遍的に頂点バッファ作成関数(ノーマル)
	戻り値：LPDIRECT3DVERTEXBUFFER9
	引数：
	int num					=0:obj全体にテクスチャを張るように座標を設定する
							=1:しない
	 D3DXVECTOR3 *Pos,		:中心座標
	 D3DXVECTOR2 *Size		:中心からのサイズ
	 OBJ_2DDATA *data		:今後回転させたい場合はこれを使う（必須でない）
	 float		Rot			:回転角(必須でない)
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Make2DVertexBuffer(int num, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size, OBJ_2DDATA *data,float Rot)
{
	D3DDEVICE(pDevice);
	LPDIRECT3DVERTEXBUFFER9 buff = NULL;
	VERTEX_2D				*pvtx = 0;
	HRESULT					hr;

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&buff,
		NULL);
	if (FAILED(hr))return NULL;
	buff->Lock(0, 0, (void**)&pvtx, 0);				// ロック

	MakeNormal2DVertex(num, pvtx, Pos, Size);		// 頂点の設置

	if (data)
	{
		SetObj2Ddata(data, Pos, Size, Rot);			// 2Dデータの設置
		if (Rot != 0.0f)
		{
			SuperSet2DVerTex(pvtx, data);			// 頂点の設置
		}
	}

	buff->Unlock();									// ロック解除

	return buff;
}

/*=====================================================================
カラー設置関数(ノーマル)
	戻り値：void
	引数：
	VERTEX_2D *vtx_data		:頂点でーた（４つの限定）
 D3DCOLOR nColor			: 新しいカラー
=====================================================================*/
void Set2DVertexColor(VERTEX_2D *vtx_data, D3DCOLOR nColor)
{
	// 反射職の設定
	(vtx_data)->diffuse =
		(vtx_data + 1)->diffuse =
		(vtx_data + 2)->diffuse =
		(vtx_data + 3)->diffuse = nColor;

}

/*=====================================================================
テクスチャ2D座標設置関数
	戻り値：void
	引数：
VERTEX_2D *vtx_data,	:4つの頂点配列
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
=====================================================================*/
void Set2DTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY)
{
	D3DXVECTOR2 size = D3DXVECTOR2(1.0f / X_parts, 1.0f / Y_parts);

	(vtx_data)->tex.x =
		(vtx_data + 2)->tex.x = size.x*nowX;
	(vtx_data + 1)->tex.x =
		(vtx_data + 3)->tex.x = size.x*(nowX + 1);

	(vtx_data)->tex.y =
		(vtx_data + 1)->tex.y = size.y*nowY;
	(vtx_data + 2)->tex.y =
		(vtx_data + 3)->tex.y = size.y*(nowY + 1);

}

/*=====================================================================
2dオブジェクトデータ設置関数
	戻り値：void
	引数：
		OBJ_2DDATA *data,	
		D3DXVECTOR3 *pos,	:中心座標
		D3DXVECTOR2 *size,	:中心からのサイズ
		float rot =0.0f		:半径
=====================================================================*/
void SetObj2Ddata(OBJ_2DDATA *data,D3DXVECTOR3 *pos,D3DXVECTOR2 *size,float rot)
{
	data->Pos = *pos;
	data->Rot = rot;
	Change2DSizeBySize(data, size->x, size->y);

	SuperSet2DVerTex(data->vtx, data);
}

/*=====================================================================
半径基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float nRadius:新しい半径
=====================================================================*/
void Change2DSizeByRadius(OBJ_2DDATA *data, float nRadius)
{
	if (nRadius == 0.0f)return;	// 半径がおかしい

	// そもそも元の半径がおかしい(このあとに続く割り算対策)
	if (data->Radius == 0.0f)
	{
		data->Radius = nRadius;
		data->Size.x = cosf(data->BaseAngle)*nRadius;
		data->Size.y = sinf(data->BaseAngle)*nRadius;
		return;
	}

	// 影響を受けるパラの調整
	float Times = (nRadius / data->Radius);
	data->Radius = nRadius;
	data->Size.x *= Times;
	data->Size.y *= Times;

	return;
}

/*=====================================================================
倍率基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float Times:新しい倍率
=====================================================================*/
void Change2DSizeByTimes(OBJ_2DDATA *data, float Times)
{
	if (Times == 0.0f)return;	// 倍率がおかしい

	// 影響を受けるパラの調整
	data->Radius *= Times;
	data->Size.x *= Times;
	data->Size.y *= Times;

	return;
}

/*=====================================================================
サイズ基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float x:ヨコノ長さ
			float y:縦の長さ
=====================================================================*/
void Change2DSizeBySize(OBJ_2DDATA *data, float x, float y)
{
	data->Size.x = x;
	data->Size.y = y;
	data->BaseAngle = atan2f(y, x);
	D3DXVECTOR2 temp = D3DXVECTOR2(x, y);
	data->Radius = D3DXVec2Length(&temp);
	return;
}

/*=====================================================================
頂点の設置を超簡単に行う関数
	戻り値：void
	引数：VERTEX_2D *vtx_data:頂点(4つの頂点限定)
	D3DX_OBJ_DATA *data
=====================================================================*/
void SuperSet2DVerTex(VERTEX_2D *vtx_data, OBJ_2DDATA *data)
{
	// 回転角度が0度の場合は三角関数を使用しない配置方法に切り替える０
	if (data->Rot == 0)
	{
		vtx_data->vtx.x = data->Pos.x - data->Size.x;
		vtx_data->vtx.y = data->Pos.y - data->Size.y;

		(vtx_data + 1)->vtx.x = data->Pos.x + data->Size.x;
		(vtx_data + 1)->vtx.y = data->Pos.y - data->Size.y;

		(vtx_data + 2)->vtx.x = data->Pos.x - data->Size.x;
		(vtx_data + 2)->vtx.y = data->Pos.y + data->Size.y;

		(vtx_data + 3)->vtx.x = data->Pos.x + data->Size.x;
		(vtx_data + 3)->vtx.y = data->Pos.y + data->Size.y;
	}
	else
	{
		// 頂点座標の設定
		vtx_data->vtx.x = data->Pos.x - cosf(data->BaseAngle + data->Rot) * data->Radius;
		vtx_data->vtx.y = data->Pos.y - sinf(data->BaseAngle + data->Rot) * data->Radius;

		(vtx_data + 1)->vtx.x = data->Pos.x + cosf(data->BaseAngle - data->Rot) * data->Radius;
		(vtx_data + 1)->vtx.y = data->Pos.y - sinf(data->BaseAngle - data->Rot) * data->Radius;

		(vtx_data + 2)->vtx.x = data->Pos.x - cosf(data->BaseAngle - data->Rot) * data->Radius;
		(vtx_data + 2)->vtx.y = data->Pos.y + sinf(data->BaseAngle - data->Rot) * data->Radius;

		(vtx_data + 3)->vtx.x = data->Pos.x + cosf(data->BaseAngle + data->Rot) * data->Radius;
		(vtx_data + 3)->vtx.y = data->Pos.y + sinf(data->BaseAngle + data->Rot) * data->Radius;
	}
	return;
}


/*=====================================================================
頂点の設置を行う関数
	戻り値：void
	引数：VERTEX_2D *vtx_data:頂点(4つの頂点限定)
	D3DXVECTOR3 *Pos,	:中心座標
	D3DXVECTOR2 *Size	:中心からのサイズ
=====================================================================*/
void Set2DVerTex(VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size)
{
	vtx_data->vtx.x = Pos->x - Size->x;
	vtx_data->vtx.y = Pos->y - Size->y;

	(vtx_data + 1)->vtx.x = Pos->x + Size->x;
	(vtx_data + 1)->vtx.y = Pos->y - Size->y;

	(vtx_data + 2)->vtx.x = Pos->x - Size->x;
	(vtx_data + 2)->vtx.y = Pos->y + Size->y;

	(vtx_data + 3)->vtx.x = Pos->x + Size->x;
	(vtx_data + 3)->vtx.y = Pos->y + Size->y;

}


