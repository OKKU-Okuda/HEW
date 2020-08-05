/**********************************************************************
[[ボーナススコアプログラム(bonusscore.cpp)]]
	作者：奥田　真規

	ユーザー行動をトリガーとするアチーブメント形式での加算スコアに関するプログラム
***********************************************************************/
#include "../Core/main.h"

#include "bonusscore.h"

#include "../Result/player.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define MAX_KETA		(10)
#define MAX_BONUSUI		(20)

// タイトルと説明を1組と考えた時のテクスチャ分割数
#define NUM_TEXDIV_X	(2)
#define NUM_TEXDIV_Y	(12)

#define ADDRAREBONUS	(0.2f)		// 1度のみ発動のボーナスによるスコア係数
#define ADDCOMMONBONUS	(0.05f)		// 無制限発動のボーナスによるスコア係数

#define SIZE_TITLE		Vec2(100,20)
#define SIZE_SUB		Vec2(80,16)
#define SIZE_SCORE		Vec2(10,30)
#define POS_TITLE		Vec3(100,200,0)
#define POS_SUB			Vec3(POS_TITLE.x + SIZE_TITLE.x - SIZE_SUB.x,220,0)
#define POS_SCORE		Vec3(105 + SIZE_TITLE.x + SIZE_SCORE.x,205,0)		// 最上位桁の場所
#define ITV_SCOREX		(15.0f)

#define ITV_Y			(60.0f)	// 同時に2つ以上発動した場合にy軸をずらす量

#define MOVE_BONUS		Vec3(0,-2,0)	// 移動量
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

typedef struct {
	VERTEX_2D TitleVtx[NUM_VERTEX];		// ボーナス名
	VERTEX_2D SubVtx[NUM_VERTEX];		// 説明用
	VERTEX_2D ScoreVtx[MAX_KETA][NUM_VERTEX];	// 加算スコア
	DWORD numShowKeta;							// スコアの表示桁

	Vec3 PosTitle;
	Vec3 PosSub;
	Vec3 PosScore[MAX_KETA];
	Color ColScore;		// スコア職
	float alpha;
	bool isUse;				// 使用中か
//	bool isActiveData;		// このデータはアクティブ状態か(頂点情報の設定をし直したか)
}BONUS_UI;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num);	// 桁取得
static float GetBonusScoreBuff();			// バフkeisuu取得関数
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static DWORD g_BonusScore;		// 加算のスコア
static Texture g_TexBonus;		// ボーナスについて記述のあるテクスチャ
static Texture g_TexScore;		// スコアテクスチャ
static MySound g_seGet;			// ボーナススコアゲット音

static BONUS_UI g_UI[MAX_BONUSUI];	// 表示用
static DWORD g_cntCreate;			// 1フレームの中でスコア加算イベントが何回発生したか（描画関数最後に０クリア）

static DWORD g_cntRareBonus;		// 1度きりのボーナス発動回数
static DWORD g_cntCommonBonus;		// 無制限のボーナス発動回数

static bool g_isAlreadyActiveRareBonus[START_COMMONBONUS]; // レアボーナスの発動確認flag

