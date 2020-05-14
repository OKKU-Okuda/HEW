/**********************************************************************
[[fieldプログラム(field.cpp)]]
	作者：奥田　真規

	フィールド全般に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "field.h"
#include "player.h"

#include "Field/road.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

// プレイヤーの方向指定角度
#define PLAYERDIRECTION_ZP
#define PLAYERDIRECTION_ZM

#define PLAYERDIRECTION_XP
#define PLAYERDIRECTION_XM

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
CHIP_ID GetFieldChipID(Vec3* pos);
FIELD_CHIP* GetChipMemory();
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static FIELD_CHIP g_Field[MAX_FIELD];
		
static FIELD_CHIP* g_pOnField = NULL;		// プレイヤー座標的にいるCHIP

/*=====================================================================
●●関数
	●●する関数
	戻り値 : void
	引数 : void
=====================================================================*/
void SetField(Vec2_Int *p_pos, FIELD_TYPE type, float rot)
{

}

void UpdateField()
{
	PLAYER* player = GetPlayer();
	CHIP_ID	id;
	Matrix mat;
	Vec3	pos;

	id = GetFieldChipID(&player->pos);

	PrintDebugProc("プレイヤーチャンク算出 X:%d Z:%d", id.vec2.x, id.vec2.z);

	D3DXMatrixInverse(&mat, NULL, &g_pOnField->WldMat);
	D3DXVec3TransformCoord(&pos, &GetPlayer()->pos, &mat);

	PrintDebugProc("チャンク座標%vec3", pos);

	if (g_pOnField->pFunc->CheckHit(g_pOnField, &pos))
	{
		PrintDebugProc("---チャンク内---");
	}

	D3DXVec3TransformCoord(&GetPlayer()->pos, &pos, &g_pOnField->WldMat);

}

void PlayerCheckHitOnField()
{
	PLAYER* player = GetPlayer();
	CHIP_ID	id;

	id = GetFieldChipID(&player->pos);

	PrintDebugProc("プレイヤーチャンク算出 X:%d Z:%d", id.vec2.x, id.vec2.z);

//	D3DXMatrixInverse(&invmat, NULL, &pData->WldMat);
//	D3DXVec3TransformCoord(&pos, &GetPlayer()->pos, &invmat);

}

void DrawField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == FSTATE_NONE)
		{
			continue;
		}

		// 描画
		g_Field[i].pFunc->Draw(&g_Field[i]);
	}
}

void InitField()
{
	InitFieldRoad();

	//テスト
	g_pOnField= GetChipMemory();
	g_pOnField->pFunc = GetFieldRoadFunc();
	g_pOnField->State = FSTATE_READY;
	GetMatrix(&g_pOnField->WldMat, &Vec3(FIELDCHIP_WIDTH*0.5f, 0, FIELDCHIP_WIDTH*0.5f));

	g_pOnField = GetChipMemory();
	g_pOnField->pFunc = GetFieldRoadFunc();
	g_pOnField->State = FSTATE_READY;
	GetMatrix(&g_pOnField->WldMat, &Vec3(FIELDCHIP_WIDTH*0.5f, 0, FIELDCHIP_WIDTH*1.5f),
		&Vec3(0, D3DX_PI / 2, 0));

	// プレイヤーのところにこれを書いてもらう
	GetPlayer()->pos.x = PLAYER_POSX;
	GetPlayer()->pos.z = PLAYER_POSZ;
	GetPlayer()->rot.y = D3DX_PI;
}

void UninitField()
{

}

void ResetField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// 未使用状態
		g_Field[i].State = FSTATE_NONE;
	}

}

CHIP_ID GetFieldChipID(Vec3* pos)
{
	CHIP_ID ans;

	// 位置をチャンクサイズで割って整数化
	ans.vec2.x = (short)(pos->x / FIELDCHIP_WIDTH);
	ans.vec2.z = (short)(pos->z / FIELDCHIP_HEIGHT);

	// 負の場合-0は危険なのでそれぞれ1マイナスする
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
	// 巡回
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State != FSTATE_NONE)
		{// 使用中のはスキップ
			continue;
		}


		return &g_Field[i];
	}

	MessageBox(0, 0, 0, 0);

	// すべて使用していた場合はnullを返す
	return NULL;
}