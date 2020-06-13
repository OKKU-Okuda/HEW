/**********************************************************************
[[Gimmick�w�b�_�[(Gimmick.h)]]
	��ҁF���c�@�^�K

	Field/Gimmick/Gimmick.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// GIMMICK�^�C�v�̎w��
enum GIMMICK_TYPE {
	GTYPE_WALL,							// QTE�̕�
	GTYPE_FORSLIDE,						// �X���C�f�B���O���Ȃ��Ƃ����Ȃ���i�ۑ��I�ȁj

	MAX_GIMMICKTYPE
};

struct FIELD_CHIP;
struct GIMMICK_HEADER;
typedef struct {
	void(*Update)(GIMMICK_HEADER*, Vec3*);		// GIMMICK�X�V�֐�(��2�����̓v���C���[�̈ʒu)
	void(*Draw)(GIMMICK_HEADER*);				// GIMMICK�`��֐�
	void(*End)(GIMMICK_HEADER*);				// GIMMICK�I���֐�
}GIMMICK_FUNC;

typedef struct GIMMICK_HEADER{
	GIMMICK_FUNC*	pFunc;		// �ʊ֐�
	GIMMICK_TYPE	Type;		// GIMMICK�̎��
	bool			isActive;	// �N�����Ă��邩�ۂ�
}GIMMICK_HEADER;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------


/*=====================================================================
GIMMICK�ݒu�֐�
�����F
FIELD_CHIP* pChip,		:�R�Â�����t�B�[���hCHIP
GIMMICK_TYPE Type,		:�ǂ�GIMMICK���o����
Vec3* pPos,				:�ݒu����ꏊ(��1������CHIP���W�j
DWORD option			:GIMMICK�ɂ���ĈقȂ����
=====================================================================*/
void SetFieldGimmick(FIELD_CHIP* pChip, GIMMICK_TYPE Type, Vec3* pPos, DWORD option = 0);

/*=====================================================================
GIMMICK�X�V�֐�
=====================================================================*/
void FieldGimmickUpdate(FIELD_CHIP* pChip, Vec3* pos);

/*=====================================================================
GIMMICK�`��֐�
=====================================================================*/
void FieldGimmickDraw(FIELD_CHIP *pChip);

/*=====================================================================
�t�B�[���h�A�h���X�w��̃M�~�b�N�폜�֐�
=====================================================================*/
void DeleteGimmickByFieldPtr(FIELD_CHIP* pChip);

// ��{�֐��Q(field.cpp���ɋL�q)
void InitFieldGimmick();
void UninitFieldGimmick();
void ResetFieldGimmick();
void EndFieldGimmick();