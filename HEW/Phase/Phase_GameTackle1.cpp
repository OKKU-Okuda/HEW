/**********************************************************************
[[�t�F�[�Y:GameTackle1�v���O����(Phase_GameTackle1.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FGameTackle1�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "Phase_GameTackle1.h"	
#include "Phase_GameTackle2.h"

#include "../GameTackle/player.h"
#include "../GameTackle/enemy.h"
#include "../GameTackle/camera.h"
#include "../GameTackle/field.h"
#include "../GameTackle/shadow.h"
#include "../GameTackle/kiseki.h"

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
static PHASE_FUNC g_PhaseFunc = { InitGameTackle1,UninitGameTackle1,UpdateGameTackle1,DrawGameTackle1 };

/*=====================================================================
GameTackle1�X�V�֐�
=====================================================================*/
void UpdateGameTackle1()
{
	PrintDebugProc("�^�b�N���P�t�F�[�Y");

	// ���̃t�F�[�Y�ɍs��
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// �^�b�N��2
		GoNextPhase(GetPhaseGameTackle2Func());
	}

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �O�Ղ̍X�V
	UpdateKiseki();

	// �e�̍X�V
	UpdateShadow();

	// �G�l�~�[�̍X�V
	GetTackleEnemyFunc()->Update();

	// �t�B�[���h�̍X�V
	UpdateField();
}

/*=====================================================================
GameTackle1�`��֐�
=====================================================================*/
void DrawGameTackle1()
{
	// �t�B�[���h�̕`��
	DrawField();


	// �e�̕`��
	DrawShadow();

	// ���f���̕`�揈��
	DrawPlayer();

	// �O�Ղ̕`��
	DrawKiseki();

	// �G�l�~�[�̕`��
	GetTackleEnemyFunc()->Draw();


}

/*=====================================================================
GameTackle1�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitGameTackle1(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------

		//�e�̏�����
		InitShadow();

	// �v���C���[�̏�����
		InitPlayer();

		//�O�Ղ̏�����
		InitKiseki(0);

		// �t�B�[���h�̏�����
		InitField();

		// �G�l�~�[�̓ǂݍ���
		GetTackleEnemyFunc()->Init(true);

		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------


	// �G�l�~�[�̏�����
	GetTackleEnemyFunc()->Init(false);

	// �t�B�[���h�̃��Z�b�g����
	ResetField();


	// �J�����X�V�֐��̐ݒ�
	GetCamera()->length = 30.0f;
	GetCamera()->rotXZ = 0.0f;
	SetCameraFunc(TackleCameraUpdate);
}

/*=====================================================================
GameTackle1�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitGameTackle1(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------

	// �G�l�~�[�̏I����
	GetTackleEnemyFunc()->Uninit(false);


	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�l�~�[�̊J��
	GetTackleEnemyFunc()->Uninit(false);

	// �t�B�[���h�̏I����
	UninitField();
}

/*=====================================================================
GameTackle1��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle1Func()
{
	return &g_PhaseFunc;
}