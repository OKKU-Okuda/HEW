/**********************************************************************
[[フェーズ:Titleプログラム(Phase_Title.cpp)]]
	作者：奥田　真規

	フェーズ：Phase_Titleに関するプログラム
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "../Core/directx_Helper3D.h"		// 描画援助関数
#include "Phase_Title.h"				
#include "Phase_GameTackle1.h"				// 次のフェーズ
#include "../Title/Title_effect.h"			// タイトルエフェクト

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

// プレイヤーの基本立ち位置
#define PLAYER_POSFROMZ	(6000.0f)
#define PLAYER_POSTOZ	(-110.0f)
#define PLAYER_DISZ		(PLAYER_POSFROMZ - PLAYER_POSTOZ)
#define PLAYER_POSRATE	(0.05f)
#define PLAYER_ROT		(&Vec3(0, 0.3f, 0))

#define BASE_ALPHA		(0.45f)
#define ADD_ALPHA		(0.05f)

#define SIZE_SELECTEFFECT		(Vec2(350,90))

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

// タイトルランキングオブジェクト
enum TITLE_BOTTON {
	BOTTON_STRAT,		// 開始ボタン
	BOTTON_RANKING,		// ランキングボタン
	BOTTON_CONFIG,		// 設定ボタン
	BOTTON_EXIT,		// 退出ボタン

	MAX_TITLEBOTTOM		/*ボタン数*/
};


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

static void SetSelectEffect();
static void UpdateSelectEffect();
static void UpdateTitleBotton(DWORD idx);

static void SetTitleVertexColor(VERTEX_2D *vtx_data, Color nColor);
static void MakeTitleVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetTitleVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetTitleTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_soundBGM;
static MySound		g_soundSelect;		// 選択音

static struct {
	Model		model;		// プレイヤーモデル
	float		sclYrot;	// 大きくする演出の際の絶対値サイン関数
	float		posZadd;	// 基本値から加算したZ位置		
}g_Player;							// プレイヤーワーク

static struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Color		col_theme;			// ボタンのtheme色
	Vec3		pos;				// ロゴの位置
	Vec2		size;				// ロゴサイズ
	Texture		tex;
	float		col_argb;			// 色の全体値調整用変数				
}g_Botton[MAX_TITLEBOTTOM];			// ボタンワーク

static DWORD	g_Select;			// ボタン

static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ロゴ頂点
	Texture    tex;				// テクスチャ
	float	   col;				// 色
}g_Logo;				// ロゴワーク

static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ロゴ頂点
	Texture    tex;				// テクスチャ
	DWORD	   cnt_show;		// 表示カウント
	bool	   isShow;			// 表示flag
}g_Effect;

/*=====================================================================
選択時発生エフェクト設置関数(cpp_func)
=====================================================================*/
void SetSelectEffect()
{
	g_Effect.isShow = true;
	g_Effect.cnt_show = 0ul;

	// 位置の設定
	SetTitleVertex(g_Effect.vtx, &g_Botton[g_Select].pos, &SIZE_SELECTEFFECT);
	SetTitleVertexColor(g_Effect.vtx, *GetTitleBottonColor());
}

/*=====================================================================
選択時発生エフェクト更新関数(cpp_func)
=====================================================================*/
void UpdateSelectEffect()
{
	// 上下の選択
	if (GetKeyboardTrigger(DIK_UP))
	{ 
		g_Select = (g_Select - 1) % MAX_TITLEBOTTOM;
		MySoundPlayOnce(g_soundSelect);
		SetTitle3DRot(true);
		SetSelectEffect();
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_Select = (g_Select + 1) % MAX_TITLEBOTTOM;
		MySoundPlayOnce(g_soundSelect);
		SetTitle3DRot(false);
		SetSelectEffect();
	}

	if (g_Effect.isShow == false)
	{	// 非表示時スキップ
		return;
	}

	// 2フレームに1度テクスチャ座標が更新される
	SetTitleTexPos(g_Effect.vtx, 1, 7, 0, g_Effect.cnt_show++ / 2);

	if (g_Effect.cnt_show % 15 == 0)
	{	// 一定以上進んだ場合は非表示にする
		g_Effect.isShow = false;
	}
}

