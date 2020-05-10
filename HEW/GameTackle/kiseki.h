//=============================================================================
//
// ���̋O�Տ��� [kiseki.h]
// Author : GP11A292 24 �����@�ċP
//
//=============================================================================
#ifndef _KISEKI_H_
#define _KISEKI_H_

#include"../Core/main.h"

//**************************************
// ���
//**************************************
enum
{
	KISEKITYPE_BULLET_PLAYER = 0,	// �v���C���[�̒e�̔���
	KISEKITYPE_BULLET_ENEMY,			// �G�̒e�̔���
	KISEKITYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKiseki(int type);
void UninitKiseki(void);
void UpdateKiseki(void);
void DrawKiseki(void);

int SetKiseki(D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2, int nType,int col_type);

#endif
