/**********************************************************************
[[DirectX�w���v�w�b�_�[(directxHelper_3D.h)]]
	��ҁF���c�@�^�K

	directxHelper_3D.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once
//#include <d3d9.h>
//#include <d3dx9.h>

#include "main.h"


// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

// �x�N�g���^�C�v�񋓌^
enum VECTER_TYPE {
	POS,
	ROT,
	SCL,
	MAX_VECTER_NAME
};

// �ۊǗp�f�[�^�̍쐬
typedef struct INTERPOLATION_STATE
{
	D3DXVECTOR3 State[MAX_VECTER_NAME];
}INTERPOLATION_STATE;

// X���b�V���\����
typedef struct Xmodel{
	LPD3DXMESH		pMesh;			// ���b�V��
	D3DXMATRIX		WldMtx;			// ���[���h�}�g���N�X
	DWORD			nNummat;		// �}�e���A��
	D3DXMATERIAL	*pMaterial;		// �}�e���A���z��
}Xmodel;
typedef Xmodel*			Model;				// X�t�@�C���̂Ł[��	

typedef LPD3DXMESH		Mesh;				// ���b�V���f�[�^

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
/*=====================================================================
3D���_�o�b�t�@�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VtxBuff pVtx:���_�o�b�t�@
=====================================================================*/
void Draw3DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx, D3DXMATRIX *Mat);

/*=====================================================================
	�|���S�����������֐�
�߂�l:LPDIRECT3DVERTEXBUFFER9
�����@:
D3DXVECTOR2 size,	:�T�C�Y
int X_parts,			�F(�e�N�X�`������)
int Y_parts,
int nowX,
int nowY
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Create3DPolygon(D3DXVECTOR2 *size, int X_parts = 1, int Y_parts = 1, int nowX = 0, int nowY = 0);

/*=====================================================================
	�{�b�N�X���b�V���쐬�֐�
�߂�l:LPD3DXMESH	:���b�V��
�����@:
D3DXVECTOR3 *size, �F�{�b�N�X�̃T�C�Y
D3DXVECTOR3 *pos	�F�����̃��[�J���ʒu
=====================================================================*/
LPD3DXMESH Create3DBoxMesh(D3DXVECTOR3 *size, D3DXVECTOR3 *pos);

/*=====================================================================
	�r���{�[�h�s��擾�֐�
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:�o�͍s��ւ̃A�h���X
CAMERA *cam,		:�J�����f�[�^
D3DXVECTOR3 *pos,	:(�ȉ�3�͕K�{�łȂ�)
D3DXVECTOR3 *rot,
D3DXVECTOR3 *scl
=====================================================================*/
D3DXMATRIX *GetBillboardMatrix(D3DXMATRIX *pOut,D3DXVECTOR3 *pos = &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3 *rot = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 *scl = &D3DXVECTOR3(1.0f, 1.0f, 1.0f));

/*=====================================================================
	�t�s��擾�֐�
�t�s����v�Z���ċ��߂�֐�
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:�ʒu�n�̂Ł[���͎��O�ɂ���邱��
D3DXMATRIX *pOri	:�t�s��ɂ������s��
=====================================================================*/
D3DXMATRIX *GetInverseMatrix(D3DXMATRIX *pOut, D3DXMATRIX *pOri);

/*=====================================================================
	�s��擾�֐�
�s����v�Z���ċ��߂�֐�
	�� �v�Z��SCL,ROT,POS�̏��ōs�� ��
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:���ʂ�ۊǂ���A�h���X
D3DXVECTOR3 *pos,	:�ʒu(�K�{�ł͂Ȃ�)
D3DXVECTOR3 *rot,	:��](�K�{�ł͂Ȃ�)
D3DXVECTOR3 *scl	:�X�P�[��(�K�{�ł͂Ȃ�)
bool isReset		:���Z�b�g�̗L���i�K�{�ł͂Ȃ��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�j
=====================================================================*/
D3DXMATRIX *GetMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 *rot = &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3 *scl = &D3DXVECTOR3(1.0f, 1.0f, 1.0f), bool isReset = true);

/*=====================================================================
3D���_�ύX�֐�
	�߂�l�Fvoid
	�����F
VERTEX_3D *vtx_data,	:4�̒��_�z��
 D3DXVECTOR2 *size		:�T�C�Y
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set3DVertex(VERTEX_3D *vtx_data, D3DXVECTOR2 *size, int X_parts = 1, int Y_parts = 1, int nowX = 0, int nowY = 0);

/*=====================================================================
�e�N�X�`��3D���W�ݒu�֐�
	�߂�l�Fvoid
	�����F
VERTEX_3D *vtx_data,	:4�̒��_�z��
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set3DTexPos(VERTEX_3D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);


/*=====================================================================
���f���`��֐�
	���f���ɕt�^����Ă���e�N�X�`�����ŕ`�悳���
�߂�l:void
�����@:Xmodel* pModel :���f���i�[�\����
=====================================================================*/
void DrawModel(Xmodel* pModel);

/*=====================================================================
�ʍs��g�p���f���`��֐�
�߂�l:void
�����@:
Xmodel* pModel		:���f���i�[�\����
D3DXMATRIX* pMat	�F�s��
=====================================================================*/
void DrawModelWithOtherMatrix(Xmodel* pModel, D3DXMATRIX* pMat);

/*=====================================================================
���f���쐬�֐�
	xfile���f����ǂݍ��݁A�֘A�̃e�N�X�`�����ǂݍ��ފ֐�
	�� xfile�Ŏg�p����e�N�X�`���͓���f�B���N�g�����ɒu������ ��
�߂�l:void
�����@:const char * filename:�t�@�C���p�X
=====================================================================*/
Xmodel *CreateModel(const char * filename);

/*=====================================================================
���f���폜�֐�
�߂�l:void
�����@:Xmodel** ppModel�@:���f���\���̂̏ꏊ��ۊǂ��Ă���ꏊ
=====================================================================*/
void DeleteModel(Xmodel** ppModel);
