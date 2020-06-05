//=============================================================================
//
// UI���� [UI.h]
// Author : �����@�
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include"../Core/main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

					/*�R�C��*/
#define	TEXTURE_COIN			"data/TEXTURE/number000.png"														// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_COIN		"data/TEXTURE/coin.png"																// �ǂݍ��ރe�N�X�`���t�@�C����
#define	COIN_SIZE_X				(35.0f)																				// �X�R�A�̐����̕�
#define	COIN_SIZE_Y				(50.0f)																				// �X�R�A�̐����̍���
#define	COIN_INTERVAL_X			(0.0f)																				// �X�R�A�̐����̕\���Ԋu

#define	COIN_PLACE				(8)																					// �X�R�A�̌���

#define	COIN_POS_X				(SCREEN_WIDTH - (COIN_INTERVAL_X + COIN_SIZE_X) * COIN_PLACE - 0.0f)				// �X�R�A�̕\����ʒu�w���W
#define	COIN_POS_Y				(25.0f)																				// �X�R�A�̕\����ʒu�x���W

#define COIN_ICON_POS_X			(SCREEN_WIDTH - 295.0f)
#define COIN_ICON_POS_Y			(55.0f)

					/*����*/
#define	TEXTURE_DISTANCE		"data/TEXTURE/number000.png"														// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// �ǂݍ��ރe�N�X�`���t�@�C����

#define	DISTANCE_SIZE_X			(35.0f)																				// �X�R�A�̐����̕�
#define	DISTANCE_SIZE_Y			(50.0f)																				// �X�R�A�̐����̍���
#define	DISTANCE_INTERVAL_X		(0.0f)																				// �X�R�A�̐����̕\���Ԋu

#define	DISTANCE_PLACE			(8)																					// �X�R�A�̌���

#define	DISTANCE_POS_X			(SCREEN_WIDTH - (DISTANCE_INTERVAL_X + DISTANCE_SIZE_X) * DISTANCE_PLACE - 35.0f)	// �X�R�A�̕\����ʒu�w���W
#define	DISTANCE_POS_Y			(100.0f)
																												// �X�R�A�̕\����ʒu�x���W
#define METER_POS_X				(SCREEN_WIDTH - 20.0f)																// m�̕\����ʒuX���W
#define METER_POS_Y				(125.0f)																			// m�̕\����ʒuY���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI(int type);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void ChangeCoin(int value);
void ChangeDistance(int value);

#endif
