/**********************************************************************
[[タイトルエフェクトプログラム(Title_effect.cpp)]]
	作者：奥田　真規

	タイトルのエフェクトに関するプログラム
***********************************************************************/
#include "Title_effect.h"
#include "Core/MyList.h"
#include "Core/directx_Helper3D.h"

#include "Core/camera.h"			// ビルぼ
#include "Core/debugproc.h"			// デバッグ用
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define BOX_SIZEXYZ	(20.0f)
#define NUM_ZANZO	(30)
#define SCL_SUB		(1.0f / NUM_ZANZO)
#define EFFECT_RADIUS	(50.0f)
#define EFFECT_POSZ_SET	(3000.0f)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// 3Dボックスエフェクト構造体
typedef struct {

	struct {
		Vec3  rot;		// 回転
		Vec3  pos;		// 位置
		float sclXYZ;	// 大きさ（xyz共通)
		Matrix matScl;	// 変異していくスケール行列
		Matrix matbox;	// ボックスの行列
		Matrix matblur;	// ボックスの周りに出すビルボードの行列
	}obj[NUM_ZANZO];

	DWORD	idx;		// objの一番最新エフェクト番地
	Vec3    addrot;		// 毎フレーム加算する回転量
	Vec2	posXY;		// xyの位置
	float   spd;		// カメラに向かってくる速度
	float	radius;		// 中心軸からの半径
	Color   color;		// ボックスの色
}TITLE_3DEFFECT;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
Matrix* BrendMatrix(Matrix* pOutMat, Matrix* pInMat, TITLE_3DEFFECT* pEffect, DWORD idx);
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static MyList		g_list3DEffect;		// 3Dボックスエフェクトのリスト	
static Model		g_modelbox;			// ボックスのメッシュ

static struct {
	Texture tex;	// 疑似ブラー表現用のテクスチャ
	VtxBuff vtx;	// ポリゴン頂点バッファ
}g_blur;								// ブラーワーク


/*=====================================================================
3Dエフェクト設置関数(簡易版）
　拡張版関数の引数にある程度調整した乱数を指定して設置します。
戻り値：void
=====================================================================*/
void SetTitle3DEffect()
{
	// 拡張関数の引数と同じものを宣言
	float len = 300;
	float agl = 0;
	float spd = 30;
	Color col(0, 0, 1, 1);
	Vec3 paddrot(0, 0, 0);

	//len += rand() % 5;
	agl += (rand() % 1000 / 1000.0f) * (2 * D3DX_PI);
	//spd += (rand() % 1000 / 1000.0f) * (2.0f);
	paddrot.x = (rand() % 1000 / 5000.0f);
	paddrot.y = (rand() % 1000 / 5000.0f);
	paddrot.z = (rand() % 1000 / 5000.0f);

	// 色の乱数指定(青系統)
	col.r = rand() % 1000 / 1000.0f;
	col.g = rand() % 1000 / 1000.0f;
	//col.b = rand() % 1000 / 1000.0f;

	SetTitle3DEffectEx(len, agl, spd, &col, &paddrot);
}

