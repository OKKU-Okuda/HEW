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

#define RATE_SPAWN_TURN		(0.1f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
//static CHIP_ID g_idlate;		// �Ō�ɐݒu����CHIPID(����n�͕ʁj

/*=====================================================================
�t�B�[���hCHIP�����֐�
�������̓v���C���[direction�Ɉˑ�����
�����Fid_start	�����J�nID���w��(��{�I�ɂ̓v���C���[�̍�����ʒu���w��)
=====================================================================*/
void SpawnField(CHIP_ID id_start)
{
	CHIP_ID addID = GetFieldIDVector(GetPlayerDirection());

	while (1)
	{
		int numRand;		//��CURVE���ݒu���̃^�C�v������ۊ�

		// 2����ID�̐V�K�ꏊ�X�V
		id_start.vec2.x += addID.vec2.x;
		id_start.vec2.z += addID.vec2.z;

		// ���򓹂̊����ݒu(hit�������I��)
		if (CheckRand(RATE_SPAWN_TURN) == true)
		{
			FIELD_TYPE type;
#if 0		// ������nCURVE���͖�����
			// ���򓹂͂R�Ȃ̂Ō�����
			if (CheckRand(1 / 3.0f) == true)
			{
				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// ���ɑ������̕���
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// ���ɑ�������id
				SetField(idleft, type, fdirleft);		// ���򓹂̐ݒu

				type = FTYPE_TURNL;
			}
			else if (CheckRand(1 / 2.0f) == true)
			{
				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// ���ɑ������̕���
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// ���ɑ�������id
				SetField(idright, type, fdirright);		// ���򓹂̐ݒu

				type = FTYPE_TURNR;
			}
			else
			{
#endif
				type = FTYPE_TURNLR;

				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// ���ɑ������̕���
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// ���ɑ�������id
				SetField(idleft, FTYPE_ROAD, fdirleft);		// ���򓹂̐ݒu

				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// ���ɑ������̕���
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// ���ɑ�������id
				SetField(idright, FTYPE_ROAD, fdirright);		// ���򓹂̐ݒu

			//}

			SetField(id_start, type, GetPlayerDirection());		// ���򓹂̐ݒu

			break;	// ���򓹌�͐ݒu���Ȃ�
		}

		
		numRand = rand() % NUM_FTYPE_NORMAL + START_FTYPE_NORMAL;
		
		//numRand = FTYPE_CLIFFC;

		SetField(id_start, (FIELD_TYPE)numRand, GetPlayerDirection());		// ���̐ݒu

	}
}

/*=====================================================================
�t�B�[���h����X�V�֐�
=====================================================================*/
void UpdateFieldControl()
{
	
	
}


