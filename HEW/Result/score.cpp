/**********************************************************************
[[���U���g�X�R�A�v���O����(Result/Score.cpp)]]
	��ҁF���c�@�^�K

	���U���g�̃X�R�A�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"

#include "score.h"


#include "../Phase/Phase_Result.h"
#include "../GameTackle/UI.h"
#include "../GameTackle/bonusscore.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define MAX_KETA		(10)		// �ő包��
#define START_POSY		(560)		// �J�n�ʒu
#define START_POSX		(SCREEN_CENTER_X+140)

#define NUMSIZE_X		(40.0f)
#define NUMSIZE_Y		(80.0f)
#define NUM_ITV_X		(15.f)
#define NUM_ITV_Y		(28.f)

#define ADD_TOTALSCORERATE		(0.03f)
#define ADD_BONUSSCORERATE		(0.02f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// �e�N�X�`����
enum SCORE_TEXTURE {
	STEX_BASE,// ���X�R�A�ȊO�̃X�R�A�\���p�g
	STEX_SCORE,			// �X�R�A
	STEX_SUBNUM,		// �T�u�X�R�A
	STEX_MAINNUM,		// ���C���X�R�A

	MAX_TEXTURE
};

// �X�R�A��
enum TYPE_SCORE {
	S_BONUS,
	S_COIN,
	S_DISTANCE,
	S_SCORE,
	MAX_SCORE
};

// �X�R�A�A�j���[�V������
enum ANIM_TYPE {
	SANIM_SUBSCORE,
	SANIM_MAINSCORE,
	SANIM_ADDBONUS,

	MAX_SCOREANIM
};

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num); // �����擾
static void Update001SubScore();
static void Update002MainScore();
static void Update003AddBonusScore();

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Texture g_Tex[MAX_TEXTURE];
static VERTEX_2D g_BaseScoreVtx[NUM_VERTEX];		// ���X�R�A�ȊO�̃X�R�A�\���p�g
static VERTEX_2D g_ScoreStrVtx[NUM_VERTEX];			// �������I���ď�����������

static VERTEX_2D g_Score[MAX_SCORE][MAX_KETA][NUM_VERTEX];	// �X�R�A�̒��_
static DWORD g_showScore[MAX_SCORE];	// �\���X�R�A
static DWORD g_numKeta[MAX_SCORE];		// �\������
static DWORD g_TotalScore;				// ���ʓI�ȃX�R�A

static ANIM_TYPE g_AnimType;		// �\���X�R�A�̑J�ڃA�j���[�V�����^�C�v
static float g_rateEx = 0.f;		// �X�R�A�\���֘A�̐i��
static void(*g_UpdateAnim[MAX_SCOREANIM])(void);	// �X�R�A���Ƃ̍X�V����

static MySound g_seScoreDeside;

// �T�u���A�\���t�F�[�Y
void Update001SubScore()
{
	g_showScore[S_COIN] = (DWORD)(GetUI()->coin * GetResultRate());
	g_showScore[S_DISTANCE] = (DWORD)(GetUI()->distance * GetResultRate());

	if (GetResultRate() >= 1.f)
	{
		g_rateEx = 0.f;
		g_AnimType = (ANIM_TYPE)(g_AnimType + 1);
	
		// �O�̂���
		g_showScore[S_COIN] = (DWORD)(GetUI()->coin);
		g_showScore[S_DISTANCE] = (DWORD)(GetUI()->distance);
	}
}

// ���C���X�R�A�\���t�F�[�Y
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

		// �O�̂���1.f�ōČv�Z
		g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))));
		Set2DVertexColor(g_ScoreStrVtx, D3DXCOLOR(1, 1, 1, 1.f));

		for (DWORD j = 0; j < MAX_KETA; j++)
		{
			Set2DVertexColor(g_Score[S_SCORE][j], D3DXCOLOR(1, 1, 1, 1.f));
		}

	}
}

// �����ă{�[�i�X�X�R�A�����Z���Ă����t�F�[�Y
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

		// �O�̂���1.f�ōČv�Z
		g_showScore[S_BONUS] = (DWORD)(GetBonusScore());
		g_showScore[S_SCORE] = (DWORD)((GetUI()->distance * (1.f + (GetUI()->coin*0.01f))))
			+ (DWORD)(GetBonusScore());

		MySoundPlayOnce(g_seScoreDeside);		// ���艹��炷
	}
}

/*=====================================================================
�X�V�֐�
=====================================================================*/
void UpdateScore()
{

	if (g_AnimType < MAX_SCOREANIM)
	{// �X�R�A�̌ʍX�V
		g_UpdateAnim[g_AnimType]();
	}


	for (DWORD i = 0; i < MAX_SCORE; i++)
	{// �S�X�R�A�̒��_�ݒu
		DWORD score = g_showScore[i];
		g_numKeta[i] = GetKeta(g_showScore[i]);

		for (DWORD j = 0; j < g_numKeta[i]; j++, score /= 10)
		{
			// �e�N�X�`���̐ݒu
			Set2DTexPos(g_Score[i][j], 10, 1, score % 10, 0);
		}
	}

}

/*=====================================================================
�`��֐�
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
�������֐�
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
	{// �[���N���A
		g_showScore[i] = 0;
	}

	for (DWORD j = 0; j < MAX_KETA; j++)
	{// ���C���X�R�A�͏����ł͕\�����Ȃ�
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
�I���֐�
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
�����擾�֐��֐�
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