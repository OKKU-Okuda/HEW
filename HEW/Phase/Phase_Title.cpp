/**********************************************************************
[[�t�F�[�Y:Title�v���O����(Phase_Title.cpp)]]
	��ҁF���c�@�^�K

	�t�F�[�Y�FPhase_Title�Ɋւ���v���O����
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "Phase_Title.h"				
#include "Phase_GameTackle1.h"				// ���̃t�F�[�Y
#include "../Title/effect.h"				// �^�C�g���G�t�F�N�g
#include "../Title/player.h"				// �v���C���[
#include "../Title/bottons.h"				// �{�^��
#include "../Title/select.h"				// �I������
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

// ��ʑJ�ڊ�{�֐��Q���܂Ƃ߂Ă���
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_soundBGM;
static MySound		g_soundSelect;		// �I����




static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ���S���_
	Texture    tex;				// �e�N�X�`��
	float	   col;				// �F
}g_Logo;				// ���S���[�N


/*=====================================================================
Title�X�V�֐�
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("�^�C�g���t�F�[�Y");


	// ���ɂ���l�p�`�G�t�F�N�g�̍X�V
	UpdateTitleEffect();

	if (GetFade() == FADE_NONE)
	{ // �t�F�[�h���������Ă��Ȃ��ꍇ�Ɏ��s

		// �v���C���[�̍X�V
		GetPlayerFunc()->Update();

		if (GetPlayerPosition()->z<=-105)
		{	// �v���C���[���w��ʒu�ɂ���ꍇ

			// ���S�̍X�V
			g_Logo.col += 0.02f;
			Set2DVertexColor(g_Logo.vtx, Color(g_Logo.col, g_Logo.col, g_Logo.col, g_Logo.col));

			// �I���̏����Ƃ��̃G�t�F�N�g�̍X�V
			GetSelectFunc()->Update();

			// �{�^���̍X�V
			GetBottonsFunc()->Update();
		}
	}

}

/*=====================================================================
Title�`��֐�
=====================================================================*/
void DrawTitle()
{
	DWORD	d3drslightning;
	D3DDEVICE;

	// ���C�e�B���O�̐ݒ�
	pDevice->GetRenderState(D3DRS_LIGHTING, &d3drslightning);	
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// �v���C���[�̕`��
	GetPlayerFunc()->Draw();

	DrawTitleEffect();				// ���͂ɕ����Ă���G�t�F�N�g�̕`��

	// ���̃��C�e�B���O��Ԃɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);

	// �{�^���̕`��
	GetBottonsFunc()->Draw();

	// �I���G�t�F�N�g�̕`�揈��
	GetSelectFunc()->Draw();

	// ���S�̕`��
	pDevice->SetTexture(0, g_Logo.tex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Logo.vtx, sizeof(VERTEX_2D));

}

/*=====================================================================
Title�������֐�
	�߂�l : void
	���� :
	bool isFirst		true:���\�[�X�ǂݍ��݌n���܂߂��������������s��
							�@�� ���̏���������ꍇ��main.cpp����		  ��
							  �� InitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏������������s��
=====================================================================*/
void InitTitle(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	���\�[�X�ǂݍ��ݏ���(Create???,Load???,�V���[�Y)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// �^�C�g�����S�̃e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleLogo_000.png", &g_Logo.tex);


		// BGM,SE�̓ǂݍ���
		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");
		g_soundSelect	= MySoundCreate("data/SE/select.wav");
		MySoundSetVolume(g_soundSelect, 3.0f);



		InitTitleEffect(true);

		// �v���C���[�̃��\�[�X�ǂݍ���
		GetPlayerFunc()->Init(true);

		// �{�^���̃��\�[�X�ǂݍ���
		GetBottonsFunc()->Init(true);

		// �I���G�t�F�N�g�̃��\�[�X�ǂݍ���
		GetSelectFunc()->Init(true);

		return;
	}


	InitTitleEffect(false);
	

	MySoundSetMasterVolume(0.1f);
	//---------------------------------------------------------------------
	//	�O���[�o���ϐ����̃X�e�[�^�X������������
	//---------------------------------------------------------------------

	// �J����
#if 1
	GetCamera()->pos	= Vec3(0.0f, 0.0f, -200.0f);	
	GetCamera()->at		= Vec3(-100.0f, 0.0f, 0);
#else
	GetCamera()->pos	= Vec3(0.0f, 0.0f, 200.0f);
	GetCamera()->at		= Vec3(200.0f, 0.0f, -200.0f);
#endif

	// �v���C���[������
	GetPlayerFunc()->Init(false);

	// �{�^��������
	GetBottonsFunc()->Init(false);

	// �I���G�t�F�N�Ƃ̕`��
	GetSelectFunc()->Init(false);

	MySoundPlayEternal(g_soundBGM);	// �i���Đ�
	


	// ���S���_�̐ݒu
	MakeNormal2DVertex(0, g_Logo.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &Vec2(350, 120));
	Set2DVertexColor(g_Logo.vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
	g_Logo.col = 0.0f;


}

/*=====================================================================
Title�I�����֐�
	�߂�l : void
	���� :
	bool isEnd			true:���\�[�X�J���n���܂߂��I�����������s��
							�@�� ���̏���������ꍇ��main.cpp����		    ��
							  �� UninitGameResource�֐��Ɉ���true�ŏ������� ��

						false;���\�[�X�J���n�ȊO�̏I�����������s��	
=====================================================================*/
void UninitTitle(bool isEnd)
{

//---------------------------------------------------------------------
//	���̑��̏I������
//---------------------------------------------------------------------

	MySoundStop(g_soundBGM);	// ��~
	MySoundStop(g_soundSelect);

	GetPlayerFunc()->Uninit(false);
	UninitTitleEffect(false);



	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	���\�[�X�J������
	//---------------------------------------------------------------------


	// �T�E���h�̊J��
	MySoundDeleteAuto(&g_soundBGM);
	MySoundDeleteAuto(&g_soundSelect);

	SAFE_RELEASE(g_Logo.tex);

	UninitTitleEffect(true);
	

}


/*=====================================================================
Title��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

