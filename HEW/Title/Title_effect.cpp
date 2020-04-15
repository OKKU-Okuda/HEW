/**********************************************************************
[[�^�C�g���G�t�F�N�g�v���O����(Title_effect.cpp)]]
	��ҁF���c�@�^�K

	�^�C�g���̃G�t�F�N�g�Ɋւ���v���O����
***********************************************************************/
#include "Title_effect.h"

#include "../Core/MyList.h"
#include "../Core/directx_Helper3D.h"

#include "../Core/camera.h"			// �r����
#include "../Core/debugproc.h"			// �f�o�b�O�p
#include "../Core/input.h"				// ����
#include "../Core/fade.h"

#include "../Phase/Phase_Title.h"		// �F�擾�֐��p
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define BOX_SIZEXYZ	(20.0f)
#define NUM_ZANZO	(30)
#define SCL_SUB		(1.0f / NUM_ZANZO)
#define EFFECT_RADIUS	(50.0f)
#define EFFECT_POSZ_SET	(3000.0f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// 3D�{�b�N�X�G�t�F�N�g�\����
typedef struct {

	struct {
		Vec3  rot;		// ��]
		Vec3  pos;		// �ʒu
		DWORD  scl_idx;	// �g�p�X�P�[���s��Ԓn
		Matrix matbox;	// �{�b�N�X�̍s��
	}obj[NUM_ZANZO];

	DWORD	idx;		// obj�̈�ԍŐV�G�t�F�N�g�Ԓn
	Vec3    addrot;		// ���t���[�����Z�����]��
	Vec2	posXY;		// xy�̈ʒu
	float   spd;		// �J�����Ɍ������Ă��鑬�x
	float	radius;		// ���S������̔��a
	Color   color;		// �{�b�N�X�̐F
}TITLE_3DEFFECT;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
static void Title3DRightRot();
static void Title3DLeftRot();
inline Matrix* BrendMatrix(Matrix* pOutMat, Matrix* pInMat, TITLE_3DEFFECT* pEffect, DWORD idx);
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Matrix		g_SclMatrix[NUM_ZANZO];		// �X�P�[���s��̎��O���Z�i�[�p
static MyList		g_list3DEffect;				// 3D�{�b�N�X�G�t�F�N�g�̃��X�g	
static Model		g_modelbox;					// �{�b�N�X�̃��b�V��

static struct {
	float		addrot;				// ���t���[����]��
	float		spdrot;				// ��]�����x
	NormalFunc	Update;				// ��]�ʒ����֐�
}g_rot;											// ��]���[�N

/*=====================================================================
3D�G�t�F�N�g��]�֐�
�߂�l�Fvoid
�����Fbool :true:�E��]
=====================================================================*/
void SetTitle3DRot(bool isRight)
{
	if (g_rot.Update != NoFunction)
	{
		return;
	}

	if (isRight)
	{
		g_rot.Update = Title3DRightRot;
	}
	else
	{
		g_rot.Update = Title3DLeftRot;
	}

	g_rot.spdrot = D3DX_PI / 2;
}

void Title3DRightRot()
{
	g_rot.spdrot += 0.06f;

	g_rot.addrot = cosf(g_rot.spdrot)*0.05f;

	if (g_rot.spdrot >= D3DX_PI * 1.5f)
	{
		g_rot.Update = NoFunction;
		g_rot.addrot = 0.0f;
	}
}

void Title3DLeftRot()
{
	g_rot.spdrot -= 0.06f;

	g_rot.addrot = cosf(g_rot.spdrot)*0.05f;

	if (g_rot.spdrot <= -D3DX_PI / 2)
	{
		g_rot.Update = NoFunction;
		g_rot.addrot = 0.0f;
	}

}

/*=====================================================================
3D�G�t�F�N�g�ݒu�֐�(�ȈՔŁj
�@�g���Ŋ֐��̈����ɂ�����x���������������w�肵�Đݒu���܂��B
�߂�l�Fvoid
=====================================================================*/
void SetTitle3DEffect()
{
	// �g���֐��̈����Ɠ������̂�錾
	float len = 300;
	float agl = 0;
	float spd = 30;
	Color col = *GetTitleBottonColor();
	Vec3 paddrot(0, 0, 0);

	//len += rand() % 5;
	agl += (rand() % 1000 / 1000.0f) * (2 * D3DX_PI);
	//spd += (rand() % 1000 / 1000.0f) * (2.0f);
	paddrot.x = (rand() % 1000 / 8000.0f);
	paddrot.y = (rand() % 1000 / 8000.0f);
	paddrot.z = (rand() % 1000 / 8000.0f);

	// �F�̗����w��(�n��)
	col.b += (rand() % 3000 / 5000.0f) - 0.3f;
	col.r += (rand() % 3000 / 5000.0f) - 0.3f;
	col.g += (rand() % 3000 / 5000.0f) - 0.3f;

	SetTitle3DEffectEx(len, agl, spd, &col, &paddrot);
}

