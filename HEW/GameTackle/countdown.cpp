//=============================================================================
//
// �X�^�[�g��ʏ��� [countdown.cpp]
// Author : GP11A292 24 �����@�ċP
//
//=============================================================================
#include "countdown.h"
#include"../Core/sound.h"
#include"../Core/fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_COUNTDOWN		"data/TEXTURE/NUM.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SHUTSUJIN		"data/TEXTURE/KickOff.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	COUNTDOWN_POS_X			(550)								// �X�^�[�g���S�̈ʒu(X���W)
#define	COUNTDOWN_POS_Y			(200)								// �X�^�[�g���S�̈ʒu(Y���W)
#define	COUNTDOWN_WIDTH			(200)								// �X�^�[�g���S�̕�
#define	COUNTDOWN_HEIGHT		(200)								// �X�^�[�g���S�̍���
#define	SHUTSUJIN_POS_X			(350)								// �X�^�[�g�{�^���̈ʒu(X���W)
#define	SHUTSUJIN_POS_Y			(80)								// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	SHUTSUJIN_WIDTH			(800)								// �X�^�[�g�{�^���̕�
#define	SHUTSUJIN_HEIGHT		(400)								// �X�^�[�g�{�^���̍���
#define	COUNT_APPERA_COUNTDOWN	(30)								// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_COUNTDOWN	(60)								// �X�^�[�g�{�^���_�ł̎���
#define	COUNT_WAIT_DEMO		(60 * 5)								// �f���܂ł̑҂�����
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X (10)						// �e�N�X�`���p�^�[��X
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y (1)						// �e�N�X�`���p�^�[��Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCountdown(LPDIRECT3DDEVICE9 pDevice);
void SetColorCountdown(void);
void SetTextureCountdown(int nPattern);
void SetVertexCountdown(float fSizeX, float fSizeY);
void SetVertexShutsujin(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCountdown = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffCountdown = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static LPDIRECT3DTEXTURE9		g_pD3DTextureShutsujin = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffShutsujin = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static int						g_nCountAppearCountdown = 0;		// �e�N�X�`���\���܂ł̃C���^�[�o��
static float					g_fAlphaLogo = 0.0f;				// �e�N�X�`����a�l
static float					g_ExpantionLogo = 0.0f;				// �e�N�X�`���̊g��l
static float					g_Shutsujin_pos_x = 0.0f;			// �e�N�X�`���̊g��l
static int						g_CountdownPattern = 3;				// �e�N�X�`���p�^�[����
static int						countdown_flag = 0;					// 0;�J�E���g�_�E�� 1;�o�w 2�퓬
static MySound					g_seCountdown;						// �J�E���g�_�E����
static MySound					g_seKickoff;						// �L�b�N�I�t��
static bool						g_sound;							// �L�b�N�I�t��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCountdown(int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//����������
	g_nCountAppearCountdown = 0;
	g_fAlphaLogo = 0.0f;
	g_ExpantionLogo = 0.0f;
	g_CountdownPattern = 3;
	countdown_flag = 0;
	g_Shutsujin_pos_x = 1000.0f;

	// ���_���̍쐬
	MakeVertexCountdown(pDevice);

	SetTextureCountdown(g_CountdownPattern);

	if (type == 0)
	{

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_COUNTDOWN,								// �t�@�C���̖��O
			&g_pD3DTextureCountdown);						// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SHUTSUJIN,							// �t�@�C���̖��O
			&g_pD3DTextureShutsujin);					// �ǂݍ��ރ������[

		g_seCountdown = MySoundCreate("data/SE/countdown.wav");

	}


	g_sound = false;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCountdown(void)
{

	if(g_pD3DTextureCountdown != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureCountdown->Release();
		g_pD3DTextureCountdown = NULL;
	}

	if(g_pD3DVtxBuffCountdown != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffCountdown->Release();
		g_pD3DVtxBuffCountdown = NULL;
	}

	if(g_pD3DTextureShutsujin != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureShutsujin->Release();
		g_pD3DTextureShutsujin = NULL;
	}

	if(g_pD3DVtxBuffShutsujin != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffShutsujin->Release();
		g_pD3DVtxBuffShutsujin = NULL;
	}

	MySoundDelete(&g_seCountdown);
	MySoundDelete(&g_seKickoff);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCountdown(void)
{
	if (GetFade() == 0)
	{
		if (g_sound == false)
		{
			MySoundSetVolume(g_seCountdown, 3.0f);
			MySoundPlayOnce(g_seCountdown);
			g_sound = true;
		}

		//�J�E���g�_�E���̍X�V
		if (countdown_flag == 0)
		{
			// BGM��~
			//MySoundStopAll();

			//�����Ȃ�
			if (g_fAlphaLogo < 1.0f)
			{
				//���X�Ɏ��̉�
				g_fAlphaLogo += 0.017f;

				//�g��
				g_ExpantionLogo += 1.0f;

				//���̉�����������
				if (g_fAlphaLogo >= 1.0f)
				{
					g_fAlphaLogo = 1.0f;

					g_nCountAppearCountdown = 0;



				}

				SetColorCountdown();

				SetVertexCountdown(g_ExpantionLogo, g_ExpantionLogo);
			}
			else
			{
				//�C���^�[�o��
				g_nCountAppearCountdown++;

				//�C���^�[�o���I�������
				if (g_nCountAppearCountdown > COUNT_APPERA_COUNTDOWN)
				{
					g_fAlphaLogo = 0.0f;

					g_CountdownPattern--;

					//�g��l��������
					g_ExpantionLogo = 0;

					//���̃e�N�X�`��
					SetTextureCountdown(g_CountdownPattern);

					SetColorCountdown();

					if (g_CountdownPattern == 0)
					{
						countdown_flag = 1;
						g_nCountAppearCountdown = 0;
					}

				}
			}
		}


		if (countdown_flag == 1)
		{
			if (g_Shutsujin_pos_x > SHUTSUJIN_POS_X)
			{
				//�ړ�
				g_Shutsujin_pos_x -= 50.0f;
			}
			else if (g_Shutsujin_pos_x <= SHUTSUJIN_POS_X)
			{
				g_nCountAppearCountdown++;

			}

			if (g_nCountAppearCountdown > COUNT_APPERA_COUNTDOWN)
			{
				//�X�^�[�g
				countdown_flag = 2;

				//MySoundSetVolume(g_seKickoff, 0.5f);
				//MySoundPlayOnce(g_seKickoff);

			}

			SetVertexShutsujin();


		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (countdown_flag == 0)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffCountdown, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureCountdown);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
	if (countdown_flag == 1)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffShutsujin, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureShutsujin);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

	
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCountdown(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffCountdown,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(COUNTDOWN_POS_X+ COUNTDOWN_WIDTH, COUNTDOWN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y+ COUNTDOWN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(COUNTDOWN_POS_X+ COUNTDOWN_WIDTH, COUNTDOWN_POS_Y+ COUNTDOWN_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.3f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.3f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCountdown->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffShutsujin,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffShutsujin->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffShutsujin->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorCountdown(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCountdown->Unlock();
	}

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexCountdown(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(COUNTDOWN_POS_X - fSizeX, COUNTDOWN_POS_Y - fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(COUNTDOWN_POS_X + COUNTDOWN_WIDTH + fSizeX, COUNTDOWN_POS_Y - fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(COUNTDOWN_POS_X - fSizeX, COUNTDOWN_POS_Y + COUNTDOWN_HEIGHT + fSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(COUNTDOWN_POS_X + COUNTDOWN_WIDTH + fSizeX, COUNTDOWN_POS_Y + COUNTDOWN_HEIGHT + fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCountdown->Unlock();
	}
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexShutsujin(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffShutsujin->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_Shutsujin_pos_x, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_Shutsujin_pos_x + SHUTSUJIN_WIDTH, SHUTSUJIN_POS_Y + SHUTSUJIN_HEIGHT, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffShutsujin->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureCountdown(int nPattern)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		int x = nPattern % COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		int y = nPattern / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		float sizeX = 1.0f / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X;
		float sizeY = 1.0f / COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y;

		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffCountdown->Unlock();
	}
}

//=============================================================================
// �X�^�[�g�擾�֐�
//=============================================================================
int GetCountdownFlag(void)
{
	return countdown_flag;
}


