/**********************************************************************
[[リザルトスコアプログラム(Result/Score.cpp)]]
	作者：奥田　真規

	リザルトのスコアに関するプログラム
***********************************************************************/
#include "../Core/main.h"

#include "score.h"


#include "../Phase/Phase_Result.h"
#include "../GameTackle/UI.h"
#include "../GameTackle/bonusscore.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define MAX_KETA		(10)		// 最大桁数
#define START_POSY		(560)		// 開始位置
#define START_POSX		(SCREEN_CENTER_X+140)

#define NUMSIZE_X		(40.0f)
#define NUMSIZE_Y		(80.0f)
#define NUM_ITV_X		(15.f)
#define NUM_ITV_Y		(28.f)

#define ADD_TOTALSCORERATE		(0.03f)
#define ADD_BONUSSCORERATE		(0.02f)

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

// スコア列挙
enum TYPE_SCORE {
	S_BONUS,
	S_COIN,
	S_DISTANCE,
	S_SCORE,
	MAX_SCORE
};

// スコアアニメーション列挙
enum ANIM_TYPE {
	SANIM_SUBSCORE,
	SANIM_MAINSCORE,
	SANIM_ADDBONUS,

	MAX_SCOREANIM
};

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num); // 桁数取得
static void Update001SubScore();
static void Update002MainScore();
static void Update003AddBonusScore();

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static Texture g_Tex[MAX_TEXTURE];
static VERTEX_2D g_BaseScoreVtx[NUM_VERTEX];		// 総スコア以外のスコア表示用枠
static VERTEX_2D g_ScoreStrVtx[NUM_VERTEX];			// すこあ！って書いた得るやつ

static VERTEX_2D g_Score[MAX_SCORE][MAX_KETA][NUM_VERTEX];	// スコアの頂点
static DWORD g_showScore[MAX_SCORE];	// 表示スコア
static DWORD g_numKeta[MAX_SCORE];		// 表示桁数
static DWORD g_TotalScore;				// 結果的なスコア

static ANIM_TYPE g_AnimType;		// 表示スコアの遷移アニメーションタイプ
static float g_rateEx = 0.f;		// スコア表示関連の進捗
static void(*g_UpdateAnim[MAX_SCOREANIM])(void);	// スコアごとの更新処理

static MySound g_seScoreDeside;

// サブｽｺア表示フェーズ
void Update001SubScore()
{
	g_showScore[S_COIN] = (DWORD)(GetUI()->coin * GetResultRate());
	g_showScore[S_DISTANCE] = (DWORD)(GetUI()->distance * GetResultRate());

	if (GetResultRate() >= 1.f)
	{
		g_rateEx = 0.f;
		g_AnimType = (ANIM_TYPE)(g_AnimType + 1);
	
		// 念のため
		g_showScore[S_COIN] = (DWORD)(GetUI()->coin);
		g_showScore[S_DISTANCE] = (DWORD)(GetUI()->distance);
	}
}

// メインスコア表示フェーズ
void Update002MainScore()
{
	g_rateEx += ADD_TOTALSCORERATE;


	g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))) * g_rateEx);
	Set2DVertexColor(g_ScoreStrVtx, D3DXCOLOR(1, 1, 1, g_rateEx));

	for (DWORD j = 0; j < MAX_KETA; j++)
	{
		Set2DVertexColor(g_Score[S_SCORE][j], D3DXCOLOR(1, 1, 1, g_rateEx));
	}

	if (g_rateEx >= 1.f)
	{
		g_rateEx = 0.f;
		g_AnimType = (ANIM_TYPE)(g_AnimType + 1);

		// 念のため1.fで再計算
		g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))));
		Set2DVertexColor(g_ScoreStrVtx, D3DXCOLOR(1, 1, 1, 1.f));

		for (DWORD j = 0; j < MAX_KETA; j++)
		{
			Set2DVertexColor(g_Score[S_SCORE][j], D3DXCOLOR(1, 1, 1, 1.f));
		}

	}
}

// 加えてボーナススコアを加算していくフェーズ
void Update003AddBonusScore()
{
	g_rateEx += ADD_BONUSSCORERATE;

	g_showScore[S_BONUS] = (DWORD)(GetBonusScore() * g_rateEx);
	g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))))
		+ (DWORD)(GetBonusScore() * g_rateEx);


	if (g_rateEx >= 1.f)
	{
		g_rateEx = 0.f;
		g_AnimType = (ANIM_TYPE)(g_AnimType + 1);

		// 念のため1.fで再計算
		g_showScore[S_BONUS] = (DWORD)(GetBonusScore());
		g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))))
			+ (DWORD)(GetBonusScore());

		MySoundPlayOnce(g_seScoreDeside);		// 決定音を鳴らす
	}
}

/*=====================================================================
更新関数
=====================================================================*/
void UpdateScore()
{

	if (g_AnimType < MAX_SCOREANIM)
	{// スコアの個別更新
		g_UpdateAnim[g_AnimType]();
	}


	for (DWORD i = 0; i < MAX_SCORE; i++)
	{// 全スコアの頂点設置
		DWORD score = g_showScore[i];
		g_numKeta[i] = GetKeta(g_showScore[i]);

		for (DWORD j = 0; j < g_numKeta[i]; j++, score /= 10)
		{
			// テクスチャの設置
			Set2DTexPos(g_Score[i][j], 10, 1, score % 10, 0);
		}
	}

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

		MakeNormal2DVertex(0, g_BaseScoreVtx, &Vec3(SCREEN_WIDTH*0.70f*0.5f, SCREEN_CENTER_Y + 130, 0.f),
			&Vec2(SCREEN_WIDTH*0.70f*0.5f, SCREEN_HEIGHT*0.3f));

		MakeNormal2DVertex(0, g_ScoreStrVtx, &Vec3(SCREEN_WIDTH*0.125f, SCREEN_CENTER_Y -180, 0.f),
			&Vec2(SCREEN_WIDTH*0.15f, SCREEN_HEIGHT*0.17f));

		g_seScoreDeside = MySoundCreate("data/SE/ResultDeside.wav");
		MySoundSetVolume(g_seScoreDeside, 3.f);

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

	for (DWORD j = 0; j < MAX_KETA; j++)
	{// メインスコアは初期では表示しない
		Set2DVertexColor(g_Score[S_SCORE][j], D3DXCOLOR(1, 1, 1, 0.f));
	}

	g_rateEx = 0.f;
	g_AnimType = SANIM_SUBSCORE;
	g_TotalScore = (DWORD)(GetUI()->distance * (1.f + (GetUI()->coin*0.01f))) + (DWORD)(GetBonusScore());

	Set2DVertexColor(g_ScoreStrVtx, D3DXCOLOR(1, 1, 1, 0.f));

	g_UpdateAnim[SANIM_SUBSCORE] = Update001SubScore;
	g_UpdateAnim[SANIM_MAINSCORE] = Update002MainScore;
	g_UpdateAnim[SANIM_ADDBONUS] = Update003AddBonusScore;

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

	MySoundDelete(&g_seScoreDeside);
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

	if (ans == 0ul)
	{// 0桁の場合は０って表示したいので1桁とする
		ans++;
	}

	return ans;
}