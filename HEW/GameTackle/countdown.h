//=============================================================================
//
// �X�^�[�g��ʏ��� [countdown.h]
// Author : GP11A292 24 �����@�ċP
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include"../Core/main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCountdown(int type);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
int GetCountdownFlag(void);

#endif
