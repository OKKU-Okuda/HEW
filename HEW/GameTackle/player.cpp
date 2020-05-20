//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �����ċP
//
//=============================================================================
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"
#include "../Core/camera.h"

#include "kiseki.h"
#include "player.h"	
#include "shadow.h"
#include "field.h"	


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��
#define	PLAYER_PARENT		(0)								// �e�̓Y����(��)
#define	VERTEX_MAX			(300)							// �O�Ղ̍ő吔
#define	JUMP_HEIGHT			(8.0f)							// �W�����v�̍���
#define	JUMP_GRAVITY		(0.25f)							// �d��
#define	GRAVITY				(1.0f)							// �d��
#define PLAYER_POSX			(FIELDCHIP_WIDTH/2)
#define PLAYER_POSZ			(FIELDCHIP_HEIGHT/2)
#define SLIDING_CNT			(120)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

static void ResetPlayerPos();	// �v���C���[���������W�ɓ]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static PLAYER				g_Player[PLAYER_PARTS_MAX];						// �v���C���[

static int kiseki_idx = 0;
D3DXVECTOR3 up_back[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back[VERTEX_MAX];					// 
D3DXVECTOR3 up_back2[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back2[VERTEX_MAX];					// 
D3DXVECTOR3 up_back3[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back3[VERTEX_MAX];					// 
D3DXVECTOR3 back_pos;									// 

D3DXVECTOR3 g_Pos;										// �v���C���[�̈ʒu
D3DXVECTOR3 g_Old_Pos;									// �v���C���[�̌��ʒu
D3DXVECTOR3 g_Rot;										// �v���C���[�̌���

int g_slidin_cnt;										// �X���C�f�B���O�J�E���g
float gravity;										    // �d��

#ifdef _DEBUG
static bool g_isDebugControl = true;					// (debug)����Ɍ�����t���Ď��R�ɑ���ł���悤�ɂ���
#endif

//=============================================================================
// �A�j���[�V�����̃e�[�u��
//=============================================================================

//=============================================================================
// �����Ă���Ƃ��̃A�j���[�V�����̃e�[�u��
//=============================================================================

INTERPOLATION_DATA stop_tbl_body[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_head[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_right_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_left_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),	  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
};

INTERPOLATION_DATA stop_tbl_right_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
};


INTERPOLATION_DATA stop_tbl_right_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_left_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_left_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};



//=============================================================================
// �����Ă��鎞�̃A�j���[�V�����̃e�[�u��
//=============================================================================

INTERPOLATION_DATA dash_tbl_body[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA dash_tbl_head[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA dash_tbl_right_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 4, -D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4, D3DX_PI / 4, D3DX_PI / 4),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4, D3DX_PI / 4, D3DX_PI / 4),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 4, -D3DX_PI / 8),	D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1},

};

INTERPOLATION_DATA dash_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA dash_tbl_left_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4, -D3DX_PI / 4, -D3DX_PI / 4),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 8, -D3DX_PI / 4, D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 8, -D3DX_PI / 4, D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4, -D3DX_PI / 4, -D3DX_PI / 4),	  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA dash_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
};

INTERPOLATION_DATA dash_tbl_right_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
};

INTERPOLATION_DATA dash_tbl_right_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA dash_tbl_left_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),       D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),       D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA dash_tbl_left_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),        D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),       D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),        D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),	      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};


//=============================================================================
// �W�����v�̃A�j���[�V�����̃e�[�u��
//=============================================================================

