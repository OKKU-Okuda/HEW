/**********************************************************************
[[フィールドの出現管理プログラム(field_control.cpp)]]
	作者：奥田　真規

	フィールドのポップに関するプログラム
***********************************************************************/
#include "../Core/main.h"

#include "Gimmick/Gimmick.h"

#include "field.h"
#include "player_control.h"
#include "field_control.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define RATE_SPAWN_TURN		(0.1f)			// 分岐道の出現率
#define LIMIT_SPAWN_TURN	(40)			// 40回直線系の道を作成した場合にpool数に考慮して強制的に分岐道を出現させル
//#define RATE_SPAWN_TURN		(1.f)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static CHIP_ID g_idlate;		// 最後に設置した直線CHIPID(分岐系は別）

/*=====================================================================
フィールドCHIP生成関数
※方向はプレイヤーdirectionに依存する
引数：id_start	生成開始IDを指定(基本的にはプレイヤーの今いる位置を指定)
=====================================================================*/
void SpawnField(CHIP_ID id_start)
{
	CHIP_ID addID = GetFieldIDVector(GetPlayerDirection());
	FIELD_TYPE Latesttype = FTYPE_VOID;
	int cntNormal = 0;
	while (1)
	{
		int numRand;		//非CURVE道設置時のタイプ乱数を保管

		// 2次元IDの新規場所更新
		id_start.vec2.x += addID.vec2.x;
		id_start.vec2.z += addID.vec2.z;

		// 分岐道の割合設置(hit時処理終了)
		if (CheckRand(RATE_SPAWN_TURN) == true || cntNormal >= LIMIT_SPAWN_TURN)
		{
			numRand = rand() % NUM_FTYPE_CURVE + START_FTYPE_CURVE;

			FIELD_TYPE type;
			// 分岐道は３つなので公平に
			if (numRand == FTYPE_TURNL)
			{
				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// 横に続く道の方向
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// 横に続く道のid
				SetField(idleft, FTYPE_ROAD, fdirleft);		// 分岐道の設置

				type = FTYPE_TURNL;
			}
			else if (numRand == FTYPE_TURNR)
			{
				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// 横に続く道の方向
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// 横に続く道のid
				SetField(idright, FTYPE_ROAD, fdirright);		// 分岐道の設置

				type = FTYPE_TURNR;
			}
			else
			{
				type = FTYPE_TURNLR;

				FIELD_DIRECTION fdirleft = AddFieldDirection(GetPlayerDirection(), -1);		// 横に続く道の方向
				CHIP_ID idleft = AddFieldID(id_start, GetFieldIDVector(fdirleft));			// 横に続く道のid
				SetField(idleft, FTYPE_ROAD, fdirleft);		// 分岐道の設置

				FIELD_DIRECTION fdirright = AddFieldDirection(GetPlayerDirection(), 1);			// 横に続く道の方向
				CHIP_ID idright = AddFieldID(id_start, GetFieldIDVector(fdirright));			// 横に続く道のid
				SetField(idright, FTYPE_ROAD, fdirright);		// 分岐道の設置

			}

			SetField(id_start, type, GetPlayerDirection());		// 分岐道の設置

			break;	// 分岐道後は設置しない
		}

		do {
			numRand = rand() % NUM_FTYPE_NORMAL + START_FTYPE_NORMAL;
		} while (Latesttype == numRand);	// 前回と同じものを作らない
		//numRand = FTYPE_CLIFFC;

		SetField(id_start, (FIELD_TYPE)numRand, GetPlayerDirection());		// 道の設置
		Latesttype = (FIELD_TYPE)numRand;
		cntNormal++;
	}
	g_idlate = id_start;
}

/*=====================================================================
フィールド制御更新関数
=====================================================================*/
void UpdateFieldControl()
{
	
	
}

/*=====================================================================
フィールド最終1取得関数
=====================================================================*/
Vec3 GetLatestFieldPosition()
{
	Vec3 ans;

	ans.x = FIELDCHIP_CENTER_X + (FIELDCHIP_WIDTH * g_idlate.vec2.x);
	ans.z = FIELDCHIP_CENTER_Z + (FIELDCHIP_HEIGHT * g_idlate.vec2.z);
	ans.y = 0.f;
	return ans;
}


