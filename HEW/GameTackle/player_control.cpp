/**********************************************************************
[[�v���C���[�s���i����n)�v���O����(player_control.cpp)]]
	��ҁF���c�@�^�K

	�v���C���[�̑���ʁi��]�ʁj�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "player.h"
#include "field.h"
#include "player_control.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define	VALUE_MOVE			(5.0f)							// �ړ���

#define ADDROT	(0.1f)				// ��]��
#define ADDPOS	(2.0f)				// �ړ���
#define ADDXPOS		(3.0f)			// ���ړ�
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static void UpdatePlayerRotation();		// ��]�`�F�b�N�֐�
static void UpdatePlayerTranslation();	// �ʒu�`�F�b�N�֐�(QTE�n�����͏���)

#ifdef _DEBUG
static void UpdatePlayerDebug();		// �f�o�b�O�ړ��֐�
#endif
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static FIELD_DIRECTION		g_dirPlayer;		// �v���C���[�̃t�B�[���h�I����

static bool					g_isRotation;		// �v���C���[��]����

#ifdef _DEBUG
static bool g_isDebugControl = false;// (debug)����Ɍ�����t���Ď��R�ɑ���ł���悤�ɂ���
static float g_spd = 0.0f;
#endif

/*=====================================================================
�v���C���[����X�V�֐�
=====================================================================*/
void UpdatePlayerControl()
{
	UpdatePlayerRotation();
	UpdatePlayerTranslation();
}

/*=====================================================================
�v���C���[���Z�b�g�֐�
=====================================================================*/
void ResetPlayerControl()
{
	g_dirPlayer = FDIRECTION_0ZP;
	g_isRotation = true;

#ifdef _DEBUG
	g_isDebugControl = false;// (debug)����Ɍ�����t���Ď��R�ɑ���ł���悤�ɂ���
	g_spd = 0.0f;
#endif

}

/*=====================================================================
�v���C���[�����ݒ�֐�
=====================================================================*/
void SetPlayerDirection(FIELD_DIRECTION fdir)
{
	g_dirPlayer = fdir;
	g_isRotation = true;	// ��]flag�𗧂Ă�
}

/*=====================================================================
cpp�Ȃ��֐�
=====================================================================*/

void UpdatePlayerRotation()
{
	float rotY = (D3DX_PI / 2)*(int)g_dirPlayer + D3DX_PI;		// ����ׂ�������]
	float rotYfrom = GetPlayerRot()->y;					// ��]�J�n�p�x
	float rotVec = rotY - rotYfrom;						// ��]����

	if (g_isRotation == false)
	{// ��]flag��false�̏ꍇ�͂��̏������΂�
		return;
	}

	// ��]�ʂ̍�������ꍇ�̓v���C���[�̃��b�g��ŖړI��]�l�����߂�
	if (rotVec > D3DX_PI)
	{
		rotY -= D3DX_PI * 2;
		rotVec = rotY - rotYfrom;
	}
	else if (rotVec < -D3DX_PI)
	{
		rotY += D3DX_PI * 2;
		rotVec = rotY - rotYfrom;
	}

	// ��]�l���قȂ�ꍇ�̏���
	if (GetPlayerRot()->y != rotY)
	{

		float addrot = ADDROT;

		if (fabsf(rotVec) < ADDROT)
		{// ��]�ʂ��w���]�ȏ����Ă��܂��ꍇ�͒���(��]flag��false�Ɂj
			g_isRotation = false;

			GetPlayerRot()->y = rotY;
		}
		else
		{
			g_isRotation = true;

			if (rotVec < 0)
			{// �}�C�i�X��]�ł���Ή��Z�ʂ̕����𔽓]����
				addrot *= -1;
			}

			GetPlayerRot()->y += addrot;
		}

		// ��]�̐�������
		if (GetPlayerRot()->y > D3DX_PI * 2)
		{
			GetPlayerRot()->y -= D3DX_PI * 2;
		}
		else if (GetPlayerRot()->y < 0)
		{
			GetPlayerRot()->y += D3DX_PI * 2;
		}

		return;
	}
	
	g_isRotation = false;
}

void UpdatePlayerTranslation()
{
	if (g_isRotation == true)
	{// ��]���͈ړ��������Ȃ�
		return;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F6))
	{// �R���g���[�����[�h���]
		g_isDebugControl = !g_isDebugControl;
		g_isRotation = true;
	}

	if (g_isDebugControl == true)
	{
		PrintDebugProc("[debug_player]F6:�f�o�b�N�R���g���[���𖳌��ɂ���(���ݗL��)");
		
		UpdatePlayerDebug();
		return;
	}

	PrintDebugProc("[debug_player]F6:�f�o�b�N�R���g���[����L���ɂ���(���ݖ���)");
	
#endif

	// ���ړ��ړ�
	*GetPlayerPos() += GetFieldVector(g_dirPlayer) * ADDPOS;

	// ���ړ�����
	if (GetKeyboardPress(DIK_A))
	{
		*GetPlayerPos() += GetFieldVector(AddFieldDirection(g_dirPlayer, -1)) * ADDXPOS;
	}

	if (GetKeyboardPress(DIK_D))
	{
		*GetPlayerPos() += GetFieldVector(AddFieldDirection(g_dirPlayer, 1)) * ADDXPOS;
	}

}

#ifdef _DEBUG
void UpdatePlayerDebug()
{

	//�ړ�����
	int		dir = 0;	// �O�F�����͕ς��Ȃ�

	if (GetKeyboardPress(DIK_LEFT))
	{
		dir |= 8;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		dir |= 4;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_spd = VALUE_MOVE;
	}

	// ���͂��ꂽ�L�[�ɍ��킹�Č��������߂�
	float roty = 0.0f;
	switch (dir)
	{
	case 1:	// ������
		roty = 0.0f;
		break;

	case 2:	// �����
		roty = D3DX_PI;
		break;

	case 4:	// �E����
		roty = -D3DX_PI / 2;
		break;

	case 8:	// ������
		roty = D3DX_PI / 2;
		break;

	case 5:	// �E������
		roty = -D3DX_PI / 4;
		break;

	case 6:	// �E�����
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// ��������
		roty = D3DX_PI / 4;
		break;

	case 10: // �������
		roty = D3DX_PI / 4 * 3;
		break;

	case 0:
		roty = GetPlayerRot()->y;
		break;

	default:
		break;


	}

	GetPlayerRot()->y = roty;


	g_spd *= 0.9f;// ����

	//�ړ�����[[��������ڲ԰���̃|�W�V�����m��]]
	GetPlayerPos()->x -= sinf(GetPlayerRot()->y) * g_spd;
	GetPlayerPos()->z -= cosf(GetPlayerRot()->y) * g_spd;

}
#endif