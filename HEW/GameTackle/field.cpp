/**********************************************************************
[[field�v���O����(field.cpp)]]
	��ҁF���c�@�^�K

	�t�B�[���h�S�ʂɊւ���v���O����
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "field.h"
#include "player.h"

#include "Field/road.h"
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

CHIP_ID GetFieldChipID(Vec3* pos);

FIELD_CHIP* SearchChipID(CHIP_ID id);
FIELD_CHIP* GetChipMemory();

void SetOnFieldWk(FIELD_CHIP* pData);		// g_OnField�̃X�e�[�^�X��ݒu
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static FIELD_CHIP		g_Field[MAX_FIELD];
		
static struct {
	FIELD_CHIP* pChip;		// �v���C���[��ɏ���Ă���Ǝv����CHIP�A�h���X
	Matrix		InvMat;		// ���[���h�s��̋t�s��
}g_OnField;		// �t�B�[���h���[�N

static FIELD_DIRECTION	g_fdir;			// ���݂̕���

/*=====================================================================
�����֐�
	��������֐�
	�߂�l : void
	���� : void
=====================================================================*/
void SetField(short x,short z, FIELD_TYPE type, FIELD_DIRECTION fdirection)
{
	FIELD_CHIP* keep_pt = NULL;

	keep_pt = GetChipMemory();
	if (keep_pt == NULL)
	{// �m�ۂł��Ȃ��ꍇ
		return;
	}

	if (fdirection == g_fdir)
	{	// ����
		keep_pt->State = FSTATE_READY;
	}
	else
	{	// �ʂ̕���
		keep_pt->State = FSTATE_CURVE;
	}

	// ���l���
	keep_pt->ID.vec2.x = x;
	keep_pt->ID.vec2.z = z;
	keep_pt->Type = type;

	GetMatrix(&keep_pt->WldMat,											// ���[���h�s������߂�
		&Vec3((FIELDCHIP_WIDTH / 2) + (FIELDCHIP_WIDTH * x), 0, (FIELDCHIP_HEIGHT / 2) + (FIELDCHIP_HEIGHT * z)),
		&Vec3(0, (D3DX_PI / 2)*(int)fdirection, 0));


	D3DXMatrixInverse(&keep_pt->InvWldMat, NULL, &keep_pt->WldMat);		// ��L�t�s��

	/*�����Ŋ֐��Q�ւ̃A�h���X����w�菈��*/
	keep_pt->pFunc = GetFieldRoadFunc();

}

void UpdateField()
{

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
	//PLAYER*		player = GetPlayer();
	CHIP_ID		id;
	bool		ans;

	if (GetKeyboardTrigger(DIK_F5))
	{
		*GetPlayerOld_Pos() = *GetPlayerPos();
	}

	if (GetPlayerPos()->y > 0.0f)
	{
		PrintDebugProc("�v���C���[���V��");
		return false;
	}

	id = GetFieldChipID(GetPlayerPos());		// �v���C���[�̂���`�����NID�Z�o

	if (id.bit != g_OnField.pChip->ID.bit)
	{// �ۊǕϐ��Ƃ�id���Ⴄ�ꍇ
		FIELD_CHIP* keep_pt;

		keep_pt = SearchChipID(id);

		if (keep_pt == NULL)
		{// �����Ƀq�b�g���Ȃ��ꍇ�͂����ŋA�҂���

			PrintDebugProc("[ERROR]�v���C���[�Ɋ�������`�����N�����݂��܂���(ID:%d-%d)", id.vec2.x, id.vec2.z);
			return true;
		}

		SetOnFieldWk(keep_pt);
	}

	// �v���C���[(���Ɛ̂̍��W)��CHIP���[�J�����W�ɕϊ�����
	D3DXVec3TransformCoord(GetPlayerPos(), GetPlayerPos(), &g_OnField.pChip->InvWldMat);
	D3DXVec3TransformCoord(GetPlayerOld_Pos(), GetPlayerOld_Pos(), &g_OnField.pChip->InvWldMat);

	// �����蔻��
	ans = g_OnField.pChip->pFunc->CheckHit(g_OnField.pChip, GetPlayerPos(), GetPlayerOld_Pos());

#ifdef _DEBUG
	PrintDebugProc("[debug:field_chip]CHIP���W %vec3", *GetPlayerPos());
#endif

	// �v���C���[(���Ɛ̂̍��W)�����[���h���W�ɕϊ�����
	D3DXVec3TransformCoord(GetPlayerPos(), GetPlayerPos(), &g_OnField.pChip->WldMat);
	D3DXVec3TransformCoord(GetPlayerOld_Pos(), GetPlayerOld_Pos(), &g_OnField.pChip->WldMat);


#ifdef _DEBUG
	if (ans == false)
	{
		PrintDebugProc("[ERROR]�v���C���[�����̊O�ɂ��܂��I������I");
	}
#endif
	return ans;
}

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
	}
}

void InitField()
{
	InitFieldRoad();

}

void UninitField()
{

}

void ResetField()
{
	CHIP_ID id;

	for (int i = 0; i < MAX_FIELD; i++)
	{
		// ���g�p���
		g_Field[i].State = FSTATE_NONE;
	}

	ZeroMemory(&g_OnField, sizeof(g_OnField));

	// 0,0��onField
	SetField(0, 0, FTYPE_ROAD, FDIRECTION_0ZP);
	id.vec2.x = 0;
	id.vec2.z = 0;
	SetOnFieldWk(SearchChipID(id));

	// ��������e�X�g
	SetField(0, 1, FTYPE_ROAD, FDIRECTION_1XP);
	// ��������e�X�g
	SetField(0, 2, FTYPE_ROAD, FDIRECTION_2ZM);
	// ��������e�X�g
	SetField(0, 3, FTYPE_ROAD, FDIRECTION_3XM);


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

FIELD_CHIP* GetChipMemory()
{
	// ����
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State != FSTATE_NONE)
		{// �g�p���̂̓X�L�b�v
			continue;
		}

		return &g_Field[i];
	}

	MessageBox(0, 0, 0, 0);

	// ���ׂĎg�p���Ă����ꍇ��null��Ԃ�
	return NULL;
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