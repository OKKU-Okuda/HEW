//=============================================================================
//
// �|�[�Y��ʏ��� [pause.cpp]
// Author : GP11A292 24 �����@�ċP
//
//=============================================================================
#include "pause.h"
#include"../Core/input.h"
#include"../Core/sound.h"
#include"../Core/fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PAUSE			"data/TEXTURE/background.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PAUSE_TUDUKERU	"data/TEXTURE/�Q�[���𑱂���.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PAUSE_YAMERU	"data/TEXTURE/�Q�[������߂�.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GAMEPAD			"data/TEXTURE/tutrial03.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PAUSE_TUDUKERU_POS_X	(400)								// �^�C�g�����S�̈ʒu(X���W)
#define	PAUSE_TUDUKERU_POS_Y	(500)								// �^�C�g�����S�̈ʒu(Y���W)
#define	PAUSE_TUDUKERU_WIDTH	(600)								// �^�C�g�����S�̕�
#define	PAUSE_TUDUKERU_HEIGHT	(130)								// �^�C�g�����S�̍���
#define	YAMERU_POS_X			(400)								// �X�^�[�g�{�^���̈ʒu(X���W)
#define	YAMERU_POS_Y			(600)								// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	YAMERU_WIDTH			(600)								// �X�^�[�g�{�^���̕�
#define	YAMERU_HEIGHT			(130)								// �X�^�[�g�{�^���̍���
#define	COUNT_APPERA_YAMERU		(60)								// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_YAMERU	(60)								// �X�^�[�g�{�^���_�ł̎���
#define	COUNT_WAIT_DEMO			(60 * 5)							// �f���܂ł̑҂�����
#define	GAMEPAD_POS_X			(350)								// �X�^�[�g�{�^���̈ʒu(X���W)
#define	GAMEPAD_POS_Y			(100)								// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	GAMEPAD_WIDTH			(450)								// �X�^�[�g�{�^���̕�
#define	GAMEPAD_HEIGHT			(330)								// �X�^�[�g�{�^���̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);
void SetColorHajimeru(bool key);
void SetColorYameru(bool key);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePause = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPause = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTudukeru = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTudukeru = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureYameru = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffYameru = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureGamepad = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffGamepad = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
bool                    g_pause_use = false;
bool                    g_pause_key = false;
static MySound			g_seSelection;					// �I����
static MySound			g_seDicidion;					// ���艹

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_pause_use = false;
	g_pause_key = false;

	// ���_���̍쐬
	MakeVertexPause(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PAUSE,				// �t�@�C���̖��O
			&g_pD3DTexturePause);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PAUSE_TUDUKERU,			// �t�@�C���̖��O
			&g_pD3DTextureTudukeru);	// �ǂݍ��ރ������[


		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PAUSE_YAMERU,			// �t�@�C���̖��O
			&g_pD3DTextureYameru);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAMEPAD,			// �t�@�C���̖��O
			&g_pD3DTextureGamepad);		// �ǂݍ��ރ������[
	}

	g_seSelection= MySoundCreate("data/SE/WallDown.wav");
	g_seDicidion = MySoundCreate("data/SE/WallDown.wav");

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{


	if(g_pD3DTexturePause != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePause->Release();
		g_pD3DTexturePause = NULL;
	}

	if(g_pD3DVtxBuffPause != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffPause->Release();
		g_pD3DVtxBuffPause = NULL;
	}

	if(g_pD3DTextureTudukeru != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTudukeru->Release();
		g_pD3DTextureTudukeru = NULL;
	}

	if(g_pD3DVtxBuffTudukeru != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffTudukeru->Release();
		g_pD3DVtxBuffTudukeru = NULL;
	}

	if(g_pD3DTextureYameru != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureYameru->Release();
		g_pD3DTextureYameru = NULL;
	}

	if(g_pD3DVtxBuffYameru != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffYameru->Release();
		g_pD3DVtxBuffYameru = NULL;
	}

	if (g_pD3DTextureGamepad != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureGamepad->Release();
		g_pD3DTextureGamepad = NULL;
	}

	if (g_pD3DVtxBuffGamepad != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffGamepad->Release();
		g_pD3DVtxBuffGamepad = NULL;
	}

	MySoundDelete(&g_seSelection);
	MySoundDelete(&g_seDicidion);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
#if 0
	if(g_nCountAppearYameru >= COUNT_APPERA_YAMERU)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_PAUSE);
		}
	}
