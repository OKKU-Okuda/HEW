/**********************************************************************
[[�E���򓹓��v���O����(GameTackle/Field/turnR.cpp)]]
	��ҁF���c�@�^�K

	�E���򓹓��Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../../Phase/Phase_GameTackle1.h"
#include "../field.h"
#include "ResourceManager.h"
#include "../field_control.h"
#include "../player_control.h"
#include "turnR.h"
#include "../UI.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define RANGE_INPUTLR	(FIELDCHIP_WIDTH*1.25f)		// ���S�܂ł�input�ǂݍ��݃G���A

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// 
enum TURNLR_QTETYPE {
	QTE_NOINPUT = 0,
	QTE_LEFT,
	QTE_RIGHT
};

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static bool CheckHitFieldTurnR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldTurnR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldTurnR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldTurnR,UpdateFieldTurnR,DrawFieldTurnR };	// ���Ǝ��̊֐�
static TURNLR_QTETYPE g_QTEState = QTE_NOINPUT;

static Mesh g_meshWallBack = NULL;

//static Mesh g_meshWallLeftFront = NULL;
///static Mesh g_meshWallRightFront = NULL;
//static Mesh g_meshWallLeftCenter = NULL;
//static Mesh g_meshWallRightCenter = NULL;


//static Mesh g_meshFlatFront = NULL;
//static Mesh g_meshFlatLeft = NULL;
//static Mesh g_meshFlatRight = NULL;


/*=====================================================================
���E���򓹓������蔻��֐�
=====================================================================*/
bool CheckHitFieldTurnR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
{
	// �ʒu�̏�ԗ�
	enum POSITION_STATE {
		PSTATE_VOID = 0x0,				// ������
		PSTATE_INVERTICAL = 0x01,		// �c�G���A�̒��ɂ���
		PSTATE_INHORIZONTAL = 0x02,		// ���G���A�̒��ɂ���
		PSTATE_INCROSSAREA = PSTATE_INVERTICAL | PSTATE_INHORIZONTAL,		// �c�Ɖ��̃G���A�ɏd�Ȃ��Ă�Ƃ��ɂ���
	};

	DWORD PosState = PSTATE_VOID;		// �ʒu�̏��

	// �G���A��Ԃ̔��ʏ���
	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2)+ PLAYER_FIELDSIZE_R &&
		pPastPos->z < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// �c�G���A�ɂ���
		PosState |= PSTATE_INVERTICAL;
	}

	if (pPastPos->z > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->z < (FIELDROAD_X / 2)+PLAYER_FIELDSIZE_R&&
		pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R)
	{	// ���G���A�ɂ���
		PosState |= PSTATE_INHORIZONTAL;
	}


	// ��ԕʏ���
	if (PosState == PSTATE_INVERTICAL)
	{	
		PrintDebugProc("tate");
		SAFE_NUMBER(pPos->x, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	else if (PosState == PSTATE_INHORIZONTAL)
	{
		PrintDebugProc("yoko");
		SAFE_NUMBER(pPos->z, -FIELDROAD_X / 2, FIELDROAD_X / 2);
		return true;
	}

	else if (PosState == PSTATE_INCROSSAREA)
	{
		PrintDebugProc("cross");
		if (pPos->z > (FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R)
		{	// �N���X�̏ꍇ�͕ǂ�艜�ɐi�����Ƃ���ƃQ�[���I�[�o�[
			GameTackle1End();
			//pPos->z = FIELDROAD_X / 2- PLAYER_FIELDSIZE_R;
		}

		return true;
	}

	return false;
}

/*=====================================================================
���E���򓹓��X�V�֐�
=====================================================================*/
void UpdateFieldTurnR(FIELD_CHIP* pData, Vec3* pPos)
{
	const float maxIptPosZ = -FIELDROAD_X / 2 - PLAYER_FIELDSIZE_R;		// ���͎�t�̂��ő�l
	const float minIptPosZ = maxIptPosZ - RANGE_INPUTLR;				//�@����ŏ��l

	const float maxRotPosZ = FIELDROAD_X / 5;						// ��]�̂��ő�l
	const float minRotPosZ = -FIELDROAD_X / 5;						// ����ŏ��l


	if (pPos->z <= maxIptPosZ && pPos->z >= minIptPosZ && g_QTEState == QTE_NOINPUT)
	{// �����͏�Ԃō��E�L�[��������CURVE�\��
#ifdef _DEBUG
		PrintDebugProc("[debug_TurnL]Q�L�[�AE�L�[:CURVE");
#endif
		SetUIGuideActive(UD_LEFT, false);
		SetUIGuideActive(UD_RIGHT, true);

		if (GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(0, BUTTON_Y))
		{
			g_QTEState = QTE_LEFT;
			SetUIGuideActive(UD_LEFT, true);
			SetUIGuideActive(UD_RIGHT, false);
			PlayUIGuideSelect();
		}
		else if (GetKeyboardTrigger(DIK_E) || IsButtonTriggered(0, BUTTON_Z))
		{
			g_QTEState = QTE_RIGHT;
			PlayUIGuideSelect();
		}
	}
	else if (g_QTEState != QTE_NOINPUT && pPos->z <= maxRotPosZ && pPos->z >= minRotPosZ)
	{// ��]�G���A�ɓ�������Ɨ\��ʂ�̉�]���s��
		FIELD_CHIP*	pDelChip = NULL;			// CURVE�Ƃ͔��Α��ɂ��铹
		CHIP_ID id_start;// ��������id

		if (g_QTEState == QTE_RIGHT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), 1));

			// ���Α��̓�����������
			pDelChip = SearchChipID(AddFieldID(pData->ID, GetFieldIDVector(AddFieldDirection(GetPlayerDirection(), 2))));
			if (pDelChip != NULL)
			{
				DeleteField(pDelChip);
			}


			id_start = AddFieldID(pData->ID, GetFieldIDVector(GetPlayerDirection()));
			SpawnField(id_start);
		}
		else if (g_QTEState == QTE_LEFT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), -1));
		}

		g_QTEState = QTE_NOINPUT;

		SetUIGuideActive(UD_LEFT, false);
		SetUIGuideActive(UD_RIGHT, false);

	}
}

/*=====================================================================
���E���򓹓��`��֐�
=====================================================================*/
void DrawFieldTurnR(FIELD_CHIP* pData)
{
	D3DDEVICE;


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	GetFieldShareMesh(FMESH_FRONTFLAT)->DrawSubset(0);
	GetFieldShareMesh(FMESH_RIGHTFLAT)->DrawSubset(0);

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_WALL));

	GetFieldShareMesh(FMESH_LEFTFRONTWALL)->DrawSubset(0);
	GetFieldShareMesh(FMESH_RIGHTFRONTWALL)->DrawSubset(0);

	g_meshWallBack->DrawSubset(0);

	GetFieldShareMesh(FMESH_RIGHTCENTERWALL)->DrawSubset(0);


}

/*=====================================================================
���E���򓹓��������֐�
=====================================================================*/
void InitFieldTurnR()
{
	// 
	g_meshWallBack = Create3DBoxMesh(&Vec3((FIELDROAD_X/2) + (FIELDCHIP_WIDTH / 2) + (ROADWALL_SIZEX/2), ROADWALL_SIZEY, ROADWALL_SIZEX),
		&Vec3((FIELDCHIP_WIDTH / 4)-(FIELDROAD_X/4)-(ROADWALL_SIZEX/2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), (FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2)));

}

/*=====================================================================
���E���򓹓��I�����֐�
=====================================================================*/
void UninitFieldTurnR()
{
}

/*=====================================================================
���E���򓹓��Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldTurnRFunc()
{
	return &g_Func;
}