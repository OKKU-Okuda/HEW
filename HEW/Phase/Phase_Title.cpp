/**********************************************************************
[[�t�F�[�Y:Title�v���O����(Phase_Title.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FPhase_Title�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "../Core/directx_Helper3D.h"	// �`�扇���֐�
#include "Phase_Title.h"				
#include "Phase_GameTackle1.h"			// ���̃t�F�[�Y
#include "../Title_effect.h"			// �^�C�g���G�t�F�N�g

// �^�C�g���d�l
// �^�C�g���ɐi�߂�{�^����3�قǗp��
// �ݒ�A�ޏo�A���ʁA�Q�[���J�n
// �w�i�͓���������
// �J�[�\���������n�����[���h���ۂ�
// �^�C�g�����S���_�C�i�~�b�N�ɓ������Ă�����񂳂�����
// ���̓r���{�[�h�œ�����
// 


//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

// �v���C���[�̊�{�����ʒu
#define PLAYER_POSFROMZ	(6000.0f)
#define PLAYER_POSTOZ	(-110.0f)
#define PLAYER_DISZ		(PLAYER_POSFROMZ - PLAYER_POSTOZ)
#define PLAYER_POSRATE	(0.05f)
#define PLAYER_ROT		(&Vec3(0, 0.3f, 0))

#define BASE_ALPHA		(0.45f)
#define ADD_ALPHA		(0.05f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// �^�C�g�������L���O�I�u�W�F�N�g
enum TITLE_BOTTON {
	BOTTON_STRAT,		// �J�n�{�^��
	BOTTON_RANKING,		// �����L���O�{�^��
	BOTTON_CONFIG,		// �ݒ�{�^��
	BOTTON_EXIT,		// �ޏo�{�^��

	MAX_TITLEBOTTOM		/*�{�^����*/
};


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

static void SetTitleVertexColor(VERTEX_2D *vtx_data, Color nColor);
static void MakeTitleVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetTitleVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_Sound;

static struct {
	Model		model;		// �v���C���[���f��
	float		sclYrot;	// �傫�����鉉�o�̍ۂ̐�Βl�T�C���֐�
	float		posZadd;	// ��{�l������Z����Z�ʒu		
}g_Player;							// �v���C���[���[�N

static struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Texture		tex;
	float		col_argb;			// �F�̑S�̒l�����p�ϐ�				
}g_Botton[MAX_TITLEBOTTOM];			// �{�^�����[�N

static DWORD	g_Select;			// �{�^��

static float	rot_spd;

/*=====================================================================
Title�X�V�֐�
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("�^�C�g���t�F�[�Y");

	SetTitle3DEffect();
	SetTitle3DEffect();


	// ���̃t�F�[�Y�ɍs��
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// �^�b�N���P
		GoNextPhase(GetPhaseGameTackle1Func());
		//GoNextPhase(GetPhaseTitleFunc());
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		rot_spd += 0.002f;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		rot_spd -= 0.002f;
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		rot_spd = 0.0f;
	}

	//SAFE_NUMBER(rot_spd, -0.05f, 0.05f);
	PrintDebugProc("��]�F%f", rot_spd);
	UpdateTitleEffect(rot_spd);


	if (GetKeyboardTrigger(DIK_UP))
	{
		g_Select = (g_Select - 1) % MAX_TITLEBOTTOM;
	}

	if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_Select = (g_Select + 1) % MAX_TITLEBOTTOM;
	}


	if (GetFade() == FADE_NONE)
	{
		// �v���C���[�̏c�ɑ傫���Ȃ鉉�o
		float sclY = 1.0f;

		g_Player.sclYrot += 0.079f;
		sclY += fabsf(sinf(g_Player.sclYrot)) * 0.07f;

		g_Player.posZadd = (PLAYER_DISZ - g_Player.posZadd) * PLAYER_POSRATE + g_Player.posZadd;
		GetMatrix(&g_Player.model->WldMtx, &Vec3(0.0f, -30.0f, PLAYER_POSFROMZ - g_Player.posZadd), PLAYER_ROT, &Vec3(1.0f, sclY, 1.0f));// �v���C���[�����ʒu


		// �v���C���[��������x�߂��ꍇ�̓{�^���̃��l���グ��
		if (PLAYER_DISZ - g_Player.posZadd <= 1.0f)
		{
			for (int i = 0; i < MAX_TITLEBOTTOM; i++)
			{
				if (g_Botton[i].col_argb < BASE_ALPHA)
				{
					g_Botton[i].col_argb += ADD_ALPHA;
				}
				else if (i == g_Select)
				{
					g_Botton[i].col_argb += ADD_ALPHA;

					if (g_Botton[i].col_argb > 1.0f)
					{
						g_Botton[i].col_argb = 1.0f;
					}
				}
				else
				{
					g_Botton[i].col_argb -= ADD_ALPHA;

					if (g_Botton[i].col_argb < BASE_ALPHA)
					{
						g_Botton[i].col_argb = BASE_ALPHA;
					}
				}

				g_Botton[i].vtx[0].diffuse =
					g_Botton[i].vtx[1].diffuse =
					g_Botton[i].vtx[2].diffuse =
					g_Botton[i].vtx[3].diffuse = Color(g_Botton[i].col_argb, g_Botton[i].col_argb, g_Botton[i].col_argb, g_Botton[i].col_argb);
			}
		}

	}

	// �v���C���[�ʒu�̕\��
	Vec4 vc(g_Player.model->WldMtx.m[3]);
	PrintDebugProc("�v���C���[�ʒu:%vec4", vc);
}

/*=====================================================================
Title�`��֐�
=====================================================================*/
void DrawTitle()
{
	DWORD	d3drslightning;
	D3DDEVICE;

	// ���C�e�B���O�̐ݒ�
	pDevice->GetRenderState(D3DRS_LIGHTING, &d3drslightning);	
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xff00ffff);
	DrawModel(g_Player.model);

	DrawTitleEffect();				// ���͂ɕ����Ă���G�t�F�N�g�̕`��

	// ���̃��C�e�B���O��Ԃɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);


	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{	// �^�C�g���{�^���̕`��
		pDevice->SetTexture(0, g_Botton[i].tex);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Botton[i].vtx, sizeof(VERTEX_2D));
	}
}