/*=====================================================================
タイトルボタン更新関数(cpp_func)
=====================================================================*/
void UpdateTitleBotton(DWORD idx)
{
	float scl;	// ボタン拡大倍率

	if (g_Botton[idx].col_argb < BASE_ALPHA)
	{	// 基本α値以下であれば一律で加算していく
		g_Botton[idx].col_argb += ADD_ALPHA;
	}
	else if (idx == g_Select)
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

	SetTitleVertexColor(g_Botton[idx].vtx, 
		Color(g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb));
	SetTitleVertex(g_Botton[idx].vtx, &g_Botton[idx].pos, &(g_Botton[idx].size*scl));
}

/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("タイトルフェーズ");

	// 選択ボタンによって変わる処理
	if (GetKeyboardTrigger(DIK_RETURN) && g_Botton[g_Select].col_argb > 0.75f)
	{	
		switch (g_Select)
		{
		case BOTTON_STRAT:	// タックル1フェーズに移行
			GoNextPhase(GetPhaseGameTackle1Func());
			break;

		case BOTTON_RANKING:
		case BOTTON_CONFIG:
			GoNextPhase(GetPhaseTitleFunc());
			break;

		case BOTTON_EXIT:	// ゲームの終了
			DestroyWindow(GetHandle());
			break;

		default:			// エラー出力
			MessageBox(0, 0, 0, 0);
			break;
		}
	}

	// 後ろにある四角形エフェクトの更新
	UpdateTitleEffect();

	if (GetFade() == FADE_NONE)
	{ // フェードが発生していない場合に実行
		float sclY = 1.0f;

		// プレイヤーの更新(大きさ演出と急に近づく演出)
		g_Player.sclYrot += 0.079f;
		sclY += fabsf(sinf(g_Player.sclYrot)) * 0.07f;
		g_Player.posZadd = (PLAYER_DISZ - g_Player.posZadd) * PLAYER_POSRATE + g_Player.posZadd;
		GetMatrix(&g_Player.model->WldMtx, &Vec3(0.0f, -30.0f, PLAYER_POSFROMZ - g_Player.posZadd), PLAYER_ROT, &Vec3(1.0f, sclY, 1.0f));// プレイヤー立ち位置


		if (PLAYER_DISZ - g_Player.posZadd <= 1.0f)
		{	// プレイヤーが指定位置にいる場合

			// ロゴの更新
			g_Logo.col += 0.02f;
			SetTitleVertexColor(g_Logo.vtx, Color(g_Logo.col, g_Logo.col, g_Logo.col, g_Logo.col));

			// 選択の処理とそのエフェクトの更新
			UpdateSelectEffect();

			for (int i = 0; i < MAX_TITLEBOTTOM; i++)
			{	// タイトルボタンの更新
				UpdateTitleBotton(i);
			}
		}
	}

	// プレイヤー位置の表示
	Vec4 vc(g_Player.model->WldMtx.m[3]);
	PrintDebugProc("プレイヤー位置:%vec4", vc);
}

/*=====================================================================
Title描画関数
=====================================================================*/
void DrawTitle()
{
	DWORD	d3drslightning;
	D3DDEVICE;

	// ライティングの設定
	pDevice->GetRenderState(D3DRS_LIGHTING, &d3drslightning);	
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	DrawModel(g_Player.model);

	DrawTitleEffect();				// 周囲に舞っているエフェクトの描画

	// 元のライティング状態に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);


	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{	// タイトルボタンの描画
		pDevice->SetTexture(0, g_Botton[i].tex);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Botton[i].vtx, sizeof(VERTEX_2D));
	}

	if (g_Effect.isShow)
	{	// 選択エフェクトの描画
		pDevice->SetTexture(0, g_Effect.tex);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Effect.vtx, sizeof(VERTEX_2D));
	}

	// ロゴの描画
	pDevice->SetTexture(0, g_Logo.tex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Logo.vtx, sizeof(VERTEX_2D));

}

