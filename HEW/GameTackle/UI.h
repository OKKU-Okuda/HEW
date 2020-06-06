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
// マクロ定義
//*****************************************************************************

					/*コイン*/
#define	TEXTURE_COIN			"data/TEXTURE/number000.png"														// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_COIN		"data/TEXTURE/coin.png"																// 読み込むテクスチャファイル名
#define	COIN_SIZE_X				(35.0f)																				// スコアの数字の幅
#define	COIN_SIZE_Y				(50.0f)																				// スコアの数字の高さ
#define	COIN_INTERVAL_X			(0.0f)																				// スコアの数字の表示間隔

#define	COIN_PLACE				(8)																					// スコアの桁数

#define	COIN_POS_X				(SCREEN_WIDTH - (COIN_INTERVAL_X + COIN_SIZE_X) * COIN_PLACE - 0.0f)				// スコアの表示基準位置Ｘ座標
#define	COIN_POS_Y				(25.0f)																				// スコアの表示基準位置Ｙ座標

#define COIN_ICON_POS_X			(SCREEN_WIDTH - 295.0f)
#define COIN_ICON_POS_Y			(55.0f)

					/*距離*/
#define	TEXTURE_DISTANCE		"data/TEXTURE/number000.png"														// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// 読み込むテクスチャファイル名

#define	DISTANCE_SIZE_X			(35.0f)																				// スコアの数字の幅
#define	DISTANCE_SIZE_Y			(50.0f)																				// スコアの数字の高さ
#define	DISTANCE_INTERVAL_X		(0.0f)																				// スコアの数字の表示間隔

#define	DISTANCE_PLACE			(8)																					// スコアの桁数

#define	DISTANCE_POS_X			(SCREEN_WIDTH - (DISTANCE_INTERVAL_X + DISTANCE_SIZE_X) * DISTANCE_PLACE - 35.0f)	// スコアの表示基準位置Ｘ座標
#define	DISTANCE_POS_Y			(100.0f)
																												// スコアの表示基準位置Ｙ座標
#define METER_POS_X				(SCREEN_WIDTH - 20.0f)																// mの表示基準位置X座標
#define METER_POS_Y				(125.0f)																			// mの表示基準位置Y座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI(int type);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void ChangeCoin(int value);
void ChangeDistance(int value);

#endif
