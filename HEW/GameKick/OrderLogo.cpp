/**********************************************************************
[[OrderLogo�v���O����(OrderLogo.cpp)]]
	��ҁF���c�@�^�K

	OrderLogo�Ɋւ���v���O����
***********************************************************************/
#include "../Core/input.h"
#include"../Core/main.h"
#include "OrderLogo.h"	

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

// ��{�֐��Q
static void InitOrderLogo(bool isFirst);
static void UninitOrderLogo(bool isEnd);
static void UpdateOrderLogo();
static void DrawOrderLogo();

static void SetLogo5Vertex(VERTEX_2D *pVtx, Vec3* pBottomPos, Vec2*pBaseSize, float difference);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static OBJ_FUNC g_Func = { InitOrderLogo,UninitOrderLogo,UpdateOrderLogo,DrawOrderLogo };

static struct {
	Texture		pTex;
	VERTEX_2D	Vtx[5];
	WORD		Idx[8] = { 0,1,2,4,3,3,2,0 };
	Vec3		bottomPos;
	Vec2		baseSize;
	float		difference;
	float		rot;
}g_Kick;

/*=====================================================================
OrderLogo�X�V�֐�
=====================================================================*/
void UpdateOrderLogo()
{

	const float spd = 10.0f;

	g_Kick.rot += 0.1f;
	g_Kick.difference = sinf(g_Kick.rot)*100.0f;

	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_Kick.bottomPos.x += spd;
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		g_Kick.bottomPos.x -= spd;
	}

	if (GetKeyboardPress(DIK_UP))
	{
		g_Kick.bottomPos.y -= spd;
	}

	if (GetKeyboardPress(DIK_DOWN))
	{
		g_Kick.bottomPos.y += spd;
	}

	SetLogo5Vertex(g_Kick.Vtx, &g_Kick.bottomPos, &g_Kick.baseSize, g_Kick.difference);
}

/*=====================================================================
OrderLogo�`��֐�
=====================================================================*/
void DrawOrderLogo()
{
	D3DDEVICE;

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, g_Kick.pTex);

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, 5, 6, g_Kick.Idx, D3DFMT_INDEX16, g_Kick.Vtx, sizeof(VERTEX_2D));
	//pDevice->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, 5, 6, g_KickIdx, D3DFMT_INDEX16, g_KickVtx, sizeof(VERTEX_2D));

//	pDevice->DrawIndexedPrimitiveUP()
}

/*=====================================================================
OrderLogo�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitOrderLogo(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		D3DDEVICE;

		for (int i = 0; i < 5; i++)
		{
			Make2DVertex(&g_Kick.Vtx[i], &Vec3(0, 0, 0), &Vec2(0, 0));
		}

		Make2DVertex(&g_Kick.Vtx[0], &Vec3(SCREEN_CENTER_X-100, SCREEN_CENTER_Y-50, 0),	&Vec2(0, 0));		// ����
		Make2DVertex(&g_Kick.Vtx[1], &Vec3(SCREEN_CENTER_X+100, SCREEN_CENTER_Y-50, 0), &Vec2(1, 0));		// �E��
		Make2DVertex(&g_Kick.Vtx[2], &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0), &Vec2(0.5f, 0.5f));			// ����
		Make2DVertex(&g_Kick.Vtx[3], &Vec3(SCREEN_CENTER_X-100, SCREEN_CENTER_Y+50, 0), &Vec2(0, 1));		// ����
		Make2DVertex(&g_Kick.Vtx[4], &Vec3(SCREEN_CENTER_X+100, SCREEN_CENTER_Y+50, 0), &Vec2(1, 1));		// �E��

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LastKick.png", &g_Kick.pTex);
		//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TEST.png", &g_KickTex);

		return;
	}

	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	// �L�b�N���[�N�n�X�e�[�^�X
	g_Kick.baseSize		= Vec2(300, 150);
	g_Kick.bottomPos	= Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0);
	g_Kick.difference	= 0;

}

/*=====================================================================
OrderLogo�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitOrderLogo(bool isEnd)
{
	//---------------------------------------------------------------------
	//	���̑��̏I������
	//---------------------------------------------------------------------





	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------




}

/*=====================================================================
OrderLogo��{�֐��Q�擾�֐�
=====================================================================*/
OBJ_FUNC* GetOrderLogoFunc()
{
	return &g_Func;
}

void SetLogo5Vertex(VERTEX_2D *pVtx, Vec3* pBottomPos, Vec2*pBaseSize, float difference)
{
	// ����̒��_�ݒu
	pVtx[3].vtx.y =
		pVtx[4].vtx.y = pBottomPos->y;

	pVtx[3].vtx.x = pBottomPos->x - pBaseSize->x;
	pVtx[4].vtx.x = pBottomPos->x + pBaseSize->x;

	// ���̒��_�ݒu
	pVtx[0].vtx.y =
		pVtx[1].vtx.y = pBottomPos->y - (pBaseSize->y * 2.0f);

	pVtx[0].vtx.x = pVtx[3].vtx.x + difference;
	pVtx[1].vtx.x = pVtx[4].vtx.x + difference;

	// �����̒��_�ݒu
	pVtx[2].vtx.y = pBottomPos->y - pBaseSize->y;

	pVtx[2].vtx.x = pBottomPos->x + (difference / 2.0f);

}