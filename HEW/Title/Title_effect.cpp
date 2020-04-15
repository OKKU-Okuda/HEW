/**********************************************************************
[[タイトルエフェクトプログラム(Title_effect.cpp)]]
	作者：奥田　真規

	タイトルのエフェクトに関するプログラム
***********************************************************************/
#include "Title_effect.h"

#include "../Core/MyList.h"
#include "../Core/directx_Helper3D.h"

#include "../Core/camera.h"			// ビルぼ
#include "../Core/debugproc.h"			// デバッグ用
#include "../Core/input.h"				// 入力
#include "../Core/fade.h"

#include "../Phase/Phase_Title.h"		// 色取得関数用
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
		DWORD  scl_idx;	// 使用スケール行列番地
		Matrix matbox;	// ボックスの行列
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
static void Title3DRightRot();
static void Title3DLeftRot();
inline Matrix* BrendMatrix(Matrix* pOutMat, Matrix* pInMat, TITLE_3DEFFECT* pEffect, DWORD idx);
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static Matrix		g_SclMatrix[NUM_ZANZO];		// スケール行列の事前演算格納用
static MyList		g_list3DEffect;				// 3Dボックスエフェクトのリスト	
static Model		g_modelbox;					// ボックスのメッシュ

static struct {
	float		addrot;				// 毎フレーム回転量
	float		spdrot;				// 回転加速度
	NormalFunc	Update;				// 回転量調整関数
}g_rot;											// 回転ワーク

/*=====================================================================
3Dエフェクト回転関数
戻り値：void
引数：bool :true:右回転
=====================================================================*/
void SetTitle3DRot(bool isRight)
{
	if (g_rot.Update != NoFunction)
	{
		return;
	}

	if (isRight)
	{
		g_rot.Update = Title3DRightRot;
	}
	else
	{
		g_rot.Update = Title3DLeftRot;
	}

	g_rot.spdrot = D3DX_PI / 2;
}

void Title3DRightRot()
{
	g_rot.spdrot += 0.06f;

	g_rot.addrot = cosf(g_rot.spdrot)*0.05f;

	if (g_rot.spdrot >= D3DX_PI * 1.5f)
	{
		g_rot.Update = NoFunction;
		g_rot.addrot = 0.0f;
	}
}

void Title3DLeftRot()
{
	g_rot.spdrot -= 0.06f;

	g_rot.addrot = cosf(g_rot.spdrot)*0.05f;

	if (g_rot.spdrot <= -D3DX_PI / 2)
	{
		g_rot.Update = NoFunction;
		g_rot.addrot = 0.0f;
	}

}

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
	Color col = *GetTitleBottonColor();
	Vec3 paddrot(0, 0, 0);

	//len += rand() % 5;
	agl += (rand() % 1000 / 1000.0f) * (2 * D3DX_PI);
	//spd += (rand() % 1000 / 1000.0f) * (2.0f);
	paddrot.x = (rand() % 1000 / 8000.0f);
	paddrot.y = (rand() % 1000 / 8000.0f);
	paddrot.z = (rand() % 1000 / 8000.0f);

	// 色の乱数指定(青系統)
	col.b += (rand() % 3000 / 5000.0f) - 0.3f;
	col.r += (rand() % 3000 / 5000.0f) - 0.3f;
	col.g += (rand() % 3000 / 5000.0f) - 0.3f;

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

		new_pt->obj[i].scl_idx = (NUM_ZANZO - 1) - i;
		new_pt->obj[i].rot		= rot;

		// ボックスの行列(スケール成分を除く）
		GetMatrix(&new_pt->obj[i].matbox, &Vec3(0, 0, 0),&new_pt->obj[i].rot);
	}
}


