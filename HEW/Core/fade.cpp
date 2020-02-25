//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FADE_RATE		(0.02f)		// �t�F�[�h�W��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
void SetColor(D3DCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static Texture		g_p3DTextureFade = NULL;	// �e�N�X�`���ւ̃|�C���^
static VtxBuff		g_pD3DVtxBuffFade = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static Color		g_color;
static PHASE_FUNC	g_NextFunc;					// ���t�F�[�Y�ւ̊֐��Q
static FADE			g_fade = FADE_NONE;


//=============================================================================
// �t�F�[�h�ݒ�֐�
// ���̃t�F�[�Y�ɔ�Ԋ֐�
// ����:
//		PHASE_FUNC* NextPhaseFunc,	:���̃t�F�[�Y��{�֐��Q
//=============================================================================
void GoNextPhase(PHASE_FUNC* NextPhaseFunc)
{
	g_NextFunc = *NextPhaseFunc;
	g_fade = FADE_OUT;
	g_color.a = 0.0f;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̐ݒ�
	MakeVertexFade(pDevice);

	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	if(g_p3DTextureFade != NULL)
	{// �e�N�X�`���̊J��
		g_p3DTextureFade->Release();
	}

	if(g_pD3DVtxBuffFade != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffFade->Release();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	if (g_fade == FADE_NONE)return;

	if(g_fade == FADE_OUT)
	{// �t�F�[�h�A�E�g����
		g_color.a += FADE_RATE;		// ���l�����Z���Č��̉�ʂ������Ă���
		if(g_color.a >= 1.0f)
		{	
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_color.a = 1.0f;

			GetPhase()->Uninit(false);	// �I������
			*GetPhase() = g_NextFunc;	// �֐��Q����ւ�
			GetPhase()->Init(false);	// �V�t�F�[�Y�̏���������

			g_fade = FADE_IN;
		}
	}
	else if(g_fade == FADE_IN)
	{// �t�F�[�h�C������
		g_color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
		if(g_color.a <= 0.0f)
		{
			// �t�F�[�h�����I��
			g_color.a = 0.0f;

			g_fade = FADE_NONE;
		}

	}

	// �F��ݒ�
	SetColor(g_color);

}

//=============================================================================
// �^�C�g�����
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pD3DVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}


//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,					// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffFade,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))							// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = g_color;
		pVtx[1].diffuse = g_color;
		pVtx[2].diffuse = g_color;
		pVtx[3].diffuse = g_color;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFade->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �F��ݒ�
//=============================================================================
void SetColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffFade->Unlock();
}

