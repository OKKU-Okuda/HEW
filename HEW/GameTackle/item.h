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
#define	MAX_ITEM				(128)						// �A�C�e���ő吔

//**************************************
// ���
//**************************************
enum
{
	ITEMTYPE_COIN = 0,		// �R�C��
	ITEMTYPE_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 rot;		// ���݂̌���
	float fRadius;			// ���a
	int nIdxShadow;			// �eID
	int nType;				// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} ITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);

D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_pos,
	float t, D3DXVECTOR2*
	p_start, D3DXVECTOR2*
	p_second, D3DXVECTOR2*
	p_third, D3DXVECTOR2*
	p_end);

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,			// �X�N���[��X���W
	float Sy,			// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,		// �X�N���[���̉���
	int Screen_h,		// �X�N���[���̏c��
	D3DXMATRIX* View,	// �r���[�}�g���b�N�X
	D3DXMATRIX* Prj);	// �v���W�F�N�V�����}�g���b�N�X
