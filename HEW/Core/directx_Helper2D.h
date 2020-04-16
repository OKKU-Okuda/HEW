/**********************************************************************
[[DirectXヘルプヘッダー(directxHelper_2D.h)]]
	作者：奥田　真規

	directxHelper_2D.cppに関するヘッダー
***********************************************************************/
#pragma once
#include "main.h"

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


// 画面いっぱいに頂点バッファを宣言するﾏｸﾛ
// 戻り値: VtxBuff 
#define MakeVertexBufferFullScreen											\
Make2DVertexBuffer(0, &D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f),	\
						&D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y));


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;


// 位置、半径、サイズの保管構造体
// ポリゴンの中央から回転させる際に使用
typedef struct {
	D3DXVECTOR3	Pos;		// 中心座標
	D3DXVECTOR2 Size;		// 中心から外に向かうサイズ（つまり全体サイズの1/2）
	float		Radius;		// 中心から座標までの距離
	float		BaseAngle;	// x軸と中心から各頂点に伸びる線が交わる角度(中心回転なので共通)
	float		Rot;		// 回転角
	VERTEX_2D	vtx[4];		// 頂点データ
}OBJ_2DDATA;

// 上記回転しない版
typedef struct {
	D3DXVECTOR3 pos;		// 中心座標
	D3DXVECTOR2 size;		// 中心から外に向かうサイズ（つまり全体サイズの1/2）
	VERTEX_2D	vtx[4];		// 頂点データ
}OBJ_BOX2DDATA;

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
頂点バッファ描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VtxBuff pVtx:頂点バッファ
=====================================================================*/
void Draw2DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx);

/*=====================================================================
頂点描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VERTEX_2D* pVtx　:頂点
=====================================================================*/
void Draw2DVertex(LPDIRECT3DTEXTURE9 pTex, VERTEX_2D* pVtx);

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
void MakeNormal2DVertex(int num, VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size);

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
LPDIRECT3DVERTEXBUFFER9 Make2DVertexBuffer(int num, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size, OBJ_2DDATA *data = NULL, float Rot = 0.0f);


/*=====================================================================
カラー設置関数(ノーマル)
	戻り値：void
	引数：
	VERTEX_2D *vtx_data		:頂点でーた（４つの限定）
 D3DCOLOR nColor			: 新しいカラー
=====================================================================*/
void Set2DVertexColor(VERTEX_2D *vtx_data, D3DCOLOR nColor);

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
void Set2DTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);

/*=====================================================================
頂点の設置を行う関数
	戻り値：void
	引数：VERTEX_2D *vtx_data:頂点(4つの頂点限定)
	D3DXVECTOR3 *Pos,	:中心座標
	D3DXVECTOR2 *Size	:中心からのサイズ
=====================================================================*/
void Set2DVerTex(VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size);

// -- OBJ_2DDATA関連---
/*=====================================================================
2dオブジェクトデータ設置関数(初期化時に使う用)
	戻り値：void
	引数：
		OBJ_2DDATA *data,
		D3DXVECTOR3 *pos,	:中心座標
		D3DXVECTOR2 *size,	:中心からのサイズ
		float rot =0.0f		:半径
=====================================================================*/
void SetObj2Ddata(OBJ_2DDATA *data, D3DXVECTOR3 *pos, D3DXVECTOR2 *size, float rot = 0.0f);

/*=====================================================================
半径基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float nRadius:新しい半径
=====================================================================*/
void Change2DSizeByRadius(OBJ_2DDATA *data, float nRadius);

/*=====================================================================
倍率基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float Times:新しい倍率
=====================================================================*/
void Change2DSizeByTimes(OBJ_2DDATA *data, float Times);

/*=====================================================================
サイズ基準での大きさの変更関数
	戻り値：void
	引数：D3DX_OBJ_DATA*
			float x:ヨコノ長さ
			float y:縦の長さ
=====================================================================*/
void Change2DSizeBySize(OBJ_2DDATA *data, float x, float y);

/*=====================================================================
頂点の設置を超簡単に行う関数
	戻り値：void
	引数：VERTEX_2D *vtx_data:頂点(4つの頂点限定)
	D3DX_OBJ_DATA *data
=====================================================================*/
void SuperSet2DVerTex(VERTEX_2D *vtx_data, OBJ_2DDATA *data);