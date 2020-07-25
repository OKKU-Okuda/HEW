/**********************************************************************
[[Phase_Result�w�b�_�[(Phase_Result.h)]]
	��ҁFResultResult�H�H

	Phase_Result.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once

#define	TEXTURE_RESULT			"data/TEXTURE/RESULT.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO		"data/TEXTURE/TITLE.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_NEWGAME_LOGO	"data/TEXTURE/NEW GAME.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EXIT_LOGO		"data/TEXTURE/EXIT.png"			// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TEXTURE_COIN			"data/TEXTURE/number.png"														// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TEXTURE_DISTANCE		"data/TEXTURE/number.png"														// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_DISTANCE	"data/TEXTURE/meter.png"															// �ǂݍ��ރe�N�X�`���t�@�C����


#define	RESULT_TITLE_LOGO_POS_X		(90)		// TITLE���S�̈ʒu(X���W)
#define	RESULT_TITLE_LOGO_POS_Y		(520)		// TITLE���S�̈ʒu(Y���W)
#define	RESULT_TITLE_LOGO_WIDTH		(320)		// TITLE���S�̕�
#define	RESULT_TITLE_LOGO_HEIGHT	(200)		// TITLE���S�̍���

#define	RESULT_NEWGAME_LOGO_POS_X	(480)		// NEWGAME���S�̈ʒu(X���W)
#define	RESULT_NEWGAME_LOGO_POS_Y	(520)		// NEWGAME���S�̈ʒu(Y���W)
#define	RESULT_NEWGAME_LOGO_WIDTH	(320)		// NEWGAME���S�̕�
#define	RESULT_NEWGAME_LOGO_HEIGHT	(200)		// NEWGAME���S�̍���

#define	RESULT_EXIT_LOGO_POS_X		(870)		// EXIT���S�̈ʒu(X���W)
#define	RESULT_EXIT_LOGO_POS_Y		(520)		// EXIT���S�̈ʒu(Y���W)
#define	RESULT_EXIT_LOGO_WIDTH		(320)		// EXIT���S�̕�
#define	RESULT_EXIT_LOGO_HEIGHT		(200)		// EXIT���S�̍���

#define	RESULT_COIN_SIZE_X			(80)																				// �X�R�A�̐����̕�
#define	RESULT_COIN_SIZE_Y			(80)																				// �X�R�A�̐����̍���
#define	RESULT_COIN_INTERVAL_X		(0.0f)																				// �X�R�A�̐����̕\���Ԋu

#define	RESULT_COIN_POS_X			(700)		// �X�R�A�̕\����ʒu�w���W
#define	RESULT_COIN_POS_Y			(400)																				// �X�R�A�̕\����ʒu�x���W


#define ICON_REN			(390)				//�A�C�R���̈ړ����鋗��
#define ICON_MIN			(90)				//�A�C�R���̈�ԏ�̈ʒu
#define ICON_MAX			(870)				//�A�C�R���̈�ԉ��̈ʒu

#define ICON_CURSOR_MAX		(3)									//�J�[�\���̈ړ�

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
typedef struct
{
	VERTEX_2D				vertexWkPauseLogo[NUM_VERTEX];		// ���_���i�[���[�N

	D3DXVECTOR3				pos;								// �|�[�Y��ʂ̈ʒu
	D3DXVECTOR3				rot;								// �|�[�Y��ʂ̉�]��

	float					Radius;								// �G�l�~�[�̔��a
	float					BaseAngle;							// �G�l�~�[�̊p�x

	int						cursor[ICON_CURSOR_MAX];			//�J�[�\��
	bool					use;
}ICON;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
Result��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseResultFunc();

// ��{�֐��Q
void InitResult(bool isFirst);
void UninitResult(bool isEnd);
void UpdateResult();
void DrawResult();