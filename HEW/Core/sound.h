//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���c �^�K
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

typedef void* MySound;	// �T�E���h�I�u�W�F�N�g�ւ̃A�h���X

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

/*=====================================================================
�S�̃T�E���h���ʐݒ�֐�
	�߂�l�Gvoid
	�����FVolume->����
=====================================================================*/
void MySoundSetMasterVolume(float Volume);

/*=====================================================================
�T�E���h�ǂݍ��݊֐�
	�� �ǂݍ��ݎ��s��null������ ��
	�߂�l�GMySound :�T�E���h�@�\
	�����Fconst char* filepath�@:�t�@�C���p�X
=====================================================================*/
MySound MySoundCreate(const char* filepath);

/*=====================================================================
 �T�E���h�����֐�
	�T�E���h�̕������s���֐��@�����ɓ����T�E���h���g�p�������ꍇ�Ȃǂɕ�������
	���̃T�E���h���l�g���I�������J���֐����Ă�
	�߂�l�GMySound	:�������ꂽ���̂��Ԃ�
	�����FMySound:�T�E���h�@�\
===================================================================== */
MySound MySoundClone(MySound sound);

/*=====================================================================
 �T�E���h�폜�֐�
	�����Œ�~���s���̂ŃT�E���h��~�֐����ĂԕK�v�͂Ȃ�
	MySoundDelete		:�C�ӂ̃T�E���h���J��
	MySoundDeleteAuto	:�����T�E���h�f�[�^���g�p���Ă���T�E���h����C�ɊJ��
						 �� �����֐��ŃR�s�[�������̂��Ώ� ��
						 !!���̊֐����g�p��R�s�[�����T�E���h���J�����悤��
						 !!���Ȃ�����(�s���ȏꍇ��MySoundDelete�ł��ׂĂ��J������)
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
===================================================================== */
void MySoundDelete(MySound* sound);
void MySoundDeleteAuto(MySound* sound);

/*=====================================================================
�T�E���h�Đ��֐�
	���̊֐��Ŏ��s����ƍŏ�����Đ������
	MySoundPlayOnce		1��Đ�
	MySoundPlayEternal�@�~�߂�֐����g�p����܂ōĐ�
	MySoundPlay			�C�Ӊ񐔕����[�v�Đ�
	MySoundPlayTemporary�~�߂��Ƃ��납��Đ�(�~�߂����ƂȂ��ꍇ��1��Đ�)
	�߂�l�Gvoid
	�����FMySound :�T�E���h�@�\
=====================================================================*/
void MySoundPlayOnce(MySound sound);
void MySoundPlayEternal(MySound sound);
void MySoundPlay(MySound sound, DWORD cntLoop);
void MySoundPlayTemporary(MySound sound);

/*=====================================================================
 �T�E���h��~�֐�
		MySoundStop		:�w��T�E���h����~�߂�
		MySoundStopAll	:�S�T�E���h��~
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
===================================================================== */
void MySoundStop(MySound sound);
void MySoundStopAll();

/*=====================================================================
 �T�E���h���擾�֐�
		MySoundGetVolume	:�C�ӂ̃T�E���h��̉��ʂ��擾����
		MySoundGetPitch		:�C�ӂ̃T�E���h��̑��x���擾����
		MySoundGetPlaying	:�C���̃T�E���h��̍Đ��󋵂��擾����
		MySoundGetNumGroup	:�������Ă���T�E���h�f�[�^�̔z�������擾����
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
		�@float	:???
===================================================================== */
float MySoundGetVolume(MySound sound);
float MySoundGetPitch(MySound sound);
bool MySoundGetPlaying(MySound sound);
DWORD MySoundGetNumGroup(MySound sound);

/*=====================================================================
 �T�E���h�ݒ�֐�
		MySoundSetVolume	:�C�ӂ̃T�E���h��̉��ʂ𒲐�����
		MySoundSetVolumeAuto:���n���T�E���h�̉��ʂ𒲐�����
		MySoundSetPitch		:�C�ӂ̃T�E���h��̑��x�𒲐�����
		MySoundSetPitchAuto	:���n���T�E���h�̉��ʂ𒲐�����
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
		�@float	:???
===================================================================== */
void MySoundSetVolume(MySound sound, float Volume);
void MySoundSetVolumeAuto(MySound sound, float Volume);
void MySoundSetPitch(MySound sound, float Pitch);
void MySoundSetPitchAuto(MySound sound, float Pitch);

// �T�E���h��{�֐��Q
HRESULT InitSound();
void UninitSound();

#endif