/*=====================================================================
Title�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitTitle(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// �{�^���e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Start.png",		&g_Botton[BOTTON_STRAT].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Ranking.png",	&g_Botton[BOTTON_RANKING].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Config.png",		&g_Botton[BOTTON_CONFIG].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Exit.png",		&g_Botton[BOTTON_EXIT].tex);

		g_Sound = MySoundCreate("data/BGM/Title.wav");

		g_Player.model = CreateModel("data/MODEL/Player.x");

		InitTitleEffect(true);
	}
	else
	{
		InitTitleEffect(false);
	}

	MySoundSetMasterVolume(0.1f);
	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	// �J����
#if 1
	GetCamera()->pos	= Vec3(0.0f, 0.0f, -200.0f);	
	GetCamera()->at		= Vec3(-100.0f, 0.0f, 0);
#else
	GetCamera()->pos	= Vec3(0.0f, 0.0f, 200.0f);
	GetCamera()->at		= Vec3(200.0f, 0.0f, -200.0f);
#endif

	g_Player.sclYrot = 0.0f;
	g_Player.posZadd = 0.0f;
	GetMatrix(&g_Player.model->WldMtx, &Vec3(0.0f, 0.0f, PLAYER_POSFROMZ), PLAYER_ROT);// �v���C���[�����ʒu

	MySoundPlayEternal(g_Sound);	// �i���Đ�


	// �{�^���̒��_�̐ݒu
	MakeTitleVertex(0, g_Botton[BOTTON_STRAT].vtx,		&Vec3(SCREEN_CENTER_X/2, 300, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_RANKING].vtx,	&Vec3(SCREEN_CENTER_X/2, 400, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_CONFIG].vtx,		&Vec3(SCREEN_CENTER_X/2, 500, 0), &Vec2(175, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_EXIT].vtx,		&Vec3(SCREEN_CENTER_X/2, 600, 0), &Vec2(150, 45));

	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{
		SetTitleVertexColor(g_Botton[i].vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
		g_Botton[i].col_argb = 0.0f;
	}

	rot_spd		= 0.0f;
	g_Select	= BOTTON_STRAT;
}

/*=====================================================================
Title�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitTitle(bool isEnd)
{

//---------------------------------------------------------------------
//	���̑��̏I������
//---------------------------------------------------------------------

	MySoundStop(g_Sound);	// ��~


	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�J������
		//---------------------------------------------------------------------

		DeleteModel(&g_Player.model);
		MySoundDeleteAuto(&g_Sound);// ���₵�����̂���C�ɊJ��

		// �{�^���e�X�N�`���̊J��
		for (int i = 0; i < MAX_TITLEBOTTOM; i++)
		{
			SAFE_RELEASE(g_Botton[i].tex)
		}

		UninitTitleEffect(true);
	}
	else
	{
		UninitTitleEffect(false);
	}

}

/*=====================================================================
Title��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

/*=====================================================================
���ՓI�ɒ��_�ݒu�֐�(�m�[�}��)(cpp_func)
	�߂�l�Fvoid
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
=====================================================================*/
void MakeTitleVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
{
	// ���ːE�̐ݒ�
	SetTitleVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

	// �p�[�X�y�N�e�B�u�R���N�g
	(vtx_data)->rhw =
		(vtx_data + 1)->rhw =
		(vtx_data + 2)->rhw =
		(vtx_data + 3)->rhw = 1.0f;

	// ���_���W
	(vtx_data)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 1)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 2)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y + Size->y, 0.0f);
	(vtx_data + 3)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y + Size->y, 0.0f);

	// �������łO�w�肪�����
	if (num == 0)
	{
		// �e�N�X�`�����W
		(vtx_data)->tex = D3DXVECTOR2(0.01f, 0.01f);
		(vtx_data + 1)->tex = D3DXVECTOR2(0.99f, 0.01f);
		(vtx_data + 2)->tex = D3DXVECTOR2(0.01f, 0.99f);
		(vtx_data + 3)->tex = D3DXVECTOR2(0.99f, 0.99f);
	}

	return;
}

/*=====================================================================
�J���[�ݒu�֐�(�m�[�}��) (cpp_func)
	�߂�l�Fvoid
	�����F
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
 Color nColor			: �V�����J���[
=====================================================================*/
void SetTitleVertexColor(VERTEX_2D *vtx_data, Color nColor)
{
	// ���ːE�̐ݒ�
	(vtx_data)->diffuse =
		(vtx_data + 1)->diffuse =
		(vtx_data + 2)->diffuse =
		(vtx_data + 3)->diffuse = nColor;

}

/*=====================================================================
���_�̐ݒu���s���֐�(cpp_func)
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	Vec3 *Pos,	:���S���W
	Vec2 *Size	:���S����̃T�C�Y
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