#endif

	//�|�[�Y�{�^����������
	if (GetKeyboardTrigger(DIK_P)|| IsButtonTriggered(0, BUTTON_M))
	{
		//��ON�Ȃ�OFF OFF�Ȃ�ON
		g_pause_use =! g_pause_use;
	}
	//�|�[�YON�Ȃ�
	if (g_pause_use == true)
	{
		//�L�[���͂����
		if (GetKeyboardTrigger(DIK_DOWN)|| GetKeyboardTrigger(DIK_UP)
			|| IsButtonTriggered(0, BUTTON_UP)|| IsButtonTriggered(0, BUTTON_DOWN))
		{
			g_pause_key = !g_pause_key;
			// SE�Đ�
			MySoundSetVolume(g_seSelection, 0.5f);
			MySoundPlayOnce(g_seSelection);
		}

		if (GetKeyboardTrigger(DIK_RETURN)|| IsButtonTriggered(0, BUTTON_C))
		{
			if (g_pause_key == true)
			{
				//SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_TITLE);

				// SE�Đ�
				MySoundSetVolume(g_seSelection, 0.5f);
				MySoundPlayOnce(g_seSelection);


			}

			else if (g_pause_key == false)
			{
				g_pause_use = !g_pause_use;
			}

		}

		SetColorHajimeru(g_pause_key);
		SetColorYameru(g_pause_key);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (g_pause_use == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffPause, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePause);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffGamepad, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureGamepad);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffTudukeru, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureTudukeru);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffYameru, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureYameru);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}


}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffPause,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffPause->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffTudukeru,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffTudukeru->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X, PAUSE_TUDUKERU_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X + PAUSE_TUDUKERU_WIDTH, PAUSE_TUDUKERU_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X, PAUSE_TUDUKERU_POS_Y + PAUSE_TUDUKERU_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(PAUSE_TUDUKERU_POS_X + PAUSE_TUDUKERU_WIDTH, PAUSE_TUDUKERU_POS_Y + PAUSE_TUDUKERU_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
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

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffTudukeru->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffYameru,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffYameru->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(YAMERU_POS_X, YAMERU_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(YAMERU_POS_X + YAMERU_WIDTH, YAMERU_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(YAMERU_POS_X, YAMERU_POS_Y + YAMERU_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(YAMERU_POS_X + YAMERU_WIDTH, YAMERU_POS_Y + YAMERU_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
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

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffYameru->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffGamepad,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffGamepad->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(GAMEPAD_POS_X, GAMEPAD_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(GAMEPAD_POS_X + GAMEPAD_WIDTH, GAMEPAD_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(GAMEPAD_POS_X, GAMEPAD_POS_Y + GAMEPAD_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(GAMEPAD_POS_X + GAMEPAD_WIDTH, GAMEPAD_POS_Y + GAMEPAD_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
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

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffGamepad->Unlock();
	}



	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorHajimeru(bool key)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffTudukeru->Lock(0, 0, (void**)&pVtx, 0);

		if (key == false)
		{
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}


		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffTudukeru->Unlock();
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorYameru(bool key)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffYameru->Lock(0, 0, (void**)&pVtx, 0);

		if (key == true)
		{
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}


		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffYameru->Unlock();
	}

}

//=============================================================================
// �|�[�Y�擾�֐�
//=============================================================================
bool GetPauseUse(void)
{
	return g_pause_use;
}




