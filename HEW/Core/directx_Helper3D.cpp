/**********************************************************************
[[DirectXヘルププログラム(directxHelper_3D.cpp)]]
	作者：奥田　真規

	DirectX(3D)のプログラム簡略化に関するプログラム
***********************************************************************/
#include "main.h"
#include "directx_Helper3D.h"
#include "MyList.h"
#include "camera.h"
#include <tchar.h>
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

/*=====================================================================
3D頂点バッファ描画関数
	戻り値：void
	引数：
	Texture pTex:テクスチャ
	VtxBuff pVtx:頂点バッファ
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
	ポリゴンさくせい関数
戻り値:LPDIRECT3DVERTEXBUFFER9 
引数　:
D3DXVECTOR2 *size,	:サイズ
int X_parts,			：(テクスチャ分割)
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

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		// ロック
	Set3DVertex(pVtx, size, X_parts, Y_parts, nowX, nowY);
	pVtxBuff->Unlock();

	return pVtxBuff;
}


/*=====================================================================
	ビルボード行列取得関数
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:出力行列へのアドレス
D3DXVECTOR3 *pos,	:(以下3つは必須でない)
D3DXVECTOR3 *rot, 
D3DXVECTOR3 *scl
=====================================================================*/
D3DXMATRIX *GetBillboardMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, D3DXVECTOR3 *scl)
{
	GetInverseMatrix(pOut, GetMtxView());	// ビュー行列の逆行列を求める	
	GetMatrix(pOut, pos, rot, scl, false);	// 逆行列を初期化しないでsrt代入
	return pOut;
}

/*=====================================================================
	逆行列取得関数
逆行列を計算して求める関数
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:位置系のでーたは事前にいれること
D3DXMATRIX *pOri	:逆行列にしたい行列
=====================================================================*/
D3DXMATRIX *GetInverseMatrix(D3DXMATRIX *pOut, D3DXMATRIX *pOri)
{
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pOut);

	// ポリゴンを正面に向ける
	// 逆行列をもとめる
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
	行列取得関数
行列を計算して求める関数
	※ 計算はSCL,ROT,POSの順で行う ※
戻り値:D3DXMATRIX *	:代１引数に入る値と同じ
引数　:
D3DXMATRIX *pOut,	:結果を保管するアドレス
D3DXVECTOR3 *pos,	:位置(必須ではない)
D3DXVECTOR3 *rot,	:回転(必須ではない)
D3DXVECTOR3 *scl	:スケール(必須ではない)
bool isReset		:リセットの有無（必須ではない　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　）
=====================================================================*/
D3DXMATRIX *GetMatrix(D3DXMATRIX *pOut, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, D3DXVECTOR3 *scl, bool isReset)
{
#if 1
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	if(isReset)D3DXMatrixIdentity(pOut);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, scl->x, scl->y, scl->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot->y, rot->x, rot->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos->x, pos->y, pos->z);
	D3DXMatrixMultiply(pOut, pOut, &mtxTranslate);
#else
	D3DXMatrixTransformation
#endif
	return pOut;
}

/*=====================================================================
3D頂点変更関数
	戻り値：void
	引数：
VERTEX_3D *vtx_data,	:4つの頂点配列
 D3DXVECTOR2 *size		:サイズ
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
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
テクスチャ3D座標設置関数
	戻り値：void
	引数：
VERTEX_3D *vtx_data,	:4つの頂点配列
int X_parts,			:ｘ分割数
int Y_parts,			:ｙ分割数
int nowX,				:x位置
int nowY				:y位置
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
// MODEL構造体関連
/////////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
モデル描画関数
	モデルに付与されているテクスチャ月で描画される
戻り値:void
引数　:Xmodel* pModel :モデル格納構造体
=====================================================================*/
void DrawModel(Xmodel* pModel)
{
	D3DDEVICE(pDevice);
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pModel->WldMtx);

	for (DWORD adr = 0; adr < pModel->nNummat; adr++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pModel->pMaterial[adr].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0,(Texture)pModel->pMaterial[adr].pTextureFilename);

		// 描画
		pModel->pMesh->DrawSubset(adr);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

