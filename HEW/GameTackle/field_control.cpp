/**********************************************************************
[[�t�B�[���h�̏o���Ǘ��v���O����(field_control.cpp)]]
	��ҁF���c�@�^�K

	�t�B�[���h�̃|�b�v�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"

#include "Gimmick/Gimmick.h"

#include "field.h"
#include "player_control.h"
#include "field_control.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define RATE_SPAWN_TURN		(0.1f)			// ���򓹂̏o����
#define LIMIT_SPAWN_TURN	(40)			// 40�񒼐��n�̓����쐬�����ꍇ��pool���ɍl�����ċ����I�ɕ��򓹂��o��������
//#define RATE_SPAWN_TURN		(1.f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static CHIP_ID g_idlate;		// �Ō�ɐݒu��������CHIPID(����n�͕ʁj

/*=====================================================================
�t�B�[���hCHIP�����֐�
�������̓v���C���[direction�Ɉˑ�����
�����Fid_start	�����J�nID���w��(��{�I�ɂ̓v���C���[�̍�����ʒu���w��)
=====================================================================*/
void SpawnField(CHIP_ID id_start)
{
	CHIP_ID addID = GetFieldIDVector(GetPlayerDirection());
	FIELD_TYPE Latesttype = FTYPE_VOID;
	int cntNormal = 0;
	while (1)
	{
		int numRand;		//��CURVE���ݒu���̃^�C�v������ۊ�

		// 2����ID�̐V�K�ꏊ�X�V
		id_start.vec2.x += addID.vec2.x;
		id_start.vec2.z += addID.vec2.z;

		// ���򓹂̊����ݒu(hit�������I��)
		if (CheckRand(RATE_SPAWN_TURN) == true || cntNormal >= LIMIT_SPAWN_TURN)
		{
			numRand = rand() % NUM_FTYPE_CURVE + START_FTYPE_CURVE;

			FIELD_TYPE type;
			// ���򓹂͂R�Ȃ̂Ō�����
			if (numRand == FTYPE_TURNL)
			{
				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// ���ɑ������̕���
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// ���ɑ�������id
				SetField(idleft, FTYPE_ROAD, fdirleft);		// ���򓹂̐ݒu

				type = FTYPE_TURNL;
			}
			else if (numRand == FTYPE_TURNR)
			{
				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// ���ɑ������̕���
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// ���ɑ�������id
				SetField(idright, FTYPE_ROAD, fdirright);		// ���򓹂̐ݒu

				type = FTYPE_TURNR;
			}
			else
			{
				type = FTYPE_TURNLR;

				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// ���ɑ������̕���
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// ���ɑ�������id
				SetField(idleft, FTYPE_ROAD, fdirleft);		// ���򓹂̐ݒu

				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// ���ɑ������̕���
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// ���ɑ�������id
				SetField(idright, FTYPE_ROAD, fdirright);		// ���򓹂̐ݒu

			}

			SetField(id_start, type, GetPlayerDirection());		// ���򓹂̐ݒu

			break;	// ���򓹌�͐ݒu���Ȃ�
		}

		do {
			numRand = rand() % NUM_FTYPE_NORMAL + START_FTYPE_NORMAL;
		} while (Latesttype == numRand);	// �O��Ɠ������̂����Ȃ�
		//numRand = FTYPE_CLIFFC;

		SetField(id_start, (FIELD_TYPE)numRand, GetPlayerDirection());		// ���̐ݒu
		Latesttype = (FIELD_TYPE)numRand;
		cntNormal++;
	}
	g_idlate = id_start;
}

/*=====================================================================
�t�B�[���h����X�V�֐�
=====================================================================*/
void UpdateFieldControl()
{
	
	
}

/*=====================================================================
�t�B�[���h�ŏI1�擾�֐�
=====================================================================*/
Vec3 GetLatestFieldPosition()
{
	Vec3 ans;

	ans.x = FIELDCHIP_CENTER_X + (FIELDCHIP_WIDTH * g_idlate.vec2.x);
	ans.z = FIELDCHIP_CENTER_Z + (FIELDCHIP_HEIGHT * g_idlate.vec2.z);
	ans.y = 0.f;
	return ans;
}


