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
static bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos);
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
�����������蔻��֐�
=====================================================================*/
bool CheckHitFieldRoad(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{

	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// �O���W�������ł���ΊO�ɏo�Ȃ��悤�ɂ���

		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2,FIELDROAD_X / 2);
		return true;
	}

	if ((pPastPos->x < -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPos->x >= -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R) ||
		(pPastPos->x > (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R && pPos->x <= (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R))
	{	// ��or�E���F�O���璆�ɓ��낤�Ƃ���ƃu���b�N����鏈��
		pPos->x = pPastPos->x;

#ifdef _DEBUG
		PrintDebugProc("[debug:field_checkhit]�F�O����̐N���j�~");
#endif
	}

	return false;
}

/*=====================================================================
�������X�V�֐�
=====================================================================*/
void UpdateFieldRoad(FIELD_CHIP* pData, Vec3* pPos)
{


}

/*=====================================================================
�������`��֐�
=====================================================================*/
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

/*=====================================================================
�������������֐�
=====================================================================*/
void InitFieldRoad()
{
	D3DDEVICE;

	// ���̏������쐬
	g_meshFlat = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	// ���E�̕Ǎ쐬
	g_meshRightWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));
	g_meshLeftWall = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texFlat);
}

/*=====================================================================
�������I�����֐�
=====================================================================*/
void UninitFieldRoad()
{
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshFlat);
	SAFE_RELEASE(g_meshLeftWall);
	SAFE_RELEASE(g_meshRightWall);
	SAFE_RELEASE(g_texFlat);
}

/*=====================================================================
�������Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldRoadFunc()
{
	return &g_Func;
}