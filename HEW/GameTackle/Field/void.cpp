/**********************************************************************
[[���i���v���O����(GameTackle/Field/void.cpp)]]
	��ҁF���c�@�^�K

	���i���Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "../player.h"
#include "void.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define FIELDVOID_SIZEY		(1000.0f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldVoid(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldVoid(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldVoid(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldVoid,UpdateFieldVoid,DrawFieldVoid };	// ���Ǝ��̊֐�

#ifdef _DEBUG		
static Mesh g_meshBorder;		// ���̋��E�����킩�郁�b�V��(�f�o�b�O�̂݁j
static Texture g_texBorder;		// ���E�e�N�X�`��(�f�o�b�O�̂݁j
#endif
/*=====================================================================
�����������蔻��֐�
=====================================================================*/
bool CheckHitFieldVoid(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
#ifdef _DEBUG
	PrintDebugProc("[debug:field_checkhit]�F�ޗ��̈ז������ŗ��Ƃ��܂�");

#endif
	return false;

}

/*=====================================================================
�������X�V�֐�
=====================================================================*/
void UpdateFieldVoid(FIELD_CHIP* pData, Vec3* pPos)
{


}

/*=====================================================================
�������`��֐�
=====================================================================*/
void DrawFieldVoid(FIELD_CHIP* pData)
{
#ifdef _DEBUG		
	D3DDEVICE;
	pDevice->SetTexture(0, g_texBorder);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshBorder->DrawSubset(0);
#endif
}

/*=====================================================================
�������������֐�
=====================================================================*/
void InitFieldVoid()
{
#ifdef _DEBUG
	D3DDEVICE;

	// ���̏������쐬
	g_meshBorder = Create3DBoxMesh(&Vec3(FIELDCHIP_WIDTH, FIELDVOID_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3(0, 0, 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/yellow_halfalpha.png", &g_texBorder);
#endif
}

/*=====================================================================
�������I�����֐�
=====================================================================*/
void UninitFieldVoid()
{
#ifdef _DEBUG
	// ���\�[�X�̊J��
	SAFE_RELEASE(g_meshBorder);
	SAFE_RELEASE(g_texBorder);
#endif
}

/*=====================================================================
�������Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldVoidFunc()
{
	return &g_Func;
}