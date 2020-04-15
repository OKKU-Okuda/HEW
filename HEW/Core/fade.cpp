//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"
#include "sound.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.02f)		// フェード係数

#define FADE_TEXPLAYER	("data/TEXTURE/Fade_Player.png")
#define FADE_TEXCITY	("data/TEXTURE/Fade_City000.png")
#define FADE_TEXCITY1	("data/TEXTURE/Fade_City001.png")
#define FADE_TEXGREEN	("data/TEXTURE/Fade_Green.png")
#define FADE_TEXSKY		("data/TEXTURE/Fade_Sky.png")

enum FADE_OBJECT_DEFAULT
{
	DEFAULT_PLAYER,
	DEFAULT_CITY,
	DEFAULT_CITY1,
	DEFAULT_GREEN,
	DEFAULT_SKY,

	MAX_DEFAULTOBJECT
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
//void SetColor(D3DCOLOR col);

static void UpdateAnimDefault();
static void DrawAnimDefault();

static void SetFadeVertexColor(VERTEX_2D *vtx_data, Color nColor);
static void MakeFadeVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetFadeVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static NormalFunc	g_UpdateAnimFunc;			// アニメーション更新関数
static NormalFunc	g_DrawAnimFunc;				// アニメーション描画関数
static PHASE_FUNC	g_NextFunc;					// 次フェーズへの関数群
static FADE			g_fade = FADE_NONE;			// フェード状態
static FADE_ANIM	g_AnimType;					// アニメーションタイプ

static float		g_Time;						// アニメーション現在時間
												

static struct {
	Texture		Tex[MAX_DEFAULTOBJECT];
	VERTEX_2D	Vtx[MAX_DEFAULTOBJECT][NUM_VERTEX];
	MySound		SE;
}g_DefaultWk;									// デフォルトアニメーションワーク

//=============================================================================
// フェード設定関数
// 次のフェーズに飛ぶ関数
// 引数:
//		PHASE_FUNC* NextPhaseFunc,	:次のフェーズ基本関数群
//		FADE_ANIM AnimType			:アニメーションタイプ（必須でない)
//=============================================================================
void GoNextPhase(PHASE_FUNC* NextPhaseFunc,FADE_ANIM AnimType)
{
	if (g_fade != FADE_NONE)
	{	// 他のフェード起動中の場合は無視
		return;
	}

	// パラメータの初期化
	g_NextFunc = *NextPhaseFunc;
	g_fade = FADE_OUT;
	g_Time = 0.0f;
	g_AnimType = AnimType;

	// アニメーション関数の入れ替え
	switch (AnimType)
	{
	// デフォルト処理
	case FADEANIM_DEFAULT:
	default:
		MySoundPlayOnce(g_DefaultWk.SE);
		g_UpdateAnimFunc = UpdateAnimDefault;
		g_DrawAnimFunc = DrawAnimDefault;
		break;
	}
}


//=============================================================================
// デフォルトアニメーション関数群(cpp_func)
//=============================================================================
void UpdateAnimDefault()
{
	Vec3 pos[MAX_DEFAULTOBJECT];
	Color	col = Color(1, 1, 1, 1);		// 通常オブジェクトの色
	//Color	col_imp = Color(1, 1, 1, 1);	// 重要オブジェクトの色
	if (g_fade == FADE_OUT)
	{
		float rate = cosf(D3DX_PI / 2.0f - (g_Time * (D3DX_PI / 2.0f)));
		g_Time += 0.01f;
		
		pos[DEFAULT_PLAYER] = Vec3(200.0f *rate + 100, SCREEN_CENTER_Y + 100, 0);
	//	pos[DEFAULT_GREEN] = Vec3(200.0f *rate + 100, SCREEN_HEIGHT - 150, 0);
		pos[DEFAULT_CITY] = Vec3(-100.0f*rate + SCREEN_CENTER_X + 150, 250, 0);
		pos[DEFAULT_CITY1] = Vec3(-70.0f*rate + SCREEN_CENTER_X + 100, 250, 0);
		pos[DEFAULT_SKY] = Vec3(-20.0f*rate + SCREEN_CENTER_X + 100, SCREEN_CENTER_Y, 0);

	}
	else if (g_fade == FADE_IN)
	{
		float rate = sinf(D3DX_PI / 2.0f + (g_Time * (D3DX_PI / 2.0f)));
		g_Time -= 0.01f;

		pos[DEFAULT_PLAYER] = Vec3(200.0f *rate + 300, SCREEN_CENTER_Y + 100, 0);
	//	pos[DEFAULT_GREEN] = Vec3(200.0f *rate + 300, SCREEN_HEIGHT - 150, 0);
		pos[DEFAULT_CITY] = Vec3(-100.0f*rate + SCREEN_CENTER_X+ 50, 250, 0);
		pos[DEFAULT_CITY1] = Vec3(-70.0f*rate + SCREEN_CENTER_X + 30, 250, 0);
		pos[DEFAULT_SKY] = Vec3(-20.0f*rate + SCREEN_CENTER_X + 80, SCREEN_CENTER_Y, 0);

	}


	pos[DEFAULT_GREEN] = Vec3(SCREEN_CENTER_X, SCREEN_HEIGHT-150, 0.0f);
	col.a = g_Time / 0.5f;

	// 頂点設置
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_PLAYER],	&pos[DEFAULT_PLAYER],	&Vec2(275,240));
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_CITY],	&pos[DEFAULT_CITY],		&Vec2(SCREEN_CENTER_X+150,225));	// 前面都市
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_CITY1],	&pos[DEFAULT_CITY1],	&Vec2(SCREEN_CENTER_X+100,225));	// 背面都市
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_GREEN],	&pos[DEFAULT_GREEN],	&Vec2(SCREEN_CENTER_X,150));
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_SKY],		&pos[DEFAULT_SKY],		&Vec2(SCREEN_CENTER_X+100,SCREEN_CENTER_Y));

	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_PLAYER], col);
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_CITY], col);//
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_CITY1], col);//
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_GREEN], col);
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_SKY], col);// 
}

