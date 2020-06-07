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

#include "directx_Helper2D.h"
#include "directx_Helper3D.h"
#include "input.h"
#include "sound.h"
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

#define SAFE_RELEASE(ptr)		{ if((ptr)) { (ptr)->Release(); (ptr) = NULL; } }		// Resource�J���w���p�[�}�N��
#define SAFE_NUMBER(x,mi,ma)	{ x = max(min(x,ma),mi); }							// �i���o�[�`�F�b�N(�������͈͊O�̏ꍇ�������������������)
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
typedef void(*NormalFunc)(void);


// ��ʑJ�ڊ�{�֐��Q�W���\���́A
// ���t�F�[�Y�ێ��ɕK�v�ȏ��(���̊֐��͑S��main.cpp����fade.cpp���ł̂ݎ��s�����)
// �@�\�ʊ֐��Q�W���\����
// ���@�\���Ƃɕ�������ۂɕK�v�ȏ��
typedef struct {
	void(*Init)		(bool isFirst);	// �������֐� (����:Resource�ǂݍ��݂̗v��->true)
	void(*Uninit)	(bool isEnd);	// �I�����֐� (����:Resource�J�������̗v��->true)
	void(*Update)	(void);			// �X�V�֐�
	void(*Draw)		(void);			// �`��֐�
}PHASE_FUNC, OBJ_FUNC;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//=============================================================================
// �i�j���V�i�C...
//=============================================================================
void NoFunction();

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);

//=============================================================================
// ���t�F�[�Y�̊֐��Q�擾�֐�
//=============================================================================
PHASE_FUNC *GetPhase();

//=============================================================================
// �E�B���h�E�n���h���̎擾
//=============================================================================
HWND GetHandle();

//=============================================================================
// �X�V�֐��g�p�񐔂̎擾
//=============================================================================
DWORD GetCount();

/*=====================================================================
�����w�胉���_���擾�֐�
�߂�l�Fbool			true:������true�ɂ���
�����F�@float rate			:0~1f�@�@true��Ԃ��������w��
=====================================================================*/
bool CheckRand(float rate);

// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
	D3DXVECTOR3 pos;		// ���_���W
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scl;		// �g��k��
	float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};

#endif