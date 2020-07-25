/**********************************************************************
[[�t�F�[�Y:Result�v���O����(Phase_Result.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FResult�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"
#include "../Phase/Phase_GameTackle1.h"


#include "Phase_Result.h"	
#include "Phase_Title.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static PHASE_FUNC g_PhaseFunc = { InitResult,UninitResult,UpdateResult,DrawResult };

LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureNewGameLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffNewGameLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureExitLogo = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExitLogo = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;				// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D				g_vertexWk[NUM_VERTEX];				// ���_���i�[���[�N

static ICON						g_Icon[ICON_CURSOR_MAX];			// �A�C�R��

/*=====================================================================
Result�X�V�֐�
=====================================================================*/
void UpdateResult()
{
	PrintDebugProc("���U���g�t�F�[�Y");

	//// ���̃t�F�[�Y�ɍs��
	//if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
	//{	// �^�b�N���P
	//	GoNextPhase(GetPhaseTitleFunc());
	//}

	for (int i = 0; i < ICON_CURSOR_MAX; i++)
	{
		/*�Z�Z�L�[����������|�[�Y���*/
		if (g_Icon[i].use == true)
		{
			if ((GetKeyboardTrigger(DIK_RETURN))/* || (IsButtonTriggered(0, BUTTON_C))*/)
			{

				/*TITLE*/
				if (g_Icon[i].pos.x == RESULT_TITLE_LOGO_POS_X)
				{
					GoNextPhase(GetPhaseTitleFunc());


					g_Icon[i].use = false;										//���O�r�[�A�C�R��������

					g_Icon->pos.x = ICON_MIN;									//���O�r�[�A�C�R���������ʒu�ɖ߂�

				}

				/*NEWGAME*/
				if (g_Icon[i].pos.x == RESULT_NEWGAME_LOGO_POS_X)
				{
					GoNextPhase(GetPhaseGameTackle1Func());



					g_Icon[i].use = false;										//���O�r�[�A�C�R��������

					g_Icon->pos.x = ICON_MIN;									//���O�r�[�A�C�R���������ʒu�ɖ߂�
				}

				/*EXIT*/
				if (g_Icon[i].pos.x == RESULT_EXIT_LOGO_POS_X)
				{
					DestroyWindow(GetHandle());
				}
			}

			/*���L�[*/
			if (GetKeyboardTrigger(DIK_LEFT)/* || IsButtonTriggered(0, BUTTON_UP)*/)
			{

				if (g_Icon[i].pos.x >= ICON_MIN)
				{
					g_Icon[i].pos.x -= ICON_REN;
				}
				if (g_Icon[i].pos.x < ICON_MIN)
				{
					g_Icon[i].pos.x = ICON_MAX;
				}
			}

			/*�E�L�[*/
			if (GetKeyboardTrigger(DIK_RIGHT)/* || IsButtonTriggered(0, BUTTON_DOWN)*/)
			{

				if (g_Icon[i].pos.x <= ICON_MAX)
				{
					g_Icon[i].pos.x += ICON_REN;
				}
				if (g_Icon[i].pos.x > ICON_MAX)
				{
					g_Icon[i].pos.x = ICON_MIN;
				}
			}
			//SetVertexIcon(i);
		}
	}

}

/*=====================================================================
Result�`��֐�
=====================================================================*/
void DrawResult()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*���U���g���*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResult);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*TITLE*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResultTitleLogo, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResultTitleLogo);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*NEW GAME*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffNewGameLogo, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureNewGameLogo);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	/*EXIT*/
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffExitLogo, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureExitLogo);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}

/*=====================================================================
Result�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitResult(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
				TEXTURE_RESULT,				// �t�@�C���̖��O
				&g_pD3DTextureResult);		// �ǂݍ��ރ������[

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
				TEXTURE_NEWGAME_LOGO,		// �t�@�C���̖��O
				&g_pD3DTextureNewGameLogo);	// �ǂݍ��ރ������[

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
				TEXTURE_TITLE_LOGO,		// �t�@�C���̖��O
				&g_pD3DTextureResultTitleLogo);	// �ǂݍ��ރ������[

					// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
				TEXTURE_EXIT_LOGO,		// �t�@�C���̖��O
				&g_pD3DTextureExitLogo);	// �ǂݍ��ރ������[


		// ���_���̍쐬
		MakeVertexResult(pDevice);



		for (int i = 0; i < ICON_CURSOR_MAX; i++)
		{
			g_Icon[i].use = true;

			/*�A�C�R���̈ʒu*/
			g_Icon[i].pos = D3DXVECTOR3(90.0f, 520.0f, 0.0f);

			// ���_���̍쐬
			//MakeVertexIcon();
		}


		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------


}

/*=====================================================================
Result�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitResult(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------

	if (g_pD3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if (g_pD3DVtxBuffResult != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if (g_pD3DTextureNewGameLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureNewGameLogo->Release();
		g_pD3DTextureNewGameLogo = NULL;
	}

	if (g_pD3DVtxBuffNewGameLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffNewGameLogo->Release();
		g_pD3DVtxBuffNewGameLogo = NULL;
	}

	if (g_pD3DTextureResultTitleLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultTitleLogo->Release();
		g_pD3DTextureResultTitleLogo = NULL;
	}

	if (g_pD3DVtxBuffResultTitleLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResultTitleLogo->Release();
		g_pD3DVtxBuffResultTitleLogo = NULL;
	}

	if (g_pD3DTextureExitLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureExitLogo->Release();
		g_pD3DTextureExitLogo = NULL;
	}

	if (g_pD3DVtxBuffExitLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffExitLogo->Release();
		g_pD3DVtxBuffExitLogo = NULL;
	}



	if (isEnd == false)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------


}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResult,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResult->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffNewGameLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffNewGameLogo->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X, RESULT_NEWGAME_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X + RESULT_NEWGAME_LOGO_WIDTH, RESULT_NEWGAME_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X, RESULT_NEWGAME_LOGO_POS_Y + RESULT_NEWGAME_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_NEWGAME_LOGO_POS_X + RESULT_NEWGAME_LOGO_WIDTH, RESULT_NEWGAME_LOGO_POS_Y + RESULT_NEWGAME_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffNewGameLogo->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResultTitleLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResultTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X, RESULT_TITLE_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X + RESULT_TITLE_LOGO_WIDTH, RESULT_TITLE_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X, RESULT_TITLE_LOGO_POS_Y + RESULT_TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_TITLE_LOGO_POS_X + RESULT_TITLE_LOGO_WIDTH, RESULT_TITLE_LOGO_POS_Y + RESULT_TITLE_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffResultTitleLogo->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffExitLogo,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExitLogo->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X, RESULT_EXIT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X + RESULT_EXIT_LOGO_WIDTH, RESULT_EXIT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X, RESULT_EXIT_LOGO_POS_Y + RESULT_EXIT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_EXIT_LOGO_POS_X + RESULT_EXIT_LOGO_WIDTH, RESULT_EXIT_LOGO_POS_Y + RESULT_EXIT_LOGO_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffExitLogo->Unlock();
	}


	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorResultLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResult->Unlock();
	}
}

/*=====================================================================
Result��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc()
{
	return &g_PhaseFunc;
}