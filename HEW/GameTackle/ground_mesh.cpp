/**********************************************************************
[[�n�ʐ����v���O����(ground_mesh.cpp)]]
	��ҁF���c�@�^�K

	�v���C���[�����Ȃ��t�B�[���h�����Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "ground_mesh.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define INDEX_TYPE				WORD
#define NUM_GROUNDVERTEX		(50)

#define NUM_GROUNDINDEX			(2 * (NUM_GROUNDVERTEX * NUM_GROUNDVERTEX - NUM_GROUNDVERTEX + 2))

#define NUM_GROUNDAROUND		(2)
#define POOL_GROUND				((NUM_GROUNDAROUND*2)*(NUM_GROUNDAROUND*2))
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

typedef INDEX_TYPE* IndexPtr;
typedef VERTEX_3D* VertexPtr;

typedef struct {
	VtxBuff		Vtx;
	Matrix		WldMat;
}GROUND_MESH;


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static void CreateGroundIndexBuffer();			// �n�ʃC���f�b�N�X�����֐�


//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static IdxBuff g_IdxGroundMesh;
static GROUND_MESH g_Ground[POOL_GROUND];



void CreateGroundVertexBuffer()
{

}

/*=====================================================================
�C���f�b�N�X�ݒ�v���O����
=====================================================================*/
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