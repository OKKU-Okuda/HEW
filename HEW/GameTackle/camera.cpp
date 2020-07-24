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
#include "countdown.h"
#include "field.h"
#include "field_control.h"
#include "ground_mesh.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define CAMERA_ATOFFSET	(30)	// �����_�̃v���C���[���W�����3�����I�t�Z�b�g(y�ȊO�͂�����Ƃ�΂��j
#define CAMERA_RANGE	(150.0f)		// �v���C���[����̃����W
#define CAMERA_OFFSETY	(70.0f)		// �����_����̎��_�I�t�Z�b�g��

#define CAMERA_DEBUGOFFSET		(Vec3(0, 55.0f, -180.0f))		// �f�o�b�O�p�J������3�����I�t�Z�b�g
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
//static Vec3 OffsetCamPos(0, 55.0f, -180.0f);

/*=====================================================================
�X�^�[�g�J����1�Ԗڊ֐�
=====================================================================*/
void GameStartCam001Update(CAMERA* pCam)
{
	Vec3 StartPos(PLAYER_POSX, 20, -500);
	Vec3 EndPos(PLAYER_POSX, 20, GetLatestFieldPosition().z);

	int sec;
	float per;

	GetCountdownTime(&sec, &per);
	if (sec == 2)
	{// ���̃J�������[�N�ɂ��邩�𔻒f
		GameStartCam002Update(pCam);
		SetCameraFunc(GameStartCam002Update);

		return;
	}

	//perPow = per * per;

	if (EndPos.z > GROUNDSIZE )
	{
		EndPos.z = GROUNDSIZE;
	}

	pCam->at = StartPos + ((EndPos - StartPos)* per);

	pCam->pos = pCam->at + Vec3(0, 300, -100);

	pCam->up = Vec3(0, 0, 1);
	//����̐؂�ڂ��������Ⴄ�̂ŏC��
}

/*=====================================================================
�X�^�[�g�J����2�Ԗڊ֐�
=====================================================================*/
void GameStartCam002Update(CAMERA* pCam)
{
	Vec3 StartPos(PLAYER_POSX +50, 40, PLAYER_POSZ + 50);
	Vec3 EndPos(PLAYER_POSX+50, 40, PLAYER_POSZ -50);

	int sec;
	float per;

	GetCountdownTime(&sec, &per);
	if (sec == 1)
	{// ���̃J�������[�N�ɂ��邩�𔻒f
		TackleCameraUpdate(pCam);
		SetCameraFunc(TackleCameraUpdate);

		return;
	}

	pCam->at = *GetPlayerPos() + Vec3(0, 20.f, 0);

	pCam->pos = StartPos + ((EndPos - StartPos)* per);

}

/*=====================================================================
���C���J�����֐�
=====================================================================*/
void TackleCameraUpdate(CAMERA* pCam)
{	
	Vec3 at = *GetPlayerPos();

	at.y = CAMERA_ATOFFSET;
	pCam->at = at;
	//pCam->pos = *GetPlayerPos() + OffsetCamPos;
	// �J�����̒����_�Ǝ��_����l���ɒǏ]������
	pCam->pos.x = sinf(GetPlayerRot()->y) * CAMERA_RANGE + pCam->at.x ; //+ OffsetCamPos.x;
	pCam->pos.z = cosf(GetPlayerRot()->y) * CAMERA_RANGE + pCam->at.z ; //+ OffsetCamPos.z;
	pCam->pos.y = pCam->at.y + CAMERA_OFFSETY;
	pCam->up = Vec3(0, 1, 0);

}

/*=====================================================================
�f�o�b�O�J�����֐�(�Œ�j
=====================================================================*/
void DebugCameraUpdate(CAMERA* pCam)
{
	pCam->at = *GetPlayerPos();
	pCam->pos = *GetPlayerPos() + CAMERA_DEBUGOFFSET;
}