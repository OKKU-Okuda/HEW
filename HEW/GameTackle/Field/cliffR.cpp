/**********************************************************************
[[�E�R���v���O����(GameTackle/Field/cliffR.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "cliffR.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldCliffR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldCliffR,UpdateFieldCliffR,DrawFieldCliffR };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��
static Texture g_texFlat;		// �^�񒆂̃e�N�X�`��

static Mesh g_meshLeftWall;		// ���̕�

/*=====================================================================
�E�R�������蔻��֐�
=====================================================================*/
bool CheckHitFieldCliffR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���(�E���͔����ޗ��̈׏o��j

		if (pPos->x < -FIELDROAD_X / 2)
		{
			pPos->x = -FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
�E�R���X�V�֐�
=====================================================================*/
void UpdateFieldCliffR(FIELD_CHIP* pData, Vec3* pPos)
{

}

/*=====================================================================
�E�R���`��֐�
=====================================================================*/
void DrawFieldCliffR(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, g_texFlat);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	g_meshLeftWall->DrawSubset(0);
}

/*=====================================================================
�E�R���������֐�
=====================================================================*/
void InitFieldCliffR()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X / 2, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(-FIELDROAD_X / 4, 0, 0));

	// ���E�̕Ǎ쐬
	g_meshLeftWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texFlat);
}

/*=====================================================================
�E�R���I�����֐�
=====================================================================*/
void UninitFieldCliffR()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
	SAFE_RELEASE(g_meshLeftWall);
	SAFE_RELEASE(g_texFlat);
}

/*=====================================================================
�E�R���Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldCliffRFunc()
{
	return &g_Func;
}