void DrawAnimDefault()
{
	D3DDEVICE;
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, 0);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, 0);	// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_SKY]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_SKY], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_CITY1]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_CITY1], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_CITY]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_CITY], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_GREEN]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_GREEN], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_PLAYER]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_PLAYER], sizeof(VERTEX_2D));

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// デフォルトアニメの初期化
	{
		// サウンド
		g_DefaultWk.SE = MySoundCreate("data/SE/laugh.wav");

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	
			FADE_TEXPLAYER, &g_DefaultWk.Tex[DEFAULT_PLAYER]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXCITY, &g_DefaultWk.Tex[DEFAULT_CITY]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXCITY1, &g_DefaultWk.Tex[DEFAULT_CITY1]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXGREEN, &g_DefaultWk.Tex[DEFAULT_GREEN]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXSKY, &g_DefaultWk.Tex[DEFAULT_SKY]);

		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_PLAYER], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_CITY], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_CITY1], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_GREEN], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_SKY], &Vec3(0, 0, 0), &Vec2(0, 0));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	// デフォアニメの開放
	for (int i = 0; i < MAX_DEFAULTOBJECT; i++)
	{
		SAFE_RELEASE(g_DefaultWk.Tex[i]);
	}

	MySoundDelete(&g_DefaultWk.SE);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	if (g_fade == FADE_NONE)return;

	g_UpdateAnimFunc();		// アニメーション更新関数

	if(g_fade == FADE_OUT&& g_Time >= 1.0f)
	{// フェードアウト処理
		// フェードイン処理に切り替え
		g_Time = 1.0f;

		GetPhase()->Uninit(false);	// 終了処理
		*GetPhase() = g_NextFunc;	// 関数群入れ替え

		InitCamera();		
		GetPhase()->Init(false);	// 新フェーズの初期化処理

		g_fade = FADE_IN;
	}
	else if(g_fade == FADE_IN&& g_Time <= 0.0f)
	{// フェードイン処理
		// フェード処理終了
		g_Time = 0.0f;

		g_fade = FADE_NONE;
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	if (g_fade == FADE_NONE)return;

	// アニメーション描画
	g_DrawAnimFunc();
}

//=============================================================================
// フェードの状態取得
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
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
void MakeFadeVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
{
	// 反射職の設定
	SetFadeVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

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
void SetFadeVertexColor(VERTEX_2D *vtx_data, Color nColor)
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
void SetFadeVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
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

