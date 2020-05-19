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

// ���̕ǂ̃T�C�Y
#define ROADWALL_SIZEX	(5.0f)
#define ROADWALL_SIZEY	(20.0f)

// �v���C���[�̑������a
#define PLAYER_FIELDSIZE_R		(ROADWALL_SIZEX)

// 
#define FIELD_FAILED_Y			(-30.0f)
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
	FTYPE_ROAD,			// ��{��
	FTYPE_CLIFFR,		// �E�����R�̓��F������
	FTYPE_CLIFFL,		// �������R�̓��F������
	FTYPE_JUMP,			// �C�b�c�A�W�����v���[���h
	FTYPE_TURNLR,		// ���E����_�̓��F������
	FTYPE_TURNR,		// �E�ɋȂ��铹�F������
	FTYPE_TURNL,		// ���ɋȂ��铹�F������

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
typedef struct FIELD_CHIP{
	FIELD_TYPE		Type;		// ���̎��(�ꉞ�����Q�Ƃł���悤�ɕۊ�)
	FIELD_STATE		State;		// ��ԕۊ�
	CHIP_ID			ID;			// CHIP�ꏊ����ʒu�𔻕ʂ���ID
	FIELD_OBJFUNC*	pFunc;		// �����ƂɈقȂ�X�V�֐����擾�֐��|�C���^

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
	�߂�l : void
	���� :
	short x,						:�`�����N�ꏊx�̎w��
	short z,						:���゚
	FIELD_TYPE type,				:�ݒu�t�B�[���h�^�C�v
	FIELD_DIRECTION fdirection		:����
=====================================================================*/
void SetField(short x, short z, FIELD_TYPE type, FIELD_DIRECTION fdirection);


/*=====================================================================
�ŐV�ݒu����ID�擾�֐�
=====================================================================*/
CHIP_ID GetLatestChipID();

/*=====================================================================
�t�B�[���h�����擾�֐�
=====================================================================*/
FIELD_DIRECTION GetPlayerFieldDirection();

// �t�B�[���h��{�֐��Q
void InitField();
void UninitField();
void ResetField();
void UpdateField();
void DrawField();