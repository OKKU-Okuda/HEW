/**********************************************************************
[[Tackle�J�����v���O����(GameTackle/camera.cpp)]]
	��ҁF���c�@�^�K

	�^�b�N���̃J�����Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"
#include "../Core/input.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "camera.h"	
#include "player.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// �J�����̉�]��

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Vec3 OffsetCamPos(0, 55.0f, -180.0f);

void TackleCameraUpdate(CAMERA* pCam)
{
	const float add = 2.0f;

	PLAYER *player = GetPlayer();
	

	// �L�[����ł̃J�����ړ�
	//if (GetKeyboardPress(DIK_W))
	//{
	//	OffsetCamPos.z -= add;
	//}

	//if (GetKeyboardPress(DIK_S))
	//{
	//	OffsetCamPos.z += add;
	//}

	//if (GetKeyboardPress(DIK_A))
	//{
	//	OffsetCamPos.x -= add;
	//}

	//if (GetKeyboardPress(DIK_D))
	//{
	//	OffsetCamPos.x += add;
	//}

	//if (GetKeyboardPress(DIK_SPACE))
	//{
	//	OffsetCamPos.y += add;
	//}

	//if (GetKeyboardPress(DIK_LSHIFT))
	//{
	//	OffsetCamPos.y -= add;
	//}


	pCam->at = *GetPlayerPos();// +Vec3(0, 0.0f, 0);
	pCam->pos = *GetPlayerPos() + OffsetCamPos;
	// �J�����̒����_�Ǝ��_����l���ɒǏ]������
	//pCam->pos.x = pCam->at.x + sinf(pCam->rot.y) + OffsetCamPos.x;
	//pCam->pos.z = pCam->at.z + cosf(pCam->rot.y) + OffsetCamPos.z;

	

}