/*=====================================================================
タイトルエフェクト更新関数
引数：float  エフェクトを回転させる
=====================================================================*/
void UpdateTitleEffect()
{
	TITLE_3DEFFECT* work_pt = NULL;

	PrintDebugProc("回転：%f", g_rot.addrot);

	SetTitle3DEffect();
	SetTitle3DEffect();

	g_rot.Update();

	// エフェクトの巡回
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect,(void**)&work_pt))
	{
		DWORD keep_idx = work_pt->idx;	// 一番最新の番地をキープ

		// 全要素数分スケール値を減少させる
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			work_pt->obj[i].scl_idx++;
		}

		work_pt->idx = (work_pt->idx + 1) % NUM_ZANZO;	// 更新番地の変更

		if (work_pt->obj[work_pt->idx].pos.z < GetCamera()->pos.z)
		{	// 最後尾のz値が視点のZ値よりも手前にある場合は削除
			MyListDeleteObject(g_list3DEffect, (void**)&work_pt);
			continue;
		}

		if (g_rot.addrot != 0.0f)
		{	// 回転させる
			Vec2 keep_pos = work_pt->posXY;

			work_pt->posXY.x = (cosf(g_rot.addrot)*keep_pos.x) - (sinf(g_rot.addrot)*keep_pos.y);
			work_pt->posXY.y = (sinf(g_rot.addrot)*keep_pos.x) + (cosf(g_rot.addrot)*keep_pos.y);
		}

		
		work_pt->obj[work_pt->idx].rot		= work_pt->obj[keep_idx].rot + work_pt->addrot;
		work_pt->obj[work_pt->idx].scl_idx	= 0;

		work_pt->obj[work_pt->idx].pos.x	= work_pt->posXY.x;
		work_pt->obj[work_pt->idx].pos.y	= work_pt->posXY.y;
		work_pt->obj[work_pt->idx].pos.z	= work_pt->obj[keep_idx].pos.z - work_pt->spd;

		// ボックスの行列(スケール成分を除く）
		GetMatrix(&work_pt->obj[work_pt->idx].matbox, &Vec3(0, 0, 0),
			&work_pt->obj[work_pt->idx].rot);
	}

	PrintDebugProc("総エフェクト数:%d * %d = %d", g_list3DEffect->numObj, NUM_ZANZO, g_list3DEffect->numObj*NUM_ZANZO);
}

/*=====================================================================
タイトルエフェクト描画関数
=====================================================================*/
void DrawTitleEffect()
{
	DWORD			d3drs_fog, d3drs_mode, d3drs_start, d3drs_end, d3drs_col;
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
	const float End		= EFFECT_POSZ_SET - (NUM_ZANZO * 30);
	const float Start	= End / 3.0f;
	pDevice->SetRenderState(D3DRS_FOGENABLE,	TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR,		Color(0, 0, 0, 1.0f));
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART,		*(DWORD*)&Start);
	pDevice->SetRenderState(D3DRS_FOGEND,		*(DWORD*)&End);


	// エフェクトの巡回
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect, (void**)&work_pt))
	{
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			// 色適用
			g_modelbox->pMaterial->MatD3D.Diffuse = work_pt->color;

			// ボックスの描画
			DrawModelWithOtherMatrix(g_modelbox, BrendMatrix(&Mat, &work_pt->obj[i].matbox, work_pt, i));
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
		float		scl = 1.0f;

		// リストの作成
		g_list3DEffect = MyListCreate(sizeof(TITLE_3DEFFECT));

		// ボックスの作成
		g_modelbox = CreateModel("data/MODEL/box.x");

		// スケール行列の事前演算
		for (int i = 0; i < NUM_ZANZO; i++, scl -= SCL_SUB)
		{
			GetMatrix(&g_SclMatrix[i], &Vec3(0, 0, 0), &Vec3(0, 0, 0), &Vec3(scl, scl, scl));
		}
	}

	g_rot.addrot = 0.0f;
	g_rot.Update = NoFunction;
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
	*pOutMat = *pInMat * g_SclMatrix[pEffect->obj[idx].scl_idx];		// スケール行列をかける

	// オフセットもスケーリングされるので入れなおす
	pOutMat->_41 = pEffect->obj[idx].pos.x;
	pOutMat->_42 = pEffect->obj[idx].pos.y;
	pOutMat->_43 = pEffect->obj[idx].pos.z;

	return pOutMat;
}