/**********************************************************************
[[���i���v���O����(GameTackle/Field/road.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../field.h"
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
static bool CheckHitFieldRoad(FIELD_CHIP* pData);
static void UpdateFieldRoad(FIELD_CHIP* pData);
static void DrawFieldRoad(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldRoad,UpdateFieldRoad,DrawFieldRoad };	// ���Ǝ��̊֐�

static Mesh g_meshFlat;			// ���̐^��
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

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	// ���E�̕Ǎ쐬
	g_meshRightWall= Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(FIELDROAD_X / 2, 0, 0));
	g_meshLeftWall = Create3DBoxMesh(&Vec3(5.0f, 20.0f, FIELDCHIP_HEIGHT),
		&Vec3(-FIELDROAD_X / 2, 0, 0));

}

bool CheckHitFieldRoad(FIELD_CHIP* pData)
{
	
}
// ���[���h�s��̋t�s���
void UpdateFieldRoad(FIELD_CHIP* pData)
{

}

void DrawFieldRoad(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, NULL);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshFlat->DrawSubset(0);
	g_meshRightWall->DrawSubset(0);
	g_meshLeftWall->DrawSubset(0);
}