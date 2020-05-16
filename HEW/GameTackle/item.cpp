//=============================================================================
//
// �R�C������ [item.cpp]
// Author : ���쑾�q
//
//=============================================================================
#include"../Core/main.h"
#include "item.h"	
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)		// ��]���x
#define	ITEM_RADIUS				(10.0f)					// ���a

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureItem[ITEMTYPE_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pMeshItem[ITEMTYPE_MAX];			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXMatBuffItem[ITEMTYPE_MAX];	// ���b�V���̃}�e���A�������i�[
DWORD				g_aNumMatItem[ITEMTYPE_MAX];		// �������̑���

D3DXMATRIX			g_mtxWorldItem;						// ���[���h�}�g���b�N�X

ITEM				g_aItem[MAX_ITEM];					// �A�C�e�����[�N

const char *c_aFileNameItem[ITEMTYPE_MAX] =
{
	"data/MODEL/item000.x",		// �R�C��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(c_aFileNameItem[nCntItemType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffItem[nCntItemType],
			NULL,
			&g_aNumMatItem[nCntItemType],
			&g_pMeshItem[nCntItemType])))
		{
			return E_FAIL;
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,						// �t�@�C���̖��O
			&g_pD3DTextureModelItem[nCntItemType]);	// �ǂݍ��ރ������[
#endif
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fRadius = 0.0f;
		g_aItem[nCntItem].nIdxShadow = -1;
		g_aItem[nCntItem].nType = ITEMTYPE_COIN;
		g_aItem[nCntItem].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		if (g_pD3DTextureItem[nCntItemType] != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTextureItem[nCntItemType]->Release();
			g_pD3DTextureItem[nCntItemType] = NULL;
		}

		if (g_pMeshItem[nCntItemType] != NULL)
		{// ���b�V���̊J��
			g_pMeshItem[nCntItemType]->Release();
			g_pMeshItem[nCntItemType] = NULL;
		}

		if (g_pD3DXMatBuffItem[nCntItemType] != NULL)
		{// �}�e���A���̊J��
			g_pD3DXMatBuffItem[nCntItemType]->Release();
			g_pD3DXMatBuffItem[nCntItemType] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].rot.y += VALUE_ROTATE_ITEM;
			if (g_aItem[nCntItem].rot.y > D3DX_PI)
			{
				g_aItem[nCntItem].rot.y -= D3DX_PI * 2.0f;
			}

			float fSizeX = 20.0f + (g_aItem[nCntItem].pos.y - 10.0f) * 0.05f;
			if (fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (g_aItem[nCntItem].pos.y - 10.0f) * 0.05f;
			if (fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			float colA = (200.0f - (g_aItem[nCntItem].pos.y - 10.0f)) / 400.0f;
			if (colA < 0.0f)
			{
				colA = 0.0f;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[g_aItem[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[g_aItem[nCntItem].nType]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);

				// �`��
				g_pMeshItem[g_aItem[nCntItem].nType]->DrawSubset(nCntMat);
			}
		}
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].fRadius = ITEM_RADIUS;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;

			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if (nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		g_aItem[nIdxItem].bUse = false;
	}
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

/*=====================================================================
BezierCurve�֐�
	�x�W�F�Ȑ��̏�������֐�
	�߂�l : D3DXVECTOR3*
	���� : ( D3DXVECTOR3* , float , D3DXVECTOR2* , D3DXVECTOR2* , D3DXVECTOR2* , D3DXVECTOR2*)
=====================================================================*/
D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_out,
	float t,
	D3DXVECTOR2* p_start,
	D3DXVECTOR2* p_second,
	D3DXVECTOR2* p_third,
	D3DXVECTOR2* p_end)
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

/*=====================================================================
CalcScreenToWorld�֐�
	�X�N���[�����W�����[���h���W�ɕϊ�����֐�
	�߂�l : D3DXVECTOR3*
	���� : 	(D3DXVECTOR3* , float , float , float , int , int , D3DXMATRIX* , D3DXMATRIX* )
=====================================================================*/
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

	VP._11 = Screen_w / 2.0f;
	VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f;
	VP._42 = Screen_h / 2.0f;

	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(p_out, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return p_out;
}