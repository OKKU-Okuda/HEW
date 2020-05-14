//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(10000.0f)				// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �J�����̉�]��

#define	INTERVAL_CAMERA_R	(12.5f)					// ���f���̎����̐�܂ł̋���
#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.20f)					// �J�����̒����_�ւ̕␳�W��

#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_R		(10.0f)					// �ǐՎ��̒����_�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

static void NoCameraFunc(CAMERA*);		// �X�V���Ȃ��J�����֐��|�C���^�p

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;					// �J�����f�[�^

static CameraFunc	g_UpdateFunc;			// �J�����X�V�֐��|�C���^

static Matrix		g_mtxView;					// �r���[�}�g���b�N�X
static Matrix		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

//=============================================================================
// �J�����̏�����
//=============================================================================
HRESULT InitCamera(void)
{
	// �S�f�t�H���g�l�̑��
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
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	Vec3 length( g_Camera.pos - g_Camera.at);

	// �X�V�֐�
	g_UpdateFunc(&g_Camera);

	// ���_�ƒ����_�̋��������Z
	g_Camera.length = D3DXVec3Length(&length);

	// ��]
	g_Camera.rotXZ = atan2f(-length.z, length.x);

	//D3DXVECTOR3 posPlayer;
	//D3DXVECTOR3 rotPlayer;
	//D3DXVECTOR3 movePlayer;
	//float fLength;
	// ���f���̈ʒu�擾
	//posPlayer = GetPositionPlayer();
	//// ���f���̖ړI�̌����擾
	//rotPlayer = GetRotationPlayer();
	//
	//// ���f���̈ړ��ʎ擾
	//movePlayer = GetMovePlayer();
	//
	//// ���_�̖ړI�ʒu
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sinf(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cosf(rotPlayer.y) * fLength;
	//
	//// �����_�̖ړI�ʒu
	//fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sinf(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cosf(rotPlayer.y) * fLength;
	//
	//// ���_�̕␳
	//g_posCameraP.x += (g_posCameraPDest.x - g_posCameraP.x) * RATE_CHASE_CAMERA_P;
	//g_posCameraP.y += (g_posCameraPDest.y - g_posCameraP.y) * RATE_CHASE_CAMERA_P;
	//g_posCameraP.z += (g_posCameraPDest.z - g_posCameraP.z) * RATE_CHASE_CAMERA_P;
	//
	//// �����_�̕␳
	//g_posCameraR.x += (g_posCameraRDest.x - g_posCameraR.x) * RATE_CHASE_CAMERA_R;
	//g_posCameraR.y += (g_posCameraRDest.y - g_posCameraR.y) * RATE_CHASE_CAMERA_R;
	//g_posCameraR.z += (g_posCameraRDest.z - g_posCameraR.z) * RATE_CHASE_CAMERA_R;

	PrintDebugProc("*****�J����****\nXZ��]�F%f\n�J���������F%f\n���_�F%vec3\n�����_�F%vec3", 
		g_Camera.rotXZ, g_Camera.length, g_Camera.pos, g_Camera.at);

}

//=============================================================================
// �J�����̐ݒ菈��	(���t���[��Draw�����J�n�O�Ɏ��s����)
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_Camera.pos,		// �J�����̎��_
						&g_Camera.at,		// �J�����̒����_
						&g_Camera.up);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								g_Camera.angle,									// ����p
								g_Camera.aspect,								// �A�X�y�N�g��
								g_Camera.nearZ,									// �r���[���ʂ�NearZ�l
								g_Camera.farZ);									// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// �J�����X�V�֐��|�C���^�ݒu�֐�
//=============================================================================
void SetCameraFunc(CameraFunc pFunc)
{
	g_UpdateFunc = pFunc;
	return;
}

//=============================================================================
// �J�����X�V�֐��|�C���^�폜�֐�
//=============================================================================
void ClearCameraFunc()
{
	g_UpdateFunc = NoCameraFunc;
	return;
}

//=============================================================================
// �J�����X�V�֐��|�C���^�擾�֐�
//=============================================================================
CameraFunc GetCameraFunc()
{
	return g_UpdateFunc;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX *GetMtxView(void)
{
	return &g_mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX *GetMtxProjection(void)
{
	return &g_mtxProjection;
}

//=============================================================================
// �J�������̎擾(�ʒu�ύX�Ȃǂ����̊֐�����s��)
//=============================================================================
CAMERA *GetCamera()
{
	return &g_Camera;
}

//=============================================================================
// �J�����X�V���Ȃ��֐�(cpp_func)
//=============================================================================
void NoCameraFunc(CAMERA*)
{
	return;
}