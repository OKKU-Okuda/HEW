/**********************************************************************
[[DirectX�w���v�v���O����(directxHelper_3D.cpp)]]
	��ҁF���c�@�^�K

	DirectX(3D)�̃v���O�����ȗ����Ɋւ���v���O����
***********************************************************************/
#include "main.h"
#include "directx_Helper3D.h"
#include "MyList.h"
#include "camera.h"
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
3D���_�o�b�t�@�`��֐�
	�߂�l�Fvoid
	�����F
	Texture pTex:�e�N�X�`��
	VtxBuff pVtx:���_�o�b�t�@
=====================================================================*/
void Draw3DVertexBuffer(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DVERTEXBUFFER9 pVtx, D3DXMATRIX *Mat)
{
	D3DDEVICE;
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, pTex);
	pDevice->SetTransform(D3DTS_WORLD, Mat);
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));
	
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	return;
}

/*=====================================================================
	�|���S�����������֐�
�߂�l:LPDIRECT3DVERTEXBUFFER9 
�����@:
D3DXVECTOR2 *size,	:�T�C�Y
int X_parts,			�F(�e�N�X�`������)
int Y_parts,
int nowX,
int nowY
=====================================================================*/
LPDIRECT3DVERTEXBUFFER9 Create3DPolygon(D3DXVECTOR2 *size, int X_parts, int Y_parts, int nowX, int nowY)
{
	D3DDEVICE(pDevice);
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	VERTEX_3D				*pVtx = NULL;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)* NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		// ���b�N
	Set3DVertex(pVtx, size, X_parts, Y_parts, nowX, nowY);
	pVtxBuff->Unlock();

	return pVtxBuff;
}


/*=====================================================================
	�r���{�[�h�s��擾�֐�
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:�o�͍s��ւ̃A�h���X
D3DXVECTOR3 *pos,	:(�ȉ�3�͕K�{�łȂ�)
D3DXVECTOR3 *rot, 
D3DXVECTOR3 *scl
=====================================================================*/
D3DXMATRIX *GetBillboardMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, D3DXVECTOR3 *scl)
{
	GetInverseMatrix(pOut, GetMtxView());	// �r���[�s��̋t�s������߂�	
	GetMatrix(pOut, pos, rot, scl, false);	// �t�s������������Ȃ���srt���
	return pOut;
}

/*=====================================================================
	�t�s��擾�֐�
�t�s����v�Z���ċ��߂�֐�
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:�ʒu�n�̂Ł[���͎��O�ɂ���邱��
D3DXMATRIX *pOri	:�t�s��ɂ������s��
=====================================================================*/
D3DXMATRIX *GetInverseMatrix(D3DXMATRIX *pOut, D3DXMATRIX *pOri)
{
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pOut);

	// �|���S���𐳖ʂɌ�����
	// �t�s������Ƃ߂�
#if 1
	pOut->_11 = pOri->_11;
	pOut->_12 = pOri->_21;
	pOut->_13 = pOri->_31;

	pOut->_21 = pOri->_12;
	pOut->_22 = pOri->_22;
	pOut->_23 = pOri->_32;

	pOut->_31 = pOri->_13;
	pOut->_32 = pOri->_23;
	pOut->_33 = pOri->_33;

#else	

	D3DXMatrixInverse(&g_aTree[nCntTree].mtxWorld, NULL, &mtxView);
	g_aTree[nCntTree].mtxWorld._41 =
		g_aTree[nCntTree].mtxWorld._42 =
		g_aTree[nCntTree].mtxWorld._43 = 0.0f;

