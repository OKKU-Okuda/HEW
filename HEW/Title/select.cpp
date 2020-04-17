/**********************************************************************
[[Select�v���O����(Select.cpp)]]
	��ҁF���c�@�^�K

	�^�C�g���{�^���I���v���O����
***********************************************************************/
#include "../Core/fade.h"

#include "../Phase/Phase_GameTackle1.h"
#include "../Phase/Phase_Title.h"

#include "select.h"	
#include "bottons.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define SIZE_SELECTEFFECT		(Vec2(350,90))

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static void SetSelectEffectActive();
static void SetSelectEffectPassive();

// ��{�֐��Q
static void InitSelect(bool isFirst);
static void UninitSelect(bool isEnd);
static void UpdateSelect();

static void DrawSelectEffect();

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static OBJ_FUNC g_Func = { InitSelect,UninitSelect,UpdateSelect,NoFunction };

// �Z���N�g�G�t�F�N�g
static NormalFunc	g_updatefunc;		// �G�t�F�N�g�X�V�֐�
static VERTEX_2D	g_vtx[NUM_VERTEX];	// ���S���_
static Texture		g_tex;				// �e�N�X�`��
static DWORD		g_cnt_show;			// �\���J�E���g


static DWORD		g_Select;			// �{�^��

/*=====================================================================
Select�X�V�֐�
=====================================================================*/
void UpdateSelect()
{

	// �㉺�̑I��
	if (GetKeyboardTrigger(DIK_UP))
	{
		g_Select = (g_Select - 1) % MAX_TITLESELECT;
		//MySoundPlayOnce(g_soundSelect);
	//	SetTitle3DRot(true);
		SetSelectEffectActive();
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_Select = (g_Select + 1) % MAX_TITLESELECT;
		//MySoundPlayOnce(g_soundSelect);
	//	SetTitle3DRot(false);
	//	SetSelectEffect();
		SetSelectEffectActive();
	}

	// �I���{�^���ɂ���ĕς�鏈��
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		switch (g_Select)
		{
		case SELECT_STRAT:	// �^�b�N��1�t�F�[�Y�Ɉڍs
			GoNextPhase(GetPhaseGameTackle1Func());
			break;

		case SELECT_RANKING:
		case SELECT_CONFIG:
			GoNextPhase(GetPhaseTitleFunc());
			break;

		case SELECT_EXIT:	// �Q�[���̏I��
			DestroyWindow(GetHandle());
			break;

		default:			// �G���[�o��
			MessageBox(0, 0, 0, 0);
			break;
		}
	}

	// �G�t�F�N�g�̃e�N�X�`���X�V����
	g_updatefunc();


}

/*=====================================================================
Select�`��֐�
=====================================================================*/
void DrawSelectEffect()
{
	// �G�t�F�N�g�̕`��
	Draw2DVertex(g_tex, g_vtx);
}

/*=====================================================================
Select�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitSelect(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// �Z���N�g�G�t�F�N�g�̃e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/select_effect.png", &g_tex);
		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	// �G�t�F�N�g���_�̐ݒu
	MakeNormal2DVertex(0, g_vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &SIZE_SELECTEFFECT);

	SetSelectEffectPassive();

	g_Select = SELECT_STRAT;

}

/*=====================================================================
Select�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitSelect(bool isEnd)
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

	SAFE_RELEASE(g_tex);
}

/*=====================================================================
Select��{�֐��Q�擾�֐�
=====================================================================*/
OBJ_FUNC* GetSelectFunc()
{
	return &g_Func;
}

/*=====================================================================
�I��Ԓn�擾�֐�
=====================================================================*/
DWORD GetSelect()
{
	return g_Select;
}

/*=====================================================================
�I���G�t�F�N�g�p�b�V�u�֐�(cpp_func)
=====================================================================*/
void UpdateSelectEffect()
{
	// 2�t���[����1�x�e�N�X�`�����W���X�V�����
	Set2DTexPos(g_vtx, 1, 7, 0, g_cnt_show++ / 2);

	if (g_cnt_show % 15 == 0)
	{	// ���ȏ�i�񂾏ꍇ�͔�\���ɂ���
		SetSelectEffectPassive();
	}
}

/*=====================================================================
�I���G�t�F�N�g�A�N�e�B�u�֐�(cpp_func)
=====================================================================*/
void SetSelectEffectActive()
{
	g_Func.Draw		= DrawSelectEffect;
	g_updatefunc	= UpdateSelectEffect;

	g_cnt_show = 0ul;

	// �ʒu�̐ݒ�
	Set2DVerTex(g_vtx, GetTitleBottonPosition(), &SIZE_SELECTEFFECT);
	Set2DVertexColor(g_vtx, *GetTitleBottonColor());
}

/*=====================================================================
�I���G�t�F�N�g�p�b�V�u�֐�(cpp_func)
=====================================================================*/
void SetSelectEffectPassive()
{
	g_Func.Draw		= NoFunction;
	g_updatefunc	= NoFunction;
}