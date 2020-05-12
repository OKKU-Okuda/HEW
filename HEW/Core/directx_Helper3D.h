/**********************************************************************
[[DirectXヘルプヘッダー(directxHelper_3D.h)]]
	作者：奥田　真規

	directxHelper_3D.cppに関するヘッダー
***********************************************************************/
#pragma once
//#include <d3d9.h>
//#include <d3dx9.h>

#include "main.h"


// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

// ベクトルタイプ列挙型
enum VECTER_TYPE {
	POS,
	ROT,
	SCL,
	MAX_VECTER_NAME
};

// 保管用データの作成
typedef struct INTERPOLATION_STATE
{
	D3DXVECTOR3 State[MAX_VECTER_NAME];
}INTERPOLATION_STATE;

// Xメッシュ構造体
typedef struct Xmodel{
	LPD3DXMESH		pMesh;			// メッシュ
	D3DXMATRIX		WldMtx;			// ワールドマトリクス
	DWORD			nNummat;		// マテリアル
	D3DXMATERIAL	*pMaterial;		// マテリアル配列
}Xmodel;
typedef Xmodel*			Model;				// Xファイルのでーた	

typedef LPD3DXMESH		Mesh;				// メッシュデータ

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------
/*=====================================================================
3D頂点バッファ描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VtxBuff pVtx:頂点バッファ
=====================================================================*/
void Draw3DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx, D3DXMATRIX *Mat);

/*=====================================================================
	ポリゴンさくせい関数
戻り値:LPDIRECT3DVERTEXBUFFER9
引数　:
D3DXVECTOR2 size,	:サイズ
int X_parts,			：(テクスチャ分割)
int Y_parts,
int nowX,
int nowY
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Create3DPolygon(D3DXVECTOR2 *size, int X_parts = 1, int Y_parts = 1, int nowX = 0, int nowY = 0);

/*=====================================================================
	ボックスメッシュ作成関数
戻り値:LPD3DXMESH	:メッシュ
引数　:
D3DXVECTOR3 *size, ：ボックスのサイズ
D3DXVECTOR3 *pos	：中央のローカル位置
=====================================================================*/
LPD3DXMESH Create3DBoxMesh(D3DXVECTOR3 *size, D3DXVECTOR3 *pos);

/*=====================================================================
	ビルボード行列取得関数
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:出力行列へのアドレス
CAMERA *cam,		:カメラデータ
D3DXVECTOR3 *pos,	:(以下3つは必須でない)
D3DXVECTOR3 *rot,
D3DXVECTOR3 *scl
=====================================================================*/
D3DXMATRIX *GetBillboardMatrix(D3DXMATRIX *pOut,D3DXVECTOR3 *pos = &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3 *rot = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 *scl = &D3DXVECTOR3(1.0f, 1.0f, 1.0f));

/*=====================================================================
	逆行列取得関数
逆行列を計算して求める関数
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:位置系のでーたは事前にいれること
D3DXMATRIX *pOri	:逆行列にしたい行列
=====================================================================*/
D3DXMATRIX *GetInverseMatrix(D3DXMATRIX *pOut, D3DXMATRIX *pOri);

/*=====================================================================
	行列取得関数
行列を計算して求める関数
	※ 計算はSCL,ROT,POSの順で行う ※
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:結果を保管するアドレス
D3DXVECTOR3 *pos,	:位置(必須ではない)
D3DXVECTOR3 *rot,	:回転(必須ではない)
D3DXVECTOR3 *scl	:スケール(必須ではない)
bool isReset		:リセットの有無（必須ではない　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　）
=====================================================================*/
D3DXMATRIX *GetMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 *rot = &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3 *scl = &D3DXVECTOR3(1.0f, 1.0f, 1.0f), bool isReset = true);

/*=====================================================================
3D頂点変更関数
	戻り値：void
	引数：
VERTEX_3D *vtx_data,	:4つの頂点配列
 D3DXVECTOR2 *size		:サイズ
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
=====================================================================*/
void Set3DVertex(VERTEX_3D *vtx_data, D3DXVECTOR2 *size, int X_parts = 1, int Y_parts = 1, int nowX = 0, int nowY = 0);

/*=====================================================================
テクスチャ3D座標設置関数
	戻り値：void
	引数：
VERTEX_3D *vtx_data,	:4つの頂点配列
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
=====================================================================*/
void Set3DTexPos(VERTEX_3D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);


/*=====================================================================
モデル描画関数
	モデルに付与されているテクスチャ月で描画される
戻り値:void
引数　:Xmodel* pModel :モデル格納構造体
=====================================================================*/
void DrawModel(Xmodel* pModel);

/*=====================================================================
別行列使用モデル描画関数
戻り値:void
引数　:
Xmodel* pModel		:モデル格納構造体
D3DXMATRIX* pMat	：行列
=====================================================================*/
void DrawModelWithOtherMatrix(Xmodel* pModel, D3DXMATRIX* pMat);

/*=====================================================================
モデル作成関数
	xfileモデルを読み込み、関連のテクスチャも読み込む関数
	※ xfileで使用するテクスチャは同一ディレクトリ内に置くこと ※
戻り値:void
引数　:const char * filename:ファイルパス
=====================================================================*/
Xmodel *CreateModel(const char * filename);

/*=====================================================================
モデル削除関数
戻り値:void
引数　:Xmodel** ppModel　:モデル構造体の場所を保管している場所
=====================================================================*/
void DeleteModel(Xmodel** ppModel);
