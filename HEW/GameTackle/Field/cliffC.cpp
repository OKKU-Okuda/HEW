/**********************************************************************
[[�����R���v���O����(GameTackle/Field/cliffC.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "ResourceManager.h"
#include "cliffC.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffC(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffC(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffC(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffC,UpdateFieldCliffC,DrawFieldCliffC };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��


/*=====================================================================
�����R�������蔻��֐�
=====================================================================*/
bool CheckHitFieldCliffC(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x > -(FIELDROAD_X / 6) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 6) + PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���(�E���͔����ޗ��̈׏o��j

		return true;
	}

	return false;
}

/*=====================================================================
�����R���X�V�֐�
=====================================================================*/
void UpdateFieldCliffC(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
�����R���`��֐�
=====================================================================*/
void DrawFieldCliffC(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);
}

/*=====================================================================
�����R���������֐�
=====================================================================*/
void InitFieldCliffC()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 3, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));
}

/*=====================================================================
�����R���I�����֐�
=====================================================================*/
void UninitFieldCliffC()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
}

/*=====================================================================
�����R���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffCFunc()
{
	return &g_Func;
}