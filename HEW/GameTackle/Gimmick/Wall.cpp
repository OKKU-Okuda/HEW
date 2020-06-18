/**********************************************************************
[[GIMMICK�̕ǃv���O����(Gimmick/Wall.cpp)]]
	��ҁF���c�@�^�K

	�ǂ�4�̃L�[�ŉ󂷃v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Phase/Phase_GameTackle1.h"

#include "../field.h"
#include "Wall.h"
#include "Gimmick.h"

#include "../effect.h"
#include "../player.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define NUM_POOL		(30)			// �ۊǗ̈搔
#define Wk				GIMMICK_WALL*		// �I�u�W�F�N�g���[�N�|�C���^
#define var(name,arg)	Wk name = (Wk)arg;	// �e�\���̃A�h���X�̕ϊ�ϸ�

#define RANGE_INPUTSTART		(750.0f)			// ���͎�t�͈�(�v���C���[�Ƃ̋���)
#define RANGE_INPUTEND			(75.0f)				// ���͏I���͈�	(����)
#define RANGE_DOWN				(30.0f)				// �������v����܂ł͈̔�

#define WALLSIZE_Z		(20.0f)
#define EFFECT_SIZE		(40.0f)
#define MAX_DOWNVOLUME	(5.0f)		// �ǂ�������Ƃ��̍ő剹��
#define MIN_DOWNVOLUME	(0.5f)		// ����ŏ�����
#define DOWN_VOLUME		(0.15f)		// ��������
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// ��GIMMICK��ԗ�
enum WALL_STATE {
	WSTATE_WAIT,
	WSTATE_FAILEDQTE,
	WSTATE_SUCCESSQTE,
};

// ��GIMMICK�\����
typedef struct {
	GIMMICK_HEADER	Head;			// �w�b�_

	Matrix			WldMat;			// �ǂ̃��[���h�}�g���b�N�X
	Vec3			Pos;			// 3�������W(�t�B�[���h(chip)���W)
	Vec3			WldPos;			// ��L���[���h���W
	//Vec3			Rot;			// ��]

	GIMICKWALL_QTE	Type;			// QTE�{�^���̎��
	WALL_STATE		State;			// �{�^���̏��
	float			lenZSuc;		// QTE���������Ƃ���playerz�l�Ƃ̋���
	bool			isSoundPlaying;	// �T�E���h�𗬂��Ƃ���

}GIMMICK_WALL;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

static void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos);	// �X�V�֐�
static void DrawGimmick(GIMMICK_HEADER* pHead);					// �`��֐�
static void EndGimmick(GIMMICK_HEADER* pHead);					// �I���֐�
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static GIMMICK_FUNC g_Func = { UpdateGimmick ,DrawGimmick ,EndGimmick };
static GIMMICK_WALL g_aGimmick[NUM_POOL];
static Texture		g_tex[MAX_GIMICKWALLQTE];
static Mesh			g_meshWall;	
static MySound		g_seWalldowm;	// �ǂ��I���鉹
static MySound		g_seWallFailed;	// GIMMICK���~�X������
/*=====================================================================
�Ǐ������֐�
=====================================================================*/
GIMMICK_HEADER *GetWallMemory(FIELD_CHIP* pChip, Vec3* pos, GIMICKWALL_QTE Type)
{
	for (int i = 0; i < NUM_POOL; i++)
	{// ���g�p�ӏ���{��

		float Roty = pChip->Dir*(D3DX_PI / 2);			// ��]���̌���

		if (g_aGimmick[i].Head.isActive == true)
		{
			continue;
		}

		// �[���N���A
		ZeroMemory(&g_aGimmick[i], sizeof(GIMMICK_WALL));

		g_aGimmick[i].Head.isActive = true;
		g_aGimmick[i].Head.Type		= GTYPE_WALL;
		g_aGimmick[i].Head.pFunc	= &g_Func;

		if (Type >= MAX_GIMICKWALLQTE || Type < 0)
		{// �����̃`�F�b�N
			Type = (GIMICKWALL_QTE)(rand() % MAX_GIMICKWALLQTE);
		}

		// z���݈̂������g�p
		g_aGimmick[i].Pos = *pos;
		g_aGimmick[i].Pos.y = 0.0f;
		g_aGimmick[i].Pos.x = 0.0f;

		g_aGimmick[i].State = WSTATE_WAIT;
		g_aGimmick[i].Type = Type;
		
		// �ʒu���烏�[���h���W�����߂�
		D3DXVec3TransformCoord(&g_aGimmick[i].WldPos, &g_aGimmick[i].Pos, &pChip->WldMat);
		GetMatrix(&g_aGimmick[i].WldMat, &g_aGimmick[i].WldPos, &Vec3(0, Roty, 0));

		return (GIMMICK_HEADER *)&g_aGimmick[i];
	}

	return NULL;	// �擾�ł��Ȃ��ꍇ
}

