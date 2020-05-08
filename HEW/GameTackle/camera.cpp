/**********************************************************************
[[Tackle�J�����v���O����(GameTackle/camera.cpp)]]
	��ҁF���c�@�^�K

	�^�b�N���̃J�����Ɋւ���v���O����
***********************************************************************/
#include "../Core/input.h"
#include "../Core/camera.h"
#include"../Core/main.h"
#include "../Core/debugproc.h"

#include "camera.h"	
#include "player.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Vec3 OffsetCamPos(0, 0, -80.0f);

void TackleCameraUpdate(CAMERA* pCam)
{
	const float add = 2.0f;

	PLAYER *player = GetPlayer();

	// �L�[����ł̃J�����ړ�
	if (GetKeyboardPress(DIK_DOWN))
	{
		OffsetCamPos.z -= add;
	}

	if (GetKeyboardPress(DIK_UP))
	{
		OffsetCamPos.z += add;
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		OffsetCamPos.x -= add;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		OffsetCamPos.x += add;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		OffsetCamPos.y += add;
	}

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		OffsetCamPos.y -= add;
	}

	pCam->at = player[0].pos + Vec3(0, 20.0f, 0);
	pCam->pos = pCam->at + OffsetCamPos;
 
}