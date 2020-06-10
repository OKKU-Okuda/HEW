//=============================================================================
//
// �R�C���̏��� [item.h]
// Author : ���쑾�q
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_POLE				(1)							// �A�C�e���ő吔
#define	VALUE_ROTATE_POLE		(D3DX_PI * 0.025f)			// ��]���x
#define	POLE_RADIUS				(10.0f)						// ���a
#define POLE_SIZE_X				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y
#define POLE_SIZE_Y				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y
#define POLE_SIZE_Z				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y

#define PLAYER_SIZE_X			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y
#define PLAYER_SIZE_Y			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y
#define PLAYER_SIZE_Z			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y

//**************************************
// ���
//**************************************
enum
{
	POLETYPE_COIN = 0,		// �R�C��
	POLETYPE_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 scl;		// �A�C�e���̃X�P�[������
	D3DXVECTOR3 rot;		// ���݂̌���
	float fRadius;			// ���a
	int nIdxShadow;			// �eID
	int nType;				// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
	bool bHit;				// �����蔻��
} POLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPole(void);
void UninitPole(void);
void UpdatePole(void);
void DrawPole(void);

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetPole(FIELD_CHIP* pParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = POLETYPE_COIN);

void DeletePole(int nIdxPole);
void ResetPole();
POLE *GetPole(void);
