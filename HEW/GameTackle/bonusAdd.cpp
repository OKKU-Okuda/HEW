/**********************************************************************
[[ボーナススコア加算処理判断プログラム(bonusAdd.cpp)]]
	作者：奥田　真規

	スコアの加算判断に関するプログラム
	ボーナス表示や計算等はbonusscore.cppで
	各ギミックの個別スコアは担当cppで

***********************************************************************/
#include "../Core/main.h"

#include "BonusAdd.h"

#include "player_control.h"
#include "bonusscore.h"
#include "UI.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static DWORD g_timeGameStart = 0ul;

static struct {
	float nextM;	// 次リセットするときのメートル
	int cntCoin;	// 集めたコイン
}g_CoinPerMeter;		// 区間内に何枚集めた家計

static struct {
	int next_coin;
	BONUSSCORE_TYPE next_bst;

	int next_Excoin;			// Excoinの取得ができるか
}g_CoinAchive;

static struct {
	float next_distance;
	BONUSSCORE_TYPE next_bst;

	float next_Exdistance;		// Exの取得ができるか
}g_DistanceAchive;

static struct {
	int cntGimmick;
	int nextGimmick;
	BONUSSCORE_TYPE next_bst;

	int next_ExGimmick;// Exの取得ができるか
}g_GimmickAchive;
//static const int g_coin
/*=====================================================================
更新関数
=====================================================================*/
void UpdateBonusAdd()
{
	if (GetPlayerMoveState() != MSTATE_RUNNING)
	{
		return;
	}

	if (IsSetRareBonus(BST_LIVETIME1)==false && timeGetTime() - g_timeGameStart > 1000 * 90)
	{// 時間でアチーブ
		AddBonusScore(BST_LIVETIME1);
	}
}

/*=====================================================================
リセット関数
=====================================================================*/
void ResetBonusAdd()
{
	g_timeGameStart = 0;
	g_CoinPerMeter.cntCoin = 0;
	g_CoinPerMeter.nextM = 100.0f;

	g_CoinAchive.next_bst = BST_COIN1;
	g_CoinAchive.next_coin = 1;
	g_CoinAchive.next_Excoin = 300;

	g_DistanceAchive.next_bst = BST_RUN1;
	g_DistanceAchive.next_distance = 1;
	g_DistanceAchive.next_Exdistance = 500;

	g_GimmickAchive.cntGimmick = 0;
	g_GimmickAchive.nextGimmick = 1;
	g_GimmickAchive.next_bst = BST_GIMMICK1;
	g_GimmickAchive.next_ExGimmick = 100;
}

/*=====================================================================
GIMMICKクリア時にReferenceを行う関数
=====================================================================*/
void AddGimmickPassCount()
{
	g_GimmickAchive.cntGimmick++;

	if (g_GimmickAchive.cntGimmick == g_GimmickAchive.nextGimmick)
	{// ﾚｱアチーブ
		AddBonusScore(g_GimmickAchive.next_bst);

		if (g_GimmickAchive.nextGimmick < 50)
		{
			g_GimmickAchive.next_bst = (BONUSSCORE_TYPE)(g_GimmickAchive.next_bst + 1);
		
			if (g_GimmickAchive.nextGimmick == 1)g_GimmickAchive.nextGimmick = 10;
			else if (g_GimmickAchive.nextGimmick == 10)g_GimmickAchive.nextGimmick = 50;
		}
	}

	if (g_GimmickAchive.cntGimmick == g_GimmickAchive.next_ExGimmick)
	{
		AddBonusScore(BST_GIMMICKEX);
		g_GimmickAchive.next_ExGimmick += 10;
	}
}

/*=====================================================================
コイン取得時にReferenceを行う関数
=====================================================================*/
void ReferenceCoin()
{
	const int coin = GetUI()->coin;

	if (++g_CoinPerMeter.cntCoin == 20)
	{// コインper距離のアチーブ
		AddBonusScore(BST_COINPERRUN);
	}

	if (coin == g_CoinAchive.next_coin)
	{//　コインのﾚｱアチーブ
		AddBonusScore(g_CoinAchive.next_bst);

		if (g_CoinAchive.next_coin == 100 && IsSetRareBonus(BST_RUN4))
		{// 距離との複合アチーブ
			AddBonusScore(BST_COIN3RUN4);
		}
		else if (g_CoinAchive.next_coin == 1000 && IsSetRareBonus(BST_RUN5))
		{// 同上強化版
			AddBonusScore(BST_COIN4RUN5);
		}

		if (g_CoinAchive.next_coin != 1000)
		{// これ以上のアチーブが存在するか
			g_CoinAchive.next_bst = (BONUSSCORE_TYPE)(g_CoinAchive.next_bst + 1);
			g_CoinAchive.next_coin *= 10;
		}
	}

	if (coin == g_CoinAchive.next_Excoin)
	{// コインexアチーブ
		AddBonusScore(BST_COINEX);
		g_CoinAchive.next_Excoin += 75;
	}
}

/*=====================================================================
メートル更新時にReferenceを行う関数
=====================================================================*/
void ReferenceDistance()
{
	const float distance = (const float)GetUI()->distance;

	if (distance >= g_CoinPerMeter.nextM)
	{// コインper距離での獲得系ステータスリセット
		g_CoinPerMeter.nextM += 100.0f;
		g_CoinPerMeter.cntCoin = 0;
	}

	if (distance == g_DistanceAchive.next_distance)
	{//　コインのﾚｱアチーブ
		AddBonusScore(g_DistanceAchive.next_bst);

		if (g_DistanceAchive.next_distance == 1000.f && IsSetRareBonus(BST_COIN3))
		{// 距離との複合アチーブ
			AddBonusScore(BST_COIN3RUN4);
		}
		else if (g_DistanceAchive.next_distance == 10000.f && IsSetRareBonus(BST_COIN4))
		{// 同上強化版
			AddBonusScore(BST_COIN4RUN5);
		}

		if (g_DistanceAchive.next_distance < 10000.f)
		{// これ以上のアチーブが存在するか
			g_DistanceAchive.next_bst = (BONUSSCORE_TYPE)(g_DistanceAchive.next_bst + 1);
			g_DistanceAchive.next_distance *= 10.f;
		}
		else if(g_DistanceAchive.next_distance == 10000.f)
		{// 42.195km用
			g_DistanceAchive.next_bst = (BONUSSCORE_TYPE)(g_DistanceAchive.next_bst + 1);
			g_DistanceAchive.next_distance = 42195.f;
		}
	}

	if (distance >= g_DistanceAchive.next_Exdistance)
	{// コインexアチーブ
		AddBonusScore(BST_RUNEX);
		g_DistanceAchive.next_Exdistance += 150.f;
	}

}

/*=====================================================================
時間設定関数
=====================================================================*/
void SetGameStartTime()
{
	g_timeGameStart = timeGetTime();

}