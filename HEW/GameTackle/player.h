//=============================================================================
//
// ���f������ [player.h]
// Author : �����ċP
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum PLAYER_STATE {
	PLAYER_STOP,
	PLAYER_RUNNING,
	PLAYER_JUMPING,
	PLAYER_SLIDING
};

struct PLAYER
{
	bool     			use;				// ���f���̈ʒu
	PLAYER_STATE		anim_use;			// 0;�������Ă��Ȃ�1;�_�b�V��2;�W�����v3:�X���C�f�B���O
	PLAYER_STATE		old_anim_use;		// 0;�������Ă��Ȃ�1;�_�b�V��2;�W�����v3:�X���C�f�B���O

	float				jump_spped;			// �W�����v�X�s�[�h

	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;			// �}�e���A�����̐�
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuff;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	int                 tbl_cnt;       //�|�C���g�������
	float               move_time;          //�ړ�����
	INTERPOLATION_DATA	*tbl_adr;	    // �s���e�[�u���̃A�h���X

	PLAYER              *parent;            //�e�̃A�h���X�@�e�Ȃ�NULL
};

#define PLAYER_PARTS_MAX		(10)					// �v���C���[�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
