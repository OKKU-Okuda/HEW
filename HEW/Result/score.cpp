/**********************************************************************
[[リザルトスコアプログラム(Result/Score.cpp)]]
	作者：奥田　真規

	リザルトのスコアに関するプログラム
***********************************************************************/
#include "../Core/main.h"

#include "score.h"


#include "../Phase/Phase_Result.h"
#include "../GameTackle/UI.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define MAX_KETA		(10)		// 最大桁数
#define START_POSY		(475)		// 開始位置
#define START_POSX		(SCREEN_CENTER_X+100)

#define NUMSIZE_X		(45.0f)
#define NUMSIZE_Y		(90.0f)
#define NUM_ITV_X		(15.f)
#define NUM_ITV_Y		(40.f)

#define ADD_RATE		(0.03f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// テクスチャ列挙
enum SCORE_TEXTURE {
	STEX_BASE,// 総スコア以外のスコア表示用枠
	STEX_SCORE,			// スコア
	STEX_SUBNUM,		// サブスコア
	STEX_MAINNUM,		// メインスコア

	MAX_TEXTURE
};

// サブスコア列挙
enum TYPE_SCORE {
	S_COIN,
	S_DISTANCE,
	S_SCORE,
	MAX_SCORE
};
//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num); // 桁数取得
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static Texture g_Tex[MAX_TEXTURE];
static VERTEX_2D g_BaseScoreVtx[NUM_VERTEX];		// 総スコア以外のスコア表示用枠
static VERTEX_2D g_ScoreStrVtx[NUM_VERTEX];			// すこあ！って書いた得るやつ

static VERTEX_2D g_Score[MAX_SCORE][MAX_KETA][NUM_VERTEX];	// スコアの頂点
static DWORD g_showScore[MAX_SCORE];	// 表示スコア
static DWORD g_numKeta[MAX_SCORE];		// 表示桁数

static float g_rateEx = 0.f;		// 総スコア表示の進捗
/*=====================================================================
更新関数
=====================================================================*/
void UpdateScore()
{
	if (GetResultRate() >= 1.f)
	{
		g_rateEx += ADD_RATE;
		if(g_rateEx>1.f)
		{
			g_rateEx = 1.f;
		}
	}

	g_showScore[S_COIN] = (DWORD)(GetUI()->coin * GetResultRate());
	g_showScore[S_DISTANCE] = (DWORD)(GetUI()->distance * GetResultRate());
	g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))) * g_rateEx);


	for (DWORD i = 0; i < MAX_SCORE; i++)
	{// 全スコアの頂点設置
		DWORD score = g_showScore[i];
		g_numKeta[i] = GetKeta(g_showScore[i]);

		for (DWORD j = 0; j < g_numKeta[i]; j++, score /= 10)
		{
			// テクスチャの設置
			Set2DTexPos(g_Score[i][j], 10, 1, score % 10, 0);

			if (i == S_SCORE)
			{
				Set2DVertexColor(g_Score[i][j], D3DXCOLOR(1, 1, 1, g_rateEx));
			}
		}
	}

	Set2DVertexColor(g_ScoreStrVtx, D3DXCOLOR(1, 1, 1, g_rateEx));

}

/*=====================================================================
描画関数
=====================================================================*/
void DrawScore()
{
	Draw2DVertex(g_Tex[STEX_BASE], g_BaseScoreVtx);
	Draw2DVertex(g_Tex[STEX_SCORE], g_ScoreStrVtx);

	for (DWORD i = 0; i < MAX_SCORE-1; i++)
	{
		for (DWORD j = 0; j < g_numKeta[i]; j++)
		{
			Draw2DVertex(g_Tex[STEX_SUBNUM], g_Score[i][j]);
		}
	}

	for (DWORD j = 0; j < g_numKeta[S_SCORE]; j++)
	{
		Draw2DVertex(g_Tex[STEX_MAINNUM], g_Score[S_SCORE][j]);
	}

}

/*=====================================================================
初期化関数
=====================================================================*/
void InitScore(int type)
{
	if (type == 0)
	{
		D3DDEVICE;

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RESULT.png", &g_Tex[STEX_BASE]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &g_Tex[STEX_SUBNUM]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/NUM.png", &g_Tex[STEX_MAINNUM]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score.png", &g_Tex[STEX_SCORE]);

		MakeNormal2DVertex(0, g_BaseScoreVtx, &Vec3(SCREEN_WIDTH*0.70f*0.5f, SCREEN_CENTER_Y + 70, 0.f),
			&Vec2(SCREEN_WIDTH*0.70f*0.5f, SCREEN_HEIGHT*0.25f));

		MakeNormal2DVertex(0, g_ScoreStrVtx, &Vec3(SCREEN_WIDTH*0.125f, SCREEN_CENTER_Y -200, 0.f),
			&Vec2(SCREEN_WIDTH*0.15f, SCREEN_HEIGHT*0.2f));

		for (int i = 0; i < MAX_SCORE; i++)
		{
			float POSY = START_POSY - ((NUMSIZE_Y + NUM_ITV_Y) * i);

			if (i == S_SCORE)
			{
				POSY -= 50.0f;
			}

			for (int j = 0; j < MAX_KETA; j++)
			{
				const float POSX = START_POSX - ((NUMSIZE_X + NUM_ITV_X) * j);

				MakeNormal2DVertex(0,g_Score[i][j], &Vec3(POSX, POSY, 0.f),
					&Vec2(NUMSIZE_X, NUMSIZE_Y));
			}
		}
	}

	for (int i = 0; i < MAX_SCORE; i++)
	{// ゼロクリア
		g_showScore[i] = 0;
	}
	g_rateEx = 0.f;
}

/*=====================================================================
終了関数
=====================================================================*/
void UninitScore()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		SAFE_RELEASE(g_Tex[i]);
	}
}

/*=====================================================================
桁数取得関数関数
=====================================================================*/
DWORD GetKeta(DWORD num)
{
	DWORD ans = 0ul;
	while (num != 0)
	{
		num /= 10;
		ans++;
	}
	return ans;
}