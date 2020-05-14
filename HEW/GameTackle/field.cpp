/**********************************************************************
[[field�v���O����(field.cpp)]]
	��ҁF���c�@�^�K

	�t�B�[���h�S�ʂɊւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "field.h"
#include "player.h"

#include "Field/road.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

// �v���C���[�̕����w��p�x
#define PLAYERDIRECTION_ZP
#define PLAYERDIRECTION_ZM

#define PLAYERDIRECTION_XP
#define PLAYERDIRECTION_XM

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
CHIP_ID GetFieldChipID(Vec3* pos);
FIELD_CHIP* GetChipMemory();
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static FIELD_CHIP g_Field[MAX_FIELD];
		
static FIELD_CHIP* g_pOnField = NULL;		// �v���C���[���W�I�ɂ���CHIP

/*=====================================================================
�����֐�
	��������֐�
	�߂�l : void
	���� : void
=====================================================================*/
void SetField(Vec2_Int *p_pos, FIELD_TYPE type, float rot)
{

}

void UpdateField()
{
	PLAYER* player = GetPlayer();
	CHIP_ID	id;
	Matrix mat;
	Vec3	pos;

	id = GetFieldChipID(&player->pos);

	PrintDebugProc("�v���C���[�`�����N�Z�o X:%d Z:%d", id.vec2.x, id.vec2.z);

	D3DXMatrixInverse(&mat, NULL, &g_pOnField->WldMat);
	D3DXVec3TransformCoord(&pos, &GetPlayer()->pos, &mat);

	PrintDebugProc("�`�����N���W%vec3", pos);

	if (g_pOnField->pFunc->CheckHit(g_pOnField, &pos))
	{
		PrintDebugProc("---�`�����N��---");
	}

	D3DXVec3TransformCoord(&GetPlayer()->pos, &pos, &g_pOnField->WldMat);

}

void PlayerCheckHitOnField()
{
	PLAYER* player = GetPlayer();
	CHIP_ID	id;

	id = GetFieldChipID(&player->pos);

	PrintDebugProc("�v���C���[�`�����N�Z�o X:%d Z:%d", id.vec2.x, id.vec2.z);

//	D3DXMatrixInverse(&invmat, NULL, &pData->WldMat);
//	D3DXVec3TransformCoord(&pos, &GetPlayer()->pos, &invmat);

}

void DrawField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == FSTATE_NONE)
		{
			continue;
		}

		// �`��
		g_Field[i].pFunc->Draw(&g_Field[i]);
	}
}

void InitField()
{
	InitFieldRoad();

	//�e�X�g
	g_pOnField= GetChipMemory();
	g_pOnField->pFunc = GetFieldRoadFunc();
	g_pOnField->State = FSTATE_READY;
	GetMatrix(&g_pOnField->WldMat, &Vec3(FIELDCHIP_WIDTH*0.5f, 0, FIELDCHIP_WIDTH*0.5f));

	g_pOnField = GetChipMemory();
	g_pOnField->pFunc = GetFieldRoadFunc();
	g_pOnField->State = FSTATE_READY;
	GetMatrix(&g_pOnField->WldMat, &Vec3(FIELDCHIP_WIDTH*0.5f, 0, FIELDCHIP_WIDTH*1.5f),
		&Vec3(0, D3DX_PI / 2, 0));

	// �v���C���[�̂Ƃ���ɂ���������Ă��炤
	GetPlayer()->pos.x = PLAYER_POSX;
	GetPlayer()->pos.z = PLAYER_POSZ;
	GetPlayer()->rot.y = D3DX_PI;
}

void UninitField()
{

}

void ResetField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// ���g�p���
		g_Field[i].State = FSTATE_NONE;
	}

}

CHIP_ID GetFieldChipID(Vec3* pos)
{
	CHIP_ID ans;

	// �ʒu���`�����N�T�C�Y�Ŋ����Đ�����
	ans.vec2.x = (short)(pos->x / FIELDCHIP_WIDTH);
	ans.vec2.z = (short)(pos->z / FIELDCHIP_HEIGHT);

	// ���̏ꍇ-0�͊댯�Ȃ̂ł��ꂼ��1�}�C�i�X����
	if (pos->x < 0)
	{
		ans.vec2.x--;
	}

	if (pos->z < 0)
	{
		ans.vec2.z--;
	}

	return ans;
}

FIELD_CHIP* GetChipMemory()
{
	// ����
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State != FSTATE_NONE)
		{// �g�p���̂̓X�L�b�v
			continue;
		}


		return &g_Field[i];
	}

	MessageBox(0, 0, 0, 0);

	// ���ׂĎg�p���Ă����ꍇ��null��Ԃ�
	return NULL;
}