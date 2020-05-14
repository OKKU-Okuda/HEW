//=============================================================================
//
// ���̋O�Տ��� [kiseki.cpp]
// Author : GP11A292 24 �����@�ċP
//
//=============================================================================
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"
#include "kiseki.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_KISEKI						"data/TEXTURE/kiseki02.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X						(50.0f)							// �r���{�[�h�̕�
#define	BULLET_SIZE_Y						(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_KISEKI					(2.0f)							// �ړ����x
#define	MAX_KISEKI							(5000)							// �r���{�[�h�ő吔
#define KISEKI_TEXTURE_PATTERN_DIVIDE_X		(30)								// �e�N�X�`��X������
#define KISEKI_TEXTURE_PATTERN_DIVIDE_Y		(1)								// �e�N�X�`��Y������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nCounter;			// �J�E���^�[
	int nPattern;			// �p�^�[��No.
	int nType;				// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} KISEKI;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexKiseki(LPDIRECT3DDEVICE9 pDevice);
void SetVertexKiseki(int nIdxKiseki, D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 senntann1, D3DXVECTOR3 sentann2);
void SetColorKiseki(int nIdxKiseki, D3DXCOLOR col);
void SetTextureKiseki(int nIdxKiseki, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureKiseki = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffKiseki = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldKiseki;			// ���[���h�}�g���b�N�X

KISEKI				g_aKiseki[MAX_KISEKI];	// �������[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitKiseki(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexKiseki(pDevice);
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_KISEKI,			// �t�@�C���̖��O
			&g_pD3DTextureKiseki);	// �ǂݍ��ރ������[
	}

	for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		g_aKiseki[nCntKiseki].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKiseki[nCntKiseki].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKiseki[nCntKiseki].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aKiseki[nCntKiseki].fSizeX = BULLET_SIZE_X;
		g_aKiseki[nCntKiseki].fSizeY = BULLET_SIZE_Y;
		g_aKiseki[nCntKiseki].nCounter = 0;
		g_aKiseki[nCntKiseki].nPattern = 0;
		g_aKiseki[nCntKiseki].nType = KISEKITYPE_BULLET_PLAYER;
		g_aKiseki[nCntKiseki].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitKiseki(void)
{
	if(g_pD3DTextureKiseki != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureKiseki->Release();
		g_pD3DTextureKiseki = NULL;
	}

	if(g_pD3DVtxBuffKiseki != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffKiseki->Release();
		g_pD3DVtxBuffKiseki = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateKiseki(void)
{

	for (int i = 0; i < MAX_KISEKI; i++)
	{
		if (g_aKiseki[i].bUse)
		{

			//���ߏ���
			g_aKiseki[i].col.a -= 0.07f;
			if(g_aKiseki[i].col.a < 0.0f)
			{
				//a�l0�ȉ��ɂȂ�����0
				g_aKiseki[i].col.a = 0.0f;

				//false�ɂȂ�
				g_aKiseki[i].bUse = false;
			}
			SetColorKiseki(i, g_aKiseki[i].col);

		}


	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawKiseki(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z��r�Ȃ�

	for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		if(g_aKiseki[nCntKiseki].bUse)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldKiseki);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aKiseki[nCntKiseki].scale.x, g_aKiseki[nCntKiseki].scale.y, g_aKiseki[nCntKiseki].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldKiseki, &g_mtxWorldKiseki, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aKiseki[nCntKiseki].pos.x, g_aKiseki[nCntKiseki].pos.y, g_aKiseki[nCntKiseki].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldKiseki, &g_mtxWorldKiseki, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldKiseki);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffKiseki, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureKiseki);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntKiseki * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z��r����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexKiseki(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_KISEKI,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffKiseki,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].vtx = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffKiseki->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexKiseki(int nIdxKiseki, D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].vtx = nemoto2-nemoto1;
		pVtx[2].vtx = sentan1-nemoto1;
		pVtx[3].vtx = sentan2-nemoto1;


		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorKiseki(int nIdxKiseki, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureKiseki(int nIdxKiseki, int nPattern)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffKiseki->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxKiseki * 4);


		// �e�N�X�`�����W�̐ݒ�
		int x = nPattern % KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		int y = nPattern / KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		float sizeX = 1.0f / KISEKI_TEXTURE_PATTERN_DIVIDE_X;
		float sizeY = 1.0f / KISEKI_TEXTURE_PATTERN_DIVIDE_Y;


		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffKiseki->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetKiseki(D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2, int nType,int col_type)
{
	int nIdxKiseki = -1;
	for (int nCntKiseki = 0; nCntKiseki < MAX_KISEKI; nCntKiseki++)
	{
		if (!g_aKiseki[nCntKiseki].bUse)
		{
			g_aKiseki[nCntKiseki].pos = nemoto1;
			g_aKiseki[nCntKiseki].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aKiseki[nCntKiseki].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aKiseki[nCntKiseki].nCounter = 0;
			g_aKiseki[nCntKiseki].nPattern = 0;
			g_aKiseki[nCntKiseki].nType = nType;
			g_aKiseki[nCntKiseki].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexKiseki(nCntKiseki, nemoto1, nemoto2, sentan1, sentan2);

			if (col_type == 1)
			{
				g_aKiseki[nCntKiseki].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

			}
			else
			{
				g_aKiseki[nCntKiseki].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.5f);
			}

			SetColorKiseki(nCntKiseki, g_aKiseki[nCntKiseki].col);

			// �e�N�X�`�����W�̐ݒ�
			SetTextureKiseki(nCntKiseki, g_aKiseki[nCntKiseki].nType);

			nIdxKiseki = nCntKiseki;

			break;

		}
	}



	return nIdxKiseki;
}



