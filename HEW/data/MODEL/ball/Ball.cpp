/**********************************************************************
[[���C���v���O����(Ball.cpp)]]
	��ҁF���c�@�^�K

	���O�r�[�{�[���Ɋւ���v���O����
***********************************************************************/
#include "main.h"


#include "Ball.h"
#include "shadow.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define BALL_ADDRESS ("data/MODEL/ball.x")
#define BALL_TEX_ADDRESS ("data/TEXTURE/ball.jpg")

#define BALL_ZANZOU_RATE

#define BALL_FIRLD_Y		(0.0f)
#define BALL_SIZE_Y			(20.0f)
#define BALL_MOVE_Y			(0.08f)

#define BALL_REFLECT_RATE	(0.8f)
#define BALL_XZ_DOWN		(0.996f)
#define BALL_SPD_DOWN		(0.005f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------
typedef struct BALL {
	bool	isUse;
	bool	Moved;
	D3DXVECTOR3 pos;
	//D3DXVECTOR3 past_pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXVECTOR3 eng;
	float		rotz;

	bool	isZanzou;
	float   Zanzou_time;
	int					shadowIdx;
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

}BALL;
//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static BALL					g_Ball[MAX_BALL];
static LPDIRECT3DTEXTURE9	g_pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
static LPD3DXMESH			g_pD3DXMesh;		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			g_pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
static DWORD				g_nNumMat;			// �}�e���A�����̐�

/*=====================================================================
	���C���֐�

=====================================================================*/
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(BALL_ADDRESS,// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,					// ���b�V���̍쐬�I�v�V�������w��
		pDevice,							// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,								// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXBuffMat,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,								// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat,							// D3DXMATERIAL�\���̂̐�
		&g_pD3DXMesh)))						// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		BALL_TEX_ADDRESS,
		&g_pD3DTexture)))
	{
		return E_FAIL;
	}

	// �[���N���A
	ZeroMemory(&g_Ball[0], sizeof(BALL)*MAX_BALL);

	return S_OK;
}

int CreateBall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 eng, D3DXVECTOR3 scl)
{

	for (int Ball_ID = 0; Ball_ID < MAX_BALL; Ball_ID++)
	{
		if (g_Ball[Ball_ID].isUse == false)
		{
			// �[���N���A
			ZeroMemory(&g_Ball[Ball_ID], sizeof(BALL));

			// �X�e�[�^�X�̑��
			g_Ball[Ball_ID].isUse = true;
			g_Ball[Ball_ID].pos = pos;
			//g_Ball[Ball_ID].past_pos = pos;
			g_Ball[Ball_ID].rot = rot;
			g_Ball[Ball_ID].scl = scl;
			g_Ball[Ball_ID].eng = eng;
			g_Ball[Ball_ID].isZanzou = false;
			g_Ball[Ball_ID].Moved = true;
			g_Ball[Ball_ID].rotz = -1.0f;
			g_Ball[Ball_ID].shadowIdx = CreateShadow(pos, g_Ball[Ball_ID].scl);

			return Ball_ID;
		}
	}

	// �{�[���������ɋ󂫂�����܂���
	return -1;
}

void DeleteBall(int Ball_ID)
{
	if (Ball_ID < 0)
	{ // ���̐��������ɂ����ꍇ�͂��ׂẴ{�[�����폜����
		for (int i = 0; i < MAX_BALL;i++)
		{
			g_Ball[i].isUse = false;
			ReleaseShadow(g_Ball[i].shadowIdx);
		}
	}
	else if (Ball_ID < MAX_BALL)
	{
		g_Ball[Ball_ID].isUse = false;
		ReleaseShadow(g_Ball[Ball_ID].shadowIdx);
	}

	return;
}

