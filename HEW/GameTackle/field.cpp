/**********************************************************************
[[fieldプログラム(field.cpp)]]
	作者：奥田　真規

	フィールド全般に関するプログラム
***********************************************************************/
#include "../Core/main.h"
#include "../Core/debugproc.h"

#include "field.h"
#include "player.h"
#include "player_control.h"
#include "field_control.h"
#include "item.h"
#include "Gimmick/Gimmick.h"

#include "Field/ResourceManager.h"
#include "Field/road.h"
#include "Field/void.h"
#include "Field/cliffR.h"
#include "Field/cliffC.h"
#include "Field/cliffL.h"
#include "Field/jump.h"
#include "Field/turnLR.h"

#include "../Phase/Phase_GameTackle1.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#if 0	// c言語なりにforeach作ったけど処理重いしこの前にポインタ変数宣言しなきゃいけないから使わない
#define foreach(val,arr)	\
int size = sizeof(arr) / sizeof(arr[0]);	\
val = &arr[0];								\
for(int i = 0; i < size; i++, val++)
#endif

#define RANGE_DELCHIPKEEP			(FIELDCHIP_WIDTH*0.75f)		// プレイヤーからどの程度離れたかでの削除基準(box判定)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

static CHIP_ID GetFieldChipID(Vec3* pos);						// 3次元ワールド座標からIDを算出する

static FIELD_CHIP* GetChipMemory();								// 空きCHIPを探してアドレスを返すだけ

static void SetOnFieldWk(FIELD_CHIP* pData);					// g_OnFieldのステータスを設置
static FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type, FIELD_CHIP* pData);// typeから独自関数構造体アドレスを検索する
static void SwapAllChipState(FIELD_STATE fst_target, FIELD_STATE fst_set);	// 状態を別の状態に変換する（すべて）

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static FIELD_CHIP		g_Field[MAX_FIELD];
		
static struct {
	FIELD_CHIP* pChip;		// プレイヤー上に乗っていると思われるCHIPアドレス

	/*機能追加時使用*/

}g_OnField;		// フィールドワーク

//static FIELD_DIRECTION	g_fdir;			// 現在の方向
//static CHIP_ID			g_latestid;		// 一番最近設置したチャンク
	

/*=====================================================================
フィールド更新関数
=====================================================================*/
void UpdateField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// 直線状にあるもののみ更新を行う
		if (g_Field[i].State == FSTATE_ONPLAYER ||
			(g_Field[i].State == FSTATE_READY && g_Field[i].Dir == GetPlayerDirection()))
		{
			Vec3 pos_player, keeppos;

			// プレイヤーワールド->CHIPローカル
			D3DXVec3TransformCoord(&pos_player, GetPlayerPos(), &g_Field[i].InvWldMat);

			keeppos = pos_player;

			g_Field[i].pFunc->Update(&g_Field[i], &pos_player);		
			FieldGimmickUpdate(&g_Field[i], &pos_player);			// GIMMICKのアップデート

			if (keeppos != pos_player)
			{// 更新関数後にポジション変更があった場合
				// CHIPローカル->プレイヤーワールド
				D3DXVec3TransformCoord(GetPlayerPos(), &pos_player, &g_Field[i].WldMat);
			}
		}

		// フィールド削除判断処理
		else if (g_Field[i].State == FSTATE_USED)
		{
			Vec3 pos_field(g_Field[i].WldMat.m[3]);		// ワールド座標取得

			// レンジ外にプレイヤーが出たら非表示にする
			if (fabsf(GetPlayerPos()->x - pos_field.x) >= RANGE_DELCHIPKEEP ||
				fabsf(GetPlayerPos()->z - pos_field.z) >= RANGE_DELCHIPKEEP)
			{
				DeleteField(&g_Field[i]);
			}
		}
	}

#ifdef _DEBUG
	PrintDebugProc("[debug_field]F3:フィールド生成しなおし");
	if (GetKeyboardTrigger(DIK_F3))
	{
		ResetPlayerPos();
		ResetItem();
		ResetField();
	}
#endif
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

		// GIMMICKの描画
		FieldGimmickDraw(&g_Field[i]);
	}
}

/*=====================================================================
フィールド初期化関数
=====================================================================*/
void InitField()
{
	InitFieldResource();
	InitFieldVoid();
	InitFieldRoad();
	InitFieldCliffR();
	InitFieldCliffC();
	InitFieldCliffL();
	InitFieldJump();
	InitFieldTurnLR();

	InitFieldGimmick();
}

/*=====================================================================
フィールド終了化関数
=====================================================================*/
void UninitField()
{
	UninitFieldResource();
	UninitFieldVoid();
	UninitFieldRoad();
	UninitFieldCliffR();
	UninitFieldCliffC();
	UninitFieldCliffL();
	UninitFieldJump();
	UninitFieldTurnLR();

	UninitFieldGimmick();
}

