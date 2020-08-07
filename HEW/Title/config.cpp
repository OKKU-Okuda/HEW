/**********************************************************************
[[�ݒ�v���O����(config.cpp)]]
	��ҁF���c�@�^�K

	���[�U�[�ݒ�Ɋւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "../Core/camera.h"

#include "config.h"
#include "camera.h"
#include "select.h"

#include "../GameTackle/UI.h"
//#include <stdlib.h>

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define PATH_CONFIG	("data/RuggerMan.conf")
#define MOVEX_BASECONF		(500.f)

#define CNT_TEXADD			(4)
#define MAX_SELECT			(7)		// �J�E���g�ŃJ�[�\���J��
#define MAX_BOTTONANIM		(7)		// �J�E���g�Ń{�^���A�j���J��
#define MAX_CNTPRESS		(50)	// �������Ń��Z�b�g
#define OFFBOTTON_X			(1025.0f)
#define ONBOTTON_X			(1160.0f)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

typedef struct {
	int		Volume;				// ���[�U�[volume( MasteringVoice = base * this[0~15f] )
	bool	isShowBonusUI;		// �{�[�i�XUI��\�����邩
	bool	isActiveAssistMode;	// �������Ƃ��[�ǂ�L���ɂ��邩
}USER_CONFIG;

// �R���t�B�O����
enum CONFIG_SELECT {
	CS_SHOWBONUS,
	CS_ASSISTMODE,
	CS_RESETRANKING,
	CS_VOLUME,
	CS_BACK,
	MAX_CONFIGSELECT,
};
enum VTX_CONFIG {
	VC_CONFBASE,
	VC_CONFBACK,
	VC_RESET,
	VC_BTNBONUS,
	VC_BTNASSIST,
	VC_VOLUMECURSOR,

	VC_SELECTCURSOR,
	MAX_VTXCONFIG
};

enum TEX_CONFIG {
	TC_CONFBASE,
	TC_CONFBACK,
	TC_RESET,
	TC_BOTTONCURSOR,
	TC_SELECTCURSOR,
	TC_VOLUMECURSOR,
	MAX_TEXCONFIG
};
//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

static void SetPosX(Vec3* Out);	// x�J�ڊ֐�
static void SetVertexConfig();	// ���_�ݒ�֐�
static void MekeVertexConfig();	// ���_�쐬�֐�
static void DrawVertexConfig();		// ���_�`��
static void UpdateConfigSelect();	// �Z���N�g�X�V�֐�
static void UpdateConfigSelectEachObject();	// �Z���N�g�ʂ̍X�V
static void LoadConfig();
static void SaveConfig();
static void UpdateConfigBottonAnimation();
static void UpdateConfigVolumeAnimation();
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static USER_CONFIG g_Config;

static struct {
	VERTEX_2D vtx[NUM_VERTEX];
	Texture	Tex;
	Vec3 pos;
	Vec2 size;
	D3DXCOLOR col;
}g_Vtx[MAX_VTXCONFIG];

static Texture g_Tex[MAX_TEXCONFIG];

static struct {
	const Vec3 list[MAX_CONFIGSELECT] = {
		Vec3(940,200,0),
		Vec3(940,310,0),
		Vec3(940,410,0),
		Vec3(645,510,0),
		Vec3(645,585,0),
	};

	Vec3 pos;
	CONFIG_SELECT	select;
	DWORD			cnt_move;		// ���݃J�[�\���ʒu����ړI�ʒu�܂ł̃t���[���J�E���g��(10�œ��B�j
	DWORD			idx_tex;		// �J�[�\���̃e�N�X�`���Ԓn
}g_Select;

static bool g_isChanged = false;		// �ύX�m�Fflag

static struct {
	bool  On;
	int cnt;
}g_BottonAnim[2];

static struct {
	bool isTrigger;
	int cnt;
}g_PressBotton;

//---------------------------------------------------------------------
//	�X�V�֐�
//---------------------------------------------------------------------
void UpdateConfig()
{
	Vec3 pos;

	if (GetCameraRate() == 0.f || GetSelect() != SELECT_CONFIG)
	{
		return;
	}

	UpdateConfigSelect();				// �Z���N�g�X�V
	UpdateConfigSelectEachObject();		// �e�I�u�W�F�N�g
	UpdateConfigBottonAnimation();		// �{�^���A�j���[�V�����X�V
	UpdateConfigVolumeAnimation();		// ���ʂ̍X�V
	SetVertexConfig();

}

//---------------------------------------------------------------------
//	�`��֐�
//---------------------------------------------------------------------
void DrawConfig()
{
	if (GetCameraRate() == 0.f || GetSelect() != SELECT_CONFIG)
	{
		return;
	}

	// ���_�`��
	DrawVertexConfig();
}

//---------------------------------------------------------------------
//	�������֐�
//---------------------------------------------------------------------
void InitConfig()
{
	D3DDEVICE;

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config.png", &g_Tex[TC_CONFBASE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config_back.png", &g_Tex[TC_CONFBACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config_resetbotton.png", &g_Tex[TC_RESET]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config_cursor.png", &g_Tex[TC_BOTTONCURSOR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config_select.png", &g_Tex[TC_SELECTCURSOR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/config_ratecursor.png", &g_Tex[TC_VOLUMECURSOR]);

	// �ݒ�̓ǂݍ���
	LoadConfig();


	MySoundSetMasterVolume(0.1f * (g_Config.Volume/1.f));
	// �{�C�X�J�[�\���̏�����
	g_Vtx[VC_VOLUMECURSOR].Tex = g_Tex[TC_VOLUMECURSOR];
	g_Vtx[VC_VOLUMECURSOR].col = Color(1, 1, 1, 1);
	g_Vtx[VC_VOLUMECURSOR].pos = Vec3(12345, 507, 0);
	g_Vtx[VC_VOLUMECURSOR].size = Vec2(15, 15);

	// �x�[�X�R���t�B�O�̏�����
	g_Vtx[VC_CONFBASE].Tex = g_Tex[TC_CONFBASE];
	g_Vtx[VC_CONFBASE].col = Color(1, 1, 1, 1);
	g_Vtx[VC_CONFBASE].pos = Vec3(SCREEN_CENTER_X + 160, SCREEN_CENTER_Y, 0);
	g_Vtx[VC_CONFBASE].size = Vec2(SCREEN_CENTER_X*0.72f, SCREEN_CENTER_Y*0.8f);

	// �߂�{�^���̏�����
	g_Vtx[VC_CONFBACK].Tex = g_Tex[TC_CONFBACK];
	g_Vtx[VC_CONFBACK].col = Color(1, 1, 1, 1);
	g_Vtx[VC_CONFBACK].pos = Vec3(SCREEN_CENTER_X + 160, SCREEN_CENTER_Y*1.65f, 0);
	g_Vtx[VC_CONFBACK].size = Vec2(120, 40);

	// ���Z�b�g�{�^���̏�����
	g_Vtx[VC_RESET].Tex = g_Tex[TC_RESET];
	g_Vtx[VC_RESET].col = Color(1, 1, 1, 1);
	g_Vtx[VC_RESET].pos = Vec3(SCREEN_CENTER_X + 450, SCREEN_CENTER_Y*1.15f, 0);
	g_Vtx[VC_RESET].size = Vec2(110, 30);

	// �Z���N�g�{�^���̏�����
	g_Vtx[VC_SELECTCURSOR].Tex = g_Tex[TC_SELECTCURSOR];
	g_Vtx[VC_SELECTCURSOR].col = Color(1, 1, 1, 1);
	g_Vtx[VC_SELECTCURSOR].pos = Vec3(0, 0, 0);
	g_Vtx[VC_SELECTCURSOR].size = Vec2(30, 30);

	// �{�[�i�X�{�^���̏�����
	g_Vtx[VC_BTNBONUS].Tex = g_Tex[TC_BOTTONCURSOR];
	g_Vtx[VC_BTNBONUS].col = Color(1, 1, 1, 1);
	g_Vtx[VC_BTNBONUS].pos = Vec3(500, 197, 0);
	g_Vtx[VC_BTNBONUS].size = Vec2(50, 30);

	g_Vtx[VC_BTNASSIST].Tex = g_Tex[TC_BOTTONCURSOR];
	g_Vtx[VC_BTNASSIST].col = Color(1, 1, 1, 1);
	g_Vtx[VC_BTNASSIST].pos = Vec3(700, 301, 0);
	g_Vtx[VC_BTNASSIST].size = Vec2(50, 30);

	MekeVertexConfig();
}

//---------------------------------------------------------------------
//	�������֐�
//---------------------------------------------------------------------
void UninitConfig()
{
	for (int i = 0; i < MAX_TEXCONFIG; i++)
	{
		SAFE_RELEASE(g_Tex[i]);
	}
}

//---------------------------------------------------------------------
//	���Z�b�g�֐�
//---------------------------------------------------------------------
void ResetConfig()
{
	g_Select.cnt_move = 0;
	g_Select.select = CS_SHOWBONUS;
	g_Select.idx_tex = 0;
	g_Select.pos = g_Select.list[CS_SHOWBONUS];

	if (g_Config.isShowBonusUI == true)
	{
		g_Vtx[VC_BTNBONUS].pos.x = ONBOTTON_X;
		g_BottonAnim[0].On = true;
	}
	else
	{
		g_Vtx[VC_BTNBONUS].pos.x = OFFBOTTON_X;
		g_BottonAnim[0].On = false;
	}

	if (g_Config.isActiveAssistMode == true)
	{
		g_Vtx[VC_BTNASSIST].pos.x = ONBOTTON_X;
		g_BottonAnim[1].On = true;
	}
	else
	{
		g_Vtx[VC_BTNASSIST].pos.x = OFFBOTTON_X;
		g_BottonAnim[1].On = false;
	}

	//g_Vtx[VC_VOLUMECURSOR].pos.x = 100 + (300 - 100)*(g_Config.Volume / (float)15);
}

//---------------------------------------------------------------------
//	�A�V�X�g��Ԏ擾�֐�
//---------------------------------------------------------------------
bool IsAssistModeActive()
{
	return g_Config.isActiveAssistMode;
}

//---------------------------------------------------------------------
//	�{�[�i�XUI�\���擾�֐�
//---------------------------------------------------------------------
bool IsBonusUIShow()
{
	return g_Config.isShowBonusUI;
}

//---------------------------------------------------------------------
//	���ʐݒ�|���S���X�V�֐�
//---------------------------------------------------------------------
void UpdateConfigVolumeAnimation()
{
	const float TO_X = 667 + (1160 - 667)*(g_Config.Volume / (float)15);
	g_Vtx[VC_VOLUMECURSOR].pos.x += (TO_X - g_Vtx[VC_VOLUMECURSOR].pos.x)*0.72f;
}

//---------------------------------------------------------------------
//	�{�^���X�V�֐�
//---------------------------------------------------------------------
void UpdateConfigBottonAnimation()
{
	for (int i = 0; i < 2; i++)
	{
		DWORD idx = VC_BTNBONUS + i;

		if (g_BottonAnim[i].On == true)
		{
			g_BottonAnim[i].cnt++;
			if (g_BottonAnim[i].cnt > MAX_BOTTONANIM)
			{
				g_BottonAnim[i].cnt = MAX_BOTTONANIM;
			}

			// on�͗�
			g_Vtx[idx].col = Color(0, 1, 0, 1);
		}
		else
		{
			g_BottonAnim[i].cnt--;
			if (g_BottonAnim[i].cnt < 0)
			{
				g_BottonAnim[i].cnt = 0;
			}

			// off�͊D�F
			g_Vtx[idx].col = Color(0.4f, 0.4f, 0.4f, 1);
		}

		g_Vtx[idx].pos.x = OFFBOTTON_X + ((ONBOTTON_X - OFFBOTTON_X)*(g_BottonAnim[i].cnt / (float)MAX_BOTTONANIM));
	}
}

//---------------------------------------------------------------------
//	�Z���N�g�n�ʂ̍X�V�֐�(cpp_func)
//---------------------------------------------------------------------
void UpdateConfigSelectEachObject()
{
	switch (g_Select.select)
	{
	case CS_SHOWBONUS:	// �{�[�i�XUI�̕\��
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C)||
			GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_Y)||
			GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_Z))
		{// �G���^�[or ��or�E�Ŕ��]
			g_isChanged = true;
			g_Config.isShowBonusUI = !g_Config.isShowBonusUI;
			g_BottonAnim[0].On = g_Config.isShowBonusUI;
			PlayUIGuideSelect();
		}
		break;

	case CS_ASSISTMODE:	// �A�V�X�g���[�h�̕\����\��
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C) ||
			GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_Y) ||
			GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_Z))
		{// �G���^�[or ��or�E�Ŕ��]
			g_isChanged = true;
			g_Config.isActiveAssistMode = !g_Config.isActiveAssistMode;
			g_BottonAnim[1].On = g_Config.isActiveAssistMode;
			PlayUIGuideSelect();
		}
		break;

	case CS_RESETRANKING: // ���Z�b�g
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
		{
			g_PressBotton.isTrigger = true;
		}

		if ((GetKeyboardPress(DIK_RETURN) || IsButtonPressed(0, BUTTON_C)) && g_PressBotton.isTrigger == true)
		{
			g_PressBotton.cnt++;

			if (g_PressBotton.cnt >= MAX_CNTPRESS)
			{// ���Z�b�g����

				PlayUIGuideSelect();
				g_PressBotton.isTrigger = false;
				g_PressBotton.cnt = 0;
			}
		}
		else
		{
			g_PressBotton.cnt -= 2;
			if (g_PressBotton.cnt < 0)
			{
				g_PressBotton.cnt = 0;
			}
		}

		// �Ԃ��Ȃ鏈��
		g_Vtx[VC_RESET].col.a = g_Vtx[VC_RESET].col.r = 1.f;
		g_Vtx[VC_RESET].col.g = g_Vtx[VC_RESET].col.b = 1.f - (g_PressBotton.cnt/(float)MAX_CNTPRESS);
		break;

	case CS_VOLUME:
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_Z))
		{
			g_isChanged = true;
			g_Config.Volume++;

			if (g_Config.Volume > 15)
			{
				g_Config.Volume = 15;
			}
			MySoundSetMasterVolume(0.1f * (g_Config.Volume / 1.f));
		}

		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_Y))
		{
			g_isChanged = true;
			g_Config.Volume--;

			if (g_Config.Volume < 0)
			{
				g_Config.Volume = 0;
			}
			MySoundSetMasterVolume(0.1f * (g_Config.Volume / 1.f));
		}

		break;

	case CS_BACK:// �߂�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_C))
		{
			SetSubTitleMenu(false);
		}

		if (g_isChanged == true)
		{// �ύX������ꍇ�͕ύX
			SaveConfig();
		}
		break;

	default:
	case MAX_CONFIGSELECT:
		break;
	}
}

//---------------------------------------------------------------------
//	�Z���N�g�n�̍X�V�֐�(cpp_func)
//---------------------------------------------------------------------
void UpdateConfigSelect()
{
	if (GetCount() % CNT_TEXADD == 0)
	{// ���邮��e�N�X�`���̍X�V
		g_Select.idx_tex++;
		g_Select.idx_tex %= 5 * 4;

		Set2DTexPos(g_Vtx[VC_SELECTCURSOR].vtx, 5, 4, g_Select.idx_tex % 5, g_Select.idx_tex / 5);
	}

	if (g_Select.cnt_move < MAX_SELECT)
	{// �Z���N�g�ʒu�̑J��
		g_Select.cnt_move++;

		g_Vtx[VC_SELECTCURSOR].pos = g_Select.pos + ((g_Select.list[g_Select.select] - g_Select.pos)*(g_Select.cnt_move / (float)MAX_SELECT));
	}

	if (GetCameraRate() != 1.f)
	{// ���S�ɑJ�ڂ��Ă��Ȃ���΂肽��
		return;
	}

	if (GetKeyboardTrigger(DIK_UP))
	{
		if (g_Select.select > 0)
		{
			g_Select.select = (CONFIG_SELECT)(g_Select.select - 1);
			MySoundPlayOnce(GetSoundSelect());
		}
		g_Select.cnt_move = 0;
		g_Select.pos = g_Vtx[VC_SELECTCURSOR].pos;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		if (g_Select.select < CS_BACK)
		{
			g_Select.select = (CONFIG_SELECT)(g_Select.select + 1);
			MySoundPlayOnce(GetSoundSelect());
		}
		g_Select.cnt_move = 0;
		g_Select.pos = g_Vtx[VC_SELECTCURSOR].pos;
	}
}

//---------------------------------------------------------------------
//	���_�`��֐�(cpp_func)
//---------------------------------------------------------------------
void DrawVertexConfig()
{
	for (int i = 0; i < MAX_VTXCONFIG; i++)
	{
		Draw2DVertex(g_Vtx[i].Tex, g_Vtx[i].vtx);
	}
}

//---------------------------------------------------------------------
//	���_�ݒ�֐�(cpp_func)
//---------------------------------------------------------------------
void SetVertexConfig()
{
	Vec3 pos;
	Color col;
	for (int i = 0; i < MAX_VTXCONFIG; i++)
	{
		pos = g_Vtx[i].pos;
		SetPosX(&pos);

		col = g_Vtx[i].col;
		col.a *= GetCameraRate();

		Set2DVerTex(g_Vtx[i].vtx, &pos, &g_Vtx[i].size);
		Set2DVertexColor(g_Vtx[i].vtx, col);
	}
}

//---------------------------------------------------------------------
//	���_�쐬�֐�(cpp_func)
//---------------------------------------------------------------------
void MekeVertexConfig()
{
	Vec3 pos;
	Color col;
	for (int i = 0; i < MAX_VTXCONFIG; i++)
	{
		pos = g_Vtx[i].pos;
		SetPosX(&pos);

		col = g_Vtx[i].col;
		col.a *= GetCameraRate();

		MakeNormal2DVertex(0,g_Vtx[i].vtx, &pos, &g_Vtx[i].size);
		Set2DVertexColor(g_Vtx[i].vtx, col);
	}
}

//---------------------------------------------------------------------
//	x���W�J�ڕύX�֐�(cpp_func)
//---------------------------------------------------------------------
void SetPosX(Vec3* Out)
{
	Out->x += MOVEX_BASECONF * (1.f - GetCameraRate());
}

//---------------------------------------------------------------------
//	�ۑ�(cpp_func)
//---------------------------------------------------------------------
void SaveConfig()
{
	FILE	*fp;

	fopen_s(&fp, PATH_CONFIG, "wb");	// �o�C�i���������݃��[�h

	if (fp != NULL)
	{
		fwrite(&g_Config, 1, sizeof(USER_CONFIG), fp);
		g_isChanged = false;

		fclose(fp);
	}
	else
	{
		MessageBox(GetHandle(), "�ݒ�̕ۑ��Ɏ��s", 0, 0);
	}
}

//---------------------------------------------------------------------
//	��������(cpp_func)
//---------------------------------------------------------------------
void LoadConfig()
{
	FILE	*fp;

	fopen_s(&fp, PATH_CONFIG, "rb");	// �o�C�i���ǂݍ��݃��[�h

	if (fp != NULL)
	{
		fread(&g_Config, 1, sizeof(USER_CONFIG), fp);
		fclose(fp);
	}
	else
	{// ���݂��Ȃ��̂ŐV�K�쐬
		g_Config.isActiveAssistMode = false;
		g_Config.isShowBonusUI = true;
		g_Config.Volume = 7;
	}
}