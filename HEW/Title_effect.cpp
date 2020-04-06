/**********************************************************************
[[�^�C�g���G�t�F�N�g�v���O����(Title_effect.cpp)]]
	��ҁF���c�@�^�K

	�^�C�g���̃G�t�F�N�g�Ɋւ���v���O����
***********************************************************************/
#include "Title_effect.h"
#include "Core/MyList.h"
#include "Core/directx_Helper3D.h"

#include "Core/camera.h"			// �r����
//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
#define BOX_SIZEXYZ	(20.0f)
#define NUM_ZANZO	(15)
#define SCL_SUB		(1.0f / NUM_ZANZO)
#define EFFECT_RADIUS	(50.0f)
#define EFFECT_POSZ_SET	(2000.0f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// 3D�{�b�N�X�G�t�F�N�g�\����
typedef struct {

	struct {
		Vec3  rot;		// ��]
		float sclXYZ;	// �傫���ixyz����)
		float posZ;		// ���s
		Matrix matScl;	// �ψق��Ă����X�P�[���s��
		Matrix matbox;	// �{�b�N�X�̍s��
		Matrix matblur;	// �{�b�N�X�̎���ɏo���r���{�[�h�̍s��
	}obj[NUM_ZANZO];

	DWORD	idx;		// obj�̈�ԍŐV�G�t�F�N�g�Ԓn
	Vec3    addrot;		// ���t���[�����Z�����]��
	Vec2	posXY;		// xy�̈ʒu
	float   spd;		// �J�����Ɍ������Ă��鑬�x
	Color   color;		// �{�b�N�X�̐F
}TITLE_3DEFFECT;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
Matrix* BrendMatrix(Matrix* pMat, TITLE_3DEFFECT* pEffect, DWORD idx);
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static MyList		g_list3DEffect;		// 3D�{�b�N�X�G�t�F�N�g�̃��X�g	
static Model		g_modelbox;			// �{�b�N�X�̃��b�V��

static struct {
	Texture tex;	// �^���u���[�\���p�̃e�N�X�`��
	VtxBuff vtx;	// �|���S�����_�o�b�t�@
}g_blur;								// �u���[���[�N


/*=====================================================================
3D�G�t�F�N�g�ݒu�֐�(�ȈՔŁj
�@�g���Ŋ֐��̈����ɂ�����x���������������w�肵�Đݒu���܂��B
�߂�l�Fvoid
=====================================================================*/
void SetTitle3DEffect()
{
	// �g���֐��̈����Ɠ������̂�錾
	float len = 150;
	float agl = 0;
	float spd = 20;
	Color col(0, 0, 0, 1);
	Vec3 paddrot(0, 0, 0);

	//len += rand() % 5;
	agl += (rand() % 1000 / 1000.0f) * (2 * D3DX_PI);
	spd += (rand() % 1000 / 1000.0f) * (2.0f);
	paddrot.x = (rand() % 1000 / 4000.0f);
	paddrot.y = (rand() % 1000 / 4000.0f);
	paddrot.z = (rand() % 1000 / 4000.0f);
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
	new_pt->addrot = *paddrot;
	new_pt->color = *col;
	new_pt->spd = spd;

	// �~��ɂ���XY�ʒu�̎Z�o
	new_pt->posXY.x = -sinf(agl)*len;
	new_pt->posXY.y = cosf(agl)*len;

	// ��ԍŏ��̃{�b�N�X�ݒu�Ԓn���O�ɂ��邽�߂ɒ�������
	new_pt->idx = NUM_ZANZO - 1;

	// �c�������f�[�^���Z�o����
	for (int i = 0; i < NUM_ZANZO; i++, posZ -= spd, rot += *paddrot)
	{
		new_pt->obj[i].posZ		= posZ;
		new_pt->obj[i].sclXYZ	= 1.0f - ((NUM_ZANZO - i)*SCL_SUB);
		new_pt->obj[i].rot		= rot;

		// �X�P�[���s��
		GetMatrix(&new_pt->obj[i].matScl, &Vec3(0, 0, 0), &Vec3(0, 0, 0), 
			&Vec3(new_pt->obj[i].sclXYZ, new_pt->obj[i].sclXYZ, new_pt->obj[i].sclXYZ));

		// �{�b�N�X�̍s��(�X�P�[�������������j
		GetMatrix(&new_pt->obj[i].matbox, &Vec3(0, 0, 0),
			&new_pt->obj[i].rot);

		// �^���u���[�̍s��(�X�P�[������������)->���Ȃ艓���̂ŒP�ʍs�����
		GetMatrix(&new_pt->obj[i].matblur, &Vec3(new_pt->posXY.x, new_pt->posXY.y, new_pt->obj[i].posZ));
	}
}


