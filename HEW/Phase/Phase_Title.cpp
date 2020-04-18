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

#include "Phase_Title.h"

#include "../Title/effect.h"				// �^�C�g���G�t�F�N�g
#include "../Title/player.h"				// �v���C���[
#include "../Title/bottons.h"				// �{�^��
#include "../Title/select.h"				// �I������
#include "../Title/logo.h"					// ���S
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
static MySound		g_soundBGM;

/*=====================================================================
Title�X�V�֐�
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("�^�C�g���t�F�[�Y");


	// ���ɂ���l�p�`�G�t�F�N�g�̍X�V
	GetEffectFunc()->Update();

	if (GetFade() == FADE_NONE)
	{ // �t�F�[�h���������Ă��Ȃ��ꍇ�Ɏ��s

		// �v���C���[�̍X�V
		GetPlayerFunc()->Update();

		if (GetPlayerPosition()->z <= -105)
		{	// �v���C���[���w��ʒu�ɂ���ꍇ

			// �I���̏����Ƃ��̃G�t�F�N�g�̍X�V
			GetSelectFunc()->Update();

			// �{�^���̍X�V
			GetBottonsFunc()->Update();

			// �^�C�g�����S�̍X�V
			GetLogoFunc()->Update();
		}
	}

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

	// �v���C���[�̕`��
	GetPlayerFunc()->Draw();

	// ���͂ɕ����Ă���G�t�F�N�g�̕`��
	GetEffectFunc()->Draw();

	// ���̃��C�e�B���O��Ԃɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);

	// �{�^���̕`��
	GetBottonsFunc()->Draw();

	// �I���G�t�F�N�g�̕`�揈��
	GetSelectFunc()->Draw();

	// ���S�̕`��
	GetLogoFunc()->Draw();
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

		// BGM,SE�̓ǂݍ���
		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");

		// �G�t�F�N�g�̃��\�[�X�ǂݍ���
		GetEffectFunc()->Init(true);

		// �v���C���[�̃��\�[�X�ǂݍ���
		GetPlayerFunc()->Init(true);

		// �{�^���̃��\�[�X�ǂݍ���
		GetBottonsFunc()->Init(true);

		// �I���G�t�F�N�g�̃��\�[�X�ǂݍ���
		GetSelectFunc()->Init(true);

		// ���S�̃��\�[�X�ǂݍ���
		GetLogoFunc()->Init(true);

		return;
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

	// �v���C���[������
	GetPlayerFunc()->Init(false);

	// �{�^��������
	GetBottonsFunc()->Init(false);

	// �I���G�t�F�N�Ə�����
	GetSelectFunc()->Init(false);

	// ���S������
	GetLogoFunc()->Init(false);

	// �G�t�F�N�g������
	GetEffectFunc()->Init(false);

	MySoundPlayEternal(g_soundBGM);	// �i���Đ�

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

	// �v���C���[�I����
	GetPlayerFunc()->Uninit(false);

	// �{�^���I����
	GetBottonsFunc()->Uninit(false);

	// �I���G�t�F�N�ƏI����
	GetSelectFunc()->Uninit(false);

	// ���S�I����
	GetLogoFunc()->Uninit(false);

	// �G�t�F�N�g�I����
	GetEffectFunc()->Uninit(false);


	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------


	// �T�E���h�̊J��
	MySoundDeleteAuto(&g_soundBGM);


	// �v���C���[�J��
	GetPlayerFunc()->Uninit(true);

	// �{�^���J��
	GetBottonsFunc()->Uninit(true);

	// �I���G�t�F�N�ƊJ��
	GetSelectFunc()->Uninit(true);

	// ���S�J��
	GetLogoFunc()->Uninit(true);

	// �G�t�F�N�g�J��
	GetEffectFunc()->Uninit(true);


}


/*=====================================================================
Title��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

