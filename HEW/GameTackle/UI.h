//=============================================================================
//
// UI処理 [UI.h]
// Author : 福島　凜
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include"../Core/main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float f;

	/*コイン*/
	int			coin;
	int			coin_target;		// イベント変更の目安数
	int			COIN_PLACE;			// スコアの桁数
	float		COIN_POS_X;		// スコアの表示基準位置Ｘ座標
	float		COIN_SIZE_X;
	float		COIN_SIZE_Y;

	LPDIRECT3DTEXTURE9		pD3DTextureCoin[2] = {};		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffCoin = NULL;		// 頂点バッファインターフェースへのポインタ

	D3DXVECTOR3				posCoin;						// 位置
	D3DXVECTOR3				rotCoin;						// 向き

						/*距離*/
	int			distance;
	int			distance_target;	// スコアの桁イベント用
	int			DISTANCE_PLACE;		//距離の桁数
	float		DISTANCE_POS_X;	// スコアの表示基準位置Ｘ座標
	float		DISTANCE_SIZE_X;
	float		DISTANCE_SIZE_Y;

	LPDIRECT3DTEXTURE9		pD3DTextureDistance[2] = {};	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffDistance = NULL;	// 頂点バッファインターフェースへのポインタ

	D3DXVECTOR3				posDistance;					// 位置
	D3DXVECTOR3				rotDistance;					// 向き

	float			add_timer;
	float			add_timer_max;

	float			dec_timer;
	float			dec_timer_max;

} UI;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

					/*コイン*/
#define	TEXTURE_COIN			"data/TEXTURE/number.png"														// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_COIN		"data/TEXTURE/coin.png"																// 読み込むテクスチャファイル名
//#define	COIN_SIZE_X				(35.0f)																				// スコアの数字の幅
//#define	COIN_SIZE_Y				(50.0f)																				// スコアの数字の高さ
#define	COIN_INTERVAL_X			(0.0f)																				// スコアの数字の表示間隔

#define	COIN_POS_Y				(25.0f)																				// スコアの表示基準位置Ｙ座標

#define COIN_ICON_POS_X			(SCREEN_WIDTH - 20.0f)
#define COIN_ICON_POS_Y			(55.0f)

					/*距離*/
#define	TEXTURE_DISTANCE		"data/TEXTURE/number.png"														// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// 読み込むテクスチャファイル名

//#define	DISTANCE_SIZE_X			(35.0f)																				// スコアの数字の幅
//#define	DISTANCE_SIZE_Y			(50.0f)																				// スコアの数字の高さ
#define	DISTANCE_INTERVAL_X		(0.0f)																				// スコアの数字の表示間隔

#define	DISTANCE_POS_Y			(100.0f)
																												// スコアの表示基準位置Ｙ座標
#define METER_POS_X				(SCREEN_WIDTH - 20.0f)																// mの表示基準位置X座標
#define METER_POS_Y				(125.0f)										// mの表示基準位置Y座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI(int type);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void ChangeCoin(int value);
void ChangeDistance(int value);

HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice);
void SetTextureDistance(int idx, int number);

UI *GetUI(void);
void ResetUI(void);

#endif