/*=====================================================================
Title初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitTitle(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// ボタンテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Start.png",		&g_Botton[BOTTON_STRAT].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Ranking.png",	&g_Botton[BOTTON_RANKING].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Config.png",		&g_Botton[BOTTON_CONFIG].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Exit.png",		&g_Botton[BOTTON_EXIT].tex);

		// タイトルロゴのテクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleLogo_000.png", &g_Logo.tex);

		// セレクトエフェクトのテクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/select_effect.png", &g_Effect.tex);

		// BGM,SEの読み込み
		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");
		g_soundSelect	= MySoundCreate("data/SE/select.wav");
		MySoundSetVolume(g_soundSelect, 3.0f);

		// プレイヤーのモデル読み込み
		g_Player.model = CreateModel("data/MODEL/Player.x");

		g_Botton[BOTTON_STRAT].pos		= POS_BOTTONSTART;
		g_Botton[BOTTON_RANKING].pos	= POS_BOTTONRANKING;
		g_Botton[BOTTON_CONFIG].pos		= POS_BOTTONCONFIG;
		g_Botton[BOTTON_EXIT].pos		= POS_BOTTONEXIT;

		g_Botton[BOTTON_STRAT].size		= SIZE_BOTTONSTART;
		g_Botton[BOTTON_RANKING].size	= SIZE_BOTTONRANKING;
		g_Botton[BOTTON_CONFIG].size	= SIZE_BOTTONCONFIG;
		g_Botton[BOTTON_EXIT].size		= SIZE_BOTTONEXIT;

		g_Botton[BOTTON_STRAT].col_theme	= { 1.0f, 0.5f, 0.0f, 1.0f };
		g_Botton[BOTTON_RANKING].col_theme	= { 1.0f, 1.0f, 0.0f, 1.0f };
		g_Botton[BOTTON_CONFIG].col_theme	= { 1.0f, 1.0f, 0.5f, 1.0f };
		g_Botton[BOTTON_EXIT].col_theme		= { 0.5f, 0.5f, 0.5f, 1.0f };

		InitTitleEffect(true);
	}
	else
	{
		InitTitleEffect(false);
	}

	MySoundSetMasterVolume(0.1f);
	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// カメラ
#if 1
	GetCamera()->pos	= Vec3(0.0f, 0.0f, -200.0f);	
	GetCamera()->at		= Vec3(-100.0f, 0.0f, 0);
#else
	GetCamera()->pos	= Vec3(0.0f, 0.0f, 200.0f);
	GetCamera()->at		= Vec3(200.0f, 0.0f, -200.0f);
#endif

	// プレイヤーの数値入れ替え
	g_Player.sclYrot = 0.0f;
	g_Player.posZadd = 0.0f;
	GetMatrix(&g_Player.model->WldMtx, &Vec3(0.0f, 0.0f, PLAYER_POSFROMZ), PLAYER_ROT);// プレイヤー立ち位置

	MySoundPlayEternal(g_soundBGM);	// 永遠再生
	

	// ボタンの頂点の設置
	MakeTitleVertex(0, g_Botton[BOTTON_STRAT].vtx,		&POS_BOTTONSTART,	&SIZE_BOTTONSTART);
	MakeTitleVertex(0, g_Botton[BOTTON_RANKING].vtx,	&POS_BOTTONRANKING, &SIZE_BOTTONRANKING);
	MakeTitleVertex(0, g_Botton[BOTTON_CONFIG].vtx,		&POS_BOTTONCONFIG,	&SIZE_BOTTONCONFIG);
	MakeTitleVertex(0, g_Botton[BOTTON_EXIT].vtx,		&POS_BOTTONEXIT,	&SIZE_BOTTONEXIT);
	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{
		SetTitleVertexColor(g_Botton[i].vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
		g_Botton[i].col_argb = 0.0f;
	}

	// ロゴ頂点の設置
	MakeTitleVertex(0, g_Logo.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &Vec2(350, 120));
	SetTitleVertexColor(g_Logo.vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
	g_Logo.col = 0.0f;

	// エフェクト頂点の設置
	MakeTitleVertex(0, g_Effect.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &SIZE_SELECTEFFECT);
	g_Effect.cnt_show	= 0ul;
	g_Effect.isShow		= false;

	g_Select	= BOTTON_STRAT;
}

/*=====================================================================
Title終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitTitle(bool isEnd)
{

//---------------------------------------------------------------------
//	その他の終了処理
//---------------------------------------------------------------------

	MySoundStop(g_soundBGM);	// 停止
	MySoundStop(g_soundSelect);

	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	リソース開放処理
		//---------------------------------------------------------------------

		DeleteModel(&g_Player.model);

		// サウンドの開放
		MySoundDeleteAuto(&g_soundBGM);
		MySoundDeleteAuto(&g_soundSelect);

		// ボタンテスクチャの開放
		for (int i = 0; i < MAX_TITLEBOTTOM; i++)
		{
			SAFE_RELEASE(g_Botton[i].tex)
		}

		SAFE_RELEASE(g_Logo.tex);
		SAFE_RELEASE(g_Effect.tex);

		UninitTitleEffect(true);
	}
	else
	{
		UninitTitleEffect(false);
	}

}

/*=====================================================================
Titleボタンtheme色取得関数
=====================================================================*/
Color* GetTitleBottonColor()
{
	return &g_Botton[g_Select].col_theme;
}

