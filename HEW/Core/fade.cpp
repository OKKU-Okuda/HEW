//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"
#include "sound.h"
#include "camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FADE_RATE		(0.02f)		// �t�F�[�h�W��

#define FADE_TEXPLAYER	("data/TEXTURE/Fade_Player.png")
#define FADE_TEXCITY	("data/TEXTURE/Fade_City000.png")
#define FADE_TEXCITY1	("data/TEXTURE/Fade_City001.png")
#define FADE_TEXGREEN	("data/TEXTURE/Fade_Green.png")
#define FADE_TEXSKY		("data/TEXTURE/Fade_Sky.png")

enum FADE_OBJECT_DEFAULT
{
	DEFAULT_PLAYER,
	DEFAULT_CITY,
	DEFAULT_CITY1,
	DEFAULT_GREEN,
	DEFAULT_SKY,

	MAX_DEFAULTOBJECT
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
//void SetColor(D3DCOLOR col);

static void UpdateAnimDefault();
static void DrawAnimDefault();

static void SetFadeVertexColor(VERTEX_2D *vtx_data, Color nColor);
static void MakeFadeVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
static void SetFadeVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static NormalFunc	g_UpdateAnimFunc;			// �A�j���[�V�����X�V�֐�
static NormalFunc	g_DrawAnimFunc;				// �A�j���[�V�����`��֐�
static PHASE_FUNC	g_NextFunc;					// ���t�F�[�Y�ւ̊֐��Q
static FADE			g_fade = FADE_NONE;			// �t�F�[�h���
static FADE_ANIM	g_AnimType;					// �A�j���[�V�����^�C�v

static float		g_Time;						// �A�j���[�V�������ݎ���
												

static struct {
	Texture		Tex[MAX_DEFAULTOBJECT];
	VERTEX_2D	Vtx[MAX_DEFAULTOBJECT][NUM_VERTEX];
	MySound		SE;
}g_DefaultWk;									// �f�t�H���g�A�j���[�V�������[�N

//=============================================================================
// �t�F�[�h�ݒ�֐�
// ���̃t�F�[�Y�ɔ�Ԋ֐�
// ����:
//		PHASE_FUNC* NextPhaseFunc,	:���̃t�F�[�Y��{�֐��Q
//		FADE_ANIM AnimType			:�A�j���[�V�����^�C�v�i�K�{�łȂ�)
//=============================================================================
void GoNextPhase(PHASE_FUNC* NextPhaseFunc,FADE_ANIM AnimType)
{
	if (g_fade != FADE_NONE)
	{	// ���̃t�F�[�h�N�����̏ꍇ�͖���
		return;
	}

	// �p�����[�^�̏�����
	g_NextFunc = *NextPhaseFunc;
	g_fade = FADE_OUT;
	g_Time = 0.0f;
	g_AnimType = AnimType;

	// �A�j���[�V�����֐��̓���ւ�
	switch (AnimType)
	{
	// �f�t�H���g����
	case FADEANIM_DEFAULT:
	default:
		MySoundPlayOnce(g_DefaultWk.SE);
		g_UpdateAnimFunc = UpdateAnimDefault;
		g_DrawAnimFunc = DrawAnimDefault;
		break;
	}
}


//=============================================================================
// �f�t�H���g�A�j���[�V�����֐��Q(cpp_func)
//=============================================================================
void UpdateAnimDefault()
{
	Vec3 pos[MAX_DEFAULTOBJECT];
	Color	col = Color(1, 1, 1, 1);		// �ʏ�I�u�W�F�N�g�̐F
	//Color	col_imp = Color(1, 1, 1, 1);	// �d�v�I�u�W�F�N�g�̐F
	if (g_fade == FADE_OUT)
	{
		float rate = cosf(D3DX_PI / 2.0f - (g_Time * (D3DX_PI / 2.0f)));
		g_Time += 0.01f;
		
		pos[DEFAULT_PLAYER] = Vec3(200.0f *rate + 100, SCREEN_CENTER_Y + 100, 0);
	//	pos[DEFAULT_GREEN] = Vec3(200.0f *rate + 100, SCREEN_HEIGHT - 150, 0);
		pos[DEFAULT_CITY] = Vec3(-100.0f*rate + SCREEN_CENTER_X + 150, 250, 0);
		pos[DEFAULT_CITY1] = Vec3(-70.0f*rate + SCREEN_CENTER_X + 100, 250, 0);
		pos[DEFAULT_SKY] = Vec3(-20.0f*rate + SCREEN_CENTER_X + 100, SCREEN_CENTER_Y, 0);

	}
	else if (g_fade == FADE_IN)
	{
		float rate = sinf(D3DX_PI / 2.0f + (g_Time * (D3DX_PI / 2.0f)));
		g_Time -= 0.01f;

		pos[DEFAULT_PLAYER] = Vec3(200.0f *rate + 300, SCREEN_CENTER_Y + 100, 0);
	//	pos[DEFAULT_GREEN] = Vec3(200.0f *rate + 300, SCREEN_HEIGHT - 150, 0);
		pos[DEFAULT_CITY] = Vec3(-100.0f*rate + SCREEN_CENTER_X+ 50, 250, 0);
		pos[DEFAULT_CITY1] = Vec3(-70.0f*rate + SCREEN_CENTER_X + 30, 250, 0);
		pos[DEFAULT_SKY] = Vec3(-20.0f*rate + SCREEN_CENTER_X + 80, SCREEN_CENTER_Y, 0);

	}


	pos[DEFAULT_GREEN] = Vec3(SCREEN_CENTER_X, SCREEN_HEIGHT-150, 0.0f);
	col.a = g_Time / 0.5f;

	// ���_�ݒu
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_PLAYER],	&pos[DEFAULT_PLAYER],	&Vec2(275,240));
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_CITY],	&pos[DEFAULT_CITY],		&Vec2(SCREEN_CENTER_X+150,225));	// �O�ʓs�s
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_CITY1],	&pos[DEFAULT_CITY1],	&Vec2(SCREEN_CENTER_X+100,225));	// �w�ʓs�s
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_GREEN],	&pos[DEFAULT_GREEN],	&Vec2(SCREEN_CENTER_X,150));
	SetFadeVertex(g_DefaultWk.Vtx[DEFAULT_SKY],		&pos[DEFAULT_SKY],		&Vec2(SCREEN_CENTER_X+100,SCREEN_CENTER_Y));

	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_PLAYER], col);
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_CITY], col);//
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_CITY1], col);//
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_GREEN], col);
	SetFadeVertexColor(g_DefaultWk.Vtx[DEFAULT_SKY], col);// 
}

