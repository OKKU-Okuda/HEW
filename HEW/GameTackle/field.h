/**********************************************************************
[[field�w�b�_�[(field.h)]]
	��ҁF���c�@�^�K

	field.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once
#include "item.h"
#include "Gimmick/Gimmick.h"

#define MAX_FIELD		(100)		// �t�B�[���h�v�[���ő吔
#define MAX_GIMMICK		(5)			// �t�B�[���h���M�~�b�N��

// 1�t�B�[���h�̏c���T�C�Y
#define FIELDCHIP_WIDTH		(1000.0f)
#define FIELDCHIP_HEIGHT	(FIELDCHIP_WIDTH)

#define FIELDCHIP_CENTER_X	(FIELDCHIP_WIDTH/2 )
#define FIELDCHIP_CENTER_Z	(FIELDCHIP_HEIGHT/2 )
// ���̃T�C�Y
#define FIELDROAD_X		(100.0f)
#define FIELDROAD_Y		(10.0f)	

// ���̕ǂ̃T�C�Y
#define ROADWALL_SIZEX	(5.0f)
#define ROADWALL_SIZEY	(20.0f)

// �v���C���[�̑������a
#define PLAYER_FIELDSIZE_R		(ROADWALL_SIZEX)

// �v���C���[�̎��S�m����W
#define FIELD_FAILED_Y			(-30.0f)

#define NUM_FTYPE_NORMAL			(FTYPE_JUMP + 1 - FTYPE_ROAD)// �ʏ퓹���擾
#define START_FTYPE_NORMAL			(FTYPE_ROAD)					// �ʏ퓹�̊J�n�Ԓn

// �R�C���ݒu�����W
#define POSY_COIN	(10.0f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// �v���C���[�̑��������
enum FIELD_DIRECTION {
	FDIRECTION_0ZP,	// ��
	FDIRECTION_1XP,	// ��
	FDIRECTION_2ZM,	// ��
	FDIRECTION_3XM,	// ��

	MAX_FIELDDIRECTION	/*�ő吔�擾*/
};

// �t�B�[���h�^�C�v��
enum FIELD_TYPE {
	FTYPE_VOID,			// �ޗ��i�����Ȃ�)

	// ��:NORMAL
	FTYPE_ROAD,			// ��{��
	FTYPE_CLIFFR,		// �E�����R�̓�
	FTYPE_CLIFFC,		// ���E�R�̓�
	FTYPE_CLIFFL,		// �������R�̓�
	FTYPE_JUMP,			// �C�b�c�A�W�����v���[���h

	// �J�[�u��:CURVE
	FTYPE_TURNLR,		// ���E����_�̓�
	//FTYPE_TURNR,		// �E�ɋȂ��铹
	FTYPE_TURNL,		// ���ɋȂ��铹

	MAX_FIELDTYPE		/* �ő吔�擾 */
};

// �t�B�[���h�J��
enum FIELD_STATE {
	FSTATE_NONE,		// �g�p���ĂȂ����

	FSTATE_READY,		// �����ς�(�v���C���[�̒�����ɂ���)
//	FSTATE_CURVE,		// �����ς�(�v���C���[�̒�����ɂ͂Ȃ��j

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

	// CHIP�����蔻��֐�
	// �߂�l�Fbool     true:���ɐڂ��Ă邩
	// Vec3*now		:���݂̃v���C���[CHIP���W
	// Vec3*past	:1�t���[���O�̃v���C���[CHIP���W
	bool(*CheckHit)(FIELD_CHIP*, Vec3*now, Vec3*past);


	// CHIP�X�V�֐�
	// Vec3*now		:���݂̃v���C���[CHIP���W
	void(*Update)(FIELD_CHIP*, Vec3*now);

	// CHIP�`��֐�
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
struct GIMMICK_HEADER;
typedef struct FIELD_CHIP{
	FIELD_TYPE		Type;		// ���̎��(�ꉞ�����Q�Ƃł���悤�ɕۊ�)
	FIELD_DIRECTION	Dir;		// ���̕���
	FIELD_STATE		State;		// ��ԕۊ�
	CHIP_ID			ID;			// CHIP�ꏊ����ʒu�𔻕ʂ���ID
	FIELD_OBJFUNC*	pFunc;		// �����ƂɈقȂ�X�V�֐����擾�֐��|�C���^

	ITEM*			paItem[MAXITEM_PERFIELD];	// �R�Â���ꂽ�A�C�e���̃�������
	GIMMICK_HEADER* paGimmick[MAX_GIMMICK];		// �R�Â���ꂽ�M�~�b�N�̃�������

	Matrix			WldMat;		// ���t�B�[���h�̃��[���h�s��
	Matrix			InvWldMat;	// ��L�t�s��
}FIELD_CHIP;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
�v���C���[�t�B�[���h�㓖���蔻��֐�
	(GIMMICK�����蔻��͕ʓr����)
	���̏�Q���ɂ���ăv���C���[�̈ʒu��ς��܂�

	�߂�l : void
	���� : bool		true:���ƃv���C���[���ݒu�Ă���
=====================================================================*/
bool PlayerCheckHitOnField();


/*=====================================================================
�t�B�[���h�ݒu�֐�
	�߂�l : FIELD_CHIP*
	���� :
	CHIP_ID id
	FIELD_TYPE type,				:�ݒu�t�B�[���h�^�C�v
	FIELD_DIRECTION fdirection		:����
=====================================================================*/
FIELD_CHIP* SetField(CHIP_ID id, FIELD_TYPE type, FIELD_DIRECTION fdirection);

/*=====================================================================
�t�B�[���h�폜�֐�
=====================================================================*/
void DeleteField(FIELD_CHIP* pData);

/*=====================================================================
�v���C���[�ɐڂ��Ă���t�B�[���h�`�����N�擾�֐�
=====================================================================*/
FIELD_CHIP* GetFieldOnPlayer();

/*=====================================================================
ID�Z�o�擾�֐�
=====================================================================*/
CHIP_ID GetChipID(short x, short z);

/*=====================================================================
��{�x�N�g���擾�֐�
=====================================================================*/
Vec3 GetFieldVector(FIELD_DIRECTION fdir);

/*=====================================================================
��{�x�N�g���擾�֐�
=====================================================================*/
CHIP_ID GetFieldIDVector(FIELD_DIRECTION fdir);

/*=====================================================================
ID���Z�֐�
=====================================================================*/
CHIP_ID AddFieldID(CHIP_ID id1, CHIP_ID id2);

/*=====================================================================
�^�C�v�ʃN���G�C�g�J�E���^�擾�֐�
=====================================================================*/
int GetCountCreateFieldType(FIELD_TYPE type);

/*=====================================================================
�t�B�[���h�������Z�֐�
=====================================================================*/
FIELD_DIRECTION AddFieldDirection(FIELD_DIRECTION fdir, int add);

/*=====================================================================
CHIP�����֐�
=====================================================================*/
FIELD_CHIP* SearchChipID(CHIP_ID id);

// �t�B�[���h��{�֐��Q
void InitField();
void UninitField();
void ResetField();
void UpdateField();
void DrawField();