/*=====================================================================
3Dエフェクト設置関数(拡張版）
戻り値：void
引数：
float	len,		中心軸からの距離
float	agl,		中心軸の回転角度
float	spd,		カメラに向かってくる速度
Color*	col,		エフェクトの色
Vec3*	paddrot		毎フレーム毎にエフェクト自体を回転させる量
=====================================================================*/
void SetTitle3DEffectEx(float len, float agl, float spd, Color *col, Vec3* paddrot)
{
	// リストオブジェクトの確保
	TITLE_3DEFFECT* new_pt = (TITLE_3DEFFECT*)MyListCreateObjectBottom(g_list3DEffect);
	Vec3 rot(0, 0, 0);
	float posZ = EFFECT_POSZ_SET;

	// 代入箇所はそのまま代入
	new_pt->addrot	= *paddrot;
	new_pt->color	= *col;
	new_pt->spd		= spd;
	new_pt->radius	= len;

	// 円状にあるXY位置の算出
	new_pt->posXY.x = -sinf(agl)*len;
	new_pt->posXY.y = cosf(agl)*len;

	// 一番最初のボックス設置番地を０にするために調整する
	new_pt->idx = NUM_ZANZO - 1;

	// 残像数分データを算出する
	for (int i = 0; i < NUM_ZANZO; i++, posZ -= spd, rot += *paddrot)
	{
		new_pt->obj[i].pos.x		= new_pt->posXY.x;
		new_pt->obj[i].pos.y		= new_pt->posXY.y;
		new_pt->obj[i].pos.z		= posZ;

		new_pt->obj[i].sclXYZ	= 1.0f - ((NUM_ZANZO - i)*SCL_SUB);
		new_pt->obj[i].rot		= rot;

		// スケール行列
		GetMatrix(&new_pt->obj[i].matScl, &Vec3(0, 0, 0), &Vec3(0, 0, 0), 
			&Vec3(new_pt->obj[i].sclXYZ, new_pt->obj[i].sclXYZ, new_pt->obj[i].sclXYZ));

		// ボックスの行列(スケール成分を除く）
		GetMatrix(&new_pt->obj[i].matbox, &Vec3(0, 0, 0),
			&new_pt->obj[i].rot);

		// 疑似ブラーの行列(スケール成分を除く)->かなり遠いので単位行列を代入
		GetMatrix(&new_pt->obj[i].matblur, &new_pt->obj[i].pos);
	}
}


/*=====================================================================
タイトルエフェクト更新関数
引数：float  エフェクトを回転させる
=====================================================================*/
void UpdateTitleEffect(float rot)
{
	TITLE_3DEFFECT* work_pt = NULL;
	int				cnt_effect = 0;

	// エフェクトの巡回
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect,(void**)&work_pt))
	{
		DWORD keep_idx = work_pt->idx;	// 一番最新の番地をキープ

		// 全要素数分スケール値を減少させる
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			work_pt->obj[i].sclXYZ -= SCL_SUB;

			// スケール行列
			GetMatrix(&work_pt->obj[i].matScl, &Vec3(0, 0, 0), &Vec3(0, 0, 0),
				&Vec3(work_pt->obj[i].sclXYZ, work_pt->obj[i].sclXYZ, work_pt->obj[i].sclXYZ));
		}

		work_pt->idx = (work_pt->idx + 1) % NUM_ZANZO;	// 更新番地の変更

		work_pt->obj[work_pt->idx].rot		= work_pt->obj[keep_idx].rot + work_pt->addrot;
		work_pt->obj[work_pt->idx].sclXYZ	= 1.0f;

		work_pt->obj[work_pt->idx].pos.x	= work_pt->posXY.x;
		work_pt->obj[work_pt->idx].pos.y	= work_pt->posXY.y;
		work_pt->obj[work_pt->idx].pos.z	= work_pt->obj[keep_idx].pos.z - work_pt->spd;

		if (work_pt->obj[(work_pt->idx + 1) % NUM_ZANZO].pos.z < GetCamera()->pos.z)
		{	// 最後尾のz値が視点のZ値よりも手前にある場合は削除
			MyListDeleteObject(g_list3DEffect, (void**)&work_pt);
			continue;
		}

		if (rot != 0.0f)
		{
			Vec2 keep_pos = work_pt->posXY;

			work_pt->posXY.x = (cosf(rot)*keep_pos.x) - (sinf(rot)*keep_pos.y);
			work_pt->posXY.y = (sinf(rot)*keep_pos.x) + (cosf(rot)*keep_pos.y);
		}

		// スケール行列
		GetMatrix(&work_pt->obj[work_pt->idx].matScl);

		// ボックスの行列(スケール成分を除く）
		GetMatrix(&work_pt->obj[work_pt->idx].matbox, &Vec3(0, 0, 0),
			&work_pt->obj[work_pt->idx].rot);

		cnt_effect++;
	}

	PrintDebugProc("総エフェクト数:%d * %d = %d", cnt_effect, NUM_ZANZO, cnt_effect*NUM_ZANZO);
}

