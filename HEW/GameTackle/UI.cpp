//=============================================================================
//
// UI���� [UI.cpp]
// Author : �����@�
//
//=============================================================================
#include"../Core/main.h"

#include "UI.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCoin(LPDIRECT3DDEVICE9 pDevice);
void SetTextureCoin(int idx, int number);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
					/*�R�C��*/
UI		g_ui;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_COIN,			// �t�@�C���̖��O
			&g_ui.pD3DTextureCoin[0]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FRAME_COIN,		// �t�@�C���̖��O
			&g_ui.pD3DTextureCoin[1]);	// �ǂݍ��ރ������[

				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_DISTANCE,			// �t�@�C���̖��O
			&g_ui.pD3DTextureDistance[0]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FRAME_DISTANCE,		// �t�@�C���̖��O
			&g_ui.pD3DTextureDistance[1]);	// �ǂݍ��ރ������[

	}

	/*�R�C��*/
	{
		g_ui.posCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ui.rotCoin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �X�R�A�̏�����
		g_ui.coin = 0;
		g_ui.coin_target = 10;

		g_ui.COIN_SIZE_X = 35.0f;
		g_ui.COIN_SIZE_Y = 50.0f;

		g_ui.COIN_PLACE = 1;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		// ���_���̍쐬
		MakeVertexCoin(pDevice);
	}

	/*����*/
	{
		g_ui.posDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ui.rotDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �X�R�A�̏�����
		g_ui.distance = 0;
		g_ui.distance_target = 10;

		g_ui.DISTANCE_SIZE_X = 35.0f;
		g_ui.DISTANCE_SIZE_Y = 50.0f;

		g_ui.DISTANCE_PLACE = 1;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		// ���_���̍쐬
		MakeVertexDistance(pDevice);

	}

	g_ui.add_timer = 0.0f;
	g_ui.add_timer_max = 60 * 1;

	g_ui.dec_timer = 0.0f;
	g_ui.dec_timer_max = 30 * 1;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
{
	/*�R�C��*/
	{
		if (g_ui.pD3DTextureCoin[0] != NULL)
		{// �e�N�X�`���̊J��
			g_ui.pD3DTextureCoin[0]->Release();
			g_ui.pD3DTextureCoin[0] = NULL;
		}
		if (g_ui.pD3DTextureCoin[1] != NULL)
		{// �e�N�X�`���̊J��
			g_ui.pD3DTextureCoin[1]->Release();
			g_ui.pD3DTextureCoin[1] = NULL;
		}

		if (g_ui.pD3DVtxBuffCoin != NULL)
		{// ���_�o�b�t�@�̊J��
			g_ui.pD3DVtxBuffCoin->Release();
			g_ui.pD3DVtxBuffCoin = NULL;
		}
	}

	/*����*/
	{
		if (g_ui.pD3DTextureDistance[0] != NULL)
		{// �e�N�X�`���̊J��
			g_ui.pD3DTextureDistance[0]->Release();
			g_ui.pD3DTextureDistance[0] = NULL;
		}
		if (g_ui.pD3DTextureDistance[1] != NULL)
		{// �e�N�X�`���̊J��
			g_ui.pD3DTextureDistance[1]->Release();
			g_ui.pD3DTextureDistance[1] = NULL;
		}

		if (g_ui.pD3DVtxBuffDistance != NULL)
		{// ���_�o�b�t�@�̊J��
			g_ui.pD3DVtxBuffDistance->Release();
			g_ui.pD3DVtxBuffDistance = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUI(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*�R�C��*/
	for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++)
	{
		int number;

		number = (g_ui.coin % (int)(powf(10.0f, (float)(g_ui.COIN_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(g_ui.COIN_PLACE - nCntPlace - 1))));

		SetTextureCoin(nCntPlace, number);
	}

	/*��*/
	if (g_ui.coin >= g_ui.coin_target)
	{
		g_ui.coin_target *= 10;	// ���̌��܂ł̃^�[�Q�b�g�Ƃ���
		g_ui.COIN_PLACE++;

		g_ui.COIN_SIZE_X = 55.0f;
		g_ui.COIN_SIZE_Y = 70.0f;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		MakeVertexCoin(pDevice);

	}

	else if ((g_ui.coin == 15) || (g_ui.coin == 105) || (g_ui.coin == 1005) || (g_ui.coin == 10005) || (g_ui.coin == 100005) || (g_ui.coin == 1000005))
	{
		g_ui.COIN_SIZE_X = 35.0f;
		g_ui.COIN_SIZE_Y = 50.0f;

		g_ui.COIN_POS_X = (SCREEN_WIDTH - (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) * g_ui.COIN_PLACE - 35.0f);

		MakeVertexCoin(pDevice);
	}

	/*����*/
	for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++)
	{
		int number;

		number = (g_ui.distance % (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE - nCntPlace))) / (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE - nCntPlace - 1))));

		SetTextureDistance(nCntPlace, number);
	}

	/*��*/
	if (g_ui.distance >= g_ui.distance_target)
	{
		g_ui.distance_target *= 10;	// ���̌��܂ł̃^�[�Q�b�g�Ƃ���
		g_ui.DISTANCE_PLACE++;

		g_ui.DISTANCE_SIZE_X = 55.0f;
		g_ui.DISTANCE_SIZE_Y = 70.0f;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		MakeVertexDistance(pDevice);

	}
	else if((g_ui.distance == 50) || (g_ui.distance == 150) || (g_ui.distance == 1050) || (g_ui.distance == 10050) || (g_ui.distance == 100050) || (g_ui.distance == 1000050))
	{
		g_ui.DISTANCE_SIZE_X = 35.0f;
		g_ui.DISTANCE_SIZE_Y = 50.0f;

		g_ui.DISTANCE_POS_X = (SCREEN_WIDTH - (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) * g_ui.DISTANCE_PLACE - 35.0f);

		MakeVertexDistance(pDevice);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*�R�C��*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_ui.pD3DVtxBuffCoin, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_ui.pD3DTextureCoin[0]);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_ui.pD3DTextureCoin[1]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (g_ui.COIN_PLACE * 4), NUM_POLYGON);
	}

	/*����*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_ui.pD3DVtxBuffDistance, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_ui.pD3DTextureDistance[0]);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
		}
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_ui.pD3DTextureDistance[1]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (g_ui.DISTANCE_PLACE * 4), NUM_POLYGON);
	}
}
//=============================================================================
// ���_�̍쐬	�R�C��
//=============================================================================
HRESULT MakeVertexCoin(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * g_ui.COIN_PLACE + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_ui.pD3DVtxBuffCoin,								// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))											// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_ui.pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < g_ui.COIN_PLACE; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace)* g_ui.COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace) * (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) + g_ui.COIN_SIZE_X, COIN_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace)* g_ui.COIN_SIZE_X + COIN_INTERVAL_X, COIN_POS_Y + g_ui.COIN_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_ui.COIN_POS_X + (nCntPlace) * (COIN_INTERVAL_X + g_ui.COIN_SIZE_X) + g_ui.COIN_SIZE_X, COIN_POS_Y + g_ui.COIN_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(COIN_ICON_POS_X - 15, COIN_ICON_POS_Y - 15, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X)+15, COIN_ICON_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(COIN_ICON_POS_X - 15, COIN_ICON_POS_Y + 15, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(COIN_ICON_POS_X + (COIN_INTERVAL_X)+15, COIN_ICON_POS_Y + 15, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

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
		g_ui.pD3DVtxBuffCoin->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬	����
//=============================================================================
HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * g_ui.DISTANCE_PLACE + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_ui.pD3DVtxBuffDistance,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))											// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_ui.pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < g_ui.DISTANCE_PLACE; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace)* g_ui.DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y, 0.0f) + g_ui.posDistance;
			pVtx[1].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace) * (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) + g_ui.DISTANCE_SIZE_X, DISTANCE_POS_Y, 0.0f) + g_ui.posDistance;
			pVtx[2].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace)* g_ui.DISTANCE_SIZE_X + DISTANCE_INTERVAL_X, DISTANCE_POS_Y + g_ui.DISTANCE_SIZE_Y, 0.0f) + g_ui.posDistance;
			pVtx[3].vtx = D3DXVECTOR3(g_ui.DISTANCE_POS_X + (nCntPlace) * (DISTANCE_INTERVAL_X + g_ui.DISTANCE_SIZE_X) + g_ui.DISTANCE_SIZE_X, DISTANCE_POS_Y + g_ui.DISTANCE_SIZE_Y, 0.0f) + g_ui.posDistance;

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(METER_POS_X - 15, METER_POS_Y - 15, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)+15, METER_POS_Y - 15, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(METER_POS_X - 15, METER_POS_Y + 25, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(METER_POS_X + (DISTANCE_INTERVAL_X)+15, METER_POS_Y + 25, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

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
		g_ui.pD3DVtxBuffDistance->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�		�R�C��
//=============================================================================
void SetTextureCoin(int idx, int number)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_ui.pD3DVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_ui.pD3DVtxBuffCoin->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�		����
//=============================================================================
void SetTextureDistance(int idx, int number)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_ui.pD3DVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_ui.pD3DVtxBuffDistance->Unlock();
}

//=============================================================================
// �R�C���̕ύX
//=============================================================================
void ChangeCoin(int value)
{
	g_ui.coin += value;

	if (g_ui.coin < 0)
	{
		g_ui.coin = 0;
	}
	else if (g_ui.coin >= (int)(powf(10.0f, (float)(g_ui.COIN_PLACE + 1))))
	{
		g_ui.coin = (int)(powf(10.0f, (float)(g_ui.COIN_PLACE + 1))) - 1;
	}
}

//=============================================================================
// �����̕ύX
//=============================================================================
void ChangeDistance(int value)
{
	g_ui.distance += value;

	if (g_ui.distance < 0)
	{
		g_ui.distance = 0;
	}
	else if (g_ui.distance >= (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE + 1))))
	{
		g_ui.distance = (int)(powf(10.0f, (float)(g_ui.DISTANCE_PLACE + 1))) - 1;
	}
}

UI *GetUI(void)
{
	return &g_ui;
}

void ResetUI(void)
{
	GetUI()->coin = 0;
	GetUI()->distance = 0;
}