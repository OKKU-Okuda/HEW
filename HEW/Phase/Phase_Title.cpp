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

#include "../Core/directx_Helper3D.h"
#include "Phase_Title.h"	
#include "Phase_GameTackle1.h"


// タイトル仕様
// タイトルに進めるボタンを3つほど用意
// 設定、退出、結果、ゲーム開始
// 背景は動かしたい
// カーソルをモンハンワールドっぽく
// タイトルロゴをダイナミックに動かしてしゅつげんさせたい
// 後ろはビルボードで動かす
// 


//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

// プレイヤーの基本立ち位置
#define PLAYER_POS	(&Vec3(40.0f, 0.0f, -125.0f))
#define PLAYER_ROT	(&Vec3(0, 0.5f, 0))

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

static void SetTitleVertexColor(VERTEX_2D *vtx_data, Color nColor);
static void MakeTitleVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetTitleVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_Sound;

static struct {
	Model		model;		// プレイヤーモデル
	float		sclYrot;	// 大きくする演出の際の絶対値サイン関数
}g_Player;							// プレイヤーワーク

static struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Texture		tex;
}g_Botton[MAX_TITLEBOTTOM];			// ボタンワーク

static struct {
	VtxBuff		pvtx;	// 頂点バッファ
	Texture		tex;	// 地面テクスチャ
	Matrix		Mat;	// 回転行列
}g_Field;							// フィールドワーク

/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("タイトルフェーズ");

	// 次のフェーズに行く
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// タックル１
		GoNextPhase(GetPhaseGameTackle1Func());
	}
	
	// プレイヤーの縦に大きくなる演出
	{
		float sclY = 1.0f;

		g_Player.sclYrot += 0.079f;
		sclY += fabsf(sinf(g_Player.sclYrot)) * 0.07f;

		GetMatrix(&g_Player.model->WldMtx, PLAYER_POS, PLAYER_ROT, &Vec3(1.0f, sclY, 1.0f));// プレイヤー立ち位置

		
	}
	//g_mdlPlayer->WldMtx._44 -= 0.01f;
	Vec4 vc(g_Player.model->WldMtx.m[3]);
	PrintDebugProc("プレイヤー位置:%vec4", vc);

	PrintDebugProc("オフセット座標変更↑↓←→");
	//g_modelCity->WldMtx = g_mdlPlayer->WldMtx;
}

/*=====================================================================
Title描画関数
=====================================================================*/
void DrawTitle()
{
	D3DDEVICE;

	Draw3DVertexBuffer(g_Field.tex, g_Field.pvtx, &g_Field.Mat);

	DrawModel(g_Player.model);


	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{	// タイトルボタンの描画
		pDevice->SetTexture(0, g_Botton[i].tex);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Botton[i].vtx, sizeof(VERTEX_2D));
	}





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

		g_Sound = MySoundCreate("data/BGM/Title.wav");

		g_Player.model = CreateModel("data/MODEL/Player.x");

		// 地面関係
		g_Field.pvtx = Create3DPolygon(&Vec2(400.0f, 400.0f));
		GetMatrix(&g_Field.Mat, &Vec3(0, 0, -200.0f), &Vec3(D3DX_PI/2, 0, 0));
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/grass.png", &g_Field.tex);

	}
	MySoundSetMasterVolume(0.4f);
	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// カメラ
	GetCamera()->pos = Vec3(0.0f, 50.0f, -200.0f);	
	GetCamera()->at = Vec3(0, 0.0f, 0);

	g_Player.sclYrot = 0.0f;

	MySoundPlayEternal(g_Sound);	// 永遠再生


	// ボタンの頂点の設置
	MakeTitleVertex(0, g_Botton[BOTTON_STRAT].vtx,		&Vec3(SCREEN_CENTER_X/2, 300, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_RANKING].vtx,	&Vec3(SCREEN_CENTER_X/2, 400, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_CONFIG].vtx,		&Vec3(SCREEN_CENTER_X/2, 500, 0), &Vec2(175, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_EXIT].vtx,		&Vec3(SCREEN_CENTER_X/2, 600, 0), &Vec2(150, 45));

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

	MySoundStop(g_Sound);	// 停止


	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	リソース開放処理
		//---------------------------------------------------------------------
		DeleteModel(&g_Player.model);
		MySoundDeleteAuto(&g_Sound);// 増やしたものも一気に開放

		// ボタンテスクチャの開放
		for (int i = 0; i < MAX_TITLEBOTTOM; i++)
		{
			SAFE_RELEASE(g_Botton[i].tex)
		}
	}

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