/*=====================================================================
Title基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

/*=====================================================================
普遍的に頂点設置関数(ノーマル)(cpp_func)
	戻り値：void
	引数：
	int num					=0:obj全体にテクスチャを張るように座標を設定する
							=1:しない
	VERTEX_2D *vtx_data		:頂点でーた（４つの限定）
	 D3DXVECTOR3 *Pos,		:中心座標
	 D3DXVECTOR2 *Size		:中心からのサイズ
=====================================================================*/
void MakeTitleVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
{
	// 反射職の設定
	SetTitleVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

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
		(vtx_data)->tex = D3DXVECTOR2(0.01f, 0.01f);
		(vtx_data + 1)->tex = D3DXVECTOR2(0.99f, 0.01f);
		(vtx_data + 2)->tex = D3DXVECTOR2(0.01f, 0.99f);
		(vtx_data + 3)->tex = D3DXVECTOR2(0.99f, 0.99f);
	}

	return;
}

/*=====================================================================
カラー設置関数(ノーマル) (cpp_func)
	戻り値：void
	引数：
	VERTEX_2D *vtx_data		:頂点でーた（４つの限定）
 Color nColor			: 新しいカラー
=====================================================================*/
void SetTitleVertexColor(VERTEX_2D *vtx_data, Color nColor)
{
	// 反射職の設定
	(vtx_data)->diffuse =
		(vtx_data + 1)->diffuse =
		(vtx_data + 2)->diffuse =
		(vtx_data + 3)->diffuse = nColor;

}

/*=====================================================================
頂点の設置を行う関数(cpp_func)
	戻り値：void
	引数：VERTEX_2D *vtx_data:頂点(4つの頂点限定)
	Vec3 *Pos,	:中心座標
	Vec2 *Size	:中心からのサイズ
=====================================================================*/
void SetTitleVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
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

/*=====================================================================
テクスチャ2D座標設置関数(cpp_func)
	戻り値：void
	引数：
VERTEX_2D *vtx_data,	:4つの頂点配列
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
=====================================================================*/
void SetTitleTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY)
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
