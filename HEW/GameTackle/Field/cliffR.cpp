/**********************************************************************
[[�E�R���v���O����(GameTackle/Field/cliffR.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "ResourceManager.h"
#include "cliffR.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffR,UpdateFieldCliffR,DrawFieldCliffR };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��


/*=====================================================================
�E�R�������蔻��֐�
=====================================================================*/
bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 6) + PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���(�E���͔����ޗ��̈׏o��j

		if (pPos->x < -FIELDROAD_X / 2)
		{
			pPos->x = -FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
�E�R���X�V�֐�
=====================================================================*/
void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
�E�R���`��֐�
=====================================================================*/
void DrawFieldCliffR(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	GetFieldShareMesh(FMESH_LONGWALLLEFT)->DrawSubset(0);
}

/*=====================================================================
���RAwake�֐�
=====================================================================*/
void AwakeFieldCliffR(FIELD_CHIP* pData)
{
	if (CheckRand(0.5f) == true)
	{// �R�C���̐ݒu
		const int numCoin = 10;													// �R�C���̐ݒu��
		const float itv_coin = FIELDCHIP_HEIGHT / (numCoin + 1);				// �R�C���ݒu�Ԋu
		const float PosX = rand() % (int)(FIELDROAD_X / 3) - (FIELDROAD_X / 6) - (FIELDROAD_X / 2 - FIELDROAD_X / 6);	// �R�C���ݒu��

		for (int i = 0; i < numCoin; i++)
		{
			Vec3 pos(PosX, POSY_COIN, -FIELDCHIP_CENTER_Z + ((i + 1) * itv_coin));		// �ݒu�ʒu

			D3DXVec3TransformCoord(&pos, &pos, &pData->WldMat);			// �ʒu�����[���h���W�ɕϊ�

			SetItem(pData, pos, Vec3(0, 0.01f * i, 0));							// �R�C���ݒu
		}

	}

}

/*=====================================================================
�E�R���������֐�
=====================================================================*/
void InitFieldCliffR()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 3, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(-(FIELDROAD_X / 2 - FIELDROAD_X / 6), 0, 0));
}

/*=====================================================================
�E�R���I�����֐�
=====================================================================*/
void UninitFieldCliffR()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
}

/*=====================================================================
�E�R���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffRFunc()
{
	return &g_Func;
}