/*=====================================================================
�ǍX�V�֐�
=====================================================================*/
void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos)
{
	var(pData, pHead);

	if (pData->State == WSTATE_WAIT && 
		pData->Pos.z - pPos->z < RANGE_INPUTSTART)
	{// ���͎�t����
		DWORD bitIpt = 0ul;								// �g���K�[���ꂽ���̂��i�[�����r�b�g��
		DWORD bitType = 0x00000001 << pData->Type;		// �������ׂ��r�b�g�𗧂Ă��r�b�g��

		// �{�^�������ꂽ���̃`�F�b�N
		if (GetKeyboardTrigger(DIK_1)|| IsButtonTriggered(0, BUTTON_A))
		{
			bitIpt |= 0x00000001 << GQTE_X;
		}

		if (GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_B))
		{
			bitIpt |= 0x00000001 << GQTE_Y;
		}

		//if (GetKeyboardTrigger(DIK_3))
		//{
		//	bitIpt |= 0x00000001 << GQTE_A;
		//}
		//
		//if (GetKeyboardTrigger(DIK_4))
		//{
		//	bitIpt |= 0x00000001 << GQTE_B;
		//}


		if (bitIpt)
		{// ���Âꂩ�̃{�^���������ꂽ�ꍇ�̏���

			if (bitIpt & bitType)
			{// �w��r�b�g��������Ă���
				pData->State = WSTATE_SUCCESSQTE;
				pData->lenZSuc = pData->Pos.z - WALLSIZE_Z - pPos->z;

				pData->isSoundPlaying = true;

				// �Đ�
				MySoundSetVolume(g_seWalldowm, MIN_DOWNVOLUME);
				MySoundPlayOnce(g_seWalldowm);
				SetPlayerSE(PSE_WALLATTACK);
			}
			else
			{// �w��r�b�g�ȊO��������Ă���
				pData->State = WSTATE_FAILEDQTE;
				MySoundPlayOnce(g_seWallFailed);
			}
		}
		else if (pData->Pos.z - pPos->z < RANGE_INPUTEND)
		{// ���͏I���@�ւɓ������ꍇ�����s����
			pData->State = WSTATE_FAILEDQTE;
			MySoundPlayOnce(g_seWallFailed);
		}

	}
	else if (pData->State == WSTATE_SUCCESSQTE)
	{// ���͐������o����

		float posY_rate = 1.f - ((pData->Pos.z - WALLSIZE_Z - pPos->z) / pData->lenZSuc);
		
		pData->WldMat.m[3][1] = -FIELDROAD_X * posY_rate;
		//Vec3 vec =  *GetPlayerPos()- Vec3(pData->WldMat.m[3]);
		//D3DXVec3Normalize(&vec, &vec);
		//pData->WldMat.m[3][0] = vec.x;
		//pData->WldMat.m[3][1] = vec.y;
		//pData->WldMat.m[3][2] = vec.z;

		if (posY_rate > 1.0f)
		{// ������؂����特��������

			float volume_se = MySoundGetVolume(g_seWalldowm);

			MySoundSetVolume(g_seWalldowm, volume_se - DOWN_VOLUME);
		}
		else
		{
			// SE�̉��ʒ���
			float volume_se = MIN_DOWNVOLUME + (MAX_DOWNVOLUME - MIN_DOWNVOLUME)*posY_rate;

			MySoundSetVolume(g_seWalldowm, volume_se);

			if (GetCount() % 5 == 0)
			{
				// �G�t�F�N�g���o��
				Vec3 pos = pData->WldPos;
		
				pos.x += ((rand() % 1000 - 500) / 1000.0f ) * WALLSIZE_Z*4;
				pos.z += ((rand() % 1000 - 500) / 1000.0f ) * WALLSIZE_Z*4;

				SetEffect(pos, EFFECT_SIZE, EFFECT_SIZE);
			}

		}

		if (GetCount() % 2 == 0)
		{// �w��t���[�����ɂ��炮�珈��
			pData->WldMat.m[3][0] = pData->WldPos.x + (rand() % 1000 - 500) / 400.0f;
			pData->WldMat.m[3][2] = pData->WldPos.z + (rand() % 1000 - 500) / 400.0f;
		}

	}
	else if (pData->State == WSTATE_FAILEDQTE && pData->Pos.z - pPos->z < WALLSIZE_Z / 2)
	{// GAMEOVER����
		GameTackle1End();
	}
}

