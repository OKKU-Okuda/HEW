/**********************************************************************
[[�t�F�[�Y:Result�v���O����(Phase_Result.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FResult�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"
#include "../Phase/Phase_GameTackle1.h"


#include "Phase_Result.h"	
#include "Phase_Title.h"

#include "../Result/meshsky.h"
#include "../Result/player.h"
#include "../Result/camera.h"
#include "../Result/score.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define ADD_RATE	(0.008f)

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
static PHASE_FUNC g_PhaseFunc = { InitResult,UninitResult,UpdateResult,DrawResult };
static float g_Rate = 0.f;		// �\���i��

static MySound g_bgm = NULL;
/*=====================================================================
Result�X�V�֐�
=====================================================================*/
void UpdateResult()
{
	PrintDebugProc("���U���g�t�F�[�Y");

	if (GetFade() == FADE_NONE)
	{// fade���I�������i���J�n
		g_Rate += ADD_RATE;
		if (g_Rate > 1.f)
		{
			g_Rate = 1.f;

			if (GetKeyboardTrigger(DIK_RETURN))
			{
				GoNextPhase(GetPhaseTitleFunc());
			}

		}

	}

	UpdateResultPlayer();
	UpdateMeshSky();

	UpdateScore();
}

 
/*=====================================================================
Result�`��֐�
=====================================================================*/
void DrawResult()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 3D
	DrawMeshSky();
	DrawResultPlayer();

	// 2D
	DrawScore();
}

/*=====================================================================
Result�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitResult(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		g_bgm = MySoundCreate("data/BGM/Result.wav");

		InitMeshSky(Vec3(0, -220, -200), Vec3(0, 0, 0), 100, 100, 600.0f, 0.f);

		InitResultPlayer(0);

		InitScore(0);
		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------
	g_Rate = 0.f;

	MySoundPlayEternal(g_bgm);

	InitResultPlayer(1);

	ResetResultCam();

	InitScore(1);
	SetCameraFunc(ShowResultPlayerCam);		// �����̃J������ݒ�
}

/*=====================================================================
Result�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitResult(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------

	MySoundStop(g_bgm);



	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------

	MySoundDelete(&g_bgm);

	UninitMeshSky();

	UninitResultPlayer();

	UninitScore();
}



/*=====================================================================
Result��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc()
{
	return &g_PhaseFunc;
}

/*=====================================================================
Result�i���擾�֐�
=====================================================================*/
float GetResultRate()
{
	return g_Rate;
}

#if 0
/*
/*�Z�Z�L�[����������|�[�Y���*/
if (g_Icon.use == true)
{
	if ((GetKeyboardTrigger(DIK_RETURN))/* || (IsButtonTriggered(0, BUTTON_C))*/)
	{

		/*TITLE*/
		if (g_Icon.pos.x == RESULT_TITLE_LOGO_POS_X)
		{
			GoNextPhase(GetPhaseTitleFunc());


			g_Icon.use = false;										//���O�r�[�A�C�R��������

			g_Icon.pos.x = ICON_MIN;									//���O�r�[�A�C�R���������ʒu�ɖ߂�

		}

		/*NEWGAME*/
		if (g_Icon.pos.x == RESULT_NEWGAME_LOGO_POS_X)
		{
			GoNextPhase(GetPhaseGameTackle1Func());



			g_Icon.use = false;										//���O�r�[�A�C�R��������

			g_Icon.pos.x = ICON_MIN;									//���O�r�[�A�C�R���������ʒu�ɖ߂�
		}

		/*EXIT*/
		if (g_Icon.pos.x == RESULT_EXIT_LOGO_POS_X)
		{
			DestroyWindow(GetHandle());
		}
	}

	/*���L�[*/
	if (GetKeyboardTrigger(DIK_LEFT)/* || IsButtonTriggered(0, BUTTON_UP)*/)
	{

		if (g_Icon.pos.x >= ICON_MIN)
		{
			g_Icon.pos.x -= ICON_REN;
		}
		if (g_Icon.pos.x < ICON_MIN)
		{
			g_Icon.pos.x = ICON_MAX;
		}
	}

	/*�E�L�[*/
	if (GetKeyboardTrigger(DIK_RIGHT)/* || IsButtonTriggered(0, BUTTON_DOWN)*/)
	{

		if (g_Icon.pos.x <= ICON_MAX)
		{
			g_Icon.pos.x += ICON_REN;
		}
		if (g_Icon.pos.x > ICON_MAX)
		{
			g_Icon.pos.x = ICON_MIN;
		}
	}
	SetVertexResultIcon();

}
*/
#endif
