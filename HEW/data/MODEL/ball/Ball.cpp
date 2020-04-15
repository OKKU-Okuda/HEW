/**********************************************************************
[[メインプログラム(Ball.cpp)]]
	作者：奥田　真規

	ラグビーボールに関するプログラム
***********************************************************************/
#include "main.h"


#include "Ball.h"
#include "shadow.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
#define BALL_ADDRESS ("data/MODEL/ball.x")
#define BALL_TEX_ADDRESS ("data/TEXTURE/ball.jpg")

#define BALL_ZANZOU_RATE

#define BALL_FIRLD_Y		(0.0f)
#define BALL_SIZE_Y			(20.0f)
#define BALL_MOVE_Y			(0.08f)

#define BALL_REFLECT_RATE	(0.8f)
#define BALL_XZ_DOWN		(0.996f)
#define BALL_SPD_DOWN		(0.005f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------
typedef struct BALL {
	bool	isUse;
	bool	Moved;
	D3DXVECTOR3 pos;
	//D3DXVECTOR3 past_pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXVECTOR3 eng;
	float		rotz;

	bool	isZanzou;
	float   Zanzou_time;
	int					shadowIdx;
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

}BALL;
//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

static BALL					g_Ball[MAX_BALL];
static LPDIRECT3DTEXTURE9	g_pD3DTexture;		// テクスチャへのポインタ
static LPD3DXMESH			g_pD3DXMesh;		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pD3DXBuffMat;		// マテリアル情報へのポインタ
static DWORD				g_nNumMat;			// マテリアル情報の数

/*=====================================================================
	メイン関数

=====================================================================*/
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(BALL_ADDRESS,// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,					// メッシュの作成オプションを指定
		pDevice,							// IDirect3DDevice9インターフェイスへのポインタ
		NULL,								// 隣接性データを含むバッファへのポインタ
		&g_pD3DXBuffMat,					// マテリアルデータを含むバッファへのポインタ
		NULL,								// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat,							// D3DXMATERIAL構造体の数
		&g_pD3DXMesh)))						// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		BALL_TEX_ADDRESS,
		&g_pD3DTexture)))
	{
		return E_FAIL;
	}

	// ゼロクリア
	ZeroMemory(&g_Ball[0], sizeof(BALL)*MAX_BALL);

	return S_OK;
}

int CreateBall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 eng, D3DXVECTOR3 scl)
{

	for (int Ball_ID = 0; Ball_ID < MAX_BALL; Ball_ID++)
	{
		if (g_Ball[Ball_ID].isUse == false)
		{
			// ゼロクリア
			ZeroMemory(&g_Ball[Ball_ID], sizeof(BALL));

			// ステータスの代入
			g_Ball[Ball_ID].isUse = true;
			g_Ball[Ball_ID].pos = pos;
			//g_Ball[Ball_ID].past_pos = pos;
			g_Ball[Ball_ID].rot = rot;
			g_Ball[Ball_ID].scl = scl;
			g_Ball[Ball_ID].eng = eng;
			g_Ball[Ball_ID].isZanzou = false;
			g_Ball[Ball_ID].Moved = true;
			g_Ball[Ball_ID].rotz = -1.0f;
			g_Ball[Ball_ID].shadowIdx = CreateShadow(pos, g_Ball[Ball_ID].scl);

			return Ball_ID;
		}
	}

	// ボールメモリに空きがありません
	return -1;
}

void DeleteBall(int Ball_ID)
{
	if (Ball_ID < 0)
	{ // 負の数を引数にした場合はすべてのボールを削除する
		for (int i = 0; i < MAX_BALL;i++)
		{
			g_Ball[i].isUse = false;
			ReleaseShadow(g_Ball[i].shadowIdx);
		}
	}
	else if (Ball_ID < MAX_BALL)
	{
		g_Ball[Ball_ID].isUse = false;
		ReleaseShadow(g_Ball[Ball_ID].shadowIdx);
	}

	return;
}

