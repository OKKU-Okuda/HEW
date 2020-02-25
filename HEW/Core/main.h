//=============================================================================
//
// ���C������ [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "windows.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"			// DirectInput�֘A�̃w�b�_�[
#include "xaudio2.h"		// XAudio2�֘A�̃w�b�_�[

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectX�̃o�b�t�@�T�C�Y�ƃE�B���h�E�̃N���C�A���g�T�C�Y
#if 0
#define SCREEN_WIDTH	(800)				// �X�N���[���E�C���h�E�̕�
#define SCREEN_HEIGHT	(600)				// �X�N���[���E�C���h�E�̍���
#else
#define SCREEN_WIDTH	(1280)				// �X�N���[���E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �X�N���[���E�C���h�E�̍���
#endif

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)		// ���_��
#define	NUM_POLYGON		(2)		// �|���S����

#define SAFE_RELEASE(ptr)		{ if(ptr) { (ptr)->Release(); (ptr) = NULL; } }		// Resource�J���w���p�[�}�N��
#define D3DDEVICE				LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �Q�b�g�f�o�C�X�ȗ���ϸ�

// �^���Z�k�錾
typedef D3DXVECTOR2				Vec2;
typedef D3DXVECTOR3				Vec3;
typedef D3DXVECTOR4				Vec4;
typedef D3DXCOLOR				Color;
typedef D3DXMATRIX				Matrix;
typedef LPDIRECT3DTEXTURE9		Texture;
typedef LPDIRECT3DVERTEXBUFFER9 VtxBuff;
typedef LPDIRECT3DINDEXBUFFER9	IdxBuff;

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

// ��ʑJ�ڊ�{�֐��Q�W���\����
// �t�F�[�Y�ێ��ɕK�v�ȏ��(���̊֐��͑S��main.cpp����fade.cpp���ł̂ݎ��s�����)
typedef struct {
	void(*Init)		(bool isFirst);	// �������֐� (����:Resource�ǂݍ��݂̗v��->true)
	void(*Uninit)	(bool isEnd);	// �I�����֐� (����:Resource�J�������̗v��->true)
	void(*Update)	(void);			// �X�V�֐�
	void(*Draw)		(void);			// �`��֐�
}PHASE_FUNC;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);

//=============================================================================
// ���t�F�[�Y�̊֐��Q�擾�֐�
//=============================================================================
PHASE_FUNC *GetPhase();


#endif