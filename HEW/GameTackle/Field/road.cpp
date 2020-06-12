/**********************************************************************
[[���i���v���O����(GameTackle/Field/road.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../Gimmick/Wall.h"
#include "ResourceManager.h"
#include "road.h"
#include "../item.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldRoad(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldRoad,UpdateFieldRoad,DrawFieldRoad };	// ���Ǝ��̊֐�

/*=====================================================================
�����������蔻��֐�
=====================================================================*/
bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���

		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2,FIELDROAD_X / 2);
		return true;
	}

	return false;
}

/*=====================================================================
�������X�V�֐�
=====================================================================*/
void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
�������`��֐�
=====================================================================*/
void DrawFieldRoad(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);


	GetFieldShareMesh(FMESH_LONGFLAT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_LONGWALLLEFT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_LONGWALLRIGHT)->DrawSubset(0);

}

/*=====================================================================
������Awake�֐�
=====================================================================*/
void AwakeFieldRoad(FIELD_CHIP* pData)
{
	if (CheckRand(0.2f) == true)
	{// �R�C���̐ݒu
		const int numCoin = 20;													// �R�C���̐ݒu��
		const float itv_coin = FIELDCHIP_HEIGHT / (numCoin + 1);				// �R�C���ݒu�Ԋu
		const float PosX = rand() % (int)FIELDROAD_X - (FIELDROAD_X / 2);		// �R�C���ݒu��

		for (int i = 0; i < numCoin; i++)
		{
			Vec3 pos(PosX, POSY_COIN, -FIELDCHIP_CENTER_Z + (i * itv_coin));		// �ݒu�ʒu

			D3DXVec3TransformCoord(&pos, &pos, &pData->WldMat);			// �ʒu�����[���h���W�ɕϊ�

			SetItem(pData,pos, Vec3(0, 0, 0));							// �R�C���ݒu
		}

	}
	else if (CheckRand(0.5f) == true)
	{// GIMMICK:�ǂ̐ݒu
		int typeWall = rand() % MAX_GIMICKWALLQTE;
		SetFieldGimmick(pData, GTYPE_WALL, &Vec3(0, 0, 250.0f), typeWall);
	}
}

/*=====================================================================
�������������֐�
=====================================================================*/
void InitFieldRoad()
{
}

/*=====================================================================
�������I�����֐�
=====================================================================*/
void UninitFieldRoad()
{
}

/*=====================================================================
�������Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldRoadFunc()
{
	return &g_Func;
}