/**********************************************************************
[[Bottonsプログラム(Bottons.cpp)]]
	作者：奥田　真規

	Bottonsに関するプログラム
***********************************************************************/
#include "Bottons.h"	
#include "select.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define BASE_ALPHA		(0.45f)
#define ADD_ALPHA		(0.05f)

#define SIZE_BOTTONSTART		(Vec2(200, 45))
#define SIZE_BOTTONRANKING		(Vec2(200, 45))
#define SIZE_BOTTONCONFIG		(Vec2(175, 45))
#define SIZE_BOTTONEXIT			(Vec2(150, 45))

#define POS_BOTTONSTART		(Vec3(SCREEN_CENTER_X/1.5f, 350, 0))
#define POS_BOTTONRANKING	(Vec3(SCREEN_CENTER_X/1.5f, 450, 0))
#define POS_BOTTONCONFIG	(Vec3(SCREEN_CENTER_X/1.5f, 550, 0))
#define POS_BOTTONEXIT		(Vec3(SCREEN_CENTER_X/1.5f, 650, 0))

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


typedef struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Color		col_theme;			// ボタンのtheme色
	Vec3		pos;				// ロゴの位置
	Vec2		size;				// ロゴサイズ
	Texture		tex;
	float		col_argb;			// 色の全体値調整用変数				
}BOTTON;


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

// 基本関数群
static void InitBottons(bool isFirst);
static void UninitBottons(bool isEnd);
static void UpdateBottons();
static void DrawBottons();

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static OBJ_FUNC g_Func = { InitBottons,UninitBottons,UpdateBottons,DrawBottons };

static BOTTON	g_Botton[MAX_TITLESELECT];			// ボタンワーク


/*=====================================================================
Bottons更新関数
=====================================================================*/
void UpdateBottons()
{
	for (int idx = 0; idx < MAX_TITLESELECT; idx++)
	{
		float scl;	// ボタン拡大倍率

		if (g_Botton[idx].col_argb < BASE_ALPHA)
		{	// 基本α値以下であれば一律で加算していく
			g_Botton[idx].col_argb += ADD_ALPHA;
		}
		else if (idx == GetSelect())
		{	// 基本α値以上で選択されている場合は完全表示するために加算していく
			g_Botton[idx].col_argb += ADD_ALPHA;

			if (g_Botton[idx].col_argb > 1.0f)
			{
				g_Botton[idx].col_argb = 1.0f;
			}
		}
		else
		{	// 選択されていない場合は半透明表示の為、基本α値に戻す
			g_Botton[idx].col_argb -= ADD_ALPHA;

			if (g_Botton[idx].col_argb < BASE_ALPHA)
			{
				g_Botton[idx].col_argb = BASE_ALPHA;
			}
		}

		// col_argbを参考にサイズを変更
		scl = (1.0f - BASE_ALPHA / 2) + (g_Botton[idx].col_argb / 2);

		Set2DVertexColor(g_Botton[idx].vtx,
			Color(g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb));
		Set2DVerTex(g_Botton[idx].vtx, &g_Botton[idx].pos, &(g_Botton[idx].size*scl));
	}


}

/*=====================================================================
Bottons描画関数
=====================================================================*/
void DrawBottons()
{
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{	// タイトルボタンの描画
		Draw2DVertex(g_Botton[i].tex, g_Botton[i].vtx);
	}
}

/*=====================================================================
Bottons初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitBottons(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// ボタンテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Start.png",		&g_Botton[SELECT_STRAT].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Ranking.png",	&g_Botton[SELECT_RANKING].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Config.png",		&g_Botton[SELECT_CONFIG].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Exit.png",		&g_Botton[SELECT_EXIT].tex);

		g_Botton[SELECT_STRAT].pos		= POS_BOTTONSTART;
		g_Botton[SELECT_RANKING].pos	= POS_BOTTONRANKING;
		g_Botton[SELECT_CONFIG].pos		= POS_BOTTONCONFIG;
		g_Botton[SELECT_EXIT].pos		= POS_BOTTONEXIT;

		g_Botton[SELECT_STRAT].size		= SIZE_BOTTONSTART;
		g_Botton[SELECT_RANKING].size	= SIZE_BOTTONRANKING;
		g_Botton[SELECT_CONFIG].size	= SIZE_BOTTONCONFIG;
		g_Botton[SELECT_EXIT].size		= SIZE_BOTTONEXIT;

		g_Botton[SELECT_STRAT].col_theme	= { 1.0f, 0.5f, 0.0f, 1.0f };
		g_Botton[SELECT_RANKING].col_theme	= { 1.0f, 1.0f, 0.0f, 1.0f };
		g_Botton[SELECT_CONFIG].col_theme	= { 1.0f, 1.0f, 0.5f, 1.0f };
		g_Botton[SELECT_EXIT].col_theme		= { 0.5f, 0.5f, 0.5f, 1.0f };

		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

		// ボタンの頂点の設置
	MakeNormal2DVertex(0, g_Botton[SELECT_STRAT].vtx,	&POS_BOTTONSTART,	&SIZE_BOTTONSTART);
	MakeNormal2DVertex(0, g_Botton[SELECT_RANKING].vtx, &POS_BOTTONRANKING, &SIZE_BOTTONRANKING);
	MakeNormal2DVertex(0, g_Botton[SELECT_CONFIG].vtx,	&POS_BOTTONCONFIG,	&SIZE_BOTTONCONFIG);
	MakeNormal2DVertex(0, g_Botton[SELECT_EXIT].vtx,	&POS_BOTTONEXIT,	&SIZE_BOTTONEXIT);
	
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{
		Set2DVertexColor(g_Botton[i].vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
		g_Botton[i].col_argb = 0.0f;
	}
}

/*=====================================================================
Bottons終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitBottons(bool isEnd)
{
	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------


	if (isEnd == true)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	// ボタンテスクチャの開放
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{
		SAFE_RELEASE(g_Botton[i].tex)
	}
}

/*=====================================================================
Titleボタンtheme色取得関数
=====================================================================*/
Color* GetTitleBottonColor()
{
	return &g_Botton[GetSelect()].col_theme;
}

/*=====================================================================
Titleボタン位置取得関数
=====================================================================*/
Vec3* GetTitleBottonPosition()
{
	return &g_Botton[GetSelect()].pos;
}

/*=====================================================================
Bottons基本関数群取得関数
=====================================================================*/
OBJ_FUNC* GetBottonsFunc()
{
	return &g_Func;
}
