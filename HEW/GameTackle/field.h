/**********************************************************************
[[field�w�b�_�[(field.h)]]
	��ҁF���c�@�^�K

	field.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once

#define MAX_FIELD		(50)		// �t�B�[���h�v�[���ő吔
#define MAX_GIMMICK		(5)			// �t�B�[���h���M�~�b�N��

// 1�t�B�[���h�̏c���T�C�Y
#define FIELDCHIP_WIDTH		(1000.0f)
#define FIELDCHIP_HEIGHT	(FIELDCHIP_WIDTH)

// ���̃T�C�Y
#define FIELDROAD_X		(100.0f)
#define FIELDROAD_Y		(10.0f)	

#define PLAYER_POSX		(FIELDCHIP_WIDTH/2)
#define PLAYER_POSZ		(FIELDCHIP_HEIGHT/2)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// �t�B�[���h�^�C�v
enum FIELD_TYPE {
	FTYPE_ROAD,			// ��{��
	FTYPE_CLIFFR,		// �E�����R�̓�
	FTYPE_CLIFFL,		// �������R�̓�
	FTYPE_JUMP,			// �C�b�c�A�W�����v���[���h
	FTYPE_TURNLR,		// ���E����_�̓�
	FTYPE_TURNR,		// �E�ɋȂ��铹
	FTYPE_TURNL,		// ���ɋȂ��铹

	MAX_FIELDTYPE		/* �ő吔�擾 */
};

// �t�B�[���h�J��
enum FIELD_STATE {
	FSTATE_NONE,		// �g�p���ĂȂ����

	FSTATE_READY,		// �����ς�(�v���C���[�̒�����ɂ���)
	FSTATE_CURVE,		// �����ς�(�v���C���[�̒�����ɂ͂Ȃ��j

	FSTATE_ONPLAYER,	// �v���C���[����ɂ̂������Ă�
	FSTATE_USED,		// �v���C���[���ʂ�߂������`���c��
};

typedef struct {
	int x;
//	int y;
	int z;
}Vec2_Int;

// �t�B�[���h�Ǝ��֐��\����
struct FIELD_CHIP;	// �O���錾
typedef struct {
	bool(*CheckHit)(FIELD_CHIP*);
	void(*Update)(FIELD_CHIP*);
	void(*Draw)(FIELD_CHIP*);
}FIELD_OBJFUNC;

// CHIP����ID���p��
union CHIP_ID {
	struct {
		short x;
		short z;
	}vec2;

	DWORD bit;
};

// �t�B�[���h
typedef struct FIELD_CHIP{
	FIELD_TYPE		Type;		// ���̎��(�ꉞ�����Q�Ƃł���悤�ɕۊ�)
	FIELD_STATE		State;		// ��ԕۊ�
	CHIP_ID			ID;			// CHIP�ꏊ����ʒu�𔻕ʂ���ID
	FIELD_OBJFUNC*	pFunc;		// �����ƂɈقȂ�X�V�֐����擾�֐��|�C���^

	Matrix			WldMat;		// ���t�B�[���h�̃��[���h�s��

}FIELD_CHIP;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// �t�B�[���h��{�֐��Q
void InitField();
void UninitField();
void ResetField();
void UpdateField();
void DrawField();