/**********************************************************************
[[Phase_Resultヘッダー(Phase_Result.h)]]
	作者：ResultResult？？

	Phase_Result.cppに関するヘッダー
***********************************************************************/
#pragma once

#define	TEXTURE_RESULT			"data/TEXTURE/RESULT.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO		"data/TEXTURE/TITLE.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_NEWGAME_LOGO	"data/TEXTURE/NEW GAME.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_EXIT_LOGO		"data/TEXTURE/EXIT.png"			// 読み込むテクスチャファイル名

#define	TEXTURE_COIN			"data/TEXTURE/number.png"														// 読み込むテクスチャファイル名

#define	TEXTURE_DISTANCE		"data/TEXTURE/number.png"														// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// 読み込むテクスチャファイル名


#define	RESULT_TITLE_LOGO_POS_X		(90)		// TITLEロゴの位置(X座標)
#define	RESULT_TITLE_LOGO_POS_Y		(520)		// TITLEロゴの位置(Y座標)
#define	RESULT_TITLE_LOGO_WIDTH		(320)		// TITLEロゴの幅
#define	RESULT_TITLE_LOGO_HEIGHT	(200)		// TITLEロゴの高さ

#define	RESULT_NEWGAME_LOGO_POS_X	(480)		// NEWGAMEロゴの位置(X座標)
#define	RESULT_NEWGAME_LOGO_POS_Y	(520)		// NEWGAMEロゴの位置(Y座標)
#define	RESULT_NEWGAME_LOGO_WIDTH	(320)		// NEWGAMEロゴの幅
#define	RESULT_NEWGAME_LOGO_HEIGHT	(200)		// NEWGAMEロゴの高さ

#define	RESULT_EXIT_LOGO_POS_X		(870)		// EXITロゴの位置(X座標)
#define	RESULT_EXIT_LOGO_POS_Y		(520)		// EXITロゴの位置(Y座標)
#define	RESULT_EXIT_LOGO_WIDTH		(320)		// EXITロゴの幅
#define	RESULT_EXIT_LOGO_HEIGHT		(200)		// EXITロゴの高さ

#define	RESULT_COIN_SIZE_X			(80)																				// スコアの数字の幅
#define	RESULT_COIN_SIZE_Y			(80)																				// スコアの数字の高さ
#define	RESULT_COIN_INTERVAL_X		(0.0f)																				// スコアの数字の表示間隔

#define	RESULT_COIN_POS_X			(700)		// スコアの表示基準位置Ｘ座標
#define	RESULT_COIN_POS_Y			(400)																				// スコアの表示基準位置Ｙ座標


#define ICON_REN			(390)				//アイコンの移動する距離
#define ICON_MIN			(90)				//アイコンの一番上の位置
#define ICON_MAX			(870)				//アイコンの一番下の位置

#define ICON_CURSOR_MAX		(3)									//カーソルの移動

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------
typedef struct
{
	VERTEX_2D				vertexWkPauseLogo[NUM_VERTEX];		// 頂点情報格納ワーク

	D3DXVECTOR3				pos;								// ポーズ画面の位置
	D3DXVECTOR3				rot;								// ポーズ画面の回転量

	float					Radius;								// エネミーの半径
	float					BaseAngle;							// エネミーの角度

	int						cursor[ICON_CURSOR_MAX];			//カーソル
	bool					use;
}ICON;

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
Result基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc();

// 基本関数群
void InitResult(bool isFirst);
void UninitResult(bool isEnd);
void UpdateResult();
void DrawResult();