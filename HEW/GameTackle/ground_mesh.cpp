/**********************************************************************
[[�n�ʐ����v���O����(ground_mesh.cpp)]]
	��ҁF���c�@�^�K

	�v���C���[�����Ȃ��t�B�[���h�����Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "field.h"
#include "ground_mesh.h"
#include "player.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define GROUNDSIZE				(FIELDCHIP_WIDTH * 3.5)

#define GROUND_POSY				(-75.f)
//#define FOR(i,exit)				for(int (i) = 0; (i) < (exit); (i)++)
#define RATE_POS				(0.0002f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
//static void CreateGroundIndexBuffer();			// �n�ʃC���f�b�N�X�����֐�
static void ChangeTexturePos();

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static VtxBuff	g_Vtx;
static Matrix	g_Mat;		// �s��
static Texture	g_Tex;

/*=====================================================================
�n�ʍX�V�֐�
=====================================================================*/
void UpdateGround()
{
	Vec3 pos_ground = *GetPlayerPos();
	pos_ground.y = GROUND_POSY;
	GetMatrix(&g_Mat, &pos_ground, &Vec3(D3DX_PI / 2, 0, 0));


	// �ʒu�Ńe�N�X�`���̕ύX
	ChangeTexturePos();

}

/*=====================================================================
�n�ʕ`��֐�
=====================================================================*/
void DrawGround()
{
	Draw3DVertexBuffer(g_Tex, g_Vtx, &g_Mat);
}

/*=====================================================================
�n�ʏ������֐�
=====================================================================*/
void InitGround()
{
	D3DDEVICE;
	g_Vtx = Create3DPolygon(&Vec2(GROUNDSIZE, GROUNDSIZE));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field005.jpg", &g_Tex);

}


/*=====================================================================
�n�ʃe�N�X�`�����W�ύX�֐�
=====================================================================*/
void ChangeTexturePos()
{
	VERTEX_3D* pVtx = NULL;
	Vec2	  TexPos;

	TexPos.x = GetPlayerPos()->x * RATE_POS;
	TexPos.y = -GetPlayerPos()->z * RATE_POS;

	g_Vtx->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex.x =
		pVtx[2].tex.x = TexPos.x - 0.5f;
	pVtx[1].tex.x =
		pVtx[3].tex.x = TexPos.x + 0.5f;

	pVtx[0].tex.y =
		pVtx[1].tex.y = TexPos.y - 0.5f;
	pVtx[2].tex.y =
		pVtx[3].tex.y = TexPos.y + 0.5f;

	g_Vtx->Unlock();
}

#if 0
void CreateGroundIndexBuffer()
{

	D3DDEVICE;
	IndexPtr	pIdx = NULL;
	DWORD		idx_array = 0u;

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(INDEX_TYPE) * NUM_GROUNDINDEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_IdxGroundMesh,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�


	g_IdxGroundMesh->Lock(0, 0,(void**)&pIdx, 0);

	for (int i = 0; i < NUM_GROUNDVERTEX - 1; i++)
	{
		if (i != 0)
		{
			pIdx[idx_array++] = i;
		}

		for (int j = i; j <= (NUM_GROUNDVERTEX - 1) * 10 + i + 1;)
		{
			pIdx[idx_array++] = j;


			if (j % 10 == i % 10)
			{
				j++;
			}
			else
			{
				j += 10;
			}
		}

		if (i != NUM_GROUNDVERTEX - 2)
		{
			pIdx[idx_array++] = (NUM_GROUNDVERTEX - 1) * 10 + i + 1;
		}
	}

	g_IdxGroundMesh->Unlock();
}
#endif