//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 高橋翔輝
//
//=============================================================================
#include"../Core/main.h"
#include "player.h"	
#include "../Core/input.h"
#include "../Core/directx_Helper3D.h"
#include "../Core/debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(5.0f)							// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量
#define	PLAYER_PARENT		(0)								// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static PLAYER				g_Player[PLAYER_PARTS_MAX];						// プレイヤー


//=============================================================================
// アニメーションのテーブル
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
// 歩いているときのアニメーションのテーブル
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
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_right_arm[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(-2.0f, -5.2f, 2.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

};

INTERPOLATION_DATA stop_tbl_left_shoulder[] = {	// pos, rot, scl, frame
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),   D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },
	{ D3DXVECTOR3(8.1f, 8.9f, 2.3f),  D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20 },

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

static const char *const filename[] =
{
	"data/MODEL/body.x",			// 読み込むモデル名
	"data/MODEL/head.x",			// 読み込むモデル名
	"data/MODEL/right_shoulder.x",	// 読み込むモデル名
	"data/MODEL/right_arm.x",		// 読み込むモデル名
	"data/MODEL/left_shoulder.x",	// 読み込むモデル名
	"data/MODEL/left_arm.x",		// 読み込むモデル名
	"data/MODEL/right_thigh.x",		// 読み込むモデル名
	"data/MODEL/right_foot.x",		// 読み込むモデル名
	"data/MODEL/left_thigh.x",		// 読み込むモデル名
	"data/MODEL/left_foot.x",		// 読み込むモデル名

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Player[i].use = true;

		// 位置・回転・スケールの初期設定
		g_Player[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Player[i].spd = 0.0f;		// 移動スピードクリア

		g_Player[i].dash_use = false;		// 移動スピードクリア

		g_Player[i].pD3DTexture = NULL;
		g_Player[i].pD3DXMesh = NULL;
		g_Player[i].pD3DXBuffMat = NULL;
		g_Player[i].nNumMat = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(filename[i],			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
			pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
			NULL,					// 隣接性データを含むバッファへのポインタ
			&g_Player[i].pD3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ
			NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_Player[i].nNumMat,		// D3DXMATERIAL構造体の数
			&g_Player[i].pD3DXMesh)))	// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&g_Player[i].pD3DTexture);	// 読み込むメモリー
