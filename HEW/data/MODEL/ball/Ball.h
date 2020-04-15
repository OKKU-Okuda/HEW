/**********************************************************************
[[�{�[���w�b�_�[(Ball.h)]]
	��ҁF���c�@�^�K

	Ball.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once

#define MAX_BALL	(100)

#define MAX_EFFECT_BALL	(10)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
HRESULT InitBall(void);
int CreateBall(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	, D3DXVECTOR3 eng = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f)
	);

void DeleteBall(int Ball_ID);
void UpdateBall(void);
void DrawBall(void);
void UninitBall(void);