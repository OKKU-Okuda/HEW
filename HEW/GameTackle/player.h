//=============================================================================
//
// モデル処理 [player.h]
// Author : 高橋翔輝
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************

enum PLAYER_STATE {
	PLAYER_STOP,
	PLAYER_RUNNING,
	PLAYER_JUMPING,
	PLAYER_SLIDING
};

struct PLAYER
{
	bool     			use;				// モデルの位置
	PLAYER_STATE		anim_use;			// 0;何もしていない1;ダッシュ2;ジャンプ3:スライディング
	PLAYER_STATE		old_anim_use;		// 0;何もしていない1;ダッシュ2;ジャンプ3:スライディング

	float				jump_spped;			// ジャンプスピード

	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	float				spd;				// 移動スピード

	LPDIRECT3DTEXTURE9	pD3DTexture;		// テクスチャへのポインタ
	LPD3DXMESH			pD3DXMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		pD3DXBuffMat;		// マテリアル情報へのポインタ
	DWORD				nNumMat;			// マテリアル情報の数
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuff;		// インデックスバッファインターフェースへのポインタ


	int					shadowIdx;			// 影のインデックス番号

	int                 tbl_cnt;       //ポイントを入れる個数
	float               move_time;          //移動時間
	INTERPOLATION_DATA	*tbl_adr;	    // 行動テーブルのアドレス

	PLAYER              *parent;            //親のアドレス　親ならNULL
};

#define PLAYER_PARTS_MAX		(10)					// プレイヤーの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
