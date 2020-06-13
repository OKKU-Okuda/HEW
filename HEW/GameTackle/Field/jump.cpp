/**********************************************************************
[[�W�����v���v���O����(GameTackle/Field/jump.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../../Core/CheckHit.h"

#include "../field.h"
#include "ResourceManager.h"
#include "jump.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define JUMPVOID_ITV		(300.0f)		// �����ɐݒu���闎�Ƃ����̊Ԋu

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldJump(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldJump(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldJump(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldJump,UpdateFieldJump,DrawFieldJump };	// ���Ǝ��̊֐�

// ���ł��2�ɂ��ĊԂ�ޗ��ɂ���(0��O�A1��)
static Mesh g_meshFlat[2];		// ���̐^��

static Mesh g_meshRightWall[2];	// �E�̕�
static Mesh g_meshLeftWall[2];	// ���̕�

/*=====================================================================
�W�����v�������蔻��֐�
=====================================================================*/
bool CheckHitFieldJump(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	if ((pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R) &&
		(pPastPos->z < (-JUMPVOID_ITV / 2) + PLAYER_FIELDSIZE_R || pPastPos->z > (JUMPVOID_ITV / 2) - PLAYER_FIELDSIZE_R))
	{	// (�n�ʃ��b�V���̂���ӏ��̂�)�O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���

		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	return false;
}

/*=====================================================================
�W�����v���X�V�֐�
=====================================================================*/
void UpdateFieldJump(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
�W�����v���`��֐�
=====================================================================*/
void DrawFieldJump(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	for (int i = 0; i < 2; i++)
	{
		g_meshFlat[i]->DrawSubset(0);

		g_meshRightWall[i]->DrawSubset(0);
		g_meshLeftWall[i]->DrawSubset(0);
	}
}

/*=====================================================================
jannpuAwake�֐�
=====================================================================*/
void AwakeFieldJump(FIELD_CHIP* pData)
{
	if (CheckRand(1.f) == true)
	{// �R�C���̐ݒu
		const int numCoin = 20;													// �R�C���̐ݒu��
		const float itv_coin = FIELDCHIP_HEIGHT / (numCoin + 1);				// �R�C���ݒu�Ԋu
		const float PosX = rand() % (int)FIELDROAD_X - (FIELDROAD_X / 2);		// �R�C���ݒu��

		for (int i = 0; i < numCoin; i++)
		{
			Vec3 pos(PosX, POSY_COIN, -FIELDCHIP_CENTER_Z + ((i + 1) * itv_coin));		// �ݒu�ʒu

			if (pos.z > -(JUMPVOID_ITV / 2) && pos.z < (JUMPVOID_ITV / 2))
			{
				pos.y *= 8;
			}

			D3DXVec3TransformCoord(&pos, &pos, &pData->WldMat);			// �ʒu�����[���h���W�ɕϊ�

			SetItem(pData, pos, Vec3(0, 0.01f * i, 0));							// �R�C���ݒu
		}

	}

}

/*=====================================================================
�W�����v���������֐�
=====================================================================*/
void InitFieldJump()
{
	D3DDEVICE;

	const float sizeZ = (FIELDCHIP_HEIGHT - JUMPVOID_ITV) / 2;
	const float posZ = (JUMPVOID_ITV / 2) + (sizeZ / 2);

	// ���̏������쐬
	g_meshFlat[0] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ),
		&Vec3(0, 0, posZ));

	g_meshFlat[1] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ),
		&Vec3(0, 0, -posZ));

	// ���E�̕Ǎ쐬
	g_meshRightWall[0] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), posZ));
	g_meshLeftWall[0] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), posZ));

	g_meshRightWall[1] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));
	g_meshLeftWall[1] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

}

/*=====================================================================
�W�����v���I�����֐�
=====================================================================*/
void UninitFieldJump()
{
	// ���\�[�X�̊J��
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(g_meshFlat[i]);
		SAFE_RELEASE(g_meshLeftWall[i]);
		SAFE_RELEASE(g_meshRightWall[i]);
	}
}

/*=====================================================================
�W�����v���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldJumpFunc()
{
	return &g_Func;
}