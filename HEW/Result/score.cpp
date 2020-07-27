/**********************************************************************
[[���U���g�X�R�A�v���O����(Result/Score.cpp)]]
	��ҁF���c�@�^�K

	���U���g�̃X�R�A�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"

#include "score.h"


#include "../Phase/Phase_Result.h"
#include "../GameTackle/UI.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define MAX_KETA		(10)		// �ő包��
#define START_POSY		(475)		// �J�n�ʒu
#define START_POSX		(SCREEN_CENTER_X+100)

#define NUMSIZE_X		(45.0f)
#define NUMSIZE_Y		(90.0f)
#define NUM_ITV_X		(15.f)
#define NUM_ITV_Y		(40.f)

#define ADD_RATE		(0.03f)
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

// �T�u�X�R�A��
enum TYPE_SCORE {
	S_COIN,
	S_DISTANCE,
	S_SCORE,
	MAX_SCORE
};
//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static DWORD GetKeta(DWORD num); // �����擾
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Texture g_Tex[MAX_TEXTURE];
static VERTEX_2D g_BaseScoreVtx[NUM_VERTEX];		// ���X�R�A�ȊO�̃X�R�A�\���p�g
static VERTEX_2D g_ScoreStrVtx[NUM_VERTEX];			// �������I���ď�����������

static VERTEX_2D g_Score[MAX_SCORE][MAX_KETA][NUM_VERTEX];	// �X�R�A�̒��_
static DWORD g_showScore[MAX_SCORE];	// �\���X�R�A
static DWORD g_numKeta[MAX_SCORE];		// �\������

static float g_rateEx = 0.f;		// ���X�R�A�\���̐i��
/*=====================================================================
�X�V�֐�
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
	{// �S�X�R�A�̒��_�ݒu
		DWORD score = g_showScore[i];
		g_numKeta[i] = GetKeta(g_showScore[i]);

		for (DWORD j = 0; j < g_numKeta[i]; j++, score /= 10)
		{
			// �e�N�X�`���̐ݒu
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
	{// �[���N���A
		g_showScore[i] = 0;
	}
	g_rateEx = 0.f;
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
	return ans;
}