/**********************************************************************
[[GIMMICKのスライドプログラム(Gimmick/Slide.cpp)]]
	作者：奥田　真規

	スライドを4つのキーで壊すプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Phase/Phase_GameTackle1.h"

#include "../field.h"
#include "Slide.h"
#include "Gimmick.h"

#include "../effect.h"
#include "../player.h"
#include "../bonusscore.h"
#include "../BonusAdd.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define NUM_POOL		(30)			// 保管領域数
#define Wk				GIMMICK_SLIDE*		// オブジェクトワークポインタ
#define var(name,arg)	Wk name = (Wk)arg;	// 親構造体アドレスの変換ﾏｸﾛ


#define SIZE_YZ		(20.0f)			// 丸太の大きさ

#define IS_USE_XMODEL		(true)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// スライドGIMMICK状態列挙
enum SLIDE_STATE {
	WSTATE_WAIT,
	WSTATE_FAILEDQTE,
	WSTATE_SUCCESSQTE,
};

// スライドGIMMICK構造体
typedef struct {
	GIMMICK_HEADER	Head;			// ヘッダ

	Matrix			WldMat;			// スライドのワールドマトリックス
	Vec3			Pos;			// 3次元座標(フィールド(chip)座標)
	Vec3			WldPos;			// 上記ワールド座標
	//Vec3			Rot;			// 回転
	bool			isFirstHit;		// 最初にぶつかったかどうか

}GIMMICK_SLIDE;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

static void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos);	// 更新関数
static void DrawGimmick(GIMMICK_HEADER* pHead);					// 描画関数
static void EndGimmick(GIMMICK_HEADER* pHead);					// 終了関数
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static GIMMICK_FUNC g_Func = { UpdateGimmick ,DrawGimmick ,EndGimmick };
static GIMMICK_SLIDE g_aGimmick[NUM_POOL];

#if IS_USE_XMODEL == true
static Model		g_modelSlide;
#else
static Mesh			g_meshSlide;	
static Texture		g_tex;
#endif
/*=====================================================================
スライド初期化関数
=====================================================================*/
GIMMICK_HEADER *GetSlideMemory(FIELD_CHIP* pChip, Vec3* pos)
{
	for (int i = 0; i < NUM_POOL; i++)
	{// 未使用箇所を捜索

		float Roty = pChip->Dir*(D3DX_PI / 2);			// 回転軸の決定

		if (g_aGimmick[i].Head.isActive == true)
		{
			continue;
		}

		// ゼロクリア
		ZeroMemory(&g_aGimmick[i], sizeof(GIMMICK_SLIDE));

		g_aGimmick[i].Head.isActive = true;
		g_aGimmick[i].Head.Type		= GTYPE_WALL;
		g_aGimmick[i].Head.pFunc	= &g_Func;


		// z軸のみ引数を使用
		g_aGimmick[i].Pos = *pos;
		g_aGimmick[i].Pos.y = 0.0f;
		g_aGimmick[i].Pos.x = 0.0f;

		g_aGimmick[i].isFirstHit = false;
		// 位置からワールド座標を求める
		D3DXVec3TransformCoord(&g_aGimmick[i].WldPos, &g_aGimmick[i].Pos, &pChip->WldMat);
		GetMatrix(&g_aGimmick[i].WldMat, &g_aGimmick[i].WldPos, &Vec3(0, Roty, 0));

		return (GIMMICK_HEADER *)&g_aGimmick[i];
	}

	return NULL;	// 取得できない場合
}

/*=====================================================================
スライド更新関数
=====================================================================*/
void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos)
{
	var(pData, pHead);

	if (pData->Pos.z - pPos->z < SIZE_YZ / 2 &&
		pData->Pos.z - pPos->z > -SIZE_YZ / 2 )
	{// 丸太とプレイヤーが座標乗衝突

		if (GetPlayer()->anim_use != PLAYER_SLIDING &&
			GetPlayer()->anim_use != PLAYER_JUMPING)
		{
			GameTackle1End();
			pData->isFirstHit = true;
			return;
		}

		if (pData->isFirstHit == false)
		{
			if (GetPlayer()->anim_use == PLAYER_SLIDING)
			{// スライディングで処理の場合はボーナス

				AddBonusScore(BST_SNEAK);

				if (GetSlideCnt() < SLIDING_CNT*0.2f)
				{
					AddBonusScore(BST_JUSTINTARACT);
				}
			}
			else if (GetPlayer()->anim_use == PLAYER_JUMPING &&
				GetPlayer()->jump_spped > JUMP_HEIGHT*0.1f)
			{// ジャンプのじゃすと　インタラクト判別処理
				AddBonusScore(BST_JUSTINTARACT);
			}

			AddGimmickPassCount();
		}
		pData->isFirstHit = true;

	}
}

/*=====================================================================
スライド描画関数
=====================================================================*/
void DrawGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);
	D3DDEVICE;
	DWORD is_lighting = false;

	pDevice->GetRenderState(D3DRS_LIGHTING, &is_lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#if IS_USE_XMODEL == true
	DrawModelWithOtherMatrix(g_modelSlide, &pData->WldMat);
#else
	D3DDEVICE;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, g_tex);

	g_meshSlide->DrawSubset(0);
#endif

	pDevice->SetRenderState(D3DRS_LIGHTING, is_lighting);

}

/*=====================================================================
スライド終了関数
=====================================================================*/
void EndGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);

}

/*=====================================================================
スライド初期化関数
=====================================================================*/
void InitFieldGimmickSlide()
{
	D3DDEVICE;

	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));

#if IS_USE_XMODEL == true
	g_modelSlide = CreateModel("data/MODEL/pole.x");
#else
	g_meshSlide = Create3DBoxMesh(&Vec3(FIELDROAD_X + (ROADWALL_SIZEX * 2), SIZE_YZ, SIZE_YZ),
		&Vec3(0, FIELDROAD_Y + (SIZE_YZ), 0));

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bridge_field.png", &g_tex);
#endif
}

/*=====================================================================
スライド終了化関数
=====================================================================*/
void UninitFieldGimmickSlide()
{
#if IS_USE_XMODEL == true
	DeleteModel(&g_modelSlide);
#else
	SAFE_RELEASE(g_meshSlide);
	SAFE_RELEASE(g_tex);
#endif

}

/*=====================================================================
スライドゲーム一時終了関数
=====================================================================*/
void AllEndFieldGimmickSlide()
{
	for (int i = 0; i < NUM_POOL; i++)
	{
		if (g_aGimmick[i].Head.isActive == true)
		{
			EndGimmick((GIMMICK_HEADER*)&g_aGimmick[i]);
		}
	}
}

/*=====================================================================
スライドゲームリセット関数
=====================================================================*/
void ResetFieldGimmickSlide()
{
	// ゼロクリア
	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));
}