/*=====================================================================
3D�G�t�F�N�g�ݒu�֐�(�g���Łj
�߂�l�Fvoid
�����F
float	len,		���S������̋���
float	agl,		���S���̉�]�p�x
float	spd,		�J�����Ɍ������Ă��鑬�x
Color*	col,		�G�t�F�N�g�̐F
Vec3*	paddrot		���t���[�����ɃG�t�F�N�g���̂���]�������
=====================================================================*/
void SetTitle3DEffectEx(float len, float agl, float spd, Color *col, Vec3* paddrot)
{
	// ���X�g�I�u�W�F�N�g�̊m��
	TITLE_3DEFFECT* new_pt = (TITLE_3DEFFECT*)MyListCreateObjectBottom(g_list3DEffect);
	Vec3 rot(0, 0, 0);
	float posZ = EFFECT_POSZ_SET;

	// ����ӏ��͂��̂܂ܑ��
	new_pt->addrot	= *paddrot;
	new_pt->color	= *col;
	new_pt->spd		= spd;
	new_pt->radius	= len;

	// �~��ɂ���XY�ʒu�̎Z�o
	new_pt->posXY.x = -sinf(agl)*len;
	new_pt->posXY.y = cosf(agl)*len;

	// ��ԍŏ��̃{�b�N�X�ݒu�Ԓn���O�ɂ��邽�߂ɒ�������
	new_pt->idx = NUM_ZANZO - 1;

	// �c�������f�[�^���Z�o����
	for (int i = 0; i < NUM_ZANZO; i++, posZ -= spd, rot += *paddrot)
	{
		new_pt->obj[i].pos.x		= new_pt->posXY.x;
		new_pt->obj[i].pos.y		= new_pt->posXY.y;
		new_pt->obj[i].pos.z		= posZ;

		new_pt->obj[i].scl_idx = (NUM_ZANZO - 1) - i;
		new_pt->obj[i].rot		= rot;

		// �{�b�N�X�̍s��(�X�P�[�������������j
		GetMatrix(&new_pt->obj[i].matbox, &Vec3(0, 0, 0),&new_pt->obj[i].rot);
	}
}


/*=====================================================================
�^�C�g���G�t�F�N�g�X�V�֐�
�����Ffloat  �G�t�F�N�g����]������
=====================================================================*/
void UpdateTitleEffect()
{
	TITLE_3DEFFECT* work_pt = NULL;

	PrintDebugProc("��]�F%f", g_rot.addrot);

	SetTitle3DEffect();
	SetTitle3DEffect();

	g_rot.Update();

	// �G�t�F�N�g�̏���
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect,(void**)&work_pt))
	{
		DWORD keep_idx = work_pt->idx;	// ��ԍŐV�̔Ԓn���L�[�v

		// �S�v�f�����X�P�[���l������������
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			work_pt->obj[i].scl_idx++;
		}

		work_pt->idx = (work_pt->idx + 1) % NUM_ZANZO;	// �X�V�Ԓn�̕ύX

		if (work_pt->obj[work_pt->idx].pos.z < GetCamera()->pos.z)
		{	// �Ō����z�l�����_��Z�l������O�ɂ���ꍇ�͍폜
			MyListDeleteObject(g_list3DEffect, (void**)&work_pt);
			continue;
		}

		if (g_rot.addrot != 0.0f)
		{	// ��]������
			Vec2 keep_pos = work_pt->posXY;

			work_pt->posXY.x = (cosf(g_rot.addrot)*keep_pos.x) - (sinf(g_rot.addrot)*keep_pos.y);
			work_pt->posXY.y = (sinf(g_rot.addrot)*keep_pos.x) + (cosf(g_rot.addrot)*keep_pos.y);
		}

		
		work_pt->obj[work_pt->idx].rot		= work_pt->obj[keep_idx].rot + work_pt->addrot;
		work_pt->obj[work_pt->idx].scl_idx	= 0;

		work_pt->obj[work_pt->idx].pos.x	= work_pt->posXY.x;
		work_pt->obj[work_pt->idx].pos.y	= work_pt->posXY.y;
		work_pt->obj[work_pt->idx].pos.z	= work_pt->obj[keep_idx].pos.z - work_pt->spd;

		// �{�b�N�X�̍s��(�X�P�[�������������j
		GetMatrix(&work_pt->obj[work_pt->idx].matbox, &Vec3(0, 0, 0),
			&work_pt->obj[work_pt->idx].rot);
	}

	PrintDebugProc("���G�t�F�N�g��:%d * %d = %d", g_list3DEffect->numObj, NUM_ZANZO, g_list3DEffect->numObj*NUM_ZANZO);
}

