/**********************************************************************
[[�v���C���[�s���i����n)�w�b�_�[(player_control.h)]]
	��ҁF���c�@�^�K


***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
�v���C���[�����ݒ�֐�
=====================================================================*/
void SetPlayerDirection(FIELD_DIRECTION fdir);

/*=====================================================================
�v���C���[�����擾�֐�
=====================================================================*/
FIELD_DIRECTION GetPlayerDirection();

/*=====================================================================
�v���C���[��]��Ԏ擾�֐�
=====================================================================*/
bool IsPlayerRotation();
// ��{�֐��Q
void UpdatePlayerControl();
void ResetPlayerControl();