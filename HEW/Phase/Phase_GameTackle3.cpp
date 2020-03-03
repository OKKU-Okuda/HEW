/**********************************************************************
[[�t�F�[�Y:GameTackle3�v���O����(Phase_GameTackle3.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FGameTackle3�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "Phase_GameTackle3.h"	
#include "Phase_GameKick.h"
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
static PHASE_FUNC g_PhaseFunc = { InitGameTackle3,UninitGameTackle3,UpdateGameTackle3,DrawGameTackle3 };

/*=====================================================================
GameTackle3�X�V�֐�
=====================================================================*/
void UpdateGameTackle3()
{
	PrintDebugProc("�^�b�N��3�t�F�[�Y");

	// ���̃t�F�[�Y�ɍs��
	if (GetKeyboardTrigger(DIK_RETURN))
	{	// �L�b�N
		GoNextPhase(GetPhaseGameKickFunc());
	}

}

/*=====================================================================
GameTackle3�`��֐�
=====================================================================*/
void DrawGameTackle3()
{

}

/*=====================================================================
GameTackle3�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitGameTackle3(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------

	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------


}

/*=====================================================================
GameTackle3�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitGameTackle3(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------




	if (isEnd == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�J������
		//---------------------------------------------------------------------

	}

}

/*=====================================================================
GameTackle3��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle3Func()
{
	return &g_PhaseFunc;
}