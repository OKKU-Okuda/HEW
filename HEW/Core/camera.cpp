//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// 視野角
#define	VIEW_NEAR_Z			(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(10000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// カメラの回転量

#define	INTERVAL_CAMERA_R	(12.5f)					// モデルの視線の先までの距離
#define	RATE_CHASE_CAMERA_P	(0.35f)					// カメラの視点への補正係数
#define	RATE_CHASE_CAMERA_R	(0.20f)					// カメラの注視点への補正係数

#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_R		(10.0f)					// 追跡時の注視点の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

static void NoCameraFunc(CAMERA*);		// 更新しないカメラ関数ポインタ用

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;					// カメラデータ

static CameraFunc	g_UpdateFunc;			// カメラ更新関数ポインタ

static Matrix		g_mtxView;					// ビューマトリックス
static Matrix		g_mtxProjection;			// プロジェクションマトリックス

//=============================================================================
// カメラの初期化
//=============================================================================
HRESULT InitCamera(void)
{
	// 全デフォルト値の代入
	g_Camera.pos	=	D3DXVECTOR3(0.0f, 100.0f, -200.0f);
	g_Camera.at		=	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.up		=	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.angle	= VIEW_ANGLE;
	g_Camera.aspect = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
	g_Camera.nearZ	= VIEW_NEAR_Z;
	g_Camera.farZ	= VIEW_FAR_Z;

	ClearCameraFunc();
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	Vec3 length( g_Camera.pos - g_Camera.at);

	// 更新関数
	g_UpdateFunc(&g_Camera);

	// 視点と注視点の距離を演算
	g_Camera.length = D3DXVec3Length(&length);

	// 回転
	g_Camera.rotXZ = atan2f(-length.z, length.x);

	//D3DXVECTOR3 posPlayer;
	//D3DXVECTOR3 rotPlayer;
	//D3DXVECTOR3 movePlayer;
	//float fLength;
	// モデルの位置取得
	//posPlayer = GetPositionPlayer();
	//// モデルの目的の向き取得
	//rotPlayer = GetRotationPlayer();
	//
	//// モデルの移動量取得
	//movePlayer = GetMovePlayer();
	//
	//// 視点の目的位置
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sinf(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cosf(rotPlayer.y) * fLength;
	//
	//// 注視点の目的位置
	//fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sinf(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cosf(rotPlayer.y) * fLength;
	//
	//// 視点の補正
	//g_posCameraP.x += (g_posCameraPDest.x - g_posCameraP.x) * RATE_CHASE_CAMERA_P;
	//g_posCameraP.y += (g_posCameraPDest.y - g_posCameraP.y) * RATE_CHASE_CAMERA_P;
	//g_posCameraP.z += (g_posCameraPDest.z - g_posCameraP.z) * RATE_CHASE_CAMERA_P;
	//
	//// 注視点の補正
	//g_posCameraR.x += (g_posCameraRDest.x - g_posCameraR.x) * RATE_CHASE_CAMERA_R;
	//g_posCameraR.y += (g_posCameraRDest.y - g_posCameraR.y) * RATE_CHASE_CAMERA_R;
	//g_posCameraR.z += (g_posCameraRDest.z - g_posCameraR.z) * RATE_CHASE_CAMERA_R;

	PrintDebugProc("*****カメラ****\nXZ回転：%f\nカメラ距離：%f\n視点：%vec3\n注視点：%vec3", 
		g_Camera.rotXZ, g_Camera.length, g_Camera.pos, g_Camera.at);

}

//=============================================================================
// カメラの設定処理	(毎フレームDraw処理開始前に実行する)
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_Camera.pos,		// カメラの視点
						&g_Camera.at,		// カメラの注視点
						&g_Camera.up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								g_Camera.angle,									// 視野角
								g_Camera.aspect,								// アスペクト比
								g_Camera.nearZ,									// ビュー平面のNearZ値
								g_Camera.farZ);									// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// カメラ更新関数ポインタ設置関数
//=============================================================================
void SetCameraFunc(CameraFunc pFunc)
{
	g_UpdateFunc = pFunc;
	return;
}

//=============================================================================
// カメラ更新関数ポインタ削除関数
//=============================================================================
void ClearCameraFunc()
{
	g_UpdateFunc = NoCameraFunc;
	return;
}

//=============================================================================
// カメラ更新関数ポインタ取得関数
//=============================================================================
CameraFunc GetCameraFunc()
{
	return g_UpdateFunc;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX *GetMtxView(void)
{
	return &g_mtxView;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX *GetMtxProjection(void)
{
	return &g_mtxProjection;
}

//=============================================================================
// カメラ情報の取得(位置変更などもこの関数から行う)
//=============================================================================
CAMERA *GetCamera()
{
	return &g_Camera;
}

//=============================================================================
// カメラ更新しない関数(cpp_func)
//=============================================================================
void NoCameraFunc(CAMERA*)
{
	return;
}