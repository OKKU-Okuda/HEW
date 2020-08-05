/**********************************************************************
[[GIMMICK�̃X���C�h�v���O����(Gimmick/Slide.cpp)]]
	��ҁF���c�@�^�K

	�X���C�h��4�̃L�[�ŉ󂷃v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Phase/Phase_GameTackle1.h"

#include "../field.h"
#include "Slide.h"
#include "Gimmick.h"

#include "../effect.h"
#include "../player.h"
#include "../bonusscore.h"
#include "../BonusAdd.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define NUM_POOL		(30)			// �ۊǗ̈搔
#define Wk				GIMMICK_SLIDE*		// �I�u�W�F�N�g���[�N�|�C���^
#define var(name,arg)	Wk name = (Wk)arg;	// �e�\���̃A�h���X�̕ϊ�ϸ�


#define SIZE_YZ		(20.0f)			// �ۑ��̑傫��

#define IS_USE_XMODEL		(true)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// �X���C�hGIMMICK��ԗ�
enum SLIDE_STATE {
	WSTATE_WAIT,
	WSTATE_FAILEDQTE,
	WSTATE_SUCCESSQTE,
};

// �X���C�hGIMMICK�\����
typedef struct {
	GIMMICK_HEADER	Head;			// �w�b�_

	Matrix			WldMat;			// �X���C�h�̃��[���h�}�g���b�N�X
	Vec3			Pos;			// 3�������W(�t�B�[���h(chip)���W)
	Vec3			WldPos;			// ��L���[���h���W
	//Vec3			Rot;			// ��]
	bool			isFirstHit;		// �ŏ��ɂԂ��������ǂ���

}GIMMICK_SLIDE;

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
static GIMMICK_SLIDE g_aGimmick[NUM_POOL];

#if IS_USE_XMODEL == true
static Model		g_modelSlide;
#else
static Mesh			g_meshSlide;	
static Texture		g_tex;
#endif
/*=====================================================================
�X���C�h�������֐�
=====================================================================*/
GIMMICK_HEADER *GetSlideMemory(FIELD_CHIP* pChip, Vec3* pos)
{
	for (int i = 0; i < NUM_POOL; i++)
	{// ���g�p�ӏ���{��

		float Roty = pChip->Dir*(D3DX_PI / 2);			// ��]���̌���

		if (g_aGimmick[i].Head.isActive == true)
		{
			continue;
		}

		// �[���N���A
		ZeroMemory(&g_aGimmick[i], sizeof(GIMMICK_SLIDE));

		g_aGimmick[i].Head.isActive = true;
		g_aGimmick[i].Head.Type		= GTYPE_WALL;
		g_aGimmick[i].Head.pFunc	= &g_Func;


		// z���݈̂������g�p
		g_aGimmick[i].Pos = *pos;
		g_aGimmick[i].Pos.y = 0.0f;
		g_aGimmick[i].Pos.x = 0.0f;

		g_aGimmick[i].isFirstHit = false;
		// �ʒu���烏�[���h���W�����߂�
		D3DXVec3TransformCoord(&g_aGimmick[i].WldPos, &g_aGimmick[i].Pos, &pChip->WldMat);
		GetMatrix(&g_aGimmick[i].WldMat, &g_aGimmick[i].WldPos, &Vec3(0, Roty, 0));

		return (GIMMICK_HEADER *)&g_aGimmick[i];
	}

	return NULL;	// �擾�ł��Ȃ��ꍇ
}

/*=====================================================================
�X���C�h�X�V�֐�
=====================================================================*/
void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos)
{
	var(pData, pHead);

	if (pData->Pos.z - pPos->z < SIZE_YZ / 2 &&
		pData->Pos.z - pPos->z > -SIZE_YZ / 2 )
	{// �ۑ��ƃv���C���[�����W��Փ�

		if (GetPlayer()->anim_use != PLAYER_SLIDING &&
			GetPlayer()->anim_use != PLAYER_JUMPING)
		{
			GameTackle1End();
			pData->isFirstHit = true;
			return;
		}

		if (pData->isFirstHit == false)
		{
			if (GetPlayer()->anim_use == PLAYER_SLIDING)
			{// �X���C�f�B���O�ŏ����̏ꍇ�̓{�[�i�X

				AddBonusScore(BST_SNEAK);

				if (GetSlideCnt() < SLIDING_CNT*0.2f)
				{
					AddBonusScore(BST_JUSTINTARACT);
				}
			}
			else if (GetPlayer()->anim_use == PLAYER_JUMPING &&
				GetPlayer()->jump_spped > JUMP_HEIGHT*0.1f)
			{// �W�����v�̂��Ⴗ�Ɓ@�C���^���N�g���ʏ���
				AddBonusScore(BST_JUSTINTARACT);
			}

			AddGimmickPassCount();
		}
		pData->isFirstHit = true;

	}
}

/*=====================================================================
�X���C�h�`��֐�
=====================================================================*/
void DrawGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);
	D3DDEVICE;
	DWORD is_lighting = false;

	pDevice->GetRenderState(D3DRS_LIGHTING, &is_lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#if IS_USE_XMODEL == true
	DrawModelWithOtherMatrix(g_modelSlide, &pData->WldMat);
#else
	D3DDEVICE;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, g_tex);

	g_meshSlide->DrawSubset(0);
#endif

	pDevice->SetRenderState(D3DRS_LIGHTING, is_lighting);

}

/*=====================================================================
�X���C�h�I���֐�
=====================================================================*/
void EndGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);

}

/*=====================================================================
�X���C�h�������֐�
=====================================================================*/
void InitFieldGimmickSlide()
{
	D3DDEVICE;

	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));

#if IS_USE_XMODEL == true
	g_modelSlide = CreateModel("data/MODEL/pole.x");
#else
	g_meshSlide = Create3DBoxMesh(&Vec3(FIELDROAD_X + (ROADWALL_SIZEX * 2), SIZE_YZ, SIZE_YZ),
		&Vec3(0, FIELDROAD_Y + (SIZE_YZ), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_tex);
#endif
}

/*=====================================================================
�X���C�h�I�����֐�
=====================================================================*/
void UninitFieldGimmickSlide()
{
#if IS_USE_XMODEL == true
	DeleteModel(&g_modelSlide);
#else
	SAFE_RELEASE(g_meshSlide);
	SAFE_RELEASE(g_tex);
#endif

}

/*=====================================================================
�X���C�h�Q�[���ꎞ�I���֐�
=====================================================================*/
void AllEndFieldGimmickSlide()
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
�X���C�h�Q�[�����Z�b�g�֐�
=====================================================================*/
void ResetFieldGimmickSlide()
{
	// �[���N���A
	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));
}