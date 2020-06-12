//=============================================================================
//
// �R�C������ [item.cpp]
// Author : ���쑾�q
//
//=============================================================================
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"
#include "../Core/CheckHit.h"
#include "field.h"
#include "pole.h"	
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePole[POLETYPE_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pMeshPole[POLETYPE_MAX];			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXMatBuffPole[POLETYPE_MAX];	// ���b�V���̃}�e���A�������i�[
DWORD				g_aNumMatPole[POLETYPE_MAX];		// �������̑���

D3DXMATRIX			g_mtxWorldPole;						// ���[���h�}�g���b�N�X

POLE				g_aPole[MAX_POLE];					// �A�C�e�����[�N

const char *c_aFileNamePole[POLETYPE_MAX] =
{
	"data/MODEL/pole.x",		// �R�C��
};

int g_PolePoint;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPole(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPoleType = 0; nCntPoleType < POLETYPE_MAX; nCntPoleType++)
	{
		g_pD3DTexturePole[nCntPoleType] = NULL;
		g_pMeshPole[nCntPoleType] = NULL;
		g_pD3DXMatBuffPole[nCntPoleType] = NULL;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(c_aFileNamePole[nCntPoleType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffPole[nCntPoleType],
			NULL,
			&g_aNumMatPole[nCntPoleType],
			&g_pMeshPole[nCntPoleType])))
		{
			return E_FAIL;
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,						// �t�@�C���̖��O
			&g_pD3DTextureModelPole[nCntPoleType]);	// �ǂݍ��ރ������[
#endif
	}

	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		g_aPole[nCntPole].pos = D3DXVECTOR3(500.0f, 10.0f, 600.0f + (nCntPole * 30));
		g_aPole[nCntPole].scl = D3DXVECTOR3(1.f, 1.f, 1.f);
		g_aPole[nCntPole].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPole[nCntPole].fRadius = 0.0f;
		g_aPole[nCntPole].nIdxShadow = -1;
		g_aPole[nCntPole].nType = POLETYPE_COIN;
		g_aPole[nCntPole].bUse = false;
		g_aPole[nCntPole].bHit = false;
	}

	g_PolePoint = 0;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPole(void)
{
	for (int nCntPoleType = 0; nCntPoleType < POLETYPE_MAX; nCntPoleType++)
	{
		if (g_pD3DTexturePole[nCntPoleType] != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTexturePole[nCntPoleType]->Release();
			g_pD3DTexturePole[nCntPoleType] = NULL;
		}

		if (g_pMeshPole[nCntPoleType] != NULL)
		{// ���b�V���̊J��
			g_pMeshPole[nCntPoleType]->Release();
			g_pMeshPole[nCntPoleType] = NULL;
		}

		if (g_pD3DXMatBuffPole[nCntPoleType] != NULL)
		{// �}�e���A���̊J��
			g_pD3DXMatBuffPole[nCntPoleType]->Release();
			g_pD3DXMatBuffPole[nCntPoleType] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePole(void)
{
	for (int nCntPole = 0; nCntPole < POLETYPE_MAX; nCntPole++)
	{
		g_aPole[nCntPole].pos = *GetPlayerPos();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPole(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;

	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		if (g_aPole[nCntPole].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldPole);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aPole[nCntPole].scl.x, g_aPole[nCntPole].scl.y, g_aPole[nCntPole].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPole[nCntPole].rot.y, g_aPole[nCntPole].rot.x, g_aPole[nCntPole].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aPole[nCntPole].pos.x, g_aPole[nCntPole].pos.y, g_aPole[nCntPole].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldPole, &g_mtxWorldPole, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPole);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffPole[g_aPole[nCntPole].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatPole[g_aPole[nCntPole].nType]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTexturePole[g_aPole[nCntPole].nType]);

				// �`��
				g_pMeshPole[g_aPole[nCntPole].nType]->DrawSubset(nCntMat);
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

	if (GetKeyboardPress(DIK_R))
	{
		InitPole();
	}
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetPole(FIELD_CHIP* pParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntPole = 0; nCntPole < MAX_POLE; nCntPole++)
	{
		if (!g_aPole[nCntPole].bUse)
		{
			g_aPole[nCntPole].pos = pos;
			g_aPole[nCntPole].rot = rot;
			g_aPole[nCntPole].fRadius = POLE_RADIUS;
			g_aPole[nCntPole].nType = nType;
			g_aPole[nCntPole].bUse = true;

			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void DeletePole(int nIdxPole)
{
	if (nIdxPole >= 0 && nIdxPole < MAX_POLE)
	{
		g_aPole[nIdxPole].bUse = false;
	}
}

//=============================================================================
// item�z��̃[���N���A
//=============================================================================
void ResetPole()
{
	ZeroMemory(g_aPole, sizeof(g_aPole));
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
POLE *GetPole(void)
{
	return &g_aPole[0];
}
