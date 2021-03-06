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
#define PLAYER_POSX			(FIELDCHIP_WIDTH/2)
#define PLAYER_POSZ			(FIELDCHIP_HEIGHT/2)
#define	JUMP_HEIGHT			(6.0f)							// ジャンプの高さ
#define SLIDING_CNT			(120)

enum PLAYER_STATE {
	PLAYER_STOP,
	PLAYER_RUNNING,
	PLAYER_JUMPING,
	PLAYER_SLIDING
};

// プレイヤー効果音列挙
enum PLAYER_SE {
	PSE_START,
	PSE_JUMP1,
	PSE_JUMP2,
	PSE_JUMP3,
	PSE_JUMPRARE,
	PSE_SLIDE,
	PSE_WALLATTACK,
	PSE_LOSE,

	MAX_PSE			/*最大数取得*/
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
Vec3 *GetPlayerPos(void);
Vec3 *GetPlayerOld_Pos(void);
Vec3 *GetPlayerRot(void);
void ResetPlayerPos();	// プレイヤーを初期座標に転送

void SetPlayerSE(PLAYER_SE se);	// プレイヤーサウンド
void SetPlayerJumpSE();			// ランダムでジャンプ系サウンドを選択

//=============================================================================
// スニークカウント取得
//=============================================================================
int GetSlideCnt();