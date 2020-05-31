//=============================================================================
//
// コインの処理 [item.h]
// Author : 早野太智
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ITEM				(128)						// アイテム最大数
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)			// 回転速度
#define	ITEM_RADIUS				(10.0f)						// 半径
#define ITEM_EFECT_TIME			(0.5f)						// アイテムのエフェクトに掛かる時間
#define ADD_ITEM_TIME			(1.0f/(ITEM_EFECT_TIME*60))	// エフェクトに掛かる時間の計算用(1フレームごとに加算する量)
#define ITEM_SIZE_X				(10.0f)					// アイテムの当たり判定のサイズ
#define ITEM_SIZE_Y				(10.0f)					// アイテムの当たり判定のサイズ
#define ITEM_SIZE_Z				(10.0f)					// アイテムの当たり判定のサイズ
								  
#define PLAYER_SIZE_X			(10.0f)					// プレイヤーの当たり判定のサイズ
#define PLAYER_SIZE_Y			(10.0f)					// プレイヤーの当たり判定のサイズ
#define PLAYER_SIZE_Z			(10.0f)					// プレイヤーの当たり判定のサイズ

#define ITEM_UI_POS_X			(1200.0f)					// アイテムのUIのスクリーン座標X
#define ITEM_UI_POS_Y			(50.0f)					// アイテムのUIのスクリーン座標Y

//**************************************
// 種類
//**************************************
enum
{
	ITEMTYPE_COIN = 0,		// コイン
	ITEMTYPE_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 firstpos;	// アイテムが当たった位置
	D3DXVECTOR3 endpos;		// アイテムの終点
	D3DXVECTOR3 rot;		// 現在の向き
	D3DXVECTOR3 control_F;	// 一つ目の制御点
	D3DXVECTOR3 control_S;	// 二つ目の制御点
	float fRadius;			// 半径
	float time;				// アイテムのエフェクトに掛かる時間
	int nIdxShadow;			// 影ID
	int nType;				// 種類
	bool bUse;				// 使用しているかどうか
	bool bHit;				// 当たり判定
} ITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);

D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_pos,
	float t, 
	D3DXVECTOR3* p_start, 
	D3DXVECTOR3* p_second, 
	D3DXVECTOR3* p_third, 
	D3DXVECTOR3* p_end);

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,			// スクリーンX座標
	float Sy,			// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0～1）
	int Screen_w,		// スクリーンの横幅
	int Screen_h,		// スクリーンの縦幅
	D3DXMATRIX* View,	// ビューマトリックス
	D3DXMATRIX* Prj);	// プロジェクションマトリックス
