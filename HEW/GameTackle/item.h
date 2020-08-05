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
#define	MAX_ITEM				(512)						// �A�C�e���ő吔
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)			// ��]���x
#define	ITEM_RADIUS				(10.0f)						// ���a
#define ITEM_EFECT_TIME			(0.5f)						// �A�C�e���̃G�t�F�N�g�Ɋ|����b��
#define ADD_ITEM_TIME			(1.0f/(ITEM_EFECT_TIME*60))	// �G�t�F�N�g�Ɋ|���鎞�Ԃ̌v�Z�p(1�t���[�����Ƃɉ��Z�����)
#define ITEM_SIZE_X				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y
#define ITEM_SIZE_Y				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y
#define ITEM_SIZE_Z				(10.0f)						// �A�C�e���̓����蔻��̃T�C�Y
								  
#define PLAYER_SIZE_X			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y
#define PLAYER_SIZE_Y			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y
#define PLAYER_SIZE_Z			(10.0f)						// �v���C���[�̓����蔻��̃T�C�Y

#define ITEM_UI_POS_X			(1200.0f)					// �A�C�e����UI�̃X�N���[�����WX
#define ITEM_UI_POS_Y			(50.0f)						// �A�C�e����UI�̃X�N���[�����WY

#define MAXITEM_PERFIELD		(20)						// �t�B�[���h�ɐݒu�ł���ő�A�C�e����
#define VOLUME_COIN				(4.0f)						// �R�C���擾���̉���
#define SPD_UPRATE				(1.003f)					// �R�C�����莞�̑��x�㏸��

//**************************************
// ���
//**************************************
; typedef enum
{
	ITEMTYPE_COIN = 0,	// �R�C��
	/* <----�A�C�e���̎�ނ𑝂₵�����Ƃ������ɒǉ�*/
	ITEMTYPE_MAX
}COIN;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct FIELD_CHIP;

class ITEM
{
public:
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 scl;		// �A�C�e���̃X�P�[������
	D3DXVECTOR3 firstpos;	// �A�C�e�������������ʒu
	D3DXVECTOR3 endpos;		// �A�C�e���̏I�_
	D3DXVECTOR3 rot;		// ���݂̌���
	D3DXVECTOR3 control_F;	// ��ڂ̐���_
	D3DXVECTOR3 control_S;	// ��ڂ̐���_
	float		fRadius;	// ���a
	float		fTime;		// �A�C�e���̃G�t�F�N�g�Ɋ|���鎞��
	int			nIdxShadow;	// �eID
	FIELD_CHIP*	pParent;	// �e
	int			nType;		// ���
	bool		bUse;		// �g�p���Ă��邩�ǂ���
	bool		bHit;		// �����蔻��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
struct FIELD_CHIP;

void SetItem(FIELD_CHIP* pData, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int nIdxItem);
void DeleteItemByPtr(ITEM** ppItem);
void DeleteItemByFieldPtr(FIELD_CHIP *pData);
void ResetItem();

ITEM *GetItem(void);

D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_pos,		// �߂�l
	float t, 				// 0����1�܂ł̎���
	D3DXVECTOR3* p_start, 	// �x�W�F�Ȑ��̎n�_
	D3DXVECTOR3* p_second, 	// �x�W�F�Ȑ��̑�1����_
	D3DXVECTOR3* p_third, 	// �x�W�F�Ȑ��̑�2����_
	D3DXVECTOR3* p_end);	// �x�W�F�Ȑ��̏I�_

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,			// �X�N���[���̉���
	int Screen_h,			// �X�N���[���̏c��
	D3DXMATRIX* View,		// �r���[�}�g���b�N�X
	D3DXMATRIX* Prj);		// �v���W�F�N�V�����}�g���b�N�X
