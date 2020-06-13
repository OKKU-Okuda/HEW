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
#include "item.h"	
#include "player.h"
#include"player_control.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define VOLUME_COIN		(4.0f)
#define SPD_UPRATE		(1.001f)		// �R�C�����莞�̑��x�㏸��

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

int g_ItemPoint;

static MySound g_seGetCoin;		// �R�C���Q�b�gse
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
		g_aItem[nCntItem].pos = D3DXVECTOR3(500.0f, 10.0f, 600.0f + (nCntItem * 30));
		g_aItem[nCntItem].scl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		g_aItem[nCntItem].firstpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].endpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].control_F = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].control_S = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fRadius = 0.0f;
		g_aItem[nCntItem].nIdxShadow = -1;
		g_aItem[nCntItem].nType = ITEMTYPE_COIN;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].bHit = false;
		g_aItem[nCntItem].time = 0.0f;
	}

	g_ItemPoint = 0;

	g_seGetCoin = MySoundCreate("data/SE/GetCoin.wav");
	MySoundSetVolume(g_seGetCoin, VOLUME_COIN);
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

	MySoundDelete(&g_seGetCoin);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
#ifdef _DEBUG
	int numActiveItem = 0;
#endif // _DEBUG

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
#ifdef _DEBUG
			numActiveItem++;
#endif

			g_aItem[nCntItem].rot.y += 0.05f;

			if (g_aItem[nCntItem].bHit == true)
			{
				//*************************************
				// ��������x�W�F�Ȑ��ɕK�v�ȏ��� 
				//*************************************

				// 0�`1�܂ł̎��Ԃ̏���
				if (g_aItem[nCntItem].time < 1.0f)
				{
					g_aItem[nCntItem].time += ADD_ITEM_TIME;
				}
				else
				{	// 1�𒴂����珈�����I��
					g_aItem[nCntItem].time = 1.0f;
					g_ItemPoint++;
					DeleteItem(nCntItem);

					SetPlayerSpd(GetPlayerSpd()*SPD_UPRATE);		// ���x�㏸
				}

				// �A�C�e����UI������X�N���[�����W�����[���h���W�ɕϊ�����(�I�_�̏���)
				CalcScreenToWorld( &g_aItem[nCntItem].endpos, ITEM_UI_POS_X, ITEM_UI_POS_Y, 0.95f, SCREEN_WIDTH, SCREEN_HEIGHT, GetMtxView(), GetMtxProjection());

				D3DXVECTOR3 Vec = D3DXVECTOR3( g_aItem[nCntItem].endpos.x - g_aItem[nCntItem].firstpos.x, g_aItem[nCntItem].endpos.y - g_aItem[nCntItem].firstpos.y, g_aItem[nCntItem].endpos.z - g_aItem[nCntItem].firstpos.z);

				// ��ꐧ��_�̌v�Z
				g_aItem[nCntItem].control_F = D3DXVECTOR3(g_aItem[nCntItem].firstpos.x + (Vec.x / 4), g_aItem[nCntItem].firstpos.y + (Vec.y / 4), g_aItem[nCntItem].firstpos.z + (Vec.z / 4));

				// ��񐧌�_�̌v�Z
				g_aItem[nCntItem].control_S = D3DXVECTOR3(g_aItem[nCntItem].firstpos.x + (Vec.x / 3), g_aItem[nCntItem].firstpos.y + (Vec.y / 3), g_aItem[nCntItem].firstpos.z + (Vec.z / 3));

				// �x�W�F�Ȑ��̊֐�
				BezierCurve( &g_aItem[nCntItem].pos, g_aItem[nCntItem].time, &g_aItem[nCntItem].firstpos, &g_aItem[nCntItem].control_F, &g_aItem[nCntItem].control_S, &g_aItem[nCntItem].endpos);
			
				continue;
			}


			// ��ڐG���̏���

			g_aItem[nCntItem].bHit = CheckHitBB(*GetPlayerPos(), g_aItem[nCntItem].pos, D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z), D3DXVECTOR3(ITEM_SIZE_X, ITEM_SIZE_Y, ITEM_SIZE_Z));
				
			g_aItem[nCntItem].firstpos = g_aItem[nCntItem].pos;

			if (g_aItem[nCntItem].bHit == true)
			{// �R�C���W�߂鉹
				MySoundPlayOnce(g_seGetCoin);
			}
			//if (g_aItem[nCntItem].ID_Parent.bit != g_aItem[nCntItem].pParent->ID.bit || g_aItem[nCntItem].pParent->State == FSTATE_NONE)
			//{// �����t�B�[���h�������ĂȂ��ꍇ�͏���
			//	DeleteItem(nCntItem);	
			//}

		}
	}

