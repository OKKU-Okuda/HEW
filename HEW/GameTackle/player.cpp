//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 高橋翔輝
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
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(5.0f)							// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量
#define	PLAYER_PARENT		(0)								// 親の添え字(体)
#define	VERTEX_MAX			(300)							// 軌跡の最大数
#define	JUMP_HEIGHT			(8.0f)							// ジャンプの高さ
#define	JUMP_GRAVITY		(0.25f)							// 重力
#define	GRAVITY				(1.0f)							// 重力
#define PLAYER_POSX			(FIELDCHIP_WIDTH/2)
#define PLAYER_POSZ			(FIELDCHIP_HEIGHT/2)
#define SLIDING_CNT			(120)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

static void ResetPlayerPos();	// プレイヤーを初期座標に転送

//*****************************************************************************
// グローバル変数
//*****************************************************************************

static PLAYER				g_Player[PLAYER_PARTS_MAX];						// プレイヤー

static int kiseki_idx = 0;
D3DXVECTOR3 up_back[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back[VERTEX_MAX];					// 
D3DXVECTOR3 up_back2[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back2[VERTEX_MAX];					// 
D3DXVECTOR3 up_back3[VERTEX_MAX];						// 
D3DXVECTOR3 bottom_back3[VERTEX_MAX];					// 
D3DXVECTOR3 back_pos;									// 

D3DXVECTOR3 g_Pos;										// プレイヤーの位置
D3DXVECTOR3 g_Old_Pos;									// プレイヤーの元位置
D3DXVECTOR3 g_Rot;										// プレイヤーの向き

int g_slidin_cnt;										// スライディングカウント
float gravity;										    // 重力

#ifdef _DEBUG
static bool g_isDebugControl = true;					// (debug)操作に減速を付けて自由に操作できるようにする
#endif

//=============================================================================
// アニメーションのテーブル
//=============================================================================

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
// 走っている時のアニメーションのテーブル
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
// ジャンプのアニメーションのテーブル
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
// スライディングのアニメーションのテーブル
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

	kiseki_idx = 0;

	g_slidin_cnt = 0;										// スライディングカウント

	gravity = 1.0f;

	ResetPlayerPos();		// 場所と回転を場所にセット

	for (int j = 0; j < VERTEX_MAX; j++)
	{
		up_back[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bottom_back[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	}

	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{

		g_Player[i].use = true;

		// 位置・回転・スケールの初期設定
		g_Player[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Player[i].jump_spped = JUMP_HEIGHT;

		g_Player[i].spd = VALUE_MOVE;		// 移動スピードクリア

		g_Player[i].anim_use = PLAYER_STOP;		// 移動スピードクリア
		g_Player[i].old_anim_use = PLAYER_STOP;		// 移動スピードクリア

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

		// 影の初期化
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
	//移動処理
	int		dir = 0;	// ０：向きは変えない

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


	//ジャンプ処理
	if (GetKeyboardTrigger(DIK_J))
	{
		if (g_Player[PLAYER_PARENT].jump_spped > 0.0f)
		{
			g_Player[PLAYER_PARENT].anim_use = PLAYER_JUMPING;
		}
	}

	//スライディング処理
	else if (GetKeyboardTrigger(DIK_K))
	{
		if (g_Player[PLAYER_PARENT].anim_use != PLAYER_JUMPING)
		{
			g_Player[PLAYER_PARENT].anim_use = PLAYER_SLIDING;
		}
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
		roty = g_Rot.y;
		break;

	default:
		break;


	}

	//	// Key入力があったら移動処理する
	//if (dir > 0)
	//{
	// カメラに対して入力のあった方向へプレイヤーを向かせて移動させる
	//g_Rot.y = cam->rot.y + roty;
	g_Rot.y = roty;

	//}


	//ジャンプ処理
	if (g_Player[PLAYER_PARENT].anim_use == PLAYER_JUMPING)
	{
		g_Pos.y += g_Player[PLAYER_PARENT].jump_spped;

		g_Player[PLAYER_PARENT].jump_spped -= JUMP_GRAVITY;

		g_slidin_cnt = 0;

	}

	//sliding処理
	if (g_Player[PLAYER_PARENT].anim_use == PLAYER_SLIDING)
	{
		g_slidin_cnt++;
		if (g_slidin_cnt >= SLIDING_CNT)
		{
			g_slidin_cnt = 0;

			g_Player[PLAYER_PARENT].anim_use = PLAYER_RUNNING;
		}
	}

	//移動処理[[ここでﾌﾟﾚｲﾔｰ側のポジション確定]]
	g_Pos.x -= sinf(g_Rot.y) * g_Player[PLAYER_PARENT].spd;
	g_Pos.z -= cosf(g_Rot.y) * g_Player[PLAYER_PARENT].spd;


#ifdef _DEBUG	
	// デバックモードのみ座標リセット処理
	PrintDebugProc("[debug_player]F5:プレイヤーを初期位置に戻す");
	if (GetKeyboardTrigger(DIK_F5))
	{
		ResetPlayerPos();
	}

	else if (GetKeyboardTrigger(DIK_F6))
	{// コントロールモード反転
		g_isDebugControl = !g_isDebugControl;
	}

	if (g_isDebugControl == true)
	{// 減速
		PrintDebugProc("[debug_player]F6:デバックコントロールを無効にする(現在有効)");
		g_Player[PLAYER_PARENT].spd *= 0.9f;
	}
	else
	{
		PrintDebugProc("[debug_player]F6:デバックコントロールを有効にする(現在無効)");
	}

#endif

	// 地面に接している場合
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
	//地面についていなかったら(ジャンプ以外で)
	else if (g_Player[PLAYER_PARENT].anim_use != PLAYER_JUMPING)
	{
		g_Pos.y -= gravity;

		gravity += GRAVITY;

		g_Player[PLAYER_PARENT].anim_use = PLAYER_RUNNING;

		g_Player[PLAYER_PARENT].jump_spped = 0.0f;

	}

	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
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
	}


	//背中の位置
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

	//ボディの情報に移動と回転を反映
	g_Player[PLAYER_PARENT].pos += g_Pos;

	g_Player[PLAYER_PARENT].rot += g_Rot;

	// 影もプレイヤーの位置に合わせる
	D3DXVECTOR3 pos = g_Player[PLAYER_PARENT].pos;
	pos.y = 0.0f;
	SetPositionShadow(g_Player[PLAYER_PARENT].shadowIdx, pos, g_Player[PLAYER_PARENT].scl);



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

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
Vec3 *GetPlayerPos(void)
{
	return &g_Pos;
}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
Vec3 *GetPlayerOld_Pos(void)
{
	return &g_Old_Pos;
}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
Vec3 *GetPlayerRot(void)
{
	return &g_Rot;
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

//=============================================================================
// プレイヤーを初期座標まで転送させる
//=============================================================================
void ResetPlayerPos()
{
	g_Pos = D3DXVECTOR3(PLAYER_POSX, 0.0f, PLAYER_POSZ);
	g_Rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Old_Pos = g_Pos;
}