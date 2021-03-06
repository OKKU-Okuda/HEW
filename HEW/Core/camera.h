//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

// カメラ構造体
typedef struct {
	Vec3 pos;		// 視点
	Vec3 at;		// 注視点
	Vec3 up;		// 上方向ベクトル(正規化ひっす)
	Vec3 rot;		// カメラの回転
	float len;		// カメラの視点と注視点の距離
	float rotXZ;	// XZ面回転
	float length;	// 視点注視点間距離
	float angle;	// 視野角
	float aspect;	// アスペクト比	(横/縦)
	float nearZ;	// ビュー平面のNearZ値
	float farZ;		// ビュー平面のFarZ値
}CAMERA;

// カメラ更新関数ポインタ
typedef void(*CameraFunc)(CAMERA*);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//=============================================================================
// カメラ更新関数ポインタ設置関数
//=============================================================================
void SetCameraFunc(CameraFunc pFunc);

//=============================================================================
// カメラ更新関数ポインタ削除関数
//=============================================================================
void ClearCameraFunc();

//=============================================================================
// カメラ更新関数ポインタ取得関数
//=============================================================================
CameraFunc GetCameraFunc();

//=============================================================================
// ビューマトリックスの取得(Draw関数内でのみ有効、Update内だと1フレーム前の行列になる)
//=============================================================================
D3DXMATRIX *GetMtxView(void);

//=============================================================================
// プロジェクションマトリックスの取得(Draw関数内でのみ有効、Update内だと1フレーム前の行列になる)
//=============================================================================
D3DXMATRIX *GetMtxProjection(void);

//=============================================================================
// カメラ情報の取得(位置変更などもこの関数から行う)
//=============================================================================
CAMERA *GetCamera();


// カメラ基本関数群(main.cpp専用)
HRESULT InitCamera(void);	// fadeでも使用
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);		
#endif