void UpdateBall(void)
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		float move_z_rate = 1.0f, move_x_rate = 1.0f;
		float spd = 0;
		if (g_Ball[i].isUse == false)continue;

		D3DXVECTOR3 keep_pos = g_Ball[i].pos;

		g_Ball[i].Moved = true;

		//g_Ball[i].rot += D3DXVECTOR3(0.01f, 0.01f ,0.01f);

		// �G�l���M�[���Z�̊ȗ���

		// --y���ړ�����
		{
			g_Ball[i].eng.y -= BALL_MOVE_Y;
			g_Ball[i].pos.y += g_Ball[i].eng.y;

			if (g_Ball[i].pos.y - BALL_SIZE_Y < BALL_FIRLD_Y)
			{
				g_Ball[i].pos.y = BALL_SIZE_Y;
				g_Ball[i].eng.y *= -BALL_REFLECT_RATE;

				move_z_rate = move_z_rate = 0.5f;	// �n�ʂɋ߂��ƌ������ɂ����Ȃ�
				g_Ball[i].rotz *= -1.0f;
			}
		}

		// Z�ړ�����
		{
			g_Ball[i].eng.z *= BALL_XZ_DOWN;

			if (g_Ball[i].eng.z < 0)
			{
				move_z_rate *= -1;
			}
			g_Ball[i].eng.z -= BALL_SPD_DOWN * move_z_rate;
			if (fabsf(g_Ball[i].eng.z) < 0.001f)g_Ball[i].eng.z = 0.0f;
			g_Ball[i].pos.z += g_Ball[i].eng.z;
		}
		
		// X�ړ�����
		{
			g_Ball[i].eng.x *= BALL_XZ_DOWN;

			if (g_Ball[i].eng.x < 0)
			{
				move_x_rate *= -1;
			}
			g_Ball[i].eng.x -= BALL_SPD_DOWN * move_x_rate;
			if (fabsf(g_Ball[i].eng.x) < 0.001f)g_Ball[i].eng.x = 0.0f;
			g_Ball[i].pos.x += g_Ball[i].eng.x;
		}


		// �e���v���C���[�̈ʒu�ɍ��킹��
		D3DXVECTOR3 pos = g_Ball[i].pos;
		pos.y = 0.0f;
		SetPositionShadow(g_Ball[i].shadowIdx, pos, g_Ball[i].scl);

		// �ړ������̎Z�p
		spd = D3DXVec3LengthSq(&(keep_pos - g_Ball[i].pos));
		if (spd <= 0.005f)
		{
			DeleteBall(i);
			return;
		}
		g_Ball[i].rot.z += spd / 75 * g_Ball[i].rotz;
	}
}

void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	//pD3DXMat->pTextureFilename
	/*
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);				// Z�o�b�t�@�[�̏������݂����Ȃ�
	*/

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �v���C���[�̕`��
	for (int i = 0; i < MAX_BALL; i++)
	{
		// �g�p���Ă��Ȃ��@���́@���Z��ړ������̂Ȃ��@�ꍇ�͂��̏������΂�
		if (g_Ball[i].isUse == false && g_Ball[i].Moved == false)continue;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Ball[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Ball[i].scl.x, g_Ball[i].scl.y, g_Ball[i].scl.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Ball[i].rot.y, g_Ball[i].rot.x, g_Ball[i].rot.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Ball[i].pos.x, g_Ball[i].pos.y, g_Ball[i].pos.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Ball[i].mtxWorld);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMat->GetBufferPointer();

		//g_pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTexture);

			// �`��
			g_pD3DXMesh->DrawSubset(nCntMat);
		}

		// �ғ����ǂ���
		g_Ball[i].Moved = false;

	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
	/*
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�o�b�t�@�[�̏������݂�����

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	*/
}




void UninitBall(void)
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		if (g_pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTexture->Release();
			g_pD3DTexture = NULL;
		}

		if (g_pD3DXMesh != NULL)
		{// ���b�V���̊J��
			g_pD3DXMesh->Release();
			g_pD3DXMesh = NULL;
		}

		if (g_pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_pD3DXBuffMat->Release();
			g_pD3DXBuffMat = NULL;
		}
	}
}