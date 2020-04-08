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

#define SIZE_SELECTEFFECT		(Vec2(350,90))

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
static void SetTitleTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);
static void SetSelectEffect();
static void UpdateSelectEffect();
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_soundBGM;
static MySound		g_soundSelect;		// �I����

static struct {
	Model		model;		// �v���C���[���f��
	float		sclYrot;	// �傫�����鉉�o�̍ۂ̐�Βl�T�C���֐�
	float		posZadd;	// ��{�l������Z����Z�ʒu		
}g_Player;							// �v���C���[���[�N

static struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Vec3		pos;				// ���S�̈ʒu
	Vec2		size;				// ���S�T�C�Y
	Texture		tex;
	float		col_argb;			// �F�̑S�̒l�����p�ϐ�				
}g_Botton[MAX_TITLEBOTTOM];			// �{�^�����[�N

static DWORD	g_Select;			// �{�^��
static float	rot_spd;

static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ���S���_
	Texture    tex;				// �e�N�X�`��
	float	   col;				// �F
}g_Logo;				// ���S���[�N

static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ���S���_
	Texture    tex;				// �e�N�X�`��
	DWORD	   cnt_show;		// �\���J�E���g
	bool	   isShow;			// �\��flag
}g_Effect;

/*=====================================================================
Title�X�V�֐�
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("�^�C�g���t�F�[�Y");

	SetTitle3DEffect();
	SetTitle3DEffect();

	// ���̃t�F�[�Y�ɍs��
	if (GetKeyboardTrigger(DIK_RETURN) && g_Botton[g_Select].col_argb > 0.75f)
	{	// �^�b�N���P

		switch (g_Select)
		{
		case BOTTON_STRAT:
			GoNextPhase(GetPhaseGameTackle1Func());
			break;

		case BOTTON_RANKING:
		case BOTTON_CONFIG:
			GoNextPhase(GetPhaseTitleFunc());
			break;

		case BOTTON_EXIT:
			DestroyWindow(GetHandle());
			break;

		default:
			MessageBox(0, 0, 0, 0);
			break;
		}
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

	SAFE_NUMBER(rot_spd, -0.05f, 0.05f);
	PrintDebugProc("��]�F%f", rot_spd);
	UpdateTitleEffect(rot_spd);




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
			g_Logo.col += 0.02f;
			SetTitleVertexColor(g_Logo.vtx, Color(g_Logo.col, g_Logo.col, g_Logo.col, g_Logo.col));

			if (GetKeyboardTrigger(DIK_UP))
			{
				g_Select = (g_Select - 1) % MAX_TITLEBOTTOM;
				MySoundPlayOnce(g_soundSelect);
				SetSelectEffect();
			}

			else if (GetKeyboardTrigger(DIK_DOWN))
			{
				g_Select = (g_Select + 1) % MAX_TITLEBOTTOM;
				MySoundPlayOnce(g_soundSelect);
				SetSelectEffect();
			}

			UpdateSelectEffect();

			for (int i = 0; i < MAX_TITLEBOTTOM; i++)
			{
				float scl;	// �{�^���g��{��

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

				scl = (1.0f - BASE_ALPHA / 2) + (g_Botton[i].col_argb / 2);

				SetTitleVertexColor(g_Botton[i].vtx, Color(g_Botton[i].col_argb, g_Botton[i].col_argb, g_Botton[i].col_argb, g_Botton[i].col_argb));
				SetTitleVertex(g_Botton[i].vtx, &g_Botton[i].pos, &(g_Botton[i].size*scl));
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

	if (g_Effect.isShow)
	{
		pDevice->SetTexture(0, g_Effect.tex);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Effect.vtx, sizeof(VERTEX_2D));
	}


	pDevice->SetTexture(0, g_Logo.tex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Logo.vtx, sizeof(VERTEX_2D));

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

		// �^�C�g�����S�̃e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleLogo_000.png", &g_Logo.tex);

		// �Z���N�g�G�t�F�N�g�̃e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/select_effect.png", &g_Effect.tex);

		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");
		g_soundSelect	= MySoundCreate("data/SE/select.wav");
		MySoundSetVolume(g_soundSelect, 3.0f);

		g_Player.model = CreateModel("data/MODEL/Player.x");

		g_Botton[BOTTON_STRAT].pos		= POS_BOTTONSTART;
		g_Botton[BOTTON_RANKING].pos	= POS_BOTTONRANKING;
		g_Botton[BOTTON_CONFIG].pos		= POS_BOTTONCONFIG;
		g_Botton[BOTTON_EXIT].pos		= POS_BOTTONEXIT;

		g_Botton[BOTTON_STRAT].size		= SIZE_BOTTONSTART;
		g_Botton[BOTTON_RANKING].size	= SIZE_BOTTONRANKING;
		g_Botton[BOTTON_CONFIG].size	= SIZE_BOTTONCONFIG;
		g_Botton[BOTTON_EXIT].size		= SIZE_BOTTONEXIT;
		InitTitleEffect(true);
	}
	else
	{
		InitTitleEffect(false);
	}

	MySoundSetMasterVolume(0.5f);
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

	MySoundPlayEternal(g_soundBGM);	// �i���Đ�
	

	// �{�^���̒��_�̐ݒu
	MakeTitleVertex(0, g_Botton[BOTTON_STRAT].vtx,		&POS_BOTTONSTART,	&SIZE_BOTTONSTART);
	MakeTitleVertex(0, g_Botton[BOTTON_RANKING].vtx,	&POS_BOTTONRANKING, &SIZE_BOTTONRANKING);
	MakeTitleVertex(0, g_Botton[BOTTON_CONFIG].vtx,		&POS_BOTTONCONFIG,	&SIZE_BOTTONCONFIG);
	MakeTitleVertex(0, g_Botton[BOTTON_EXIT].vtx,		&POS_BOTTONEXIT,	&SIZE_BOTTONEXIT);

	for (int i = 0; i < MAX_TITLEBOTTOM; i++)
	{
		SetTitleVertexColor(g_Botton[i].vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
		g_Botton[i].col_argb = 0.0f;
	}

	// ���S���_�̐ݒu
	MakeTitleVertex(0, g_Logo.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &Vec2(350, 120));
	SetTitleVertexColor(g_Logo.vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
	g_Logo.col = 0.0f;

	// �G�t�F�N�g���_�̐ݒu
	MakeTitleVertex(0, g_Effect.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &SIZE_SELECTEFFECT);
	g_Effect.cnt_show	= 0ul;
	g_Effect.isShow		= false;
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

	MySoundStop(g_soundBGM);	// ��~
	MySoundStop(g_soundSelect);

	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�J������
		//---------------------------------------------------------------------

		DeleteModel(&g_Player.model);
		MySoundDeleteAuto(&g_soundBGM);// ���₵�����̂���C�ɊJ��
		MySoundDeleteAuto(&g_soundSelect);// ���₵�����̂���C�ɊJ��

		// �{�^���e�X�N�`���̊J��
		for (int i = 0; i < MAX_TITLEBOTTOM; i++)
		{
			SAFE_RELEASE(g_Botton[i].tex)
		}

		SAFE_RELEASE(g_Logo.tex);
		SAFE_RELEASE(g_Effect.tex);

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

/*=====================================================================
�e�N�X�`��2D���W�ݒu�֐�(cpp_func)
	�߂�l�Fvoid
	�����F
VERTEX_2D *vtx_data,	:4�̒��_�z��
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void SetTitleTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY)
{
	D3DXVECTOR2 size = D3DXVECTOR2(1.0f / X_parts, 1.0f / Y_parts);

	(vtx_data)->tex.x =
		(vtx_data + 2)->tex.x = size.x*nowX;
	(vtx_data + 1)->tex.x =
		(vtx_data + 3)->tex.x = size.x*(nowX + 1);

	(vtx_data)->tex.y =
		(vtx_data + 1)->tex.y = size.y*nowY;
	(vtx_data + 2)->tex.y =
		(vtx_data + 3)->tex.y = size.y*(nowY + 1);

}

/*=====================================================================
�I���������G�t�F�N�g�ݒu�֐�(cpp_func)
=====================================================================*/
void SetSelectEffect()
{
	g_Effect.isShow		= true;
	g_Effect.cnt_show	= 0ul;

	SetTitleVertex(g_Effect.vtx, &g_Botton[g_Select].pos, &SIZE_SELECTEFFECT);
	SetTitleTexPos(g_Effect.vtx, 1, 7, 0, g_Effect.cnt_show);
}

void UpdateSelectEffect()
{
	if (g_Effect.isShow == false)
	{
		return;
	}

	SetTitleTexPos(g_Effect.vtx, 1, 7, 0, g_Effect.cnt_show++/2);
	

	if (g_Effect.cnt_show % 15 == 0)
	{
		g_Effect.isShow = false;
	}

}