#endif

		//// 影の初期化
		//D3DXVECTOR3 pos = g_Player[i].pos;
		//pos.y = 0.0f;
		//g_Player[i].shadowIdx = CreateShadow(pos, g_Player[i].scl);

		g_Player[i].tbl_adr = parts_dash_adr[i];
		g_Player[i].tbl_cnt = sizeof(stop_tbl_body) / sizeof(INTERPOLATION_DATA);
		g_Player[i].move_time = 0.0f;

		if (i == 0)
		{
			g_Player[i].parent = NULL;
		}

		//右腕だったら
		else if (i == 3)
		{
			//親は右肩
			g_Player[i].parent = &g_Player[2];

		}
		//左腕だったら
		else if (i == 5)
		{
			//親は左肩
			g_Player[i].parent = &g_Player[4];

		}
		//右足だったら
		else if (i == 7)
		{
			//親は右太もも
			g_Player[i].parent = &g_Player[6];

		}
		//左足なら
		else if (i == 9)
		{
			//親は左太もも
			g_Player[i].parent = &g_Player[8];

		}

		else
		{
			//それ以外の親はプレイヤー
			g_Player[i].parent = &g_Player[PLAYER_PARENT];
		}

		// 移動処理
		int		index = (int)g_Player[i].move_time;
		float	time = g_Player[i].move_time - index;
		int		size = g_Player[i].tbl_cnt;

		float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1フレームで進める時間
		g_Player[i].move_time += dt;						// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
		{
			g_Player[i].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		D3DXVECTOR3 vec = g_Player[i].tbl_adr[index + 1].pos - g_Player[i].tbl_adr[index].pos;
		g_Player[i].pos = g_Player[i].tbl_adr[index].pos + vec * time;

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		D3DXVECTOR3 rot = g_Player[i].tbl_adr[index + 1].rot - g_Player[i].tbl_adr[index].rot;
		g_Player[i].rot = g_Player[i].tbl_adr[index].rot + rot * time;

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		D3DXVECTOR3 scl = g_Player[i].tbl_adr[index + 1].scl - g_Player[i].tbl_adr[index].scl;
		g_Player[i].scl = g_Player[i].tbl_adr[index].scl + scl * time;

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Player[i].pD3DTexture != NULL)
		{// テクスチャの開放
			g_Player[i].pD3DTexture->Release();
			g_Player[i].pD3DTexture = NULL;
		}

		if (g_Player[i].pD3DXMesh != NULL)
		{// メッシュの開放
			g_Player[i].pD3DXMesh->Release();
			g_Player[i].pD3DXMesh = NULL;
		}

		if (g_Player[i].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_Player[i].pD3DXBuffMat->Release();
			g_Player[i].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//CAMERA *cam = GetCamera();

	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if (g_Player[i].use == true)
		{
			// 移動処理
			int		index = (int)g_Player[i].move_time;
			float	time = g_Player[i].move_time - index;
			int		size = g_Player[i].tbl_cnt;

			float dt = 1.0f / g_Player[i].tbl_adr[index].frame;	// 1フレームで進める時間
			g_Player[i].move_time += dt;						// アニメーションの合計時間に足す

			if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
			{
				g_Player[i].move_time = 0.0f;
				index = 0;
			}
			if (i != 0)
			{
				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				D3DXVECTOR3 vec = g_Player[i].tbl_adr[index + 1].pos - g_Player[i].tbl_adr[index].pos;
				g_Player[i].pos = g_Player[i].tbl_adr[index].pos + vec * time;

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				D3DXVECTOR3 rot = g_Player[i].tbl_adr[index + 1].rot - g_Player[i].tbl_adr[index].rot;
				g_Player[i].rot = g_Player[i].tbl_adr[index].rot + rot * time;

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				D3DXVECTOR3 scl = g_Player[i].tbl_adr[index + 1].scl - g_Player[i].tbl_adr[index].scl;
				g_Player[i].scl = g_Player[i].tbl_adr[index].scl + scl * time;
			}
		}
	}

	//移動処理
	int		dir = 0;	// ０：向きは変えない

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

	if (GetKeyboardPress(DIK_SPACE))
	{
		g_Player[PLAYER_PARENT].pos.z = g_Player[PLAYER_PARENT].pos.x = 0.0f;
		dir = 1;
		g_Player[PLAYER_PARENT].spd = 0.0f;
	}

	// 入力されたキーに合わせて向きを決める
	float roty = 0.0f;
	switch (dir)
	{
	case 1:	// 下向き
		roty = 0.0f;
		break;

	case 2:	// 上向き
		roty = D3DX_PI;
		break;

	case 4:	// 右向き
		roty = -D3DX_PI / 2;
		break;

	case 8:	// 左向き
		roty = D3DX_PI / 2;
		break;

	case 5:	// 右下向き
		roty = -D3DX_PI / 4;
		break;

	case 6:	// 右上向き
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// 左下向き
		roty = D3DX_PI / 4;
		break;

	case 10: // 左上向き
		roty = D3DX_PI / 4 * 3;
		break;

	case 0:
		//roty = g_Player[PLAYER_PARENT].rot.y - cam->rot.y;
		break;

	}

	//	// Key入力があったら移動処理する
	if (dir > 0)
	{
		// カメラに対して入力のあった方向へプレイヤーを向かせて移動させる
		//g_Player[PLAYER_PARENT].rot.y = cam->rot.y + roty;
	}
	g_Player[PLAYER_PARENT].pos.x -= sinf(g_Player[PLAYER_PARENT].rot.y) * g_Player[PLAYER_PARENT].spd;
	g_Player[PLAYER_PARENT].pos.z -= cosf(g_Player[PLAYER_PARENT].rot.y) * g_Player[PLAYER_PARENT].spd;

	// カメラの注視点と視点を主人公に追従させる
	//cam->at.x = g_Player[PLAYER_PARENT].pos.x;
	//cam->at.z = g_Player[PLAYER_PARENT].pos.z;
	//cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
	//cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;

	g_Player[PLAYER_PARENT].spd *= 0.9f;

	// 影もプレイヤーの位置に合わせる
	//D3DXVECTOR3 pos = g_Player[PLAYER_PARENT].pos;
	//pos.y = 0.0f;
	//SetPositionShadow(g_Player[PLAYER_PARENT].shadowIdx, pos, g_Player[PLAYER_PARENT].scl);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Player[i].scl.x, g_Player[i].scl.y, g_Player[i].scl.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[i].rot.y, g_Player[i].rot.x, g_Player[i].rot.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Player[i].pos.x, g_Player[i].pos.y, g_Player[i].pos.z);
		D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &mtxTranslate);

		//親かどうかチェック
		if (g_Player[i].parent != NULL)
		{
			D3DXMatrixMultiply(&g_Player[i].mtxWorld, &g_Player[i].mtxWorld, &g_Player[i].parent->mtxWorld);//->はparentがさしているアドレス(親のアドレス)親の住所の中のマトリックス　&がmtxWorldのアドレス
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player[i].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)g_Player[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player[i].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_Player[i].pD3DTexture);

			// 描画
			g_Player[i].pD3DXMesh->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);

	}
}
//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}


///*=====================================================================
//player基本関数群取得関数
//=====================================================================*/
//OBJ_FUNC* GetPlayerFuncX()
//{
//	return &g_Func;
//}
//
///*=====================================================================
//player位置取得関数(行列経由)
//=====================================================================*/
//Vec3* GetPlayerPositionX()
//{
//	return (Vec3*)(g_model->WldMtx.m[3]);
//}