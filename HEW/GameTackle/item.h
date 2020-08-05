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
#define	MAX_ITEM				(512)						// アイテム最大数
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)			// 回転速度
#define	ITEM_RADIUS				(10.0f)						// 半径
#define ITEM_EFECT_TIME			(0.5f)						// アイテムのエフェクトに掛かる秒数
#define ADD_ITEM_TIME			(1.0f/(ITEM_EFECT_TIME*60))	// エフェクトに掛かる時間の計算用(1フレームごとに加算する量)
#define ITEM_SIZE_X				(10.0f)						// アイテムの当たり判定のサイズ
#define ITEM_SIZE_Y				(10.0f)						// アイテムの当たり判定のサイズ
#define ITEM_SIZE_Z				(10.0f)						// アイテムの当たり判定のサイズ
								  
#define PLAYER_SIZE_X			(10.0f)						// プレイヤーの当たり判定のサイズ
#define PLAYER_SIZE_Y			(10.0f)						// プレイヤーの当たり判定のサイズ
#define PLAYER_SIZE_Z			(10.0f)						// プレイヤーの当たり判定のサイズ

#define ITEM_UI_POS_X			(1200.0f)					// アイテムのUIのスクリーン座標X
#define ITEM_UI_POS_Y			(50.0f)						// アイテムのUIのスクリーン座標Y

#define MAXITEM_PERFIELD		(20)						// フィールドに設置できる最大アイテム数
#define VOLUME_COIN				(4.0f)						// コイン取得時の音量
#define SPD_UPRATE				(1.003f)					// コイン入手時の速度上昇率

//**************************************
// 種類
//**************************************
; typedef enum
{
	ITEMTYPE_COIN = 0,	// コイン
	/* <----アイテムの種類を増やしたいときここに追加*/
	ITEMTYPE_MAX
}COIN;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct FIELD_CHIP;

class ITEM
{
public:
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 scl;		// アイテムのスケール調整
	D3DXVECTOR3 firstpos;	// アイテムが当たった位置
	D3DXVECTOR3 endpos;		// アイテムの終点
	D3DXVECTOR3 rot;		// 現在の向き
	D3DXVECTOR3 control_F;	// 一つ目の制御点
	D3DXVECTOR3 control_S;	// 二つ目の制御点
	float		fRadius;	// 半径
	float		fTime;		// アイテムのエフェクトに掛かる時間
	int			nIdxShadow;	// 影ID
	FIELD_CHIP*	pParent;	// 親
	int			nType;		// 種類
	bool		bUse;		// 使用しているかどうか
	bool		bHit;		// 当たり判定
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//=============================================================================
// アイテムの設定
//=============================================================================
struct FIELD_CHIP;

void SetItem(FIELD_CHIP* pData, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int nIdxItem);
void DeleteItemByPtr(ITEM** ppItem);
void DeleteItemByFieldPtr(FIELD_CHIP *pData);
void ResetItem();

ITEM *GetItem(void);

D3DXVECTOR3 *BezierCurve(
	D3DXVECTOR3* p_pos,		// 戻り値
	float t, 				// 0から1までの時間
	D3DXVECTOR3* p_start, 	// ベジェ曲線の始点
	D3DXVECTOR3* p_second, 	// ベジェ曲線の第1制御点
	D3DXVECTOR3* p_third, 	// ベジェ曲線の第2制御点
	D3DXVECTOR3* p_end);	// ベジェ曲線の終点

D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* p_out,
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float fZ,				// 射影空間でのZ値（0～1）
	int Screen_w,			// スクリーンの横幅
	int Screen_h,			// スクリーンの縦幅
	D3DXMATRIX* View,		// ビューマトリックス
	D3DXMATRIX* Prj);		// プロジェクションマトリックス