#endif
	return pOut;
}
/*=====================================================================
	�s��擾�֐�
�s����v�Z���ċ��߂�֐�
	�� �v�Z��SCL,ROT,POS�̏��ōs�� ��
�߂�l:D3DXMATRIX *	:��P�����ɓ���l�Ɠ���
�����@:
D3DXMATRIX *pOut,	:���ʂ�ۊǂ���A�h���X
D3DXVECTOR3 *pos,	:�ʒu(�K�{�ł͂Ȃ�)
D3DXVECTOR3 *rot,	:��](�K�{�ł͂Ȃ�)
D3DXVECTOR3 *scl	:�X�P�[��(�K�{�ł͂Ȃ�)
bool isReset		:���Z�b�g�̗L���i�K�{�ł͂Ȃ��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�j
=====================================================================*/
D3DXMATRIX *GetMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, D3DXVECTOR3 *scl, bool isReset)
{
#if 1
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	if(isReset)D3DXMatrixIdentity(pOut);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, scl->x, scl->y, scl->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot->y, rot->x, rot->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos->x, pos->y, pos->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxTranslate);
#else
	D3DXMatrixTransformation
#endif
	return pOut;
}

/*=====================================================================
3D���_�ύX�֐�
	�߂�l�Fvoid
	�����F
VERTEX_3D *vtx_data,	:4�̒��_�z��
 D3DXVECTOR2 *size		:�T�C�Y
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set3DVertex(VERTEX_3D *vtx_data, D3DXVECTOR2 *size, int X_parts, int Y_parts, int nowX, int nowY)
{
	vtx_data[0].vtx = D3DXVECTOR3(-size->x, +size->y, 0.0f);
	vtx_data[1].vtx = D3DXVECTOR3(+size->x, +size->y, 0.0f);
	vtx_data[2].vtx = D3DXVECTOR3(-size->x, -size->y, 0.0f);
	vtx_data[3].vtx = D3DXVECTOR3(+size->x, -size->y, 0.0f);

	vtx_data[0].nor =
		vtx_data[1].nor =
		vtx_data[2].nor =
		vtx_data[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vtx_data[0].diffuse =
		vtx_data[1].diffuse =
		vtx_data[2].diffuse =
		vtx_data[3].diffuse = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);

	Set3DTexPos(vtx_data, X_parts, Y_parts, nowX, nowY);
}
/*=====================================================================
�e�N�X�`��3D���W�ݒu�֐�
	�߂�l�Fvoid
	�����F
VERTEX_3D *vtx_data,	:4�̒��_�z��
int X_parts,			:��������
int Y_parts,			:��������
int nowX,				:x�ʒu
int nowY				:y�ʒu
=====================================================================*/
void Set3DTexPos(VERTEX_3D *vtx_data, int X_parts, int Y_parts, int nowX, int nowY)
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

//////////////////////////////////////////////////////////////////////////////////////
// MODEL�\���̊֘A
/////////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
���f���`��֐�
	���f���ɕt�^����Ă���e�N�X�`�����ŕ`�悳���
�߂�l:void
�����@:Xmodel* pModel :���f���i�[�\����
=====================================================================*/
void DrawModel(Xmodel* pModel)
{
	D3DDEVICE(pDevice);
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pModel->WldMtx);

	for (DWORD adr = 0; adr < pModel->nNummat; adr++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pModel->pMaterial[adr].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0,(Texture)pModel->pMaterial[adr].pTextureFilename);

		// �`��
		pModel->pMesh->DrawSubset(adr);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