/*=====================================================================
モデル作成関数
	xfileモデルを読み込み、関連のテクスチャも読み込む関数
	※ xfileで使用するテクスチャは同一ディレクトリ内に置くこと ※
戻り値:void
引数　:const char * filename:ファイルパス
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

	HRESULT hr= D3DXLoadMeshFromX(filename,	// 読み込むモデルファイル名(Xファイル)
					D3DXMESH_SYSTEMMEM,					// メッシュの作成オプションを指定
					pDevice,							// IDirect3DDevice9インターフェイスへのポインタ
					NULL,								// 隣接性データを含むバッファへのポインタ
					&pMat,								// マテリアルデータを含むバッファへのポインタ
					NULL,								// エフェクトインスタンスの配列を含むバッファへのポインタ
					&numMat,							// D3DXMATERIAL構造体の数
					&pMesh);							// ID3DXMeshインターフェイスへのポインタのアドレス
	if (FAILED(hr))return NULL;

	ZeroMemory(TexPath, sizeof(TexPath));	
	strcpy_s(TexPath, filename);

	for (; filename[path_adr] != '/'; path_adr--);			// 一番奥にある/を見つける

	pModel = (Xmodel*)calloc(1, sizeof(Xmodel) + (sizeof(D3DXMATERIAL)*numMat));
	pModel->pMesh		= pMesh;							// メッシュ情報
	pModel->nNummat		= numMat;							// マテリアル数情報
	pModel->pMaterial	= (D3DXMATERIAL*)(pModel + 1);		// マテリアル配列の場所(アドレスXmodelの直下)

	pMaterial = (D3DXMATERIAL*)pMat->GetBufferPointer();	// バッファのアドレスを取得

	// マテリアル数分処理する
	for (DWORD mat_adr = 0; mat_adr < numMat; mat_adr++)
	{
		if (pMaterial[mat_adr].pTextureFilename)
		{
			MatTexList *work_pt = NULL;

			MyListResetIterator(list, true);
			while (MyListLoop(list, (void**)&work_pt))
			{
				// テクスチャ名が重複しているかどうか
				if (!strcmp(pMaterial[mat_adr].pTextureFilename,
					work_pt->texfile_name))
					break;
			}

			// 存在しなければwork_ptにNULLが入る
			if (work_pt)
			{// 存在する
				work_pt->pTex->AddRef();
				pMaterial[mat_adr].pTextureFilename = (LPSTR)work_pt->pTex;
			}
			else
			{// 存在しない
				work_pt = (MatTexList*)MyListCreateObjectBottom(list);

				// テクスチャ名のコピー
				strcpy_s(work_pt->texfile_name, pMaterial[mat_adr].pTextureFilename);

				// テクスチャのファイルパスを求める
				memcpy(&TexPath[path_adr + 1], pMaterial[mat_adr].pTextureFilename,
					sizeof(char)*(strlen(pMaterial[mat_adr].pTextureFilename) + 1));

				// テクスチャのアドレスを文字列アドレスとして無理やり入れて作成
				D3DXCreateTextureFromFile(pDevice,
					TexPath, (Texture*)&pMaterial[mat_adr].pTextureFilename);

				work_pt->pTex = (Texture)pMaterial[mat_adr].pTextureFilename;

			}
		}

		// マテリアル情報のコピー
		pModel->pMaterial[mat_adr] = pMaterial[mat_adr];

	}
	
	MyListDelete(&list);									// リストの開放
	pMat->Release();										// バッファの開放
	return pModel;
}


/*=====================================================================
モデル削除関数
戻り値:void
引数　:Xmodel** ppModel　:モデル構造体の場所を保管している場所
=====================================================================*/
void DeleteModel(Xmodel** ppModel)
{
	// テクスチャの開放
	for (DWORD adr = 0; adr < (*ppModel)->nNummat; adr++)
	{
		if ((*ppModel)->pMaterial[adr].pTextureFilename)
		{
			/*int a=*/((Texture)(*ppModel)->pMaterial[adr].pTextureFilename)->Release();
		}
	}

	// メッシュの開放
	if ((*ppModel)->pMesh) (*ppModel)->pMesh->Release();


	free(*ppModel);
	*ppModel = NULL;
}