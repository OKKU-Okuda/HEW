/**********************************************************************
[[TackleEnemy�v���O����(TackleEnemy.cpp)]]
	��ҁF���c�@�^�K

	TackleEnemy�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"
#include "../Core/input.h"

#include "enemy.h"	
#include"player.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

// ��{�֐��Q
static void InitTackleEnemy(bool isFirst);
static void UninitTackleEnemy(bool isEnd);
static void UpdateTackleEnemy();
static void DrawTackleEnemy();
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static OBJ_FUNC g_Func = { InitTackleEnemy,UninitTackleEnemy,UpdateTackleEnemy,DrawTackleEnemy };

// �G�l�~�[���f��
static Model	g_modelEnemy;

/*=====================================================================
TackleEnemy�X�V�֐�
=====================================================================*/
void UpdateTackleEnemy()
{

	// �v���C���[�̉��ɃG�l�~�[��`�悷��悤�ɂ���
	g_modelEnemy->WldMtx._41 = GetPlayerPositionX()->x;
	g_modelEnemy->WldMtx._42 = GetPlayerPositionX()->y;
	g_modelEnemy->WldMtx._43 = GetPlayerPositionX()->z;

}

/*=====================================================================
TackleEnemy�`��֐�
=====================================================================*/
void DrawTackleEnemy()
{
	DrawModel(g_modelEnemy);
}

/*=====================================================================
TackleEnemy�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitTackleEnemy(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------

		// �G�l�~�[���f���̓ǂݍ���
		g_modelEnemy = CreateModel("data/MODEL/enemy.x");
		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------
}

/*=====================================================================
TackleEnemy�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitTackleEnemy(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------





	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------


	// ���f���J��
	DeleteModel(&g_modelEnemy);

}

/*=====================================================================
TackleEnemy��{�֐��Q�擾�֐�
=====================================================================*/
OBJ_FUNC* GetTackleEnemyFunc()
{
	return &g_Func;
}