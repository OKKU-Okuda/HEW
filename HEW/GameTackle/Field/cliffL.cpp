/**********************************************************************
[[���R���v���O����(GameTackle/Field/cliffL.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "ResourceManager.h"
#include "cliffL.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffL(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffL(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffL(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffL,UpdateFieldCliffL,DrawFieldCliffL };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;	// �����̔�

/*=====================================================================
���R�������蔻��֐�
=====================================================================*/
bool CheckHitFieldCliffL(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R && pPastPos->x >(FIELDROAD_X / 6) - PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���(�����͔����ޗ��̈׏o��j

		if (pPos->x > FIELDROAD_X / 2)
		{
			pPos->x = FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
���R���X�V�֐�
=====================================================================*/
void UpdateFieldCliffL(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
���R���`��֐�
=====================================================================*/
void DrawFieldCliffL(FIELD_CHIP* pData)
{
	D3DDEVICE;


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));
	g_meshFlat->DrawSubset(0);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_WALL));
	GetFieldShareMesh(FMESH_LONGWALLRIGHT)->DrawSubset(0);
}

/*=====================================================================
���RAwake�֐�
=====================================================================*/
void AwakeFieldCliffL(FIELD_CHIP* pData)
{
	if (CheckRand(0.5f) == true)
	{// �R�C���̐ݒu
		const int numCoin = 10;													// �R�C���̐ݒu��
		const float itv_coin = FIELDCHIP_HEIGHT / (numCoin + 1);				// �R�C���ݒu�Ԋu
		const float PosX = rand() % (int)(FIELDROAD_X / 3) - (FIELDROAD_X / 6) + (FIELDROAD_X / 2 - FIELDROAD_X / 6);	// �R�C���ݒu��

		for (int i = 0; i < numCoin; i++)
		{
			Vec3 pos(PosX, POSY_COIN, -FIELDCHIP_CENTER_Z + ((i+1) * itv_coin));		// �ݒu�ʒu

			D3DXVec3TransformCoord(&pos, &pos, &pData->WldMat);			// �ʒu�����[���h���W�ɕϊ�

			SetItem(pData, pos, Vec3(0, 0.01f * i, 0));							// �R�C���ݒu
		}

	}

}

/*=====================================================================
���R���������֐�
=====================================================================*/
void InitFieldCliffL()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 3, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(FIELDROAD_X/2 - FIELDROAD_X / 6, 0, 0));
}

/*=====================================================================
���R���I�����֐�
=====================================================================*/
void UninitFieldCliffL()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
}

/*=====================================================================
���R���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffLFunc()
{
	return &g_Func;
}