/**********************************************************************
[[�{�[�i�X�X�R�A�w�b�_�[(bonusscore.h)]]
	��ҁF���c�@�^�K

	bonusscore.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once

#define START_COMMONBONUS		(BST_COINEX)	// Common�{�[�i�X���̔��ʗp
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// �{�[�i�X�X�R�A�̎��
enum BONUSSCORE_TYPE {
	/* �ȉ���1��擾�n */
	// �R�C�����W�n�A�`�[�u�����g
	BST_COIN1,
	BST_COIN2,
	BST_COIN3,
	BST_COIN4,

	// �������n�A�`�[�u�����g
	BST_RUN1,
	BST_RUN2,
	BST_RUN3,
	BST_RUN4,
	BST_RUN5,
	BST_RUN6,

	// ��L�Q�̕����A�`�[�u�����g
	BST_COIN3RUN4,
	BST_COIN4RUN5,

	// �M�~�b�N�����񐔌n�A�`�[�u�����g
	BST_GIMMICK1,
	BST_GIMMICK2,
	BST_GIMMICK3,

	// �������ԃA�`�[�u�����g
	BST_LIVETIME1,

	/* �ȉ��͕����擾�n */

	BST_COINEX,
	BST_RUNEX,
	BST_GIMMICKEX,
	BST_RAREVOICE,
	BST_GIMMICKWALL,
	BST_SNEAK,
	BST_COINPERRUN,
	BST_JUSTINTARACT,

	MAX_BONUSSCORETYPE
};

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
�{�[�i�X�X�R�A�ݒ�֐�
=====================================================================*/
void AddBonusScore(BONUSSCORE_TYPE bst);

/*=====================================================================
�{�[�i�X�X�R�A�擾�֐�
=====================================================================*/
DWORD GetBonusScore();

/*=====================================================================
�{�[�i�X�t���O�擾�֐�
=====================================================================*/
bool IsSetRareBonus(BONUSSCORE_TYPE rareBonus);

// ��{�֐��Q
void InitBonusScore(int type);
void UninitBonusScore();
void DrawBonusScore();
void UpdateBonusScore();
