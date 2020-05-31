/**********************************************************************
[[�t�B�[���h�n���\�[�X�ȈՊǗ��v���O����(GameTackle/Field/ResourceManager.cpp)]]
	��ҁF���c�@�^�K

	�t�B�[���h�Ԃ��܂����Ŏg�p����Resource���ȈՓI�ɊǗ�����v���O����
***********************************************************************/
#include "../../Core/main.h"

#include "../field.h"
#include "ResourceManager.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

static void CreateFieldTexture();		// �e�N�X�`���̓ǂݍ���
static void CreateFieldMesh();			// ���b�V���̓ǂݍ���

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

Texture g_texPool[MAX_FIELDSHARETEXTURE];
Mesh	g_meshPool[MAX_FIELDSHAREMESH];

/*=====================================================================
�e�N�X�`���ǂݍ��݊֐�
=====================================================================*/
void CreateFieldTexture()
{
	D3DDEVICE;

#ifdef _DEBUG
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_texPool[FTEX_NONE]);
#else
	g_texPool[FTEX_NONE] = NULL;
#endif

}

/*=====================================================================
���b�V���ǂݍ��݊֐�
=====================================================================*/
void CreateFieldMesh()
{
	g_meshPool[FMESH_LONGFLAT]		= Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, FIELDCHIP_HEIGHT), &Vec3(0, 0, 0));

	g_meshPool[FMESH_LONGWALLRIGHT] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

	g_meshPool[FMESH_LONGWALLLEFT] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, FIELDCHIP_HEIGHT),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), 0));

}

/*=====================================================================
�t�B�[���h���\�[�X�ǂݍ��݊֐�
=====================================================================*/
void InitFieldResource()
{
	// �O�̂��߃[���N���A
	ZeroMemory(g_texPool, sizeof(g_texPool));
	ZeroMemory(g_meshPool, sizeof(g_meshPool));

	CreateFieldTexture();
	CreateFieldMesh();
}

/*=====================================================================
�t�B�[���h���\�[�X�J���֐�
=====================================================================*/
void UninitFieldResource()
{
	for (int i = 0; i < MAX_FIELDSHARETEXTURE; i++)
	{// �e�N�X�`���̊J��
		SAFE_RELEASE(g_texPool[i]);
	}

	for (int i = 0; i < MAX_FIELDSHAREMESH; i++)
	{// ���b�V���̊J��
		SAFE_RELEASE(g_meshPool[i]);
	}
}

/*=====================================================================
�t�B�[���h���ʃe�N�X�`���擾�֐�
=====================================================================*/
Texture GetFieldShareTexture(FIELD_SHARETEXTURE type)
{
	return g_texPool[type];
}

/*=====================================================================
�t�B�[���h���ʃ��b�V���擾�֐�
=====================================================================*/
Mesh GetFieldShareMesh(FIELD_SHAREMESH type)
{
	return g_meshPool[type];
}