/* ボーナススコアの基礎値 */
static const DWORD g_scorelist[NUM_TEXDIV_X][NUM_TEXDIV_Y]=
{
	/* 以下は最大1度のみ加算されるボーナス */
	100,// コイン1枚
	150,// コイン10枚
	1000,// コイン100枚
	5000,// コイン1000枚
	75,// 1m
	100,// 10m
	400,// 100m
	2000,// 1000m
	8000,// 10km
	42195,// 42.195km
	5000,// コイン100枚 && 1000m
	20000,// コイン1000毎 && 10km
	100,// ギミック1回
	300,// ギミック10回
	2000,// ギミック50回
	10000,// 90秒生存

	/* 以下は複数回加算される可能性のあるボーナス */
	300,// コイン75枚毎(別途加算条件：コイン300枚以上)
	300,// 150m毎(別途加算条件：500m以上)
	700,// ギミック10回毎(別途加算条件：ギミック100回以上)
	1000,// レアボイスを発動
	75,// 壁GIMMICKクリア
	250,// ポールのところをスライディングで通過
	200,// コイン20毎を100m区間内で回収
	100,// タイミングよくギミックをよける
};
/*=====================================================================
ボーナススコア設定関数
=====================================================================*/
void AddBonusScore(BONUSSCORE_TYPE bst)
{
	BONUS_UI* work_pt = NULL;
	float scoreRate = GetBonusScoreBuff();			// スコアの基礎値に掛け算する値
	DWORD score;
	Vec3 OffsetPos(0, -ITV_Y * g_cntCreate, 0);
	Color ColScore;

	if (IsResultPlayer() == true)
	{// リザルトの場合は処理を行わない
		return;
	}

	if (bst >= START_COMMONBONUS)
	{// Commonボーナス
		g_cntCommonBonus++;
		ColScore = Color(1, 1, 1, 1);
	}
	else if (g_isAlreadyActiveRareBonus[bst] == false)
	{// 未発動のレアボーナス
		g_isAlreadyActiveRareBonus[bst] = true;
		g_cntRareBonus++;
		ColScore = Color(1, 0, 0, 1);
	}
	else
	{// 既に発動しているレアボーナスなので終了
		return;
	}

	// 描画用のデータ準備
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == true)
		{
			continue;
		}
		// 未使用ptの代入
		work_pt = &g_UI[i];
	}
	if (work_pt == NULL)
	{// 空きがない場合は表示しない
		return;
	}

	score = (DWORD)(g_scorelist[bst / NUM_TEXDIV_Y][bst % NUM_TEXDIV_Y] * scoreRate);
	g_BonusScore += score;

	work_pt->numShowKeta = GetKeta(score);
	work_pt->alpha = 1.f;
	work_pt->isUse = true;
	work_pt->ColScore = ColScore;

	for (DWORD i = 0; i < work_pt->numShowKeta; i++, score /= 10)
	{// スコアのテクスチャ設定
		work_pt->PosScore[i] = POS_SCORE + OffsetPos + Vec3((work_pt->numShowKeta - (i + 1))*ITV_SCOREX, 0.f, 0.f);
		Set2DTexPos(work_pt->ScoreVtx[i], 10, 1, score % 10, 0);
		Set2DVerTex(work_pt->ScoreVtx[i], &work_pt->PosScore[i], &SIZE_SCORE);
	}

	// タイトルと説明のテクスチャ座標を取得
	work_pt->PosTitle = POS_TITLE + OffsetPos;
	Set2DTexPos(work_pt->TitleVtx, NUM_TEXDIV_X * 2, NUM_TEXDIV_Y, (bst / NUM_TEXDIV_Y) * 2, bst % NUM_TEXDIV_Y);
	Set2DVerTex(work_pt->TitleVtx, &work_pt->PosTitle, &SIZE_TITLE);


	work_pt->PosSub = POS_SUB + OffsetPos;
	Set2DTexPos(work_pt->SubVtx, NUM_TEXDIV_X * 2, NUM_TEXDIV_Y, ((bst / NUM_TEXDIV_Y) * 2) + 1, bst % NUM_TEXDIV_Y);
	Set2DVerTex(work_pt->SubVtx, &work_pt->PosSub, &SIZE_SUB);


	// 今フレームのカウンタをインクリメント
	g_cntCreate++;
}