void UpdateBall(void)
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		float move_z_rate = 1.0f, move_x_rate = 1.0f;
		float spd = 0;
		if (g_Ball[i].isUse == false)continue;

		D3DXVECTOR3 keep_pos = g_Ball[i].pos;

		g_Ball[i].Moved = true;

		//g_Ball[i].rot += D3DXVECTOR3(0.01f, 0.01f ,0.01f);

		// エネルギー演算の簡略化

		// --y軸移動処理
		{
			g_Ball[i].eng.y -= BALL_MOVE_Y;
			g_Ball[i].pos.y += g_Ball[i].eng.y;

			if (g_Ball[i].pos.y - BALL_SIZE_Y < BALL_FIRLD_Y)
			{
				g_Ball[i].pos.y = BALL_SIZE_Y;
				g_Ball[i].eng.y *= -BALL_REFLECT_RATE;

				move_z_rate = move_z_rate = 0.5f;	// 地面に近いと減速しにくくなる
				g_Ball[i].rotz *= -1.0f;
			}
		}

		// Z移動処理
		{
			g_Ball[i].eng.z *= BALL_XZ_DOWN;

			if (g_Ball[i].eng.z < 0)
			{
				move_z_rate *= -1;
			}
			g_Ball[i].eng.z -= BALL_SPD_DOWN * move_z_rate;
			if (fabsf(g_Ball[i].eng.z) < 0.001f)g_Ball[i].eng.z = 0.0f;
			g_Ball[i].pos.z += g_Ball[i].eng.z;
		}
		
		// X移動処理
		{
			g_Ball[i].eng.x *= BALL_XZ_DOWN;

			if (g_Ball[i].eng.x < 0)
			{
				move_x_rate *= -1;
			}
			g_Ball[i].eng.x -= BALL_SPD_DOWN * move_x_rate;
			if (fabsf(g_Ball[i].eng.x) < 0.001f)g_Ball[i].eng.x = 0.0f;
			g_Ball[i].pos.x += g_Ball[i].eng.x;
		}


		// 影もプレイヤーの位置に合わせる
		D3DXVECTOR3 pos = g_Ball[i].pos;
		pos.y = 0.0f;
		SetPositionShadow(g_Ball[i].shadowIdx, pos, g_Ball[i].scl);

		// 移動距離の算術
		spd = D3DXVec3LengthSq(&(keep_pos - g_Ball[i].pos));
		if (spd <= 0.005f)
		{
			DeleteBall(i);
			return;
		}
		g_Ball[i].rot.z += spd / 75 * g_Ball[i].rotz;
	}
}

void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	//pD3DXMat->pTextureFilename
	/*
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);				// Zバッファーの書き込みをしない
	*/

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// プレイヤーの描画
	for (int i = 0; i < MAX_BALL; i++)
	{
		// 使用していない　又は　演算後移動処理のない　場合はこの処理を飛ばす
		if (g_Ball[i].isUse == false && g_Ball[i].Moved == false)continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Ball[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Ball[i].scl.x, g_Ball[i].scl.y, g_Ball[i].scl.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Ball[i].rot.y, g_Ball[i].rot.x, g_Ball[i].rot.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Ball[i].pos.x, g_Ball[i].pos.y, g_Ball[i].pos.z);
		D3DXMatrixMultiply(&g_Ball[i].mtxWorld, &g_Ball[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Ball[i].mtxWorld);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMat->GetBufferPointer();

		//g_pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTexture);

			// 描画
			g_pD3DXMesh->DrawSubset(nCntMat);
		}

		// 稼働かどうか
		g_Ball[i].Moved = false;

	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
	/*
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zバッファーの書き込みをする

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	*/
}




void UninitBall(void)
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		if (g_pD3DTexture != NULL)
		{// テクスチャの開放
			g_pD3DTexture->Release();
			g_pD3DTexture = NULL;
		}

		if (g_pD3DXMesh != NULL)
		{// メッシュの開放
			g_pD3DXMesh->Release();
			g_pD3DXMesh = NULL;
		}

		if (g_pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_pD3DXBuffMat->Release();
			g_pD3DXBuffMat = NULL;
		}
	}
}