/*=====================================================================
タイトルエフェクト描画関数
=====================================================================*/
void DrawTitleEffect()
{
	DWORD			d3drs_fog, d3drs_mode,d3drs_start, d3drs_end, d3drs_col;
	D3DMATERIAL9	matDef;
	TITLE_3DEFFECT* work_pt = NULL;
	Matrix			Mat;				// 演算保管用行列
	D3DDEVICE;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 現在のフォグ状態を取得
	pDevice->GetRenderState(D3DRS_FOGENABLE,	&d3drs_fog);
	pDevice->GetRenderState(D3DRS_FOGCOLOR,		&d3drs_col);
	pDevice->GetRenderState(D3DRS_FOGTABLEMODE, &d3drs_mode);
	pDevice->GetRenderState(D3DRS_FOGSTART,		&d3drs_start);
	pDevice->GetRenderState(D3DRS_FOGEND,		&d3drs_end);

	// フォグ状態の設定
	const float End = EFFECT_POSZ_SET - (NUM_ZANZO * 30);
	const float Start	= End / 3.0f;

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, Color(0, 0, 0, 1.0f));
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&Start);
	pDevice->SetRenderState(D3DRS_FOGEND,	*(DWORD*)&End);


	// エフェクトの巡回
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect, (void**)&work_pt))
	{
		// 一番最新のビルボード行列の算出(スケールを除く)
		GetBillboardMatrix(&work_pt->obj[work_pt->idx].matblur,
			&work_pt->obj[work_pt->idx].pos);


		for (int i = 0; i < NUM_ZANZO; i++)
		{
			g_modelbox->pMaterial->MatD3D.Diffuse = work_pt->color;

			// ボックスの描画
			//DrawModelWithOtherMatrix(g_modelbox, &( work_pt->obj[i].matbox * work_pt->obj[i].matScl ));
			DrawModelWithOtherMatrix(g_modelbox, BrendMatrix(&Mat, &work_pt->obj[i].matbox, work_pt, i));

			// 疑似ブラーの描画(スケール行列を掛け合わせた行列を使用）
//			Draw3DVertexBuffer(g_blur.tex, g_blur.vtx, &(work_pt->obj[i].matblur * work_pt->obj[i].matScl));
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// フォグ状態を戻す
	pDevice->SetRenderState(D3DRS_FOGENABLE,	d3drs_fog);
	pDevice->SetRenderState(D3DRS_FOGCOLOR,		d3drs_col);
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, d3drs_mode);
	pDevice->SetRenderState(D3DRS_FOGSTART,		d3drs_start);
	pDevice->SetRenderState(D3DRS_FOGEND,		d3drs_end);

}

/*=====================================================================
タイトルエフェクト初期化関数
=====================================================================*/
void InitTitleEffect(bool isFirstInit)
{
	if (isFirstInit == true)
	{
		D3DDEVICE;
		
		// リストの作成
		g_list3DEffect = MyListCreate(sizeof(TITLE_3DEFFECT));

		// ボックスの作成
		g_modelbox = CreateModel("data/MODEL/box.x");

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/blur.png", &g_blur.tex);

		// テクスチャ用ポリゴンの作成
		g_blur.vtx = Create3DPolygon(&Vec2(BOX_SIZEXYZ, BOX_SIZEXYZ));
	}


}

/*=====================================================================
タイトルエフェクト終了化関数
=====================================================================*/
void UninitTitleEffect(bool isLastUninit)
{

	MyListDeleteObjectAll(g_list3DEffect);	// エフェクト全消し

	if (isLastUninit == true)
	{
		// データの開放
		SAFE_RELEASE(g_blur.vtx)
		SAFE_RELEASE(g_blur.tex)
		DeleteModel(&g_modelbox);
		MyListDelete(&g_list3DEffect);
	}
}

/*=====================================================================
行列合成関数(cpp_func)
戻り値：第1引数と同じ
引数：
Matrix* pOutMat,			：演算用行列
Matrix* pInMat,				：被合成用行列
TITLE_3DEFFECT* pEffect,	：合成要素を格納したエフェクト構造体
DWORD idx					：上記引数の残基番地
=====================================================================*/
Matrix* BrendMatrix(Matrix* pOutMat, Matrix* pInMat, TITLE_3DEFFECT* pEffect, DWORD idx)
{
	*pOutMat = *pInMat * (pEffect->obj[idx].matScl);		// スケール行列をかける

	// オフセットもスケーリングされるので入れなおす
	pOutMat->_41 = pEffect->obj[idx].pos.x;
	pOutMat->_42 = pEffect->obj[idx].pos.y;
	pOutMat->_43 = pEffect->obj[idx].pos.z;

	return pOutMat;
}