/**********************************************************************
[[���R���v���O����(GameTackle/Field/cliffL.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "cliffL.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffL(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffL(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffL(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffL,UpdateFieldCliffL,DrawFieldCliffL };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��
static Texture g_texFlat;		// �^�񒆂̃e�N�X�`��

static Mesh g_meshRightWall;	// �E�̕�

/*=====================================================================
���R�������蔻��֐�
=====================================================================*/
bool CheckHitFieldCliffL(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R && pPastPos->x > -PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���(�����͔����ޗ��̈׏o��j

		if (pPos->x > FIELDROAD_X / 2)
		{
			pPos->x = FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
���R���X�V�֐�
=====================================================================*/
void UpdateFieldCliffL(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
���R���`��֐�
=====================================================================*/
void DrawFieldCliffL(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, g_texFlat);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	g_meshRightWall->DrawSubset(0);
}

/*=====================================================================
���R���������֐�
=====================================================================*/
void InitFieldCliffL()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 2, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(FIELDROAD_X / 4, 0, 0));

	// �����̕Ǎ쐬
	g_meshRightWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texFlat);
}

/*=====================================================================
���R���I�����֐�
=====================================================================*/
void UninitFieldCliffL()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
	SAFE_RELEASE(g_meshRightWall);
	SAFE_RELEASE(g_texFlat);
}

/*=====================================================================
���R���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffLFunc()
{
	return &g_Func;
}