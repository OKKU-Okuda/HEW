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

#include "../Core/directx_Helper3D.h"
#include "Phase_Title.h"	
#include "Phase_GameTackle1.h"

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
static Model		g_mdlPlayer;

static struct {
	VERTEX_2D	vtx[NUM_VERTEX];
	Texture		tex;
}g_Botton[MAX_TITLEBOTTOM];			// �{�^�����[�N

static struct {
	VtxBuff		pVtx;	// ���_�o�b�t�@
	Texture		pTex;	// �n�ʃe�N�X�`��
	Matrix		Mat;	// ��]�s��
}g_Field;							// �t�B�[���h���[�N

/*=====================================================================
Title�X�V�֐�
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("�^�C�g���t�F�[�Y");

	// ���̃t�F�[�Y�ɍs��
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// �^�b�N���P
		GoNextPhase(GetPhaseGameTackle1Func());
	}
	

	if (GetKeyboardPress(DIK_UP))
	{
		g_mdlPlayer->WldMtx._43 += 1.0f;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		g_mdlPlayer->WldMtx._43 -= 1.0f;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		g_mdlPlayer->WldMtx._41 -= 1.0f;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_mdlPlayer->WldMtx._41 += 1.0f;
	}
	//g_mdlPlayer->WldMtx._44 -= 0.01f;
	Vec4 vc(g_mdlPlayer->WldMtx.m[3]);
	PrintDebugProc("�v���C���[�ʒu:%vec4", vc);

	PrintDebugProc("�I�t�Z�b�g���W�ύX��������");
	//g_modelCity->WldMtx = g_mdlPlayer->WldMtx;
}

/*=====================================================================
Title�`��֐�
=====================================================================*/
void DrawTitle()
{
	D3DDEVICE;

	DrawModel(g_mdlPlayer);


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

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Start.png",		&g_Botton[BOTTON_STRAT].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Ranking.png",	&g_Botton[BOTTON_RANKING].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Config.png",		&g_Botton[BOTTON_CONFIG].tex);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title_Exit.png",		&g_Botton[BOTTON_EXIT].tex);

		g_Sound = MySoundCreate("data/BGM/bgm000.wav");

		g_mdlPlayer = CreateModel("data/MODEL/Player.x");

	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	MySoundPlayEternal(g_Sound);	// �i���Đ�

	GetMatrix(&g_mdlPlayer->WldMtx);

	// �{�^���̒��_�̐ݒu
	MakeTitleVertex(0, g_Botton[BOTTON_STRAT].vtx,		&Vec3(SCREEN_CENTER_X, 100, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_RANKING].vtx,	&Vec3(SCREEN_CENTER_X, 200, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_CONFIG].vtx,		&Vec3(SCREEN_CENTER_X, 300, 0), &Vec2(200, 45));
	MakeTitleVertex(0, g_Botton[BOTTON_EXIT].vtx,		&Vec3(SCREEN_CENTER_X, 400, 0), &Vec2(200, 45));

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
		DeleteModel(&g_mdlPlayer);
		MySoundDeleteAuto(&g_Sound);// ���₵�����̂���C�ɊJ��

		// �{�^���e�X�N�`���̊J��
		for (int i = 0; i < MAX_TITLEBOTTOM; i++)
		{
			SAFE_RELEASE(g_Botton[i].tex)
		}
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