/*=====================================================================
フィールドゲームリセット関数関数
=====================================================================*/
void ResetField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		// 未使用状態
		g_Field[i].State = FSTATE_NONE;
	}

	ZeroMemory(&g_OnField, sizeof(g_OnField));

	EndFieldGimmick();
	ResetFieldGimmick();

	// 0,0にonField
	SetField(GetChipID(0, 0), FTYPE_ROAD, FDIRECTION_0ZP);
	SetOnFieldWk(SearchChipID(GetChipID(0, 0)));

	SpawnField(GetChipID(0, 0));
	// ここからテスト
//	SetField(0, 1, FTYPE_ROAD, FDIRECTION_1XP);
	// ここからテスト
//	SetField(0, 2, FTYPE_ROAD, FDIRECTION_2ZM);
	// ここからテスト
//	SetField(0, 3, FTYPE_ROAD, FDIRECTION_3XM);

}

/*=====================================================================
フィールド設置関数
	戻り値 : FIELD_CHIP* 
	引数 : 
	CHIP_ID id	
	FIELD_TYPE type,				:設置フィールドタイプ
	FIELD_DIRECTION fdirection		:方向
=====================================================================*/
FIELD_CHIP* SetField(CHIP_ID id, FIELD_TYPE type, FIELD_DIRECTION fdirection)
{
	FIELD_CHIP* keep_pt = NULL;

	keep_pt = SearchChipID(id);

	if (SearchChipID(id) == NULL)
	{// 引数箇所にチャンクが存在しない場合

		keep_pt = GetChipMemory();
		if (keep_pt == NULL)
		{// 確保できない場合
			return NULL;
		}
	}

	keep_pt->State = FSTATE_READY;

	// 算出
	GetMatrix(&keep_pt->WldMat,											// ワールド行列を求める
		&Vec3((FIELDCHIP_WIDTH / 2) + (FIELDCHIP_WIDTH * id.vec2.x), 0, (FIELDCHIP_HEIGHT / 2) + (FIELDCHIP_HEIGHT * id.vec2.z)),
		&Vec3(0, (D3DX_PI / 2)*(int)fdirection, 0));

	D3DXMatrixInverse(&keep_pt->InvWldMat, NULL, &keep_pt->WldMat);		// 上記逆行列

		// 数値代入
	keep_pt->ID = id;
	keep_pt->Dir = fdirection;
	keep_pt->Type = type;
	keep_pt->pFunc = SearchFieldObjFunc(type, keep_pt);

	return keep_pt;
}

/*=====================================================================
フィールド削除関数
=====================================================================*/
void DeleteField(FIELD_CHIP* pData)
{
	DeleteItemByFieldPtr(pData);		// アイテムの削除
	DeleteGimmickByFieldPtr(pData);		// ギミックの削除
	pData->State = FSTATE_NONE;
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

	if (GetPlayerPos()->y > 0.0f)
	{// 浮遊中は無条件で判定しない
		return false;
	}

	if (GetPlayerPos()->y >= FIELD_FAILED_Y)
	{// 床面座標域にいる場合
		Vec3 pos_pastkeep, pos_pastforfunc;	
		Vec3 pos_keep, pos_forfunc;

		id = GetFieldChipID(GetPlayerPos());		// プレイヤーのいるチャンクID算出

		if (id.bit != g_OnField.pChip->ID.bit)
		{// 保管変数とのidが違う場合
			FIELD_CHIP* keep_pt;

			keep_pt = SearchChipID(id);

			if (keep_pt == NULL)
			{// 検索にヒットしない場合はここで帰還する

				PrintDebugProc("[ERROR]プレイヤーに干渉させるチャンクが存在しません(ID:%d,%d)", id.vec2.x, id.vec2.z);
				return true;
			}

			SetOnFieldWk(keep_pt);
		}

		// プレイヤー(今と昔の座標)をCHIPローカル座標に変換する
		D3DXVec3TransformCoord(&pos_forfunc, GetPlayerPos(), &g_OnField.pChip->InvWldMat);
		D3DXVec3TransformCoord(&pos_pastforfunc, GetPlayerOld_Pos(), &g_OnField.pChip->InvWldMat);

		pos_pastkeep = pos_pastforfunc;
		pos_keep = pos_forfunc;

		// 当たり判定
		ans = g_OnField.pChip->pFunc->CheckHit(g_OnField.pChip, &pos_forfunc, &pos_pastforfunc);

#ifdef _DEBUG
		PrintDebugProc("[debug:field_chip]CHIP座標 %vec3", *GetPlayerPos());
#endif
		// プレイヤー(今と昔の座標)をワールド座標に変換する(変更の場合のみ）
		if (pos_keep != pos_forfunc)
		{
			D3DXVec3TransformCoord(GetPlayerPos(), &pos_forfunc, &g_OnField.pChip->WldMat);
		}
		if (pos_pastkeep != pos_pastforfunc)
		{
			D3DXVec3TransformCoord(GetPlayerOld_Pos(), &pos_pastforfunc, &g_OnField.pChip->WldMat);
		}
	}
	else
	{// 落下中  (床下にいる)     
	
		// 横位置固定
		GetPlayerPos()->x = GetPlayerOld_Pos()->x;
		GetPlayerPos()->z = GetPlayerOld_Pos()->z;

#ifdef _DEBUG
		PrintDebugProc("[debug:field_chip]y軸判定による落下確定処理 %vec3", *GetPlayerPos());
#endif
		GameTackle1End();

		ans = false;
	}

	return ans;
}

