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

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_Sound;
static Model		g_mdlPlayer;


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
	g_mdlPlayer->WldMtx._44 -= 0.01f;
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
	DrawModel(g_mdlPlayer);
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
		g_Sound = MySoundCreate("data/BGM/bgm000.wav");

		g_mdlPlayer = CreateModel("data/MODEL/Player.x");

	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	MySoundPlayEternal(g_Sound);	// �i���Đ�

	GetMatrix(&g_mdlPlayer->WldMtx);

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
	}

}

/*=====================================================================
Title��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}