void DrawAnimDefault()
{
	D3DDEVICE;
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, 0);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, 0);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_SKY]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_SKY], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_CITY1]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_CITY1], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_CITY]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_CITY], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_GREEN]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_GREEN], sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_DefaultWk.Tex[DEFAULT_PLAYER]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_DefaultWk.Vtx[DEFAULT_PLAYER], sizeof(VERTEX_2D));

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�

}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �f�t�H���g�A�j���̏�����
	{
		// �T�E���h
		g_DefaultWk.SE = MySoundCreate("data/SE/laugh.wav");

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	
			FADE_TEXPLAYER, &g_DefaultWk.Tex[DEFAULT_PLAYER]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXCITY, &g_DefaultWk.Tex[DEFAULT_CITY]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXCITY1, &g_DefaultWk.Tex[DEFAULT_CITY1]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXGREEN, &g_DefaultWk.Tex[DEFAULT_GREEN]);
		D3DXCreateTextureFromFile(pDevice,
			FADE_TEXSKY, &g_DefaultWk.Tex[DEFAULT_SKY]);

		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_PLAYER], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_CITY], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_CITY1], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_GREEN], &Vec3(0, 0, 0), &Vec2(0, 0));
		MakeFadeVertex(0, g_DefaultWk.Vtx[DEFAULT_SKY], &Vec3(0, 0, 0), &Vec2(0, 0));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	// �f�t�H�A�j���̊J��
	for (int i = 0; i < MAX_DEFAULTOBJECT; i++)
	{
		SAFE_RELEASE(g_DefaultWk.Tex[i]);
	}

	MySoundDelete(&g_DefaultWk.SE);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	if (g_fade == FADE_NONE)return;

	g_UpdateAnimFunc();		// �A�j���[�V�����X�V�֐�

	if(g_fade == FADE_OUT&& g_Time >= 1.0f)
	{// �t�F�[�h�A�E�g����
		// �t�F�[�h�C�������ɐ؂�ւ�
		g_Time = 1.0f;

		GetPhase()->Uninit(false);	// �I������
		*GetPhase() = g_NextFunc;	// �֐��Q����ւ�

		InitCamera();		
		GetPhase()->Init(false);	// �V�t�F�[�Y�̏���������

		g_fade = FADE_IN;
	}
	else if(g_fade == FADE_IN&& g_Time <= 0.0f)
	{// �t�F�[�h�C������
		// �t�F�[�h�����I��
		g_Time = 0.0f;

		g_fade = FADE_NONE;
	}
}