/*=====================================================================
ID算出取得関数
=====================================================================*/
CHIP_ID GetChipID(short x, short z)
{
	CHIP_ID ans;

	ans.vec2.x = x;
	ans.vec2.z = z;
	return ans;
}

/*=====================================================================
プレイヤーに接しているフィールドチャンク取得関数
=====================================================================*/
FIELD_CHIP* GetFieldOnPlayer()
{
	return g_OnField.pChip;
}

/*=====================================================================
基本ベクトル取得関数
=====================================================================*/
Vec3 GetFieldVector(FIELD_DIRECTION fdir)
{
	Vec3 ans(0, 0, 0);		// 戻り値算出用
	DWORD bit = fdir;		// 列挙型ビット演算

	if (bit & 0x00000001)
	{// 最下位ビットに１があれば横移動
		ans.x = 1.0f;
	}
	else
	{// 0であれば縦
		ans.z = 1.0f;
	}

	if (bit & 0x00000002)
	{// 2以上の場合は方向反転
		ans *= -1.0f;
	}

	return ans;
}

/*=====================================================================
基本ベクトル取得関数
=====================================================================*/
CHIP_ID GetFieldIDVector(FIELD_DIRECTION fdir)
{
	CHIP_ID ans = GetChipID(0, 0);
	DWORD bit = fdir;		// 列挙型ビット演算

	if (bit & 0x00000001)
	{// 最下位ビットに１があれば横移動
		ans.vec2.x = 1;
	}
	else
	{// 0であれば縦
		ans.vec2.z = 1;
	}

	if (bit & 0x00000002)
	{// 2以上の場合は方向反転
		ans.vec2.x *= -1;
		ans.vec2.z *= -1;
	}

	return ans;

}

/*=====================================================================
ID加算関数
=====================================================================*/
CHIP_ID AddFieldID(CHIP_ID id1, CHIP_ID id2)
{
	id1.vec2.x += id2.vec2.x;
	id1.vec2.z += id2.vec2.z;
	return id1;					// 加算場所
}

/*=====================================================================
フィールド方向加算関数
=====================================================================*/
FIELD_DIRECTION AddFieldDirection(FIELD_DIRECTION fdir, int add)
{
	int ans = fdir + add;

	if (ans < 0)
	{// 負の数は正の数に戻してから計算
		ans += MAX_FIELDDIRECTION * (ans / MAX_FIELDDIRECTION + 1);
	}

	ans %= MAX_FIELDDIRECTION;
	return (FIELD_DIRECTION)ans;
}

/*=====================================================================
CHIP検索関数
=====================================================================*/
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

/*=====================================================================
	cpp内関数
=====================================================================*/
FIELD_OBJFUNC* SearchFieldObjFunc(FIELD_TYPE type,FIELD_CHIP* pData)
{
	switch (type)
	{
	case FTYPE_VOID:
		return GetFieldVoidFunc();

	case FTYPE_ROAD:
		AwakeFieldRoad(pData);
		return GetFieldRoadFunc();

	case FTYPE_CLIFFR:
		return GetFieldCliffRFunc();

	case FTYPE_CLIFFC:
		return GetFieldCliffCFunc();

	case FTYPE_CLIFFL:
		return GetFieldCliffLFunc();

	case FTYPE_JUMP:
		return GetFieldJumpFunc();

	case FTYPE_TURNLR:
		return GetFieldTurnLRFunc();

	//case FTYPE_TURNR:
	//	break;
	//case FTYPE_TURNL:
	//	break;
	case MAX_FIELDTYPE:
		break;
	default:
		break;
	}

	// デフォルトとして奈落を設置
	return GetFieldVoidFunc();
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



FIELD_CHIP* GetChipMemory()
{
	// 巡回
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State != FSTATE_NONE)
		{// 使用中のはスキップ
			continue;
		}

		// ゼロクリア
		ZeroMemory(&g_Field[i], sizeof(FIELD_CHIP));

		return &g_Field[i];
	}

	// すべて使用していた場合はnullを返す
	return NULL;
}

void SwapAllChipState(FIELD_STATE fst_target,FIELD_STATE fst_set)
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (g_Field[i].State == fst_target)
		{
			g_Field[i].State = fst_set;
		}
	}
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

