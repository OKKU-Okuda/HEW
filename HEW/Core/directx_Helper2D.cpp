/**********************************************************************
[[DirectX2D�w���v�v���O����(directxHelper_2D.cpp)]]
	��ҁF���c�@�^�K

	DirectX�̃v���O�����ȗ���(2D)�Ɋւ���v���O����
***********************************************************************/
#include "directx_Helper2D.h"
#include <tchar.h>
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

/*=====================================================================
���_�o�b�t�@�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VtxBuff pVtx:���_�o�b�t�@
=====================================================================*/
void Draw2DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx)
{
	D3DDEVICE(pDevice);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, pTex);
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_2D));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	return;
}

/*=====================================================================
���_�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VERTEX_2D* pVtx�@:���_
=====================================================================*/
void Draw2DVertex(LPDIRECT3DTEXTURE9 pTex, VERTEX_2D* pVtx)
{
	D3DDEVICE(pDevice);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, pTex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, pVtx, sizeof(VERTEX_2D));
	return;

}

/*=====================================================================
���ՓI�ɒ��_�ݒu�֐�(�m�[�}��)
	�߂�l�Fvoid
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
=====================================================================*/
void MakeNormal2DVertex(int num, VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size)
{
	// ���ːE�̐ݒ�
	Set2DVertexColor(vtx_data, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

	// �p�[�X�y�N�e�B�u�R���N�g
	(vtx_data)->rhw =
		(vtx_data + 1)->rhw =
		(vtx_data + 2)->rhw =
		(vtx_data + 3)->rhw = 1.0f;

	// ���_���W
	(vtx_data)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 1)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y - Size->y, 0.0f);
	(vtx_data + 2)->vtx = D3DXVECTOR3(Pos->x - Size->x, Pos->y + Size->y, 0.0f);
	(vtx_data + 3)->vtx = D3DXVECTOR3(Pos->x + Size->x, Pos->y + Size->y, 0.0f);

	// �������łO�w�肪�����
	if (num == 0)
	{
		// �e�N�X�`�����W
		(vtx_data)->tex = D3DXVECTOR2(0.0f, 0.0f);
		(vtx_data + 1)->tex = D3DXVECTOR2(1.0f, 0.0f);
		(vtx_data + 2)->tex = D3DXVECTOR2(0.0f, 1.0f);
		(vtx_data + 3)->tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

/*=====================================================================
���ՓI�ɒ��_�o�b�t�@�쐬�֐�(�m�[�}��)
	�߂�l�FLPDIRECT3DVERTEXBUFFER9
	�����F
	int num					=0:obj�S�̂Ƀe�N�X�`���𒣂�悤�ɍ��W��ݒ肷��
							=1:���Ȃ�
	 D3DXVECTOR3 *Pos,		:���S���W
	 D3DXVECTOR2 *Size		:���S����̃T�C�Y
	 OBJ_2DDATA *data		:�����]���������ꍇ�͂�����g���i�K�{�łȂ��j
	 float		Rot			:��]�p(�K�{�łȂ�)
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Make2DVertexBuffer(int num, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size, OBJ_2DDATA *data,float Rot)
{
	D3DDEVICE(pDevice);
	LPDIRECT3DVERTEXBUFFER9 buff = NULL;
	VERTEX_2D				*pvtx = 0;
	HRESULT					hr;

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&buff,
		NULL);
	if (FAILED(hr))return NULL;
	buff->Lock(0, 0, (void**)&pvtx, 0);				// ���b�N

	MakeNormal2DVertex(num, pvtx, Pos, Size);		// ���_�̐ݒu

	if (data)
	{
		SetObj2Ddata(data, Pos, Size, Rot);			// 2D�f�[�^�̐ݒu
		if (Rot != 0.0f)
		{
			SuperSet2DVerTex(pvtx, data);			// ���_�̐ݒu
		}
	}

	buff->Unlock();									// ���b�N����

	return buff;
}

/*=====================================================================
�J���[�ݒu�֐�(�m�[�}��)
	�߂�l�Fvoid
	�����F
	VERTEX_2D *vtx_data		:���_�Ł[���i�S�̌���j
 D3DCOLOR nColor			: �V�����J���[
=====================================================================*/
void Set2DVertexColor(VERTEX_2D *vtx_data, D3DCOLOR nColor)
{
	// ���ːE�̐ݒ�
	(vtx_data)->diffuse =
		(vtx_data + 1)->diffuse =
		(vtx_data + 2)->diffuse =
		(vtx_data + 3)->diffuse = nColor;

}

/*=====================================================================
�e�N�X�`��2D���W�ݒu�֐�
	�߂�l�Fvoid
	�����F
VERTEX_2D *vtx_data,	:4�̒��_�z��
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set2DTexPos(VERTEX_2D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY)
{
	D3DXVECTOR2 size = D3DXVECTOR2(1.0f / X_parts, 1.0f / Y_parts);

	(vtx_data)->tex.x =
		(vtx_data + 2)->tex.x = size.x*nowX;
	(vtx_data + 1)->tex.x =
		(vtx_data + 3)->tex.x = size.x*(nowX + 1);

	(vtx_data)->tex.y =
		(vtx_data + 1)->tex.y = size.y*nowY;
	(vtx_data + 2)->tex.y =
		(vtx_data + 3)->tex.y = size.y*(nowY + 1);

}

/*=====================================================================
2d�I�u�W�F�N�g�f�[�^�ݒu�֐�
	�߂�l�Fvoid
	�����F
		OBJ_2DDATA *data,	
		D3DXVECTOR3 *pos,	:���S���W
		D3DXVECTOR2 *size,	:���S����̃T�C�Y
		float rot =0.0f		:���a
=====================================================================*/
void SetObj2Ddata(OBJ_2DDATA *data,D3DXVECTOR3 *pos,D3DXVECTOR2 *size,float rot)
{
	data->Pos = *pos;
	data->Rot = rot;
	Change2DSizeBySize(data, size->x, size->y);

	SuperSet2DVerTex(data->vtx, data);
}

/*=====================================================================
���a��ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float nRadius:�V�������a
=====================================================================*/
void Change2DSizeByRadius(OBJ_2DDATA *data, float nRadius)
{
	if (nRadius == 0.0f)return;	// ���a����������

	// �����������̔��a����������(���̂��Ƃɑ�������Z�΍�)
	if (data->Radius == 0.0f)
	{
		data->Radius = nRadius;
		data->Size.x = cosf(data->BaseAngle)*nRadius;
		data->Size.y = sinf(data->BaseAngle)*nRadius;
		return;
	}

	// �e�����󂯂�p���̒���
	float Times = (nRadius / data->Radius);
	data->Radius = nRadius;
	data->Size.x *= Times;
	data->Size.y *= Times;

	return;
}

/*=====================================================================
�{����ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float Times:�V�����{��
=====================================================================*/
void Change2DSizeByTimes(OBJ_2DDATA *data, float Times)
{
	if (Times == 0.0f)return;	// �{������������

	// �e�����󂯂�p���̒���
	data->Radius *= Times;
	data->Size.x *= Times;
	data->Size.y *= Times;

	return;
}

/*=====================================================================
�T�C�Y��ł̑傫���̕ύX�֐�
	�߂�l�Fvoid
	�����FD3DX_OBJ_DATA*
			float x:���R�m����
			float y:�c�̒���
=====================================================================*/
void Change2DSizeBySize(OBJ_2DDATA *data, float x, float y)
{
	data->Size.x = x;
	data->Size.y = y;
	data->BaseAngle = atan2f(y, x);
	D3DXVECTOR2 temp = D3DXVECTOR2(x, y);
	data->Radius = D3DXVec2Length(&temp);
	return;
}

/*=====================================================================
���_�̐ݒu�𒴊ȒP�ɍs���֐�
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	D3DX_OBJ_DATA *data
=====================================================================*/
void SuperSet2DVerTex(VERTEX_2D *vtx_data, OBJ_2DDATA *data)
{
	// ��]�p�x��0�x�̏ꍇ�͎O�p�֐����g�p���Ȃ��z�u���@�ɐ؂�ւ���O
	if (data->Rot == 0)
	{
		vtx_data->vtx.x = data->Pos.x - data->Size.x;
		vtx_data->vtx.y = data->Pos.y - data->Size.y;

		(vtx_data + 1)->vtx.x = data->Pos.x + data->Size.x;
		(vtx_data + 1)->vtx.y = data->Pos.y - data->Size.y;

		(vtx_data + 2)->vtx.x = data->Pos.x - data->Size.x;
		(vtx_data + 2)->vtx.y = data->Pos.y + data->Size.y;

		(vtx_data + 3)->vtx.x = data->Pos.x + data->Size.x;
		(vtx_data + 3)->vtx.y = data->Pos.y + data->Size.y;
	}
	else
	{
		// ���_���W�̐ݒ�
		vtx_data->vtx.x = data->Pos.x - cosf(data->BaseAngle + data->Rot) * data->Radius;
		vtx_data->vtx.y = data->Pos.y - sinf(data->BaseAngle + data->Rot) * data->Radius;

		(vtx_data + 1)->vtx.x = data->Pos.x + cosf(data->BaseAngle - data->Rot) * data->Radius;
		(vtx_data + 1)->vtx.y = data->Pos.y - sinf(data->BaseAngle - data->Rot) * data->Radius;

		(vtx_data + 2)->vtx.x = data->Pos.x - cosf(data->BaseAngle - data->Rot) * data->Radius;
		(vtx_data + 2)->vtx.y = data->Pos.y + sinf(data->BaseAngle - data->Rot) * data->Radius;

		(vtx_data + 3)->vtx.x = data->Pos.x + cosf(data->BaseAngle + data->Rot) * data->Radius;
		(vtx_data + 3)->vtx.y = data->Pos.y + sinf(data->BaseAngle + data->Rot) * data->Radius;
	}
	return;
}


/*=====================================================================
���_�̐ݒu���s���֐�
	�߂�l�Fvoid
	�����FVERTEX_2D *vtx_data:���_(4�̒��_����)
	D3DXVECTOR3 *Pos,	:���S���W
	D3DXVECTOR2 *Size	:���S����̃T�C�Y
=====================================================================*/
void Set2DVerTex(VERTEX_2D *vtx_data, D3DXVECTOR3 *Pos, D3DXVECTOR2 *Size)
{
	vtx_data->vtx.x = Pos->x - Size->x;
	vtx_data->vtx.y = Pos->y - Size->y;

	(vtx_data + 1)->vtx.x = Pos->x + Size->x;
	(vtx_data + 1)->vtx.y = Pos->y - Size->y;

	(vtx_data + 2)->vtx.x = Pos->x - Size->x;
	(vtx_data + 2)->vtx.y = Pos->y + Size->y;

	(vtx_data + 3)->vtx.x = Pos->x + Size->x;
	(vtx_data + 3)->vtx.y = Pos->y + Size->y;

}