//=============================================================================
// �^�C�g�����
//=============================================================================
void DrawFade(void)
{
	if (g_fade == FADE_NONE)return;

	// �A�j���[�V�����`��
	g_DrawAnimFunc();
}

//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}

/*=====================================================================
���ՓI�ɒ��_�ݒu�֐�(�m�[�}��)(cpp_func)
	�߂�l�Fvoid
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
=====================================================================*/
void MakeFadeVertex(int num, VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
{
	// ���ːE�̐ݒ�
	SetFadeVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

	// �p�[�X�y�N�e�B�u�R���N�g
	(vtx_data)->rhw =
		(vtx_data + 1)->rhw =
		(vtx_data + 2)->rhw =
		(vtx_data + 3)->rhw = 1.0f;

	// ���_���W
	(vtx_data)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 1)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 2)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y + Size->y, 0.0f);
	(vtx_data + 3)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y + Size->y, 0.0f);

	// �������łO�w�肪�����
	if (num == 0)
	{
		// �e�N�X�`�����W
		(vtx_data)->tex = D3DXVECTOR2(0.01f, 0.01f);
		(vtx_data + 1)->tex = D3DXVECTOR2(0.99f, 0.01f);
		(vtx_data + 2)->tex = D3DXVECTOR2(0.01f, 0.99f);
		(vtx_data + 3)->tex = D3DXVECTOR2(0.99f, 0.99f);
	}

	return;
}

/*=====================================================================
�J���[�ݒu�֐�(�m�[�}��) (cpp_func)
	�߂�l�Fvoid
	�����F
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
 Color nColor			: �V�����J���[
=====================================================================*/
void SetFadeVertexColor(VERTEX_2D *vtx_data, Color nColor)
{
	// ���ːE�̐ݒ�
	(vtx_data)->diffuse =
		(vtx_data + 1)->diffuse =
		(vtx_data + 2)->diffuse =
		(vtx_data + 3)->diffuse = nColor;

}

/*=====================================================================
���_�̐ݒu���s���֐�(cpp_func)
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	Vec3 *Pos,	:���S���W
	Vec2 *Size	:���S����̃T�C�Y
=====================================================================*/
void SetFadeVertex(VERTEX_2D *vtx_data, Vec3 *Pos, Vec2 *Size)
{
	vtx_data->vtx.x = Pos->x - Size->x;
	vtx_data->vtx.y = Pos->y - Size->y;

	(vtx_data + 1)->vtx.x = Pos->x + Size->x;
	(vtx_data + 1)->vtx.y = Pos->y - Size->y;

	(vtx_data + 2)->vtx.x = Pos->x - Size->x;
	(vtx_data + 2)->vtx.y = Pos->y + Size->y;

	(vtx_data + 3)->vtx.x = Pos->x + Size->x;
	(vtx_data + 3)->vtx.y = Pos->y + Size->y;
}

