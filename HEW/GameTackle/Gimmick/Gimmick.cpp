/**********************************************************************
[[GIMMICK�v���O����(Gimmick.cpp)]]
	��ҁF���c�@�^�K

	GIMMICK�Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../field.h"
#include "Gimmick.h"

#include "Wall.h"
#include "Slide.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

/*=====================================================================
GIMMICK�ݒu�֐�
�����F
FIELD_CHIP* pChip,		:�R�Â�����t�B�[���hCHIP
GIMMICK_TYPE Type,		:�ǂ�GIMMICK���o����
Vec3* pPos,				:�ݒu����ꏊ(��1������CHIP���W�j
DWORD option			:GIMMICK�ɂ���ĈقȂ����
=====================================================================*/
void SetFieldGimmick(FIELD_CHIP* pChip, GIMMICK_TYPE Type, Vec3* pPos, DWORD option)
{
	GIMMICK_HEADER** ppChip = NULL;
	GIMMICK_HEADER* pGHead = NULL;

	// �t�B�[���h�̋󂫂𒲂ׂ鏈��
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] == NULL)
		{
			ppChip = &pChip->paGimmick[i];
		}
	}

	if (ppChip == NULL)
	{// �t�B�[���h�����J���Ă��Ȃ���΂����ŏI����
		return;
	}

	// GIMMICK���Ƃ̏���������
	switch (Type)
	{
	case GTYPE_WALL:
		pGHead = GetWallMemory(pChip, pPos, (GIMICKWALL_QTE)option);
		break;
	case GTYPE_FORSLIDE:
		pGHead = GetSlideMemory(pChip, pPos);
		break;
	case MAX_GIMMICKTYPE:
	default:
		break;
	}


	if (pGHead == NULL)
	{// GIMMICK���ŃG���[�̏ꍇ�͂����ŏI��
		return;
	}

	// �A�h���X�̑}��
	*ppChip = pGHead;
}

/*=====================================================================
GIMMICK�������֐�
=====================================================================*/
void InitFieldGimmick()
{
	InitFieldGimmickWall();
	InitFieldGimmickSlide();
}

/*=====================================================================
GIMMICK�I�����֐�
=====================================================================*/
void UninitFieldGimmick()
{
	UninitFieldGimmickWall();
	UninitFieldGimmickSlide();
}

/*=====================================================================
GIMMICK�G���h�֐�
=====================================================================*/
void EndFieldGimmick()
{
	AllEndFieldGimmickWall();
	AllEndFieldGimmickSlide();
}

/*=====================================================================
GIMMICK���Z�b�g�֐�
=====================================================================*/
void ResetFieldGimmick()
{
	ResetFieldGimmickWall();
	ResetFieldGimmickSlide();
}

/*=====================================================================
GIMMICK�X�V�֐�
=====================================================================*/
void FieldGimmickUpdate(FIELD_CHIP* pChip, Vec3* pos)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			// GIMMICK�̍X�V
			pChip->paGimmick[i]->pFunc->Update(pChip->paGimmick[i], pos);
		}
	}
}

/*=====================================================================
GIMMICK�`��֐�
=====================================================================*/
void FieldGimmickDraw(FIELD_CHIP *pChip)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			// GIMMICK�̕`��
			pChip->paGimmick[i]->pFunc->Draw(pChip->paGimmick[i]);
		}
	}
}

/*=====================================================================
�t�B�[���h�A�h���X�w��̃M�~�b�N�폜�֐�
=====================================================================*/
void DeleteGimmickByFieldPtr(FIELD_CHIP* pChip)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			pChip->paGimmick[i]->isActive = false;
			pChip->paGimmick[i]->pFunc->End(pChip->paGimmick[i]);		// �I���֐�
			pChip->paGimmick[i] = NULL;
		}
	}
}