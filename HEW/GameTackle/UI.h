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
// �\���̒�`
//*****************************************************************************
typedef struct
{
	float f;

	/*�R�C��*/
	int			coin;
	int			coin_target;		// �C�x���g�ύX�̖ڈ���
	int			COIN_PLACE;			// �X�R�A�̌���
	float		COIN_POS_X;		// �X�R�A�̕\����ʒu�w���W
	float		COIN_SIZE_X;
	float		COIN_SIZE_Y;

	LPDIRECT3DTEXTURE9		pD3DTextureCoin[2] = {};		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffCoin = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	D3DXVECTOR3				posCoin;						// �ʒu
	D3DXVECTOR3				rotCoin;						// ����

						/*����*/
	int			distance;
	int			distance_target;	// �X�R�A�̌��C�x���g�p
	int			DISTANCE_PLACE;		//�����̌���
	float		DISTANCE_POS_X;	// �X�R�A�̕\����ʒu�w���W
	float		DISTANCE_SIZE_X;
	float		DISTANCE_SIZE_Y;

	LPDIRECT3DTEXTURE9		pD3DTextureDistance[2] = {};	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffDistance = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	D3DXVECTOR3				posDistance;					// �ʒu
	D3DXVECTOR3				rotDistance;					// ����

	float			add_timer;
	float			add_timer_max;

	float			dec_timer;
	float			dec_timer_max;

} UI;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

					/*�R�C��*/
#define	TEXTURE_COIN			"data/TEXTURE/number.png"														// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_COIN		"data/TEXTURE/coin.png"																// �ǂݍ��ރe�N�X�`���t�@�C����
//#define	COIN_SIZE_X				(35.0f)																				// �X�R�A�̐����̕�
//#define	COIN_SIZE_Y				(50.0f)																				// �X�R�A�̐����̍���
#define	COIN_INTERVAL_X			(0.0f)																				// �X�R�A�̐����̕\���Ԋu

#define	COIN_POS_Y				(25.0f)																				// �X�R�A�̕\����ʒu�x���W

#define COIN_ICON_POS_X			(SCREEN_WIDTH - 20.0f)
#define COIN_ICON_POS_Y			(55.0f)

					/*����*/
#define	TEXTURE_DISTANCE		"data/TEXTURE/number.png"														// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// �ǂݍ��ރe�N�X�`���t�@�C����

//#define	DISTANCE_SIZE_X			(35.0f)																				// �X�R�A�̐����̕�
//#define	DISTANCE_SIZE_Y			(50.0f)																				// �X�R�A�̐����̍���
#define	DISTANCE_INTERVAL_X		(0.0f)																				// �X�R�A�̐����̕\���Ԋu

#define	DISTANCE_POS_Y			(100.0f)
																												// �X�R�A�̕\����ʒu�x���W
#define METER_POS_X				(SCREEN_WIDTH - 20.0f)																// m�̕\����ʒuX���W
#define METER_POS_Y				(125.0f)										// m�̕\����ʒuY���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI(int type);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void ChangeCoin(int value);
void ChangeDistance(int value);

HRESULT MakeVertexDistance(LPDIRECT3DDEVICE9 pDevice);
void SetTextureDistance(int idx, int number);

UI *GetUI(void);
void ResetUI(void);

#endif
