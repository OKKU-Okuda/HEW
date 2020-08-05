/**********************************************************************
[[�{�[�i�X�X�R�A���Z�������f�v���O����(bonusAdd.cpp)]]
	��ҁF���c�@�^�K

	�X�R�A�̉��Z���f�Ɋւ���v���O����
	�{�[�i�X�\����v�Z����bonusscore.cpp��
	�e�M�~�b�N�̌ʃX�R�A�͒S��cpp��

***********************************************************************/
#include "../Core/main.h"

#include "BonusAdd.h"

#include "player_control.h"
#include "bonusscore.h"
#include "UI.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static DWORD g_timeGameStart = 0ul;

static struct {
	float nextM;	// �����Z�b�g����Ƃ��̃��[�g��
	int cntCoin;	// �W�߂��R�C��
}g_CoinPerMeter;		// ��ԓ��ɉ����W�߂��ƌv

static struct {
	int next_coin;
	BONUSSCORE_TYPE next_bst;

	int next_Excoin;			// Excoin�̎擾���ł��邩
}g_CoinAchive;

static struct {
	float next_distance;
	BONUSSCORE_TYPE next_bst;

	float next_Exdistance;		// Ex�̎擾���ł��邩
}g_DistanceAchive;

static struct {
	int cntGimmick;
	int nextGimmick;
	BONUSSCORE_TYPE next_bst;

	int next_ExGimmick;// Ex�̎擾���ł��邩
}g_GimmickAchive;
//static const int g_coin
/*=====================================================================
�X�V�֐�
=====================================================================*/
void UpdateBonusAdd()
{
	if (GetPlayerMoveState() != MSTATE_RUNNING)
	{
		return;
	}

	if (IsSetRareBonus(BST_LIVETIME1)==false && timeGetTime() - g_timeGameStart > 1000 * 90)
	{// ���ԂŃA�`�[�u
		AddBonusScore(BST_LIVETIME1);
	}
}

/*=====================================================================
���Z�b�g�֐�
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
GIMMICK�N���A����Reference���s���֐�
=====================================================================*/
void AddGimmickPassCount()
{
	g_GimmickAchive.cntGimmick++;

	if (g_GimmickAchive.cntGimmick == g_GimmickAchive.nextGimmick)
	{// ڱ�A�`�[�u
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
�R�C���擾����Reference���s���֐�
=====================================================================*/
void ReferenceCoin()
{
	const int coin = GetUI()->coin;

	if (++g_CoinPerMeter.cntCoin == 20)
	{// �R�C��per�����̃A�`�[�u
		AddBonusScore(BST_COINPERRUN);
	}

	if (coin == g_CoinAchive.next_coin)
	{//�@�R�C����ڱ�A�`�[�u
		AddBonusScore(g_CoinAchive.next_bst);

		if (g_CoinAchive.next_coin == 100 && IsSetRareBonus(BST_RUN4))
		{// �����Ƃ̕����A�`�[�u
			AddBonusScore(BST_COIN3RUN4);
		}
		else if (g_CoinAchive.next_coin == 1000 && IsSetRareBonus(BST_RUN5))
		{// ���㋭����
			AddBonusScore(BST_COIN4RUN5);
		}

		if (g_CoinAchive.next_coin != 1000)
		{// ����ȏ�̃A�`�[�u�����݂��邩
			g_CoinAchive.next_bst = (BONUSSCORE_TYPE)(g_CoinAchive.next_bst + 1);
			g_CoinAchive.next_coin *= 10;
		}
	}

	if (coin == g_CoinAchive.next_Excoin)
	{// �R�C��ex�A�`�[�u
		AddBonusScore(BST_COINEX);
		g_CoinAchive.next_Excoin += 75;
	}
}

/*=====================================================================
���[�g���X�V����Reference���s���֐�
=====================================================================*/
void ReferenceDistance()
{
	const float distance = (const float)GetUI()->distance;

	if (distance >= g_CoinPerMeter.nextM)
	{// �R�C��per�����ł̊l���n�X�e�[�^�X���Z�b�g
		g_CoinPerMeter.nextM += 100.0f;
		g_CoinPerMeter.cntCoin = 0;
	}

	if (distance == g_DistanceAchive.next_distance)
	{//�@�R�C����ڱ�A�`�[�u
		AddBonusScore(g_DistanceAchive.next_bst);

		if (g_DistanceAchive.next_distance == 1000.f && IsSetRareBonus(BST_COIN3))
		{// �����Ƃ̕����A�`�[�u
			AddBonusScore(BST_COIN3RUN4);
		}
		else if (g_DistanceAchive.next_distance == 10000.f && IsSetRareBonus(BST_COIN4))
		{// ���㋭����
			AddBonusScore(BST_COIN4RUN5);
		}

		if (g_DistanceAchive.next_distance < 10000.f)
		{// ����ȏ�̃A�`�[�u�����݂��邩
			g_DistanceAchive.next_bst = (BONUSSCORE_TYPE)(g_DistanceAchive.next_bst + 1);
			g_DistanceAchive.next_distance *= 10.f;
		}
		else if(g_DistanceAchive.next_distance == 10000.f)
		{// 42.195km�p
			g_DistanceAchive.next_bst = (BONUSSCORE_TYPE)(g_DistanceAchive.next_bst + 1);
			g_DistanceAchive.next_distance = 42195.f;
		}
	}

	if (distance >= g_DistanceAchive.next_Exdistance)
	{// �R�C��ex�A�`�[�u
		AddBonusScore(BST_RUNEX);
		g_DistanceAchive.next_Exdistance += 150.f;
	}

}

/*=====================================================================
���Ԑݒ�֐�
=====================================================================*/
void SetGameStartTime()
{
	g_timeGameStart = timeGetTime();

}