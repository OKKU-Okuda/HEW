//=============================================================================
//
// �R�C������ [coin.cpp]
// Author : ���쑾�q
//
//=============================================================================
#include"../Core/main.h"
#include "coin.h"	
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �x�W�F�Ȑ��̏���
//=============================================================================
D3DXVECTOR3 *BezierCurve( D3DXVECTOR3* p_out, float t, D3DXVECTOR2* p_start, D3DXVECTOR2* p_second, D3DXVECTOR2* p_third, D3DXVECTOR2* p_end)
{
	float tp = 1 - t;
	float a, b, c, d;

	// �R���̃o�[���X�e�C��������
	a = t * t * t;
	b = 3 * t * t * tp;
	c = 3 * t * tp * tp;
	d = tp * tp * tp;

	p_out->x = (a * p_end->x) + (b * p_third->x) + (c * p_second->x) + (d * p_start->x);
	p_out->y = (a * p_end->y) + (b * p_third->y) + (c * p_second->y) + (d * p_start->y);
	p_out->z = 0;

	return p_out;
}

//=============================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,			// �X�N���[��X���W
	float Sy,			// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,		// �X�N���[���̉���
	int Screen_h,		// �X�N���[���̏c��
	D3DXMATRIX* View,	// �r���[�}�g���b�N�X
	D3DXMATRIX* Prj		// �v���W�F�N�V�����}�g���b�N�X
) {
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(p_out, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return p_out;
}