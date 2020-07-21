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

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field01.png", &g_texPool[FTEX_NONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_wall.png", &g_texPool[FTEX_WALL]);

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

	// ����t�B�[���h�n�̃��b�V��
	{
		const float sizeZ = (FIELDCHIP_HEIGHT - FIELDROAD_X) / 2;
		const float posZ = FIELDCHIP_CENTER_Z - (sizeZ / 2);

		const float size_flatX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2);
		const float pos_flatX = size_flatX / 2 + (FIELDROAD_X / 2);

		const float size_WallX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2) - ROADWALL_SIZEX;
		const float pos_WallX = pos_flatX + (ROADWALL_SIZEX / 2);

		g_meshPool[FMESH_LEFTFRONTWALL] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
			&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

		g_meshPool[FMESH_RIGHTFRONTWALL] = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
			&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));



		g_meshPool[FMESH_LEFTCENTERWALL] = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
			&Vec3(-pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));

		g_meshPool[FMESH_RIGHTCENTERWALL] = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
			&Vec3(pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));


		g_meshPool[FMESH_FRONTFLAT] = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ + FIELDROAD_X), &Vec3(0, 0, -posZ + (FIELDROAD_X / 2)));


		g_meshPool[FMESH_LEFTFLAT] = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(-pos_flatX, 0, 0));
		g_meshPool[FMESH_RIGHTFLAT] = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(pos_flatX, 0, 0));
	}

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