/*=====================================================================
更新関数
=====================================================================*/
void UpdateBonusScore()
{
	/*
	if (GetKeyboardTrigger(DIK_5))
	{
		AddBonusScore(BST_COIN1);
		AddBonusScore(BST_COIN2);
		AddBonusScore(BST_JUSTINTARACT);
	}*/

	// 表示UIの設定
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == false)
		{
			continue;
		}

		// タイトルの設定
		g_UI[i].PosTitle += MOVE_BONUS;

		// タイトルの位置からα値を計算
		g_UI[i].alpha = g_UI[i].PosTitle.y / POS_TITLE.y;
		Set2DVerTex(g_UI[i].TitleVtx, &g_UI[i].PosTitle, &SIZE_TITLE);
		Set2DVertexColor(g_UI[i].TitleVtx, D3DXCOLOR(1, 1, 1, g_UI[i].alpha));


		// 説明の設定
		g_UI[i].PosSub += MOVE_BONUS;
		Set2DVerTex(g_UI[i].SubVtx, &g_UI[i].PosSub, &SIZE_SUB);
		Set2DVertexColor(g_UI[i].SubVtx, D3DXCOLOR(1, 1, 1, g_UI[i].alpha));

		g_UI[i].ColScore.a = g_UI[i].alpha;
		for (DWORD j = 0; j < g_UI[i].numShowKeta; j++)
		{// スコアの設定
			g_UI[i].PosScore[j] += MOVE_BONUS;
			Set2DVerTex(g_UI[i].ScoreVtx[j], &g_UI[i].PosScore[j], &SIZE_SCORE);
			Set2DVertexColor(g_UI[i].ScoreVtx[j], g_UI[i].ColScore);
		}

		if (g_UI[i].alpha <= 0.f)
		{// α値が0以下の場合は無効にする
			g_UI[i].isUse = false;
		}
	}
}

/*=====================================================================
描画関数
=====================================================================*/
void DrawBonusScore()
{
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == false )//|| g_UI[i].isActiveData == false)
		{
			continue;
		}

		// 加算内容の描画
		Draw2DVertex(g_TexBonus, g_UI[i].TitleVtx);

		// 上の説明を描画
		Draw2DVertex(g_TexBonus, g_UI[i].SubVtx);

		// 加算スコアを描画
		for (DWORD j = 0; j < g_UI[i].numShowKeta; j++)
		{
			Draw2DVertex(g_TexScore, g_UI[i].ScoreVtx[j]);
		}
	}


	if (g_cntCreate > 0ul)
	{
		// 再生
		MySoundPlayOnce(g_seGet);
		// 1フレームでクリエイトした数を0にする
		g_cntCreate = 0ul;
	}
}

/*=====================================================================
初期化関数
=====================================================================*/
void InitBonusScore(int type)
{
	if (type == 0)
	{
		D3DDEVICE;
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BonusList.png",&g_TexBonus);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BonusScoreNumber.png", &g_TexScore);
		//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Number.png", &g_TexScore);

		for (int i = 0; i < MAX_BONUSUI; i++)
		{
			MakeNormal2DVertex(1, g_UI[i].TitleVtx, &Vec3(0, 0, 0), &Vec2(0, 0));
			MakeNormal2DVertex(1, g_UI[i].SubVtx, &Vec3(0, 0, 0), &Vec2(0, 0));
			for (int k = 0; k < MAX_KETA; k++)
			{
				MakeNormal2DVertex(1, g_UI[i].ScoreVtx[k], &Vec3(0, 0, 0), &Vec2(0, 0));
			}
		}

		g_seGet = MySoundCreate("data/SE/GetBonusScore.wav");
		MySoundSetVolume(g_seGet, 4.f);
	}

	g_BonusScore = 0ul;
	g_cntCreate = 0ul;
	g_cntRareBonus = 0ul;
	g_cntCommonBonus = 0ul;
	for (int i = 0; i < START_COMMONBONUS; i++)
	{
		g_isAlreadyActiveRareBonus[i] = false;
	}

	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		g_UI[i].isUse = false;
	}
}

/*=====================================================================
終了化関数
=====================================================================*/
void UninitBonusScore()
{
	SAFE_RELEASE(g_TexBonus);
	SAFE_RELEASE(g_TexScore);
}

/*=====================================================================
ボーナススコア取得関数
=====================================================================*/
DWORD GetBonusScore()
{
	return g_BonusScore;
}

/*=====================================================================
ボーナスフラグ取得関数
=====================================================================*/
bool IsSetRareBonus(BONUSSCORE_TYPE rareBonus)
{
	return g_isAlreadyActiveRareBonus[rareBonus];
}

/*=====================================================================
桁数取得関数関数(cpp)
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

/*=====================================================================
ボーナススコアのバフ係数取得関数(cpp)
=====================================================================*/
float GetBonusScoreBuff()
{
	float addcommonrare = (float)(g_cntCommonBonus * ADDCOMMONBONUS);
	float addraterare = (float)(g_cntRareBonus * ADDRAREBONUS);

	return 1.f + addraterare + addcommonrare;
}