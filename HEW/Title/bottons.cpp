/**********************************************************************
[[Bottons�v���O����(Bottons.cpp)]]
	��ҁF���c�@�^�K

	Bottons�Ɋւ���v���O����
***********************************************************************/
#include "Bottons.h"	
#include "select.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define BASE_ALPHA		(0.45f)
#define ADD_ALPHA		(0.05f)

#define SIZE_BOTTONSTART		(Vec2(200, 45))
#define SIZE_BOTTONRANKING		(Vec2(200, 45))
#define SIZE_BOTTONCONFIG		(Vec2(175, 45))
#define SIZE_BOTTONEXIT			(Vec2(150, 45))

#define POS_BOTTONSTART		(Vec3(SCREEN_CENTER_X/1.5f, 350, 0))
#define POS_BOTTONRANKING	(Vec3(SCREEN_CENTER_X/1.5f, 450, 0))
#define POS_BOTTONCONFIG	(Vec3(SCREEN_CENTER_X/1.5f, 550, 0))
#define POS_BOTTONEXIT		(Vec3(SCREEN_CENTER_X/1.5f, 650, 0))

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


typedef struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Color		col_theme;			// �{�^����theme�F
	Vec3		pos;				// ���S�̈ʒu
	Vec2		size;				// ���S�T�C�Y
	Texture		tex;
	float		col_argb;			// �F�̑S�̒l�����p�ϐ�				
}BOTTON;


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

// ��{�֐��Q
static void InitBottons(bool isFirst);
static void UninitBottons(bool isEnd);
static void UpdateBottons();
static void DrawBottons();

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static OBJ_FUNC g_Func = { InitBottons,UninitBottons,UpdateBottons,DrawBottons };

static BOTTON	g_Botton[MAX_TITLESELECT];			// �{�^�����[�N


/*=====================================================================
Bottons�X�V�֐�
=====================================================================*/
void UpdateBottons()
{
	for (int idx = 0; idx < MAX_TITLESELECT; idx++)
	{
		float scl;	// �{�^���g��{��

		if (g_Botton[idx].col_argb < BASE_ALPHA)
		{	// ��{���l�ȉ��ł���Έꗥ�ŉ��Z���Ă���
			g_Botton[idx].col_argb += ADD_ALPHA;
		}
		else if (idx == GetSelect())
		{	// ��{���l�ȏ�őI������Ă���ꍇ�͊��S�\�����邽�߂ɉ��Z���Ă���
			g_Botton[idx].col_argb += ADD_ALPHA;

			if (g_Botton[idx].col_argb > 1.0f)
			{
				g_Botton[idx].col_argb = 1.0f;
			}
		}
		else
		{	// �I������Ă��Ȃ��ꍇ�͔������\���ׁ̈A��{���l�ɖ߂�
			g_Botton[idx].col_argb -= ADD_ALPHA;

			if (g_Botton[idx].col_argb < BASE_ALPHA)
			{
				g_Botton[idx].col_argb = BASE_ALPHA;
			}
		}

		// col_argb���Q�l�ɃT�C�Y��ύX
		scl = (1.0f - BASE_ALPHA / 2) + (g_Botton[idx].col_argb / 2);

		Set2DVertexColor(g_Botton[idx].vtx,
			Color(g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb, g_Botton[idx].col_argb));
		Set2DVerTex(g_Botton[idx].vtx, &g_Botton[idx].pos, &(g_Botton[idx].size*scl));
	}


}

/*=====================================================================
Bottons�`��֐�
=====================================================================*/
void DrawBottons()
{
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{	// �^�C�g���{�^���̕`��
		Draw2DVertex(g_Botton[i].tex, g_Botton[i].vtx);
	}
}

/*=====================================================================
Bottons�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitBottons(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// �{�^���e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Start.png",		&g_Botton[SELECT_STRAT].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Ranking.png",	&g_Botton[SELECT_RANKING].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Config.png",		&g_Botton[SELECT_CONFIG].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Exit.png",		&g_Botton[SELECT_EXIT].tex);

		g_Botton[SELECT_STRAT].pos		= POS_BOTTONSTART;
		g_Botton[SELECT_RANKING].pos	= POS_BOTTONRANKING;
		g_Botton[SELECT_CONFIG].pos		= POS_BOTTONCONFIG;
		g_Botton[SELECT_EXIT].pos		= POS_BOTTONEXIT;

		g_Botton[SELECT_STRAT].size		= SIZE_BOTTONSTART;
		g_Botton[SELECT_RANKING].size	= SIZE_BOTTONRANKING;
		g_Botton[SELECT_CONFIG].size	= SIZE_BOTTONCONFIG;
		g_Botton[SELECT_EXIT].size		= SIZE_BOTTONEXIT;

		g_Botton[SELECT_STRAT].col_theme	= { 1.0f, 0.5f, 0.0f, 1.0f };
		g_Botton[SELECT_RANKING].col_theme	= { 1.0f, 1.0f, 0.0f, 1.0f };
		g_Botton[SELECT_CONFIG].col_theme	= { 1.0f, 1.0f, 0.5f, 1.0f };
		g_Botton[SELECT_EXIT].col_theme		= { 0.5f, 0.5f, 0.5f, 1.0f };

		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

		// �{�^���̒��_�̐ݒu
	MakeNormal2DVertex(0, g_Botton[SELECT_STRAT].vtx,	&POS_BOTTONSTART,	&SIZE_BOTTONSTART);
	MakeNormal2DVertex(0, g_Botton[SELECT_RANKING].vtx, &POS_BOTTONRANKING, &SIZE_BOTTONRANKING);
	MakeNormal2DVertex(0, g_Botton[SELECT_CONFIG].vtx,	&POS_BOTTONCONFIG,	&SIZE_BOTTONCONFIG);
	MakeNormal2DVertex(0, g_Botton[SELECT_EXIT].vtx,	&POS_BOTTONEXIT,	&SIZE_BOTTONEXIT);
	
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{
		Set2DVertexColor(g_Botton[i].vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
		g_Botton[i].col_argb = 0.0f;
	}
}

/*=====================================================================
Bottons�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitBottons(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------


	if (isEnd == true)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------

	// �{�^���e�X�N�`���̊J��
	for (int i = 0; i < MAX_TITLESELECT; i++)
	{
		SAFE_RELEASE(g_Botton[i].tex)
	}
}

/*=====================================================================
Title�{�^��theme�F�擾�֐�
=====================================================================*/
Color* GetTitleBottonColor()
{
	return &g_Botton[GetSelect()].col_theme;
}

/*=====================================================================
Title�{�^���ʒu�擾�֐�
=====================================================================*/
Vec3* GetTitleBottonPosition()
{
	return &g_Botton[GetSelect()].pos;
}

/*=====================================================================
Bottons��{�֐��Q�擾�֐�
=====================================================================*/
OBJ_FUNC* GetBottonsFunc()
{
	return &g_Func;
}