/*=====================================================================
�^�C�g���G�t�F�N�g�`��֐�
=====================================================================*/
void DrawTitleEffect()
{
	DWORD			d3drs_fog, d3drs_mode, d3drs_start, d3drs_end, d3drs_col;
	D3DMATERIAL9	matDef;
	TITLE_3DEFFECT* work_pt = NULL;
	Matrix			Mat;				// ���Z�ۊǗp�s��
	D3DDEVICE;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// ���݂̃t�H�O��Ԃ��擾
	pDevice->GetRenderState(D3DRS_FOGENABLE,	&d3drs_fog);
	pDevice->GetRenderState(D3DRS_FOGCOLOR,		&d3drs_col);
	pDevice->GetRenderState(D3DRS_FOGTABLEMODE, &d3drs_mode);
	pDevice->GetRenderState(D3DRS_FOGSTART,		&d3drs_start);
	pDevice->GetRenderState(D3DRS_FOGEND,		&d3drs_end);

	// �t�H�O��Ԃ̐ݒ�
	const float End		= EFFECT_POSZ_SET - (NUM_ZANZO * 30);
	const float Start	= End / 3.0f;
	pDevice->SetRenderState(D3DRS_FOGENABLE,	TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR,		Color(0, 0, 0, 1.0f));
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART,		*(DWORD*)&Start);
	pDevice->SetRenderState(D3DRS_FOGEND,		*(DWORD*)&End);


	// �G�t�F�N�g�̏���
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect, (void**)&work_pt))
	{
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			// �F�K�p
			g_modelbox->pMaterial->MatD3D.Diffuse = work_pt->color;

			// �{�b�N�X�̕`��
			DrawModelWithOtherMatrix(g_modelbox, BrendMatrix(&Mat, &work_pt->obj[i].matbox, work_pt, i));
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �t�H�O��Ԃ�߂�
	pDevice->SetRenderState(D3DRS_FOGENABLE,	d3drs_fog);
	pDevice->SetRenderState(D3DRS_FOGCOLOR,		d3drs_col);
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, d3drs_mode);
	pDevice->SetRenderState(D3DRS_FOGSTART,		d3drs_start);
	pDevice->SetRenderState(D3DRS_FOGEND,		d3drs_end);

}

/*=====================================================================
�^�C�g���G�t�F�N�g�������֐�
=====================================================================*/
void InitTitleEffect(bool isFirstInit)
{
	if (isFirstInit == true)
	{
		D3DDEVICE;
		float		scl = 1.0f;

		// ���X�g�̍쐬
		g_list3DEffect = MyListCreate(sizeof(TITLE_3DEFFECT));

		// �{�b�N�X�̍쐬
		g_modelbox = CreateModel("data/MODEL/box.x");

		// �X�P�[���s��̎��O���Z
		for (int i = 0; i < NUM_ZANZO; i++, scl -= SCL_SUB)
		{
			GetMatrix(&g_SclMatrix[i], &Vec3(0, 0, 0), &Vec3(0, 0, 0), &Vec3(scl, scl, scl));
		}
	}

	g_rot.addrot = 0.0f;
	g_rot.Update = NoFunction;
}

/*=====================================================================
�^�C�g���G�t�F�N�g�I�����֐�
=====================================================================*/
void UninitTitleEffect(bool isLastUninit)
{

	MyListDeleteObjectAll(g_list3DEffect);	// �G�t�F�N�g�S����

	if (isLastUninit == true)
	{
		// �f�[�^�̊J��
		DeleteModel(&g_modelbox);
		MyListDelete(&g_list3DEffect);
	}
}

/*=====================================================================
�s�񍇐��֐�(cpp_func)
�߂�l�F��1�����Ɠ���
�����F
Matrix* pOutMat,			�F���Z�p�s��
Matrix* pInMat,				�F�퍇���p�s��
TITLE_3DEFFECT* pEffect,	�F�����v�f���i�[�����G�t�F�N�g�\����
DWORD idx					�F��L�����̎c��Ԓn
=====================================================================*/
Matrix* BrendMatrix(Matrix* pOutMat, Matrix* pInMat, TITLE_3DEFFECT* pEffect, DWORD idx)
{
	*pOutMat = *pInMat * g_SclMatrix[pEffect->obj[idx].scl_idx];		// �X�P�[���s���������

	// �I�t�Z�b�g���X�P�[�����O�����̂œ���Ȃ���
	pOutMat->_41 = pEffect->obj[idx].pos.x;
	pOutMat->_42 = pEffect->obj[idx].pos.y;
	pOutMat->_43 = pEffect->obj[idx].pos.z;

	return pOutMat;
}