#ifdef _DEBUG
	PrintDebugProc("[debug_item]Active %d (MAX%d)", numActiveItem, MAX_ITEM);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;
	D3DMATERIAL9 Matdef;

	// �}�e���A���̑ޔ�
	pDevice->GetMaterial(&Matdef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aItem[nCntItem].scl.x, g_aItem[nCntItem].scl.y, g_aItem[nCntItem].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxScl);

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

	// �}�e���A���̕���
	pDevice->SetMaterial(&Matdef);

	//if (GetKeyboardPress(DIK_R))
	{
	//	InitItem();
	}
}


//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetItem(FIELD_CHIP* pData ,D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{// ���g�p�G���A���擾
			int nCntPtr = 0;			// �t�B�[���h�ɂ���A�C�e���|�C���^�z��ւ̃I�t�Z�b�g

			// �X�e�[�^�X�̏�������
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].fRadius = ITEM_RADIUS;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].bHit = false;
			g_aItem[nCntItem].pParent = pData;
			g_aItem[nCntItem].time = 0.0f;
			// �t�B�[���h���Ɏ�����R�Â�����
			for (; nCntPtr < MAXITEM_PERFIELD; nCntPtr++)
			{
				if (pData->paItem[nCntPtr] == NULL)
				{
					pData->paItem[nCntPtr] = &g_aItem[nCntItem];
					break;
				}
			}

			if (nCntPtr == MAXITEM_PERFIELD)
			{// ���ׂĎg�p�ς݂ł���΂��̃A�C�e��������
				DeleteItem(nCntItem);
			}

			return;
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

		for (int i = 0; i < MAXITEM_PERFIELD; i++)
		{
			if (g_aItem[nIdxItem].pParent->paItem[i] == &g_aItem[nIdxItem])
			{// �t�B�[���h�����폜
				g_aItem[nIdxItem].pParent->paItem[i] = NULL;
				break;
			}
		}
	}
}

//=============================================================================
// �A�C�e���̍폜�ݒ�
//=============================================================================
void DeleteItemByPtr(ITEM** ppItem)
{
	(*ppItem)->bUse = false;
	*ppItem = NULL;
}

//=============================================================================
// �t�B�[���h�w��ł̃A�C�e���̍폜
//=============================================================================
void DeleteItemByFieldPtr(FIELD_CHIP *pData)
{
	for (int i = 0; i < MAXITEM_PERFIELD; i++)
	{
		if (pData->paItem[i] != NULL)
		{
			DeleteItemByPtr(&pData->paItem[i]);
		}
	}
}

//=============================================================================
// item�z��̃[���N���A
//=============================================================================
void ResetItem()
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_aItem[i].bUse = false;
	}
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

/*=============================================================================================
BezierCurve�֐�
	�x�W�F�Ȑ��̏�������֐�
	�߂�l : D3DXVECTOR3*
	���� : ( D3DXVECTOR3* , float , D3DXVECTOR3* , D3DXVECTOR3* , D3DXVECTOR3* , D3DXVECTOR3*)
==============================================================================================*/
D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_out,		// �߂�l
	float t,				// 0����1�܂ł̎���
	D3DXVECTOR3* p_start,	// �x�W�F�Ȑ��̎n�_
	D3DXVECTOR3* p_second,	// �x�W�F�Ȑ��̑�1����_
	D3DXVECTOR3* p_third,	// �x�W�F�Ȑ��̑�2����_
	D3DXVECTOR3* p_end)		// �x�W�F�Ȑ��̏I�_
{
	// �킩��₷�����邽�߂̕ϐ�
	float tp = 1 - t;
	float a, b, c, d;

	// �R���̃o�[���X�e�C��������
	a = t * t * t;
	b = 3 * t * t * tp;
	c = 3 * t * tp * tp;
	d = tp * tp * tp;

	// �߂�l
	p_out->x = (a * p_end->x) + (b * p_third->x) + (c * p_second->x) + (d * p_start->x);
	p_out->y = (a * p_end->y) + (b * p_third->y) + (c * p_second->y) + (d * p_start->y);
	p_out->z = (a * p_end->z) + (b * p_third->z) + (c * p_second->z) + (d * p_start->z);

	return p_out;
}

/*=============================================================================================
CalcScreenToWorld�֐�
	�X�N���[�����W�����[���h���W�ɕϊ�����֐�
	�߂�l : D3DXVECTOR3*
	���� : 	(D3DXVECTOR3* , float , float , float , int , int , D3DXMATRIX* , D3DXMATRIX* )
==============================================================================================*/
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,	// �߂�l
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