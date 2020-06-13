/**********************************************************************
[[GIMMICKプログラム(Gimmick.cpp)]]
	作者：奥田　真規

	GIMMICKに関するプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../field.h"
#include "Gimmick.h"

#include "Wall.h"
#include "Slide.h"
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
GIMMICK設置関数
引数：
FIELD_CHIP* pChip,		:紐づけするフィールドCHIP
GIMMICK_TYPE Type,		:どのGIMMICKを出すか
Vec3* pPos,				:設置する場所(第1引数のCHIP座標）
DWORD option			:GIMMICKによって異なる引数
=====================================================================*/
void SetFieldGimmick(FIELD_CHIP* pChip, GIMMICK_TYPE Type, Vec3* pPos, DWORD option)
{
	GIMMICK_HEADER** ppChip = NULL;
	GIMMICK_HEADER* pGHead = NULL;

	// フィールドの空きを調べる処理
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] == NULL)
		{
			ppChip = &pChip->paGimmick[i];
		}
	}

	if (ppChip == NULL)
	{// フィールド側が開いていなければここで終える
		return;
	}

	// GIMMICKごとの初期化処理
	switch (Type)
	{
	case GTYPE_WALL:
		pGHead = GetWallMemory(pChip, pPos, (GIMICKWALL_QTE)option);
		break;
	case GTYPE_FORSLIDE:
		pGHead = GetSlideMemory(pChip, pPos);
		break;
	case MAX_GIMMICKTYPE:
	default:
		break;
	}


	if (pGHead == NULL)
	{// GIMMICK側でエラーの場合はここで終了
		return;
	}

	// アドレスの挿入
	*ppChip = pGHead;
}

/*=====================================================================
GIMMICK初期化関数
=====================================================================*/
void InitFieldGimmick()
{
	InitFieldGimmickWall();
	InitFieldGimmickSlide();
}

/*=====================================================================
GIMMICK終了化関数
=====================================================================*/
void UninitFieldGimmick()
{
	UninitFieldGimmickWall();
	UninitFieldGimmickSlide();
}

/*=====================================================================
GIMMICKエンド関数
=====================================================================*/
void EndFieldGimmick()
{
	AllEndFieldGimmickWall();
	AllEndFieldGimmickSlide();
}

/*=====================================================================
GIMMICKリセット関数
=====================================================================*/
void ResetFieldGimmick()
{
	ResetFieldGimmickWall();
	ResetFieldGimmickSlide();
}

/*=====================================================================
GIMMICK更新関数
=====================================================================*/
void FieldGimmickUpdate(FIELD_CHIP* pChip, Vec3* pos)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			// GIMMICKの更新
			pChip->paGimmick[i]->pFunc->Update(pChip->paGimmick[i], pos);
		}
	}
}

/*=====================================================================
GIMMICK描画関数
=====================================================================*/
void FieldGimmickDraw(FIELD_CHIP *pChip)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			// GIMMICKの描画
			pChip->paGimmick[i]->pFunc->Draw(pChip->paGimmick[i]);
		}
	}
}

/*=====================================================================
フィールドアドレス指定のギミック削除関数
=====================================================================*/
void DeleteGimmickByFieldPtr(FIELD_CHIP* pChip)
{
	for (int i = 0; i < MAX_GIMMICK; i++)
	{
		if (pChip->paGimmick[i] != NULL)
		{
			pChip->paGimmick[i]->isActive = false;
			pChip->paGimmick[i]->pFunc->End(pChip->paGimmick[i]);		// 終了関数
			pChip->paGimmick[i] = NULL;
		}
	}
}