/*=====================================================================
�^�C�g���G�t�F�N�g�X�V�֐�
=====================================================================*/
void UpdateTitleEffect()
{
	TITLE_3DEFFECT* work_pt = NULL;


	// �G�t�F�N�g�̏���
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect,(void**)&work_pt))
	{
		DWORD keep_idx = work_pt->idx;	// ��ԍŐV�̔Ԓn���L�[�v

		// �S�v�f�����X�P�[���l������������
		for (int i = 0; i < NUM_ZANZO; i++)
		{
			work_pt->obj[i].sclXYZ -= SCL_SUB;

			// �X�P�[���s��
			GetMatrix(&work_pt->obj[i].matScl, &Vec3(0, 0, 0), &Vec3(0, 0, 0),
				&Vec3(work_pt->obj[i].sclXYZ, work_pt->obj[i].sclXYZ, work_pt->obj[i].sclXYZ));
		}

		work_pt->idx = (work_pt->idx + 1) % NUM_ZANZO;	// �X�V�Ԓn�̕ύX

		work_pt->obj[work_pt->idx].rot		= work_pt->obj[keep_idx].rot + work_pt->addrot;
		work_pt->obj[work_pt->idx].posZ		= work_pt->obj[keep_idx].posZ - work_pt->spd;
		work_pt->obj[work_pt->idx].sclXYZ	= 1.0f;

		if (work_pt->obj[work_pt->idx].posZ < GetCamera()->pos.z)
		{
			MyListDeleteObject(g_list3DEffect, (void**)&work_pt);
			continue;
		}

		// �X�P�[���s��
		GetMatrix(&work_pt->obj[work_pt->idx].matScl);

		// �{�b�N�X�̍s��(�X�P�[�������������j
		GetMatrix(&work_pt->obj[work_pt->idx].matbox, &Vec3(0, 0, 0),
			&work_pt->obj[work_pt->idx].rot);
	}
}

/*=====================================================================
�^�C�g���G�t�F�N�g�`��֐�
=====================================================================*/
void DrawTitleEffect()
{
	D3DMATERIAL9	matDef;
	TITLE_3DEFFECT* work_pt = NULL;
	D3DDEVICE;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);



	// �G�t�F�N�g�̏���
	MyListResetIterator(g_list3DEffect, true);
	while (MyListLoop(g_list3DEffect, (void**)&work_pt))
	{
		// ��ԍŐV�̃r���{�[�h�s��̎Z�o(�X�P�[��������)
		GetBillboardMatrix(&work_pt->obj[work_pt->idx].matblur,
			&Vec3(work_pt->posXY.x, work_pt->posXY.y, work_pt->obj[work_pt->idx].posZ));


		for (int i = 0; i < NUM_ZANZO; i++)
		{
			// �{�b�N�X�̕`��
			//DrawModelWithOtherMatrix(g_modelbox, &( work_pt->obj[i].matbox * work_pt->obj[i].matScl ));
			DrawModelWithOtherMatrix(g_modelbox, BrendMatrix(&work_pt->obj[i].matbox, work_pt, i));

			// �^���u���[�̕`��(�X�P�[���s����|�����킹���s����g�p�j
//			Draw3DVertexBuffer(g_blur.tex, g_blur.vtx, &(work_pt->obj[i].matblur * work_pt->obj[i].matScl));
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

/*=====================================================================
�^�C�g���G�t�F�N�g�������֐�
=====================================================================*/
void InitTitleEffect(bool isFirstInit)
{
	if (isFirstInit == true)
	{
		D3DDEVICE;
		
		// ���X�g�̍쐬
		g_list3DEffect = MyListCreate(sizeof(TITLE_3DEFFECT));

		// �{�b�N�X�̍쐬
		g_modelbox = CreateModel("data/MODEL/box.x");

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/blur.png", &g_blur.tex);

		// �e�N�X�`���p�|���S���̍쐬
		g_blur.vtx = Create3DPolygon(&Vec2(BOX_SIZEXYZ, BOX_SIZEXYZ));
	}


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
		SAFE_RELEASE(g_blur.vtx)
		SAFE_RELEASE(g_blur.tex)
		DeleteModel(&g_modelbox);
		MyListDelete(&g_list3DEffect);
	}
}

Matrix* BrendMatrix(Matrix* pMat, TITLE_3DEFFECT* pEffect,DWORD idx)
{
	*pMat = *pMat * (pEffect->obj[idx].matScl);		// �X�P�[���s���������

	// �I�t�Z�b�g���X�P�[�����O�����̂œ���Ȃ���
	pMat->_41 = pEffect->posXY.x;
	pMat->_42 = pEffect->posXY.y;
	pMat->_43 = pEffect->obj[idx].posZ;

	return pMat;
}