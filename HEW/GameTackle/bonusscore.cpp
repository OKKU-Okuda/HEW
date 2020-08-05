/**********************************************************************
[[�{�[�i�X�X�R�A�v���O����(bonusscore.cpp)]]
	��ҁF���c�@�^�K

	���[�U�[�s�����g���K�[�Ƃ���A�`�[�u�����g�`���ł̉��Z�X�R�A�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"

#include "bonusscore.h"

#include "../Result/player.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define MAX_KETA		(10)
#define MAX_BONUSUI		(20)

// �^�C�g���Ɛ�����1�g�ƍl�������̃e�N�X�`��������
#define NUM_TEXDIV_X	(2)
#define NUM_TEXDIV_Y	(12)

#define ADDRAREBONUS	(0.2f)		// 1�x�̂ݔ����̃{�[�i�X�ɂ��X�R�A�W��
#define ADDCOMMONBONUS	(0.05f)		// �����������̃{�[�i�X�ɂ��X�R�A�W��

#define SIZE_TITLE		Vec2(100,20)
#define SIZE_SUB		Vec2(80,16)
#define SIZE_SCORE		Vec2(10,30)
#define POS_TITLE		Vec3(100,200,0)
#define POS_SUB			Vec3(POS_TITLE.x + SIZE_TITLE.x - SIZE_SUB.x,220,0)
#define POS_SCORE		Vec3(105 + SIZE_TITLE.x + SIZE_SCORE.x,205,0)		// �ŏ�ʌ��̏ꏊ
#define ITV_SCOREX		(15.0f)

#define ITV_Y			(60.0f)	// ������2�ȏ㔭�������ꍇ��y�������炷��

#define MOVE_BONUS		Vec3(0,-2,0)	// �ړ���
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

typedef struct {
	VERTEX_2D TitleVtx[NUM_VERTEX];		// �{�[�i�X��
	VERTEX_2D SubVtx[NUM_VERTEX];		// �����p
	VERTEX_2D ScoreVtx[MAX_KETA][NUM_VERTEX];	// ���Z�X�R�A
	DWORD numShowKeta;							// �X�R�A�̕\����

	Vec3 PosTitle;
	Vec3 PosSub;
	Vec3 PosScore[MAX_KETA];
	Color ColScore;		// �X�R�A�E
	float alpha;
	bool isUse;				// �g�p����
//	bool isActiveData;		// ���̃f�[�^�̓A�N�e�B�u��Ԃ�(���_���̐ݒ������������)
}BONUS_UI;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num);	// ���擾
static float GetBonusScoreBuff();			// �o�tkeisuu�擾�֐�
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static DWORD g_BonusScore;		// ���Z�̃X�R�A
static Texture g_TexBonus;		// �{�[�i�X�ɂ��ċL�q�̂���e�N�X�`��
static Texture g_TexScore;		// �X�R�A�e�N�X�`��
static MySound g_seGet;			// �{�[�i�X�X�R�A�Q�b�g��

static BONUS_UI g_UI[MAX_BONUSUI];	// �\���p
static DWORD g_cntCreate;			// 1�t���[���̒��ŃX�R�A���Z�C�x���g�����񔭐��������i�`��֐��Ō�ɂO�N���A�j

static DWORD g_cntRareBonus;		// 1�x����̃{�[�i�X������
static DWORD g_cntCommonBonus;		// �������̃{�[�i�X������

static bool g_isAlreadyActiveRareBonus[START_COMMONBONUS]; // ���A�{�[�i�X�̔����m�Fflag

/* �{�[�i�X�X�R�A�̊�b�l */
static const DWORD g_scorelist[NUM_TEXDIV_X][NUM_TEXDIV_Y]=
{
	/* �ȉ��͍ő�1�x�̂݉��Z�����{�[�i�X */
	100,// �R�C��1��
	150,// �R�C��10��
	1000,// �R�C��100��
	5000,// �R�C��1000��
	75,// 1m
	100,// 10m
	400,// 100m
	2000,// 1000m
	8000,// 10km
	42195,// 42.195km
	5000,// �R�C��100�� && 1000m
	20000,// �R�C��1000�� && 10km
	100,// �M�~�b�N1��
	300,// �M�~�b�N10��
	2000,// �M�~�b�N50��
	10000,// 90�b����

	/* �ȉ��͕�������Z�����\���̂���{�[�i�X */
	300,// �R�C��75����(�ʓr���Z�����F�R�C��300���ȏ�)
	300,// 150m��(�ʓr���Z�����F500m�ȏ�)
	700,// �M�~�b�N10��(�ʓr���Z�����F�M�~�b�N100��ȏ�)
	1000,// ���A�{�C�X�𔭓�
	75,// ��GIMMICK�N���A
	250,// �|�[���̂Ƃ�����X���C�f�B���O�Œʉ�
	200,// �R�C��20����100m��ԓ��ŉ��
	100,// �^�C�~���O�悭�M�~�b�N���悯��
};
/*=====================================================================
�{�[�i�X�X�R�A�ݒ�֐�
=====================================================================*/
void AddBonusScore(BONUSSCORE_TYPE bst)
{
	BONUS_UI* work_pt = NULL;
	float scoreRate = GetBonusScoreBuff();			// �X�R�A�̊�b�l�Ɋ|���Z����l
	DWORD score;
	Vec3 OffsetPos(0, -ITV_Y * g_cntCreate, 0);
	Color ColScore;

	if (IsResultPlayer() == true)
	{// ���U���g�̏ꍇ�͏������s��Ȃ�
		return;
	}

	if (bst >= START_COMMONBONUS)
	{// Common�{�[�i�X
		g_cntCommonBonus++;
		ColScore = Color(1, 1, 1, 1);
	}
	else if (g_isAlreadyActiveRareBonus[bst] == false)
	{// �������̃��A�{�[�i�X
		g_isAlreadyActiveRareBonus[bst] = true;
		g_cntRareBonus++;
		ColScore = Color(1, 0, 0, 1);
	}
	else
	{// ���ɔ������Ă��郌�A�{�[�i�X�Ȃ̂ŏI��
		return;
	}

	// �`��p�̃f�[�^����
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == true)
		{
			continue;
		}
		// ���g�ppt�̑��
		work_pt = &g_UI[i];
	}
	if (work_pt == NULL)
	{// �󂫂��Ȃ��ꍇ�͕\�����Ȃ�
		return;
	}

	score = (DWORD)(g_scorelist[bst / NUM_TEXDIV_Y][bst % NUM_TEXDIV_Y] * scoreRate);
	g_BonusScore += score;

	work_pt->numShowKeta = GetKeta(score);
	work_pt->alpha = 1.f;
	work_pt->isUse = true;
	work_pt->ColScore = ColScore;

	for (DWORD i = 0; i < work_pt->numShowKeta; i++, score /= 10)
	{// �X�R�A�̃e�N�X�`���ݒ�
		work_pt->PosScore[i] = POS_SCORE + OffsetPos + Vec3((work_pt->numShowKeta - (i + 1))*ITV_SCOREX, 0.f, 0.f);
		Set2DTexPos(work_pt->ScoreVtx[i], 10, 1, score % 10, 0);
		Set2DVerTex(work_pt->ScoreVtx[i], &work_pt->PosScore[i], &SIZE_SCORE);
	}

	// �^�C�g���Ɛ����̃e�N�X�`�����W���擾
	work_pt->PosTitle = POS_TITLE + OffsetPos;
	Set2DTexPos(work_pt->TitleVtx, NUM_TEXDIV_X * 2, NUM_TEXDIV_Y, (bst / NUM_TEXDIV_Y) * 2, bst % NUM_TEXDIV_Y);
	Set2DVerTex(work_pt->TitleVtx, &work_pt->PosTitle, &SIZE_TITLE);


	work_pt->PosSub = POS_SUB + OffsetPos;
	Set2DTexPos(work_pt->SubVtx, NUM_TEXDIV_X * 2, NUM_TEXDIV_Y, ((bst / NUM_TEXDIV_Y) * 2) + 1, bst % NUM_TEXDIV_Y);
	Set2DVerTex(work_pt->SubVtx, &work_pt->PosSub, &SIZE_SUB);


	// ���t���[���̃J�E���^���C���N�������g
	g_cntCreate++;
}

/*=====================================================================
�X�V�֐�
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

	// �\��UI�̐ݒ�
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == false)
		{
			continue;
		}

		// �^�C�g���̐ݒ�
		g_UI[i].PosTitle += MOVE_BONUS;

		// �^�C�g���̈ʒu���烿�l���v�Z
		g_UI[i].alpha = g_UI[i].PosTitle.y / POS_TITLE.y;
		Set2DVerTex(g_UI[i].TitleVtx, &g_UI[i].PosTitle, &SIZE_TITLE);
		Set2DVertexColor(g_UI[i].TitleVtx, D3DXCOLOR(1, 1, 1, g_UI[i].alpha));


		// �����̐ݒ�
		g_UI[i].PosSub += MOVE_BONUS;
		Set2DVerTex(g_UI[i].SubVtx, &g_UI[i].PosSub, &SIZE_SUB);
		Set2DVertexColor(g_UI[i].SubVtx, D3DXCOLOR(1, 1, 1, g_UI[i].alpha));

		g_UI[i].ColScore.a = g_UI[i].alpha;
		for (DWORD j = 0; j < g_UI[i].numShowKeta; j++)
		{// �X�R�A�̐ݒ�
			g_UI[i].PosScore[j] += MOVE_BONUS;
			Set2DVerTex(g_UI[i].ScoreVtx[j], &g_UI[i].PosScore[j], &SIZE_SCORE);
			Set2DVertexColor(g_UI[i].ScoreVtx[j], g_UI[i].ColScore);
		}

		if (g_UI[i].alpha <= 0.f)
		{// ���l��0�ȉ��̏ꍇ�͖����ɂ���
			g_UI[i].isUse = false;
		}
	}
}

/*=====================================================================
�`��֐�
=====================================================================*/
void DrawBonusScore()
{
	for (int i = 0; i < MAX_BONUSUI; i++)
	{
		if (g_UI[i].isUse == false )//|| g_UI[i].isActiveData == false)
		{
			continue;
		}

		// ���Z���e�̕`��
		Draw2DVertex(g_TexBonus, g_UI[i].TitleVtx);

		// ��̐�����`��
		Draw2DVertex(g_TexBonus, g_UI[i].SubVtx);

		// ���Z�X�R�A��`��
		for (DWORD j = 0; j < g_UI[i].numShowKeta; j++)
		{
			Draw2DVertex(g_TexScore, g_UI[i].ScoreVtx[j]);
		}
	}


	if (g_cntCreate > 0ul)
	{
		// �Đ�
		MySoundPlayOnce(g_seGet);
		// 1�t���[���ŃN���G�C�g��������0�ɂ���
		g_cntCreate = 0ul;
	}
}

/*=====================================================================
�������֐�
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
�I�����֐�
=====================================================================*/
void UninitBonusScore()
{
	SAFE_RELEASE(g_TexBonus);
	SAFE_RELEASE(g_TexScore);
}

/*=====================================================================
�{�[�i�X�X�R�A�擾�֐�
=====================================================================*/
DWORD GetBonusScore()
{
	return g_BonusScore;
}

/*=====================================================================
�{�[�i�X�t���O�擾�֐�
=====================================================================*/
bool IsSetRareBonus(BONUSSCORE_TYPE rareBonus)
{
	return g_isAlreadyActiveRareBonus[rareBonus];
}

/*=====================================================================
�����擾�֐��֐�(cpp)
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
	{// 0���̏ꍇ�͂O���ĕ\���������̂�1���Ƃ���
		ans++;
	}

	return ans;
}

/*=====================================================================
�{�[�i�X�X�R�A�̃o�t�W���擾�֐�(cpp)
=====================================================================*/
float GetBonusScoreBuff()
{
	float addcommonrare = (float)(g_cntCommonBonus * ADDCOMMONBONUS);
	float addraterare = (float)(g_cntRareBonus * ADDRAREBONUS);

	return 1.f + addraterare + addcommonrare;
}