/*=====================================================================
�Ǖ`��֐�
=====================================================================*/
void DrawGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);
	D3DDEVICE;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, g_tex[pData->Type]);

	g_meshWall->DrawSubset(0);
}

/*=====================================================================
�ǏI���֐�
=====================================================================*/
void EndGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);

	if (pData->isSoundPlaying == true)
	{// �ǂ��󂵂��I�u�W�F�N�g�̏ꍇ�͉����~
	//	MySoundStop(g_seWalldowm);
	}
}

/*=====================================================================
�Ǐ������֐�
=====================================================================*/
void InitFieldGimmickWall()
{
	D3DDEVICE;

	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));

	g_meshWall = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_X, WALLSIZE_Z),
		&Vec3(0, FIELDROAD_X / 2, 0));

	int id = MessageBox(NULL, "�e�N�X�`�����p�b�h�p�ɕύX����H", "(�L�E�ցE)", MB_YESNOCANCEL | MB_ICONQUESTION);

	if (id == IDYES)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall_TRY.png", &g_tex[0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall_SQA.png", &g_tex[1]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall1.png", &g_tex[0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall2.png", &g_tex[1]);
	}

	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall3.png", &g_tex[2]);
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall4.png", &g_tex[3]);

	g_seWalldowm = MySoundCreate("data/SE/WallDown.wav");
	g_seWallFailed = MySoundCreate("data/SE/WallFailed.wav");
	MySoundSetVolume(g_seWallFailed, 3.0f);
}

/*=====================================================================
�ǏI�����֐�
=====================================================================*/
void UninitFieldGimmickWall()
{
	SAFE_RELEASE(g_meshWall);

	for (int i = 0; i < MAX_GIMMICKTYPE; i++)
	{
		SAFE_RELEASE(g_tex[i]);
	}

	MySoundDelete(&g_seWalldowm);
	MySoundDelete(&g_seWallFailed);
}

/*=====================================================================
�ǃQ�[���ꎞ�I���֐�
=====================================================================*/
void AllEndFieldGimmickWall()
{
	for (int i = 0; i < NUM_POOL; i++)
	{
		if (g_aGimmick[i].Head.isActive == true)
		{
			EndGimmick((GIMMICK_HEADER*)&g_aGimmick[i]);
		}
	}
}

/*=====================================================================
�ǃQ�[�����Z�b�g�֐�
=====================================================================*/
void ResetFieldGimmickWall()
{
	// �[���N���A
	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));
}