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


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

static CHIP_ID GetFieldChipID(Vec3* pos);						// 3次元ワールド座標からIDを算出する

static FIELD_CHIP* SearchChipID(CHIP_ID id);					// 使用CHIPの中からIDを検索してアドレスを返す
static FIELD_CHIP* GetChipMemory();								// 空きCHIPを探してアドレスを返すだけ

static void SetOnFieldWk(FIELD_CHIP* pData);					// g_OnFieldのステータスを設置
static FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type);		// typeから独自関数構造体アドレスを検索する

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static FIELD_CHIP		g_Field[MAX_FIELD];
		
static struct {
	FIELD_CHIP* pChip;		// プレイヤー上に乗っていると思われるCHIPアドレス

	/*機能追加時使用*/

}g_OnField;		// フィールドワーク

static FIELD_DIRECTION	g_fdir;			// 現在の方向
static CHIP_ID			g_latestid;		// 一番最近設置したチャンク
	

/*=====================================================================
フィールド更新関数
=====================================================================*/
void UpdateField()
{

}

/*=====================================================================
フィールド描画関数
=====================================================================*/
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

/*=====================================================================
フィールド初期化関数
=====================================================================*/
void InitField()
{
	InitFieldRoad();

}

/*=====================================================================
フィールド終了化関数
=====================================================================*/
void UninitField()
{
	UninitFieldRoad();
}

/*=====================================================================
フィールドゲームリセット関数関数
=====================================================================*/
void ResetField()
{
	CHIP_ID id;

	for (int i = 0; i < MAX_FIELD; i++)
	{
		// 未使用状態
		g_Field[i].State = FSTATE_NONE;
	}

	ZeroMemory(&g_OnField, sizeof(g_OnField));

	// 方向の初期化
	g_fdir = FDIRECTION_0ZP;

	// 0,0にonField
	SetField(0, 0, FTYPE_ROAD, FDIRECTION_0ZP);
	id.vec2.x = 0;
	id.vec2.z = 0;
	SetOnFieldWk(SearchChipID(id));

	// ここからテスト
	SetField(0, 1, FTYPE_ROAD, FDIRECTION_0ZP);
	// ここからテスト
	SetField(0, 2, FTYPE_ROAD, FDIRECTION_0ZP);
	// ここからテスト
	SetField(0, 3, FTYPE_ROAD, FDIRECTION_0ZP);


	// ここからテスト
//	SetField(0, 1, FTYPE_ROAD, FDIRECTION_1XP);
	// ここからテスト
//	SetField(0, 2, FTYPE_ROAD, FDIRECTION_2ZM);
	// ここからテスト
//	SetField(0, 3, FTYPE_ROAD, FDIRECTION_3XM);

}

/*=====================================================================
フィールド設置関数
	戻り値 : void
	引数 : 
	short x,						:チャンク場所xの指定
	short z,						:同上ｚ
	FIELD_TYPE type,				:設置フィールドタイプ
	FIELD_DIRECTION fdirection		:方向
=====================================================================*/
void SetField(short x, short z, FIELD_TYPE type, FIELD_DIRECTION fdirection)
{
	FIELD_CHIP* keep_pt = NULL;
	CHIP_ID		id;

	id.vec2.x = x;
	id.vec2.z = z;

	keep_pt = SearchChipID(id);

	if (SearchChipID(id) == NULL)
	{// 引数箇所にチャンクが存在しない場合

		keep_pt = GetChipMemory();
		if (keep_pt == NULL)
		{// 確保できない場合
			return;
		}
	}

	if (fdirection == g_fdir)
	{	// 直線
		keep_pt->State = FSTATE_READY;
	}
	else
	{	// 別の方向
		keep_pt->State = FSTATE_CURVE;
	}

	// 数値代入
	keep_pt->ID			= id;
	keep_pt->Type		= type;
	keep_pt->pFunc		= SearchFieldObjFunc(type);

	// グローバル変数更新
	g_latestid			= id;

	// 算出
	GetMatrix(&keep_pt->WldMat,											// ワールド行列を求める
		&Vec3((FIELDCHIP_WIDTH / 2) + (FIELDCHIP_WIDTH * x), 0, (FIELDCHIP_HEIGHT / 2) + (FIELDCHIP_HEIGHT * z)),
		&Vec3(0, (D3DX_PI / 2)*(int)fdirection, 0));

	D3DXMatrixInverse(&keep_pt->InvWldMat, NULL, &keep_pt->WldMat);		// 上記逆行列

}

/*=====================================================================
プレイヤーフィールド上当たり判定関数
	(GIMMICK当たり判定は別途存在)
	道の障害物によってプレイヤーの位置を変えます

	戻り値 : void
	引数 : bool		true:道とプレイヤーが設置ている
=====================================================================*/
bool PlayerCheckHitOnField()
{
	CHIP_ID		id;
	bool		ans;

	if (GetKeyboardTrigger(DIK_F5))
	{
		*GetPlayerOld_Pos() = *GetPlayerPos();
	}

	if (GetPlayerPos()->y > 0.0f)
	{
		PrintDebugProc("プレイヤー浮遊中");
		return false;
	}

	id = GetFieldChipID(GetPlayerPos());		// プレイヤーのいるチャンクID算出

	if (id.bit != g_OnField.pChip->ID.bit)
	{// 保管変数とのidが違う場合
		FIELD_CHIP* keep_pt;

		keep_pt = SearchChipID(id);

		if (keep_pt == NULL)
		{// 検索にヒットしない場合はここで帰還する

			PrintDebugProc("[ERROR]プレイヤーに干渉させるチャンクが存在しません(ID:%d-%d)", id.vec2.x, id.vec2.z);
			return true;
		}

		SetOnFieldWk(keep_pt);
	}

	// プレイヤー(今と昔の座標)をCHIPローカル座標に変換する
	D3DXVec3TransformCoord(GetPlayerPos(), GetPlayerPos(), &g_OnField.pChip->InvWldMat);
	D3DXVec3TransformCoord(GetPlayerOld_Pos(), GetPlayerOld_Pos(), &g_OnField.pChip->InvWldMat);

	// 当たり判定
	ans = g_OnField.pChip->pFunc->CheckHit(g_OnField.pChip, GetPlayerPos(), GetPlayerOld_Pos());

#ifdef _DEBUG
	PrintDebugProc("[debug:field_chip]CHIP座標 %vec3", *GetPlayerPos());
#endif

	// プレイヤー(今と昔の座標)をワールド座標に変換する
	D3DXVec3TransformCoord(GetPlayerPos(), GetPlayerPos(), &g_OnField.pChip->WldMat);
	D3DXVec3TransformCoord(GetPlayerOld_Pos(), GetPlayerOld_Pos(), &g_OnField.pChip->WldMat);


#ifdef _DEBUG
	if (ans == false)
	{
		PrintDebugProc("[ERROR]プレイヤーが道の外にいます！落ちる！");
	}
#endif
	return ans;
}

/*=====================================================================
最新設置識別ID取得関数
=====================================================================*/
CHIP_ID GetLatestChipID()
{
	return g_latestid;
}

/*=====================================================================
フィールド方向取得関数
=====================================================================*/
FIELD_DIRECTION GetPlayerFieldDirection()
{
	return g_fdir;
}

/*=====================================================================
	cpp内関数
=====================================================================*/
FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type)
{
	switch (type)
	{
	case FTYPE_ROAD:
		return GetFieldRoadFunc();

	case FTYPE_CLIFFR:
		break;
	case FTYPE_CLIFFL:
		break;
	case FTYPE_JUMP:
		break;
	case FTYPE_TURNLR:
		break;
	case FTYPE_TURNR:
		break;
	case FTYPE_TURNL:
		break;
	case MAX_FIELDTYPE:
		break;
	default:
		break;
	}

	// デフォルトとして直線道を設置
	return GetFieldRoadFunc();
}

CHIP_ID GetFieldChipID(Vec3* pos)
{
	CHIP_ID ans;

	// 位置をチャンクサイズで割って整数化
	ans.vec2.x = (short)(pos->x / FIELDCHIP_WIDTH);
	ans.vec2.z = (short)(pos->z / FIELDCHIP_HEIGHT);

	// 負の場合-0はこわいのでそれぞれ1マイナスする
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

FIELD_CHIP* SearchChipID(CHIP_ID id)
{
	// 巡回
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == FSTATE_NONE)
		{// 未使用の場合は検索対象に入れない
			continue;
		}

		if (g_Field[i].ID.bit == id.bit)
		{// IDのチェック
			return &g_Field[i];
		}
	}

	// 検索IDが存在しない場合はnullを返す
	return NULL;
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

void SetOnFieldWk(FIELD_CHIP* pData)
{
	if (g_OnField.pChip != NULL)
	{	// 前のブロックを使用済みにする
		g_OnField.pChip->State = FSTATE_USED;
	}

	g_OnField.pChip = pData;					
	g_OnField.pChip->State = FSTATE_ONPLAYER;	// 状態設置
}