/*=====================================================================
���f���쐬�֐�
	xfile���f����ǂݍ��݁A�֘A�̃e�N�X�`�����ǂݍ��ފ֐�
	�� xfile�Ŏg�p����e�N�X�`���͓���f�B���N�g�����ɒu������ ��
�߂�l:void
�����@:const char * filename:�t�@�C���p�X
=====================================================================*/
Xmodel *CreateModel(const char *filename)
{
	typedef struct {
		char		texfile_name[64];
		Texture		pTex;
	}MatTexList;

	D3DDEVICE(pDevice);
	DWORD			numMat		= 0l;
	LPD3DXMESH		pMesh		= NULL;
	LPD3DXBUFFER	pMat		= NULL;
	Xmodel			*pModel		= NULL;
	D3DXMATERIAL	*pMaterial	= NULL;
	DWORD			path_adr	= strlen(filename);
	MyList			list		= MyListCreate(sizeof(MatTexList));
	char			TexPath[64];

	HRESULT hr= D3DXLoadMeshFromX(filename,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
					D3DXMESH_SYSTEMMEM,					// ���b�V���̍쐬�I�v�V�������w��
					pDevice,							// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
					NULL,								// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					&pMat,								// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					NULL,								// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
					&numMat,							// D3DXMATERIAL�\���̂̐�
					&pMesh);							// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	if (FAILED(hr))return NULL;

	ZeroMemory(TexPath, sizeof(TexPath));	
	strcpy_s(TexPath, filename);

	for (; filename[path_adr] != '/'; path_adr--);			// ��ԉ��ɂ���/��������

	pModel = (Xmodel*)calloc(1, sizeof(Xmodel) + (sizeof(D3DXMATERIAL)*numMat));
	pModel->pMesh		= pMesh;							// ���b�V�����
	pModel->nNummat		= numMat;							// �}�e���A�������
	pModel->pMaterial	= (D3DXMATERIAL*)(pModel + 1);		// �}�e���A���z��̏ꏊ(�A�h���XXmodel�̒���)

	pMaterial = (D3DXMATERIAL*)pMat->GetBufferPointer();	// �o�b�t�@�̃A�h���X���擾

	// �}�e���A��������������
	for (DWORD mat_adr = 0; mat_adr < numMat; mat_adr++)
	{
		if (pMaterial[mat_adr].pTextureFilename)
		{
			MatTexList *work_pt = NULL;

			MyListResetIterator(list, true);
			while (MyListLoop(list, (void**)&work_pt))
			{
				// �e�N�X�`�������d�����Ă��邩�ǂ���
				if (!strcmp(pMaterial[mat_adr].pTextureFilename,
					work_pt->texfile_name))
					break;
			}

			// ���݂��Ȃ����work_pt��NULL������
			if (work_pt)
			{// ���݂���
				work_pt->pTex->AddRef();
				pMaterial[mat_adr].pTextureFilename = (LPSTR)work_pt->pTex;
			}
			else
			{// ���݂��Ȃ�
				work_pt = (MatTexList*)MyListCreateObjectBottom(list);

				// �e�N�X�`�����̃R�s�[
				strcpy_s(work_pt->texfile_name, pMaterial[mat_adr].pTextureFilename);

				// �e�N�X�`���̃t�@�C���p�X�����߂�
				memcpy(&TexPath[path_adr + 1], pMaterial[mat_adr].pTextureFilename,
					sizeof(char)*(strlen(pMaterial[mat_adr].pTextureFilename) + 1));

				// �e�N�X�`���̃A�h���X�𕶎���A�h���X�Ƃ��Ė���������č쐬
				D3DXCreateTextureFromFile(pDevice,
					TexPath, (Texture*)&pMaterial[mat_adr].pTextureFilename);

				work_pt->pTex = (Texture)pMaterial[mat_adr].pTextureFilename;

			}
		}

		// �}�e���A�����̃R�s�[
		pModel->pMaterial[mat_adr] = pMaterial[mat_adr];

	}
	
	MyListDelete(&list);									// ���X�g�̊J��
	pMat->Release();										// �o�b�t�@�̊J��
	return pModel;
}


/*=====================================================================
���f���폜�֐�
�߂�l:void
�����@:Xmodel** ppModel�@:���f���\���̂̏ꏊ��ۊǂ��Ă���ꏊ
=====================================================================*/
void DeleteModel(Xmodel** ppModel)
{
	// �e�N�X�`���̊J��
	for (DWORD adr = 0; adr < (*ppModel)->nNummat; adr++)
	{
		if ((*ppModel)->pMaterial[adr].pTextureFilename)
		{
			/*int a=*/((Texture)(*ppModel)->pMaterial[adr].pTextureFilename)->Release();
		}
	}

	// ���b�V���̊J��
	if ((*ppModel)->pMesh) (*ppModel)->pMesh->Release();


	free(*ppModel);
	*ppModel = NULL;
}