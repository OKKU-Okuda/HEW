/**********************************************************************
[[DirectX�w���v�w�b�_�[(directxHelper_2D.h)]]
	��ҁF���c�@�^�K

	directxHelper_2D.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once
#include "main.h"

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


// ��ʂ����ς��ɒ��_�o�b�t�@��錾����ϸ�
// �߂�l: VtxBuff 
#define MakeVertexBufferFullScreen											\
Make2DVertexBuffer(0, &D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f),	\
						&D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y));


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;


// �ʒu�A���a�A�T�C�Y�̕ۊǍ\����
// �|���S���̒��������]������ۂɎg�p
typedef struct {
	D3DXVECTOR3	Pos;		// ���S���W
	D3DXVECTOR2 Size;		// ���S����O�Ɍ������T�C�Y�i�܂�S�̃T�C�Y��1/2�j
	float		Radius;		// ���S������W�܂ł̋���
	float		BaseAngle;	// x���ƒ��S����e���_�ɐL�т���������p�x(���S��]�Ȃ̂ŋ���)
	float		Rot;		// ��]�p
	VERTEX_2D	vtx[4];		// ���_�f�[�^
}OBJ_2DDATA;

// ��L��]���Ȃ���
typedef struct {
	D3DXVECTOR3 pos;		// ���S���W
	D3DXVECTOR2 size;		// ���S����O�Ɍ������T�C�Y�i�܂�S�̃T�C�Y��1/2�j
	VERTEX_2D	vtx[4];		// ���_�f�[�^
}OBJ_BOX2DDATA;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
���_�o�b�t�@�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VtxBuff pVtx:���_�o�b�t�@
=====================================================================*/
void Draw2DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx);

/*=====================================================================
���_�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VERTEX_2D* pVtx�@:���_
=====================================================================*/
void Draw2DVertex(LPDIRECT3DTEXTURE9 pTex, VERTEX_2D* pVtx);

/*=====================================================================
���ՓI�ɒ��_�ݒu�֐�(�m�[�}��)
	�߂�l�Fvoid
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
=====================================================================*/
void MakeNormal2DVertex(int num, VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size);

/*=====================================================================
���ՓI�ɒ��_�o�b�t�@�쐬�֐�(�m�[�}��)
	�߂�l�FLPDIRECT3DVERTEXBUFFER9
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
	 OBJ_2DDATA *data		:�����]���������ꍇ�͂�����g���i�K�{�łȂ��j
	 float		Rot			:��]�p(�K�{�łȂ�)
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Make2DVertexBuffer(int num, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size, OBJ_2DDATA *data = NULL, float Rot = 0.0f);


/*=====================================================================
�J���[�ݒu�֐�(�m�[�}��)
	�߂�l�Fvoid
	�����F
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
 D3DCOLOR nColor			: �V�����J���[
=====================================================================*/
void Set2DVertexColor(VERTEX_2D *vtx_data, D3DCOLOR nColor);

/*=====================================================================
�e�N�X�`��2D���W�ݒu�֐�
	�߂�l�Fvoid
	�����F
VERTEX_2D *vtx_data,	:4�̒��_�z��
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set2DTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY);

/*=====================================================================
���_�̐ݒu���s���֐�
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	D3DXVECTOR3 *Pos,	:���S���W
	D3DXVECTOR2 *Size	:���S����̃T�C�Y
=====================================================================*/
void Set2DVerTex(VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size);

// -- OBJ_2DDATA�֘A---
/*=====================================================================
2d�I�u�W�F�N�g�f�[�^�ݒu�֐�(���������Ɏg���p)
	�߂�l�Fvoid
	�����F
		OBJ_2DDATA *data,
		D3DXVECTOR3 *pos,	:���S���W
		D3DXVECTOR2 *size,	:���S����̃T�C�Y
		float rot =0.0f		:���a
=====================================================================*/
void SetObj2Ddata(OBJ_2DDATA *data, D3DXVECTOR3 *pos, D3DXVECTOR2 *size, float rot = 0.0f);

/*=====================================================================
���a��ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float nRadius:�V�������a
=====================================================================*/
void Change2DSizeByRadius(OBJ_2DDATA *data, float nRadius);

/*=====================================================================
�{����ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float Times:�V�����{��
=====================================================================*/
void Change2DSizeByTimes(OBJ_2DDATA *data, float Times);

/*=====================================================================
�T�C�Y��ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float x:���R�m����
			float y:�c�̒���
=====================================================================*/
void Change2DSizeBySize(OBJ_2DDATA *data, float x, float y);

/*=====================================================================
���_�̐ݒu�𒴊ȒP�ɍs���֐�
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	D3DX_OBJ_DATA *data
=====================================================================*/
void SuperSet2DVerTex(VERTEX_2D *vtx_data, OBJ_2DDATA *data);