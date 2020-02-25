//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "windows.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"			// DirectInput関連のヘッダー
#include "xaudio2.h"		// XAudio2関連のヘッダー

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectXのバッファサイズとウィンドウのクライアントサイズ
#if 0
#define SCREEN_WIDTH	(800)				// スクリーンウインドウの幅
#define SCREEN_HEIGHT	(600)				// スクリーンウインドウの高さ
#else
#define SCREEN_WIDTH	(1280)				// スクリーンウインドウの幅
#define SCREEN_HEIGHT	(720)				// スクリーンウインドウの高さ
#endif

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

#define SAFE_RELEASE(ptr)		{ if(ptr) { (ptr)->Release(); (ptr) = NULL; } }		// Resource開放ヘルパーマクロ
#define D3DDEVICE				LPDIRECT3DDEVICE9 pDevice = GetDevice();			// ゲットデバイス簡略化ﾏｸﾛ

// 型名短縮宣言
typedef D3DXVECTOR2				Vec2;
typedef D3DXVECTOR3				Vec3;
typedef D3DXVECTOR4				Vec4;
typedef D3DXCOLOR				Color;
typedef D3DXMATRIX				Matrix;
typedef LPDIRECT3DTEXTURE9		Texture;
typedef LPDIRECT3DVERTEXBUFFER9 VtxBuff;
typedef LPDIRECT3DINDEXBUFFER9	IdxBuff;

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

// 画面遷移基本関数群集合構造体
// フェーズ保持に必要な情報(この関数は全てmain.cpp内とfade.cpp内でのみ実行される)
typedef struct {
	void(*Init)		(bool isFirst);	// 初期化関数 (引数:Resource読み込みの要求->true)
	void(*Uninit)	(bool isEnd);	// 終了化関数 (引数:Resource開放処理の要求->true)
	void(*Update)	(void);			// 更新関数
	void(*Draw)		(void);			// 描画関数
}PHASE_FUNC;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);

//=============================================================================
// 現フェーズの関数群取得関数
//=============================================================================
PHASE_FUNC *GetPhase();


#endif