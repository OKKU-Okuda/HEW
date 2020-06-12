/**********************************************************************
[[field�v���O����(field.cpp)]]
	��ҁF���c�@�^�K

	�t�B�[���h�S�ʂɊւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "field.h"
#include "player.h"
#include "player_control.h"
#include "field_control.h"
#include "item.h"
#include "Gimmick/Gimmick.h"

#include "Field/ResourceManager.h"
#include "Field/road.h"
#include "Field/void.h"
#include "Field/cliffR.h"
#include "Field/cliffC.h"
#include "Field/cliffL.h"
#include "Field/jump.h"
#include "Field/turnLR.h"

#include "../Phase/Phase_GameTackle1.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------

#if 0	// c����Ȃ��foreach��������Ǐ����d�������̑O�Ƀ|�C���^�ϐ��錾���Ȃ��Ⴂ���Ȃ�����g��Ȃ�
#define foreach(val,arr)	\
int size = sizeof(arr) / sizeof(arr[0]);	\
val = &arr[0];								\
for(int i = 0; i < size; i++, val++)
#endif

#define RANGE_DELCHIPKEEP			(FIELDCHIP_WIDTH*0.75f)		// �v���C���[����ǂ̒��x���ꂽ���ł̍폜�(box����)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

static CHIP_ID GetFieldChipID(Vec3* pos);						// 3�������[���h���W����ID���Z�o����

static FIELD_CHIP* GetChipMemory();								// ��CHIP��T���ăA�h���X��Ԃ�����

static void SetOnFieldWk(FIELD_CHIP* pData);					// g_OnField�̃X�e�[�^�X��ݒu
static FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type, FIELD_CHIP* pData);// type����Ǝ��֐��\���̃A�h���X����������
static void SwapAllChipState(FIELD_STATE fst_target, FIELD_STATE fst_set);	// ��Ԃ�ʂ̏�Ԃɕϊ�����i���ׂāj

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static FIELD_CHIP		g_Field[MAX_FIELD];
		
static struct {
	FIELD_CHIP* pChip;		// �v���C���[��ɏ���Ă���Ǝv����CHIP�A�h���X

	/*�@�\�ǉ����g�p*/

}g_OnField;		// �t�B�[���h���[�N

//static FIELD_DIRECTION	g_fdir;			// ���݂̕���
//static CHIP_ID			g_latestid;		// ��ԍŋߐݒu�����`�����N
	

/*=====================================================================
�t�B�[���h�X�V�֐�
=====================================================================*/
void UpdateField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// ������ɂ�����̂̂ݍX�V���s��
		if (g_Field[i].State == FSTATE_ONPLAYER ||
			(g_Field[i].State == FSTATE_READY && g_Field[i].Dir == GetPlayerDirection()))
		{
			Vec3 pos_player, keeppos;

			// �v���C���[���[���h->CHIP���[�J��
			D3DXVec3TransformCoord(&pos_player, GetPlayerPos(), &g_Field[i].InvWldMat);

			keeppos = pos_player;

			g_Field[i].pFunc->Update(&g_Field[i], &pos_player);		
			FieldGimmickUpdate(&g_Field[i], &pos_player);			// GIMMICK�̃A�b�v�f�[�g

			if (keeppos != pos_player)
			{// �X�V�֐���Ƀ|�W�V�����ύX���������ꍇ
				// CHIP���[�J��->�v���C���[���[���h
				D3DXVec3TransformCoord(GetPlayerPos(), &pos_player, &g_Field[i].WldMat);
			}
		}

		// �t�B�[���h�폜���f����
		else if (g_Field[i].State == FSTATE_USED)
		{
			Vec3 pos_field(g_Field[i].WldMat.m[3]);		// ���[���h���W�擾

			// �����W�O�Ƀv���C���[���o�����\���ɂ���
			if (fabsf(GetPlayerPos()->x - pos_field.x) >= RANGE_DELCHIPKEEP ||
				fabsf(GetPlayerPos()->z - pos_field.z) >= RANGE_DELCHIPKEEP)
			{
				DeleteField(&g_Field[i]);
			}
		}
	}

#ifdef _DEBUG
	PrintDebugProc("[debug_field]F3:�t�B�[���h�������Ȃ���");
	if (GetKeyboardTrigger(DIK_F3))
	{
		ResetPlayerPos();
		ResetItem();
		ResetField();
	}
#endif
}

/*=====================================================================
�t�B�[���h�`��֐�
=====================================================================*/
void DrawField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == FSTATE_NONE)
		{
			continue;
		}

		// �`��
		g_Field[i].pFunc->Draw(&g_Field[i]);

		// GIMMICK�̕`��
		FieldGimmickDraw(&g_Field[i]);
	}
}

/*=====================================================================
�t�B�[���h�������֐�
=====================================================================*/
void InitField()
{
	InitFieldResource();
	InitFieldVoid();
	InitFieldRoad();
	InitFieldCliffR();
	InitFieldCliffC();
	InitFieldCliffL();
	InitFieldJump();
	InitFieldTurnLR();

	InitFieldGimmick();
}

/*=====================================================================
�t�B�[���h�I�����֐�
=====================================================================*/
void UninitField()
{
	UninitFieldResource();
	UninitFieldVoid();
	UninitFieldRoad();
	UninitFieldCliffR();
	UninitFieldCliffC();
	UninitFieldCliffL();
	UninitFieldJump();
	UninitFieldTurnLR();

	UninitFieldGimmick();
}

/*=====================================================================
�t�B�[���h�Q�[�����Z�b�g�֐��֐�
=====================================================================*/
void ResetField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// ���g�p���
		g_Field[i].State = FSTATE_NONE;
	}

	ZeroMemory(&g_OnField, sizeof(g_OnField));

	EndFieldGimmick();
	ResetFieldGimmick();

	// 0,0��onField
	SetField(GetChipID(0, 0), FTYPE_ROAD, FDIRECTION_0ZP);
	SetOnFieldWk(SearchChipID(GetChipID(0, 0)));

	SpawnField(GetChipID(0, 0));
	// ��������e�X�g
//	SetField(0, 1, FTYPE_ROAD, FDIRECTION_1XP);
	// ��������e�X�g
//	SetField(0, 2, FTYPE_ROAD, FDIRECTION_2ZM);
	// ��������e�X�g
//	SetField(0, 3, FTYPE_ROAD, FDIRECTION_3XM);

}

/*=====================================================================
�t�B�[���h�ݒu�֐�
	�߂�l : FIELD_CHIP* 
	���� : 
	CHIP_ID id	
	FIELD_TYPE type,				:�ݒu�t�B�[���h�^�C�v
	FIELD_DIRECTION fdirection		:����
=====================================================================*/
FIELD_CHIP* SetField(CHIP_ID id, FIELD_TYPE type, FIELD_DIRECTION fdirection)
{
	FIELD_CHIP* keep_pt = NULL;

	keep_pt = SearchChipID(id);

	if (SearchChipID(id) == NULL)
	{// �����ӏ��Ƀ`�����N�����݂��Ȃ��ꍇ

		keep_pt = GetChipMemory();
		if (keep_pt == NULL)
		{// �m�ۂł��Ȃ��ꍇ
			return NULL;
		}
	}

	keep_pt->State = FSTATE_READY;

	// �Z�o
	GetMatrix(&keep_pt->WldMat,											// ���[���h�s������߂�
		&Vec3((FIELDCHIP_WIDTH / 2) + (FIELDCHIP_WIDTH * id.vec2.x), 0, (FIELDCHIP_HEIGHT / 2) + (FIELDCHIP_HEIGHT * id.vec2.z)),
		&Vec3(0, (D3DX_PI / 2)*(int)fdirection, 0));

	D3DXMatrixInverse(&keep_pt->InvWldMat, NULL, &keep_pt->WldMat);		// ��L�t�s��

		// ���l���
	keep_pt->ID = id;
	keep_pt->Dir = fdirection;
	keep_pt->Type = type;
	keep_pt->pFunc = SearchFieldObjFunc(type, keep_pt);

	return keep_pt;
}

/*=====================================================================
�t�B�[���h�폜�֐�
=====================================================================*/
void DeleteField(FIELD_CHIP* pData)
{
	DeleteItemByFieldPtr(pData);		// �A�C�e���̍폜
	DeleteGimmickByFieldPtr(pData);		// �M�~�b�N�̍폜
	pData->State = FSTATE_NONE;
}

/*=====================================================================
�v���C���[�t�B�[���h�㓖���蔻��֐�
	(GIMMICK�����蔻��͕ʓr����)
	���̏�Q���ɂ���ăv���C���[�̈ʒu��ς��܂�

	�߂�l : void
	���� : bool		true:���ƃv���C���[���ݒu�Ă���
=====================================================================*/
bool PlayerCheckHitOnField()
{
	CHIP_ID		id;
	bool		ans;

	if (GetPlayerPos()->y > 0.0f)
	{// ���V���͖������Ŕ��肵�Ȃ�
		return false;
	}

	if (GetPlayerPos()->y >= FIELD_FAILED_Y)
	{// ���ʍ��W��ɂ���ꍇ
		Vec3 pos_pastkeep, pos_pastforfunc;	
		Vec3 pos_keep, pos_forfunc;

		id = GetFieldChipID(GetPlayerPos());		// �v���C���[�̂���`�����NID�Z�o

		if (id.bit != g_OnField.pChip->ID.bit)
		{// �ۊǕϐ��Ƃ�id���Ⴄ�ꍇ
			FIELD_CHIP* keep_pt;

			keep_pt = SearchChipID(id);

			if (keep_pt == NULL)
			{// �����Ƀq�b�g���Ȃ��ꍇ�͂����ŋA�҂���

				PrintDebugProc("[ERROR]�v���C���[�Ɋ�������`�����N�����݂��܂���(ID:%d,%d)", id.vec2.x, id.vec2.z);
				return true;
			}

			SetOnFieldWk(keep_pt);
		}

		// �v���C���[(���Ɛ̂̍��W)��CHIP���[�J�����W�ɕϊ�����
		D3DXVec3TransformCoord(&pos_forfunc, GetPlayerPos(), &g_OnField.pChip->InvWldMat);
		D3DXVec3TransformCoord(&pos_pastforfunc, GetPlayerOld_Pos(), &g_OnField.pChip->InvWldMat);

		pos_pastkeep = pos_pastforfunc;
		pos_keep = pos_forfunc;

		// �����蔻��
		ans = g_OnField.pChip->pFunc->CheckHit(g_OnField.pChip, &pos_forfunc, &pos_pastforfunc);

#ifdef _DEBUG
		PrintDebugProc("[debug:field_chip]CHIP���W %vec3", *GetPlayerPos());
#endif
		// �v���C���[(���Ɛ̂̍��W)�����[���h���W�ɕϊ�����(�ύX�̏ꍇ�̂݁j
		if (pos_keep != pos_forfunc)
		{
			D3DXVec3TransformCoord(GetPlayerPos(), &pos_forfunc, &g_OnField.pChip->WldMat);
		}
		if (pos_pastkeep != pos_pastforfunc)
		{
			D3DXVec3TransformCoord(GetPlayerOld_Pos(), &pos_pastforfunc, &g_OnField.pChip->WldMat);
		}
	}
	else
	{// ������  (�����ɂ���)     
	
		// ���ʒu�Œ�
		GetPlayerPos()->x = GetPlayerOld_Pos()->x;
		GetPlayerPos()->z = GetPlayerOld_Pos()->z;

#ifdef _DEBUG
		PrintDebugProc("[debug:field_chip]y������ɂ�闎���m�菈�� %vec3", *GetPlayerPos());
#endif
		GameTackle1End();

		ans = false;
	}

	return ans;
}

/*=====================================================================
ID�Z�o�擾�֐�
=====================================================================*/
CHIP_ID GetChipID(short x, short z)
{
	CHIP_ID ans;

	ans.vec2.x = x;
	ans.vec2.z = z;
	return ans;
}

/*=====================================================================
�v���C���[�ɐڂ��Ă���t�B�[���h�`�����N�擾�֐�
=====================================================================*/
FIELD_CHIP* GetFieldOnPlayer()
{
	return g_OnField.pChip;
}

/*=====================================================================
��{�x�N�g���擾�֐�
=====================================================================*/
Vec3 GetFieldVector(FIELD_DIRECTION fdir)
{
	Vec3 ans(0, 0, 0);		// �߂�l�Z�o�p
	DWORD bit = fdir;		// �񋓌^�r�b�g���Z

	if (bit & 0x00000001)
	{// �ŉ��ʃr�b�g�ɂP������Ή��ړ�
		ans.x = 1.0f;
	}
	else
	{// 0�ł���Ώc
		ans.z = 1.0f;
	}

	if (bit & 0x00000002)
	{// 2�ȏ�̏ꍇ�͕������]
		ans *= -1.0f;
	}

	return ans;
}

/*=====================================================================
��{�x�N�g���擾�֐�
=====================================================================*/
CHIP_ID GetFieldIDVector(FIELD_DIRECTION fdir)
{
	CHIP_ID ans = GetChipID(0, 0);
	DWORD bit = fdir;		// �񋓌^�r�b�g���Z

	if (bit & 0x00000001)
	{// �ŉ��ʃr�b�g�ɂP������Ή��ړ�
		ans.vec2.x = 1;
	}
	else
	{// 0�ł���Ώc
		ans.vec2.z = 1;
	}

	if (bit & 0x00000002)
	{// 2�ȏ�̏ꍇ�͕������]
		ans.vec2.x *= -1;
		ans.vec2.z *= -1;
	}

	return ans;

}

/*=====================================================================
ID���Z�֐�
=====================================================================*/
CHIP_ID AddFieldID(CHIP_ID id1, CHIP_ID id2)
{
	id1.vec2.x += id2.vec2.x;
	id1.vec2.z += id2.vec2.z;
	return id1;					// ���Z�ꏊ
}

/*=====================================================================
�t�B�[���h�������Z�֐�
=====================================================================*/
FIELD_DIRECTION AddFieldDirection(FIELD_DIRECTION fdir, int add)
{
	int ans = fdir + add;

	if (ans < 0)
	{// ���̐��͐��̐��ɖ߂��Ă���v�Z
		ans += MAX_FIELDDIRECTION * (ans / MAX_FIELDDIRECTION + 1);
	}

	ans %= MAX_FIELDDIRECTION;
	return (FIELD_DIRECTION)ans;
}

/*=====================================================================
CHIP�����֐�
=====================================================================*/
FIELD_CHIP* SearchChipID(CHIP_ID id)
{
	// ����
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == FSTATE_NONE)
		{// ���g�p�̏ꍇ�͌����Ώۂɓ���Ȃ�
			continue;
		}

		if (g_Field[i].ID.bit == id.bit)
		{// ID�̃`�F�b�N
			return &g_Field[i];
		}
	}

	// ����ID�����݂��Ȃ��ꍇ��null��Ԃ�
	return NULL;
}

/*=====================================================================
	cpp���֐�
=====================================================================*/
FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type,FIELD_CHIP* pData)
{
	switch (type)
	{
	case FTYPE_VOID:
		return GetFieldVoidFunc();

	case FTYPE_ROAD:
		AwakeFieldRoad(pData);
		return GetFieldRoadFunc();

	case FTYPE_CLIFFR:
		return GetFieldCliffRFunc();

	case FTYPE_CLIFFC:
		return GetFieldCliffCFunc();

	case FTYPE_CLIFFL:
		return GetFieldCliffLFunc();

	case FTYPE_JUMP:
		return GetFieldJumpFunc();

	case FTYPE_TURNLR:
		return GetFieldTurnLRFunc();

	//case FTYPE_TURNR:
	//	break;
	//case FTYPE_TURNL:
	//	break;
	case MAX_FIELDTYPE:
		break;
	default:
		break;
	}

	// �f�t�H���g�Ƃ��ēޗ���ݒu
	return GetFieldVoidFunc();
}

CHIP_ID GetFieldChipID(Vec3* pos)
{
	CHIP_ID ans;

	// �ʒu���`�����N�T�C�Y�Ŋ����Đ�����
	ans.vec2.x = (short)(pos->x / FIELDCHIP_WIDTH);
	ans.vec2.z = (short)(pos->z / FIELDCHIP_HEIGHT);

	// ���̏ꍇ-0�͂��킢�̂ł��ꂼ��1�}�C�i�X����
	if (pos->x < 0)
	{
		ans.vec2.x--;
	}

	if (pos->z < 0)
	{
		ans.vec2.z--;
	}

	return ans;
}



FIELD_CHIP* GetChipMemory()
{
	// ����
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State != FSTATE_NONE)
		{// �g�p���̂̓X�L�b�v
			continue;
		}

		// �[���N���A
		ZeroMemory(&g_Field[i], sizeof(FIELD_CHIP));

		return &g_Field[i];
	}

	// ���ׂĎg�p���Ă����ꍇ��null��Ԃ�
	return NULL;
}

void SwapAllChipState(FIELD_STATE fst_target,FIELD_STATE fst_set)
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == fst_target)
		{
			g_Field[i].State = fst_set;
		}
	}
}

void SetOnFieldWk(FIELD_CHIP* pData)
{
	if (g_OnField.pChip != NULL)
	{	// �O�̃u���b�N���g�p�ς݂ɂ���

		g_OnField.pChip->State = FSTATE_USED;
	}

	g_OnField.pChip = pData;					
	g_OnField.pChip->State = FSTATE_ONPLAYER;	// ��Ԑݒu
}