INTERPOLATION_DATA jump_tbl_body[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(0.0f, 25.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_head[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_right_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4 * 3, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 4 * 3, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 3, -D3DX_PI / 7),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 3, -D3DX_PI / 7),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 3, -D3DX_PI / 7),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 8, D3DX_PI / 3, -D3DX_PI / 7),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_left_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 16, 0.0f, -D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 16, 0.0f, -D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 6, 0.0f, -D3DX_PI / 8),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(-D3DX_PI / 6, 0.0f, -D3DX_PI / 8),	  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(-D3DX_PI / 12, -D3DX_PI / 18, D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(-D3DX_PI / 12, -D3DX_PI / 18, D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(-D3DX_PI / 12, -D3DX_PI / 6, D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(-D3DX_PI / 12, -D3DX_PI / 6, D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
};

INTERPOLATION_DATA jump_tbl_right_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 12, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 4, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(-D3DX_PI / 4, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
};


INTERPOLATION_DATA jump_tbl_right_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(D3DX_PI / 20, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(D3DX_PI / 20, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 30, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 30, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_left_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

INTERPOLATION_DATA jump_tbl_left_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 4, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 4, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2000 },

};

//=============================================================================
// �X���C�f�B���O�̃A�j���[�V�����̃e�[�u��
//=============================================================================

INTERPOLATION_DATA sliding_tbl_body[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 8.0f, 0.0f),  D3DXVECTOR3(D3DX_PI / 2.2f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 8.0f, 0.0f),  D3DXVECTOR3(D3DX_PI / 2.2f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 8.0f, 0.0f),  D3DXVECTOR3(D3DX_PI / 2.2f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 8.0f, 0.0f),  D3DXVECTOR3(D3DX_PI / 2.2f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA sliding_tbl_head[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 5, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 5, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 5, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.0f, 12.0f, 0.0f),  D3DXVECTOR3(-D3DX_PI / 5, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA sliding_tbl_right_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 10 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.04f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.04f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 10 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA sliding_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 3, D3DX_PI / 3, -D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 3, D3DX_PI / 3, -D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 3, D3DX_PI / 3, -D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(-D3DX_PI / 3, D3DX_PI / 3, -D3DX_PI / 3),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA sliding_tbl_left_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 15 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.02f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.02f, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 15 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),	  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA sliding_tbl_left_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(3.0f, -5.2f, 1.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
};

INTERPOLATION_DATA sliding_tbl_right_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 20, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 18, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 18, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5 },
	{ D3DXVECTOR3(-2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 20, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
};


INTERPOLATION_DATA sliding_tbl_right_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-0.3f, -10.2f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA sliding_tbl_left_thigh[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 4 * 1.05f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 4 * 1.05f, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5 },
	{ D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f),      D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1 },

};

INTERPOLATION_DATA sliding_tbl_left_foot[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(0.3f, -10.2f, 1.0f), D3DXVECTOR3(-D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

static INTERPOLATION_DATA* parts_stop_adr[] =
{
	stop_tbl_body,
	stop_tbl_head,
	stop_tbl_right_shoulder,
	stop_tbl_right_arm,
	stop_tbl_left_shoulder,
	stop_tbl_left_arm,
	stop_tbl_right_thigh,
	stop_tbl_right_foot,
	stop_tbl_left_thigh,
	stop_tbl_left_foot,

};

static INTERPOLATION_DATA* parts_dash_adr[] =
{
	dash_tbl_body,
	dash_tbl_head,
	dash_tbl_right_shoulder,
	dash_tbl_right_arm,
	dash_tbl_left_shoulder,
	dash_tbl_left_arm,
	dash_tbl_right_thigh,
	dash_tbl_right_foot,
	dash_tbl_left_thigh,
	dash_tbl_left_foot,

};

static INTERPOLATION_DATA* parts_jump_adr[] =
{
	jump_tbl_body,
	jump_tbl_head,
	jump_tbl_right_shoulder,
	jump_tbl_right_arm,
	jump_tbl_left_shoulder,
	jump_tbl_left_arm,
	jump_tbl_right_thigh,
	jump_tbl_right_foot,
	jump_tbl_left_thigh,
	jump_tbl_left_foot,

};

static INTERPOLATION_DATA* parts_sliding_adr[] =
{
	sliding_tbl_body,
	sliding_tbl_head,
	sliding_tbl_right_shoulder,
	sliding_tbl_right_arm,
	sliding_tbl_left_shoulder,
	sliding_tbl_left_arm,
	sliding_tbl_right_thigh,
	sliding_tbl_right_foot,
	sliding_tbl_left_thigh,
	sliding_tbl_left_foot,

};


static const char *const filename[] =
{
	"data/MODEL/body.x",			// �ǂݍ��ރ��f����
	"data/MODEL/head.x",			// �ǂݍ��ރ��f����
	"data/MODEL/right_shoulder.x",	// �ǂݍ��ރ��f����
	"data/MODEL/right_arm.x",		// �ǂݍ��ރ��f����
	"data/MODEL/left_shoulder.x",	// �ǂݍ��ރ��f����
	"data/MODEL/left_arm.x",		// �ǂݍ��ރ��f����
	"data/MODEL/right_thigh.x",		// �ǂݍ��ރ��f����
	"data/MODEL/right_foot.x",		// �ǂݍ��ރ��f����
	"data/MODEL/left_thigh.x",		// �ǂݍ��ރ��f����
	"data/MODEL/left_foot.x",		// �ǂݍ��ރ��f����

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	kiseki_idx = 0;

	g_slidin_cnt = 0;										// �X���C�f�B���O�J�E���g

	gravity = 1.0f;

	ResetPlayerPos();		// �ꏊ�Ɖ�]���ꏊ�ɃZ�b�g

	for (int j = 0; j < VERTEX_MAX; j++)
	{
		up_back[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bottom_back[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	}

	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{

		g_Player[i].use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Player[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Player[i].jump_spped = JUMP_HEIGHT;

		g_Player[i].spd = VALUE_MOVE;		// �ړ��X�s�[�h�N���A

		g_Player[i].anim_use = PLAYER_STOP;		// �ړ��X�s�[�h�N���A
		g_Player[i].old_anim_use = PLAYER_STOP;		// �ړ��X�s�[�h�N���A

		g_Player[i].pD3DTexture = NULL;
		g_Player[i].pD3DXMesh = NULL;
		g_Player[i].pD3DXBuffMat = NULL;
		g_Player[i].nNumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(filename[i],			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
			pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_Player[i].pD3DXBuffMat,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_Player[i].nNumMat,		// D3DXMATERIAL�\���̂̐�
			&g_Player[i].pD3DXMesh)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&g_Player[i].pD3DTexture);	// �ǂݍ��ރ������[
#endif

		// �e�̏�����
		D3DXVECTOR3 pos = g_Player[i].pos;
		pos.y = 0.0f;
		g_Player[i].shadowIdx = CreateShadow(pos, g_Player[i].scl);

		g_Player[i].tbl_adr = parts_stop_adr[i];
		g_Player[i].tbl_cnt = sizeof(stop_tbl_body) / sizeof(INTERPOLATION_DATA);
		g_Player[i].move_time = 0.0f;

		if (i == 0)
		{
			g_Player[i].parent = NULL;
		}

		//�E�r��������
		else if (i == 3)
		{
			//�e�͉E��
			g_Player[i].parent = &g_Player[2];

		}
		//���r��������
		else if (i == 5)
		{
			//�e�͍���
			g_Player[i].parent = &g_Player[4];

		}
		//�E����������
		else if (i == 7)
		{
			//�e�͉E������
			g_Player[i].parent = &g_Player[6];

		}
		//�����Ȃ�
		else if (i == 9)
		{
			//�e�͍�������
			g_Player[i].parent = &g_Player[8];

		}

		else
		{
			//����ȊO�̐e�̓v���C���[
			g_Player[i].parent = &g_Player[PLAYER_PARENT];
		}

		// �ړ�����
		int		index = (int)g_Player[i].move_time;
		float	time = g_Player[i].move_time - index;
		int		size = g_Player[i].tbl_cnt;

		float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
		g_Player[i].move_time += dt;						// �A�j���[�V�����̍��v���Ԃɑ���

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
		{
			g_Player[i].move_time = 0.0f;
			index = 0;
		}

		// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
		D3DXVECTOR3 vec = g_Player[i].tbl_adr[index + 1].pos - g_Player[i].tbl_adr[index].pos;
		g_Player[i].pos = g_Player[i].tbl_adr[index].pos + vec * time;

		// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
		D3DXVECTOR3 rot = g_Player[i].tbl_adr[index + 1].rot - g_Player[i].tbl_adr[index].rot;
		g_Player[i].rot = g_Player[i].tbl_adr[index].rot + rot * time;

		// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
		D3DXVECTOR3 scl = g_Player[i].tbl_adr[index + 1].scl - g_Player[i].tbl_adr[index].scl;
		g_Player[i].scl = g_Player[i].tbl_adr[index].scl + scl * time;

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Player[i].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_Player[i].pD3DTexture->Release();
			g_Player[i].pD3DTexture = NULL;
		}

		if (g_Player[i].pD3DXMesh != NULL)
		{// ���b�V���̊J��
			g_Player[i].pD3DXMesh->Release();
			g_Player[i].pD3DXMesh = NULL;
		}

		if (g_Player[i].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_Player[i].pD3DXBuffMat->Release();
			g_Player[i].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//�ړ�����
	int		dir = 0;	// �O�F�����͕ς��Ȃ�

	g_Old_Pos = g_Pos;

	if (GetKeyboardPress(DIK_LEFT))
	{
		dir |= 8;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		dir |= 4;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_Player[PLAYER_PARENT].spd = VALUE_MOVE;
	}


	//�W�����v����
	if (GetKeyboardTrigger(DIK_J))
	{
		if (g_Player[PLAYER_PARENT].jump_spped > 0.0f)
		{
			g_Player[PLAYER_PARENT].anim_use = PLAYER_JUMPING;
		}
	}

	//�X���C�f�B���O����
	else if (GetKeyboardTrigger(DIK_K))
	{
		if (g_Player[PLAYER_PARENT].anim_use != PLAYER_JUMPING)
		{
			g_Player[PLAYER_PARENT].anim_use = PLAYER_SLIDING;
		}
	}


	// ���͂��ꂽ�L�[�ɍ��킹�Č��������߂�
	float roty = 0.0f;
	switch (dir)
	{
	case 1:	// ������
		roty = 0.0f;
		break;

	case 2:	// �����
		roty = D3DX_PI;
		break;

	case 4:	// �E����
		roty = -D3DX_PI / 2;
		break;

	case 8:	// ������
		roty = D3DX_PI / 2;
		break;

	case 5:	// �E������
		roty = -D3DX_PI / 4;
		break;

	case 6:	// �E�����
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// ��������
		roty = D3DX_PI / 4;
		break;

	case 10: // �������
		roty = D3DX_PI / 4 * 3;
		break;

	case 0:
		roty = g_Rot.y;
		break;

	default:
		break;


	}

	//	// Key���͂���������ړ���������
	//if (dir > 0)
	//{
	// �J�����ɑ΂��ē��͂̂����������փv���C���[���������Ĉړ�������
	//g_Rot.y = cam->rot.y + roty;
	g_Rot.y = roty;

	//}


	//�W�����v����
	if (g_Player[PLAYER_PARENT].anim_use == PLAYER_JUMPING)
	{
		g_Pos.y += g_Player[PLAYER_PARENT].jump_spped;

		g_Player[PLAYER_PARENT].jump_spped -= JUMP_GRAVITY;

		g_slidin_cnt = 0;

	}

	//sliding����
	if (g_Player[PLAYER_PARENT].anim_use == PLAYER_SLIDING)
	{
		g_slidin_cnt++;
		if (g_slidin_cnt >= SLIDING_CNT)
		{
			g_slidin_cnt = 0;

			g_Player[PLAYER_PARENT].anim_use = PLAYER_RUNNING;
		}
	}

	//�ړ�����[[��������ڲ԰���̃|�W�V�����m��]]
	g_Pos.x -= sinf(g_Rot.y) * g_Player[PLAYER_PARENT].spd;
	g_Pos.z -= cosf(g_Rot.y) * g_Player[PLAYER_PARENT].spd;


#ifdef _DEBUG	
	// �f�o�b�N���[�h�̂ݍ��W���Z�b�g����
	PrintDebugProc("[debug_player]F5:�v���C���[�������ʒu�ɖ߂�");
	if (GetKeyboardTrigger(DIK_F5))
	{
		ResetPlayerPos();
	}

	else if (GetKeyboardTrigger(DIK_F6))
	{// �R���g���[�����[�h���]
		g_isDebugControl = !g_isDebugControl;
	}

	if (g_isDebugControl == true)
	{// ����
		PrintDebugProc("[debug_player]F6:�f�o�b�N�R���g���[���𖳌��ɂ���(���ݗL��)");
		g_Player[PLAYER_PARENT].spd *= 0.9f;
	}
	else
	{
		PrintDebugProc("[debug_player]F6:�f�o�b�N�R���g���[����L���ɂ���(���ݖ���)");
	}

#endif

	// �n�ʂɐڂ��Ă���ꍇ
	if (PlayerCheckHitOnField() == true)
	{
		g_Pos.y = 0.0f;

		gravity = 1.0f;

		if (g_Player[PLAYER_PARENT].anim_use != PLAYER_SLIDING)
		{
			g_Player[PLAYER_PARENT].anim_use = PLAYER_RUNNING;
		}

		g_Player[PLAYER_PARENT].jump_spped = JUMP_HEIGHT;

	}
	//�n�ʂɂ��Ă��Ȃ�������(�W�����v�ȊO��)
	else if (g_Player[PLAYER_PARENT].anim_use != PLAYER_JUMPING)
	{
		g_Pos.y -= gravity;

		gravity += GRAVITY;

		g_Player[PLAYER_PARENT].anim_use = PLAYER_RUNNING;

		g_Player[PLAYER_PARENT].jump_spped = 0.0f;

	}

	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if (g_Player[i].use == true)
		{
			g_Player[i].anim_use = g_Player[PLAYER_PARENT].anim_use;

			if (g_Player[i].anim_use == PLAYER_RUNNING)
			{
				g_Player[i].tbl_adr = parts_dash_adr[i];
			}

			else if (g_Player[i].anim_use == PLAYER_JUMPING)
			{
				g_Player[i].tbl_adr = parts_jump_adr[i];
			}

			else if (g_Player[i].anim_use == PLAYER_SLIDING)
			{
				g_Player[i].tbl_adr = parts_sliding_adr[i];
			}

			if (g_Player[i].anim_use != g_Player[i].old_anim_use)
			{
				g_Player[i].move_time = 0.0f;
			}
			g_Player[i].old_anim_use = g_Player[i].anim_use;

			// �ړ�����
			int		index = (int)g_Player[i].move_time;
			float	time = g_Player[i].move_time - index;
			int		size = g_Player[i].tbl_cnt;

			float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
			g_Player[i].move_time += dt;						// �A�j���[�V�����̍��v���Ԃɑ���

			if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
			{
				g_Player[i].move_time = 0.0f;
				index = 0;
			}
			// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
			D3DXVECTOR3 vec = g_Player[i].tbl_adr[index + 1].pos - g_Player[i].tbl_adr[index].pos;
			g_Player[i].pos = g_Player[i].tbl_adr[index].pos + vec * time;

			// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
			D3DXVECTOR3 rot = g_Player[i].tbl_adr[index + 1].rot - g_Player[i].tbl_adr[index].rot;
			g_Player[i].rot = g_Player[i].tbl_adr[index].rot + rot * time;

			// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
			D3DXVECTOR3 scl = g_Player[i].tbl_adr[index + 1].scl - g_Player[i].tbl_adr[index].scl;
			g_Player[i].scl = g_Player[i].tbl_adr[index].scl + scl * time;
		}
	}


	//�w���̈ʒu
	D3DXVec3TransformCoord(&back_pos, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &g_Player[0].mtxWorld);

	up_back[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 15.0f, 0.0f);

	bottom_back[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	up_back2[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	bottom_back2[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	up_back3[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 5.0f, 5.0f);

	bottom_back3[kiseki_idx] = back_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (kiseki_idx > 0)
	{
		if (g_Player[PLAYER_PARENT].anim_use == PLAYER_RUNNING || g_Player[PLAYER_PARENT].anim_use == PLAYER_JUMPING)
		{
			SetKiseki(up_back[kiseki_idx - 1], up_back[kiseki_idx], bottom_back[kiseki_idx - 1], bottom_back[kiseki_idx], 0, 0);
			SetKiseki(up_back[kiseki_idx - 1], bottom_back[kiseki_idx - 1], up_back[kiseki_idx], bottom_back[kiseki_idx], 0, 0);

			SetKiseki(up_back2[kiseki_idx - 1], up_back2[kiseki_idx], bottom_back2[kiseki_idx - 1], bottom_back2[kiseki_idx], 0, 1);
			SetKiseki(up_back2[kiseki_idx - 1], bottom_back2[kiseki_idx - 1], up_back2[kiseki_idx], bottom_back2[kiseki_idx], 0, 1);

			SetKiseki(up_back3[kiseki_idx - 1], up_back3[kiseki_idx], bottom_back3[kiseki_idx - 1], bottom_back3[kiseki_idx], 0, 0);
			SetKiseki(up_back3[kiseki_idx - 1], bottom_back3[kiseki_idx - 1], up_back3[kiseki_idx], bottom_back3[kiseki_idx], 0, 0);
		}
	}

	kiseki_idx++;

	if (kiseki_idx >= 300)
	{
		kiseki_idx = 0;
	}

	//�{�f�B�̏��Ɉړ��Ɖ�]�𔽉f
	g_Player[PLAYER_PARENT].pos += g_Pos;

	g_Player[PLAYER_PARENT].rot += g_Rot;

	// �e���v���C���[�̈ʒu�ɍ��킹��
	D3DXVECTOR3 pos = g_Player[PLAYER_PARENT].pos;
	pos.y = 0.0f;
	SetPositionShadow(g_Player[PLAYER_PARENT].shadowIdx, pos, g_Player[PLAYER_PARENT].scl);



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Player[i].scl.x, g_Player[i].scl.y, g_Player[i].scl.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[i].rot.y, g_Player[i].rot.x, g_Player[i].rot.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Player[i].pos.x, g_Player[i].pos.y, g_Player[i].pos.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxTranslate);

		//�e���ǂ����`�F�b�N
		if (g_Player[i].parent != NULL)
		{
			D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &g_Player[i].parent->mtxWorld);//->��parent�������Ă���A�h���X(�e�̃A�h���X)�e�̏Z���̒��̃}�g���b�N�X�@&��mtxWorld�̃A�h���X
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player[i].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)g_Player[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player[i].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Player[i].pD3DTexture);

			// �`��
			g_Player[i].pD3DXMesh->DrawSubset(nCntMat);
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);

	}
}
//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
Vec3 *GetPlayerPos(void)
{
	return &g_Pos;
}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
Vec3 *GetPlayerOld_Pos(void)
{
	return &g_Old_Pos;
}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
Vec3 *GetPlayerRot(void)
{
	return &g_Rot;
}


///*=====================================================================
//player��{�֐��Q�擾�֐�
//=====================================================================*/
//OBJ_FUNC* GetPlayerFuncX()
//{
//	return &g_Func;
//}
//
///*=====================================================================
//player�ʒu�擾�֐�(�s��o�R)
//=====================================================================*/
//Vec3* GetPlayerPositionX()
//{
//	return (Vec3*)(g_model->WldMtx.m[3]);
//}

//=============================================================================
// �v���C���[���������W�܂œ]��������
//=============================================================================
void ResetPlayerPos()
{
	g_Pos = D3DXVECTOR3(PLAYER_POSX, 0.0f, PLAYER_POSZ);
	g_Rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Old_Pos = g_Pos;
}