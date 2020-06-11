/**********************************************************************
[[���E���򓹓��v���O����(GameTackle/Field/turnLR.cpp)]]
	��ҁF���c�@�^�K

	���E���򓹓��Ɋւ���v���O����
***********************************************************************/
#include "../../Core/main.h"
#include "../../Core/debugproc.h"
#include "../field.h"
#include "ResourceManager.h"
#include "../field_control.h"
#include "../player_control.h"
#include "turnLR.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#define RANGE_INPUTLR	(FIELDCHIP_WIDTH*0.75f)		// ���S�܂ł�input�ǂݍ��݃G���A

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
static bool CheckHitFieldTurnLR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos);
static void UpdateFieldTurnLR(FIELD_CHIP* pData, Vec3* pPos);
static void DrawFieldTurnLR(FIELD_CHIP* pData);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

static FIELD_OBJFUNC g_Func = { CheckHitFieldTurnLR,UpdateFieldTurnLR,DrawFieldTurnLR };	// ���Ǝ��̊֐�
static TURNLR_QTETYPE g_QTEState = QTE_NOINPUT;

static Mesh g_meshWallLeftFront = NULL;
static Mesh g_meshWallRightFront = NULL;
static Mesh g_meshWallBack = NULL;
static Mesh g_meshWallLeftCenter = NULL;
static Mesh g_meshWallRightCenter = NULL;


static Mesh g_meshFlatFront = NULL;
static Mesh g_meshFlatLeft = NULL;
static Mesh g_meshFlatRight = NULL;


/*=====================================================================
���E���򓹓������蔻��֐�
=====================================================================*/
bool CheckHitFieldTurnLR(FIELD_CHIP* pData, Vec3* pPos, Vec3* pPastPos)
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
	if (pPastPos->x > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->x < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R &&
		pPastPos->z < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
	{	// �c�G���A�ɂ���
		PosState |= PSTATE_INVERTICAL;
	}

	if (pPastPos->z > -(FIELDROAD_X / 2) - PLAYER_FIELDSIZE_R && pPastPos->z < (FIELDROAD_X / 2) + PLAYER_FIELDSIZE_R)
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
		if (pPos->z > FIELDROAD_X / 2)
		{	// �N���X�̏ꍇ�͕ǂ�艜�ɐi�߂Ȃ������ɂ���
			pPos->z = FIELDROAD_X / 2;
		}

		return true;
	}

	return false;
}

/*=====================================================================
���E���򓹓��X�V�֐�
=====================================================================*/
void UpdateFieldTurnLR(FIELD_CHIP* pData, Vec3* pPos)
{
	const float maxIptPosZ = -FIELDROAD_X / 2 - PLAYER_FIELDSIZE_R;		// ���͎�t�̂��ő�l
	const float minIptPosZ = maxIptPosZ - RANGE_INPUTLR;				//�@����ŏ��l

	const float maxRotPosZ = FIELDROAD_X / 5;						// ��]�̂��ő�l
	const float minRotPosZ = -FIELDROAD_X / 5;						// ����ŏ��l


	if (pPos->z <= maxIptPosZ && pPos->z >= minIptPosZ && g_QTEState == QTE_NOINPUT)
	{// �����͏�Ԃō��E�L�[��������CURVE�\��
#ifdef _DEBUG
		PrintDebugProc("[debug_TurnLR]Q�L�[�AE�L�[:CURVE");
#endif
		if (GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(0, BUTTON_Y))
		{
			g_QTEState = QTE_LEFT;
		}
		else if (GetKeyboardTrigger(DIK_E) || IsButtonTriggered(0, BUTTON_Z))
		{
			g_QTEState = QTE_RIGHT;
		}
	}
	else if (g_QTEState != QTE_NOINPUT && pPos->z <= maxRotPosZ && pPos->z >= minRotPosZ)
	{// ��]�G���A�ɓ�������Ɨ\��ʂ�̉�]���s��
		FIELD_CHIP*	pDelChip = NULL;			// CURVE�Ƃ͔��Α��ɂ��铹
		CHIP_ID id_start;// ��������id

		if (g_QTEState == QTE_LEFT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), -1));
		}
		else if (g_QTEState == QTE_RIGHT)
		{
			SetPlayerDirection(AddFieldDirection(GetPlayerDirection(), 1));
		}

		// ���Α��̓�����������
		pDelChip = SearchChipID(AddFieldID(pData->ID, GetFieldIDVector(AddFieldDirection(GetPlayerDirection(), 2))));
		if (pDelChip != NULL)
		{
			DeleteField(pDelChip);
		}

		g_QTEState = QTE_NOINPUT;

		id_start = AddFieldID(pData->ID, GetFieldIDVector(GetPlayerDirection()));
		SpawnField(id_start);

	}
}

/*=====================================================================
���E���򓹓��`��֐�
=====================================================================*/
void DrawFieldTurnLR(FIELD_CHIP* pData)
{
	D3DDEVICE;

	pDevice->SetTexture(0, GetFieldShareTexture(FTEX_NONE));

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	g_meshWallLeftFront	  ->DrawSubset(0);
	g_meshWallRightFront	 ->DrawSubset(0);

	g_meshWallBack->DrawSubset(0);
	g_meshWallLeftCenter->DrawSubset(0);
	g_meshWallRightCenter->DrawSubset(0);

	g_meshFlatFront->DrawSubset(0);
	g_meshFlatLeft->DrawSubset(0);
	g_meshFlatRight->DrawSubset(0);

}

/*=====================================================================
���E���򓹓��������֐�
=====================================================================*/
void InitFieldTurnLR()
{

	const float sizeZ = (FIELDCHIP_HEIGHT - FIELDROAD_X) / 2;
	const float posZ = FIELDCHIP_CENTER_Z - (sizeZ / 2);

	const float size_flatX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2);
	const float pos_flatX = size_flatX / 2 + (FIELDROAD_X / 2);

	const float size_WallX = FIELDCHIP_CENTER_X - (FIELDROAD_X / 2) - ROADWALL_SIZEX;
	const float pos_WallX = pos_flatX + (ROADWALL_SIZEX / 2);

	g_meshWallLeftFront = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((-FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

	g_meshWallRightFront = Create3DBoxMesh(&Vec3(ROADWALL_SIZEX, ROADWALL_SIZEY, sizeZ),
		&Vec3((FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2), (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -posZ));

	g_meshWallBack = Create3DBoxMesh(&Vec3(FIELDCHIP_WIDTH, ROADWALL_SIZEY, ROADWALL_SIZEX),
		&Vec3(0, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), (FIELDROAD_X / 2) + (ROADWALL_SIZEX / 2)));


	g_meshWallLeftCenter = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
		&Vec3(-pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));

	g_meshWallRightCenter = Create3DBoxMesh(&Vec3(size_WallX, ROADWALL_SIZEY, ROADWALL_SIZEX),
		&Vec3(pos_WallX, (ROADWALL_SIZEY / 2) - (FIELDROAD_Y / 2), -(FIELDROAD_X / 2) - (ROADWALL_SIZEX / 2)));


	g_meshFlatFront = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_Y, sizeZ + FIELDROAD_X) , &Vec3(0, 0, -posZ+ (FIELDROAD_X/2)));
	

	g_meshFlatLeft = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(-pos_flatX, 0, 0));
	g_meshFlatRight = Create3DBoxMesh(&Vec3(size_flatX, FIELDROAD_Y, FIELDROAD_X), &Vec3(pos_flatX, 0, 0));

	// 
}

/*=====================================================================
���E���򓹓��I�����֐�
=====================================================================*/
void UninitFieldTurnLR()
{
}

/*=====================================================================
���E���򓹓��Ǝ��֐��A�h���X�擾�֐�
=====================================================================*/
FIELD_OBJFUNC* GetFieldTurnLRFunc()
{
	return &g_Func;
}