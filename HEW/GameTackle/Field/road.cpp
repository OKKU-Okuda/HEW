/**********************************************************************
[[���i���v���O����(GameTackle/Field/road.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../player.h"
#include "road.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos);
static void UpdateFieldRoad(FIELD_CHIP* pData);
static void DrawFieldRoad(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldRoad,UpdateFieldRoad,DrawFieldRoad };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��
static Texture g_texFlat;		// �^�񒆂̃e�N�X�`��

static Mesh g_meshRightWall;	// �E�̕�
static Mesh g_meshLeftWall;		// ���̕�




/*=====================================================================
�����֐�
	��������֐�
	�߂�l : void
	���� : void
=====================================================================*/
void InitFieldRoad()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	// ���E�̕Ǎ쐬
	g_meshRightWall= Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(FIELDROAD_X / 2, 25.0f/3, 0));
	g_meshLeftWall = Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(-FIELDROAD_X / 2, 25.0f / 3, 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texFlat);
}

bool CheckHitFieldRoad(FIELD_CHIP* pData,Vec3* pPos)
{
	if (pPos->x >= -FIELDROAD_X / 2 && pPos->x <= FIELDROAD_X / 2)
	{
		return true;
	}

	//SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2, FIELDROAD_X / 2);
	return false;
}

void UpdateFieldRoad(FIELD_CHIP* pData)
{


}

void DrawFieldRoad(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, g_texFlat);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);

	g_meshRightWall->DrawSubset(0);
	g_meshLeftWall->DrawSubset(0);
}

FIELD_OBJFUNC* GetFieldRoadFunc()
{
	return &g_Func;
}