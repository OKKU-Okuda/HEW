/**********************************************************************
[[Select�v���O����(Select.cpp)]]
	��ҁF���c�@�^�K

	�^�C�g���{�^���I���v���O����
***********************************************************************/
#include "../Core/fade.h"
#include "../Core/camera.h"

#include "../Phase/Phase_GameTackle1.h"
#include "../Phase/Phase_Title.h"

#include "select.h"	
#include "bottons.h"
#include "effect.h"
#include "camera.h"
#include "config.h"

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
static OBJ_FUNC g_Func = { InitSelect,UninitSelect,NoFunction,NoFunction };

// �Z���N�g�G�t�F�N�g
static NormalFunc	g_updatefunc;		// �G�t�F�N�g�X�V�֐�
static VERTEX_2D	g_vtx[NUM_VERTEX];	// ���S���_
static Texture		g_tex;				// �e�N�X�`��
static DWORD		g_cnt_show;			// �\���J�E���g

static MySound		g_soundSelect;		// �I����

static DWORD		g_Select;			// �{�^��

static bool g_isFirstError = false;
/*=====================================================================
Select���擾�֐�
=====================================================================*/
MySound GetSoundSelect()
{
	return g_soundSelect;
}

/*=====================================================================
Select�X�V�֐�
=====================================================================*/
void UpdateSelect()
{
	if (GetCameraRate() != 0.f)
	{
		return;
	}

	// �㉺�̑I��
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		g_Select = (g_Select - 1) % MAX_TITLESELECT;
		SetTitle3DRot(true);
		SetBottonScaleChanged();
		SetSelectEffectActive();
	}
	else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		g_Select = (g_Select + 1) % MAX_TITLESELECT;
		SetTitle3DRot(false);
		SetBottonScaleChanged();
		SetSelectEffectActive();
	}

	// �I���{�^���ɂ���ĕς�鏈��
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
	{
		switch (g_Select)
		{
		case SELECT_STRAT:	// �^�b�N��1�t�F�[�Y�Ɉڍs
			GoNextPhase(GetPhaseGameTackle1Func());
			break;

		case SELECT_RANKING:
			if (g_isFirstError == false)
			{
				MessageBox(GetHandle(), "�����L���O�͍����݁A�������ł��I", "SORRY!!( �M�[�L)�m", 0);
				g_isFirstError = true;
			}
			break;
		case SELECT_CONFIG:
			ResetConfig();
			SetSelectEffectPassive();
			SetSubTitleMenu(true);
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
	if (GetCameraRate() != 0.f)
	{
		return;
	}

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
		
		// �Z���N�g�T�E���h�̓ǂݍ���
		g_soundSelect = MySoundCreate("data/SE/select.wav");
		MySoundSetVolume(g_soundSelect, 3.0f);

		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	// �G�t�F�N�g���_�̐ݒu
	MakeNormal2DVertex(0, g_vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &SIZE_SELECTEFFECT);

	SetSelectEffectPassive();

	g_Select = SELECT_STRAT;
	g_isFirstError = false;

	g_Func = { InitSelect,UninitSelect,NoFunction,NoFunction };
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
	MySoundStop(g_soundSelect);

	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------

	SAFE_RELEASE(g_tex);

	MySoundDeleteAuto(&g_soundSelect);

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

	// �T�E���h�̍Đ�
	MySoundPlayOnce(g_soundSelect);
}

/*=====================================================================
�I���G�t�F�N�g�p�b�V�u�֐�(cpp_func)
=====================================================================*/
void SetSelectEffectPassive()
{
	g_Func.Draw		= NoFunction;
	g_updatefunc	= NoFunction;
}

/*=====================================================================
�I���G�t�F�N�g�X�V�֐��A�N�e�B�u�֐�
=====================================================================*/
void SetSelectFuncActive()
{
	g_Func.Update = UpdateSelect;
}