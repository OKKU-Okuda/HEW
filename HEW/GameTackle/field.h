/**********************************************************************
[[fieldヘッダー(field.h)]]
	作者：奥田　真規

	field.cppに関するヘッダー
***********************************************************************/
#pragma once
#include "item.h"
#include "Gimmick/Gimmick.h"

#define MAX_FIELD		(100)		// フィールドプール最大数
#define MAX_GIMMICK		(5)			// フィールド毎ギミック数

// 1フィールドの縦横サイズ
#define FIELDCHIP_WIDTH		(1000.0f)
#define FIELDCHIP_HEIGHT	(FIELDCHIP_WIDTH)

#define FIELDCHIP_CENTER_X	(FIELDCHIP_WIDTH/2 )
#define FIELDCHIP_CENTER_Z	(FIELDCHIP_HEIGHT/2 )
// 道のサイズ
#define FIELDROAD_X		(100.0f)
#define FIELDROAD_Y		(10.0f)	

// 道の壁のサイズ
#define ROADWALL_SIZEX	(5.0f)
#define ROADWALL_SIZEY	(20.0f)

// プレイヤーの足元半径
#define PLAYER_FIELDSIZE_R		(ROADWALL_SIZEX)

// プレイヤーの死亡確定座標
#define FIELD_FAILED_Y			(-30.0f)

#define NUM_FTYPE_NORMAL			(FTYPE_JUMP + 1 - FTYPE_ROAD)// 通常道数取得
#define START_FTYPE_NORMAL			(FTYPE_ROAD)					// 通常道の開始番地

// コイン設置ｙ座標
#define POSY_COIN	(10.0f)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// プレイヤーの走る方向列挙
enum FIELD_DIRECTION {
	FDIRECTION_0ZP,	// ↑
	FDIRECTION_1XP,	// →
	FDIRECTION_2ZM,	// ↓
	FDIRECTION_3XM,	// ←

	MAX_FIELDDIRECTION	/*最大数取得*/
};

// フィールドタイプ列挙
enum FIELD_TYPE {
	FTYPE_VOID,			// 奈落（何もない)

	// 道:NORMAL
	FTYPE_ROAD,			// 一本道
	FTYPE_CLIFFR,		// 右半分崖の道
	FTYPE_CLIFFC,		// 左右崖の道
	FTYPE_CLIFFL,		// 左半分崖の道
	FTYPE_JUMP,			// イッツアジャンプワールド

	// カーブ道:CURVE
	FTYPE_TURNLR,		// 左右分岐点の道
	//FTYPE_TURNR,		// 右に曲がる道
	FTYPE_TURNL,		// 左に曲がる道

	MAX_FIELDTYPE		/* 最大数取得 */
};

// フィールド遷移
enum FIELD_STATE {
	FSTATE_NONE,		// 使用してない状態

	FSTATE_READY,		// 生成済み(プレイヤーの直線状にある)
//	FSTATE_CURVE,		// 生成済み(プレイヤーの直線状にはない）

	FSTATE_ONPLAYER,	// プレイヤーが上にのっかってる
	FSTATE_USED,		// プレイヤーが通り過ぎたが描画上残す
};

typedef struct {
	int x;
//	int y;
	int z;
}Vec2_Int;

// フィールド独自関数構造体
struct FIELD_CHIP;	// 前方宣言
typedef struct {

	// CHIP当たり判定関数
	// 戻り値：bool     true:道に接してるか
	// Vec3*now		:現在のプレイヤーCHIP座標
	// Vec3*past	:1フレーム前のプレイヤーCHIP座標
	bool(*CheckHit)(FIELD_CHIP*, Vec3*now, Vec3*past);


	// CHIP更新関数
	// Vec3*now		:現在のプレイヤーCHIP座標
	void(*Update)(FIELD_CHIP*, Vec3*now);

	// CHIP描画関数
	void(*Draw)(FIELD_CHIP*);
}FIELD_OBJFUNC;

// CHIP識別ID共用体
union CHIP_ID {
	struct {
		short x;
		short z;
	}vec2;

	DWORD bit;
};

// フィールド
struct GIMMICK_HEADER;
typedef struct FIELD_CHIP{
	FIELD_TYPE		Type;		// 道の種類(一応すぐ参照できるように保管)
	FIELD_DIRECTION	Dir;		// 道の方向
	FIELD_STATE		State;		// 状態保管
	CHIP_ID			ID;			// CHIP場所から位置を判別するID
	FIELD_OBJFUNC*	pFunc;		// 道ごとに異なる更新関数等取得関数ポインタ

	ITEM*			paItem[MAXITEM_PERFIELD];	// 紐づけられたアイテムのメモリ先
	GIMMICK_HEADER* paGimmick[MAX_GIMMICK];		// 紐づけられたギミックのメモリ先

	Matrix			WldMat;		// 当フィールドのワールド行列
	Matrix			InvWldMat;	// 上記逆行列
}FIELD_CHIP;

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

/*=====================================================================
プレイヤーフィールド上当たり判定関数
	(GIMMICK当たり判定は別途存在)
	道の障害物によってプレイヤーの位置を変えます

	戻り値 : void
	引数 : bool		true:道とプレイヤーが設置ている
=====================================================================*/
bool PlayerCheckHitOnField();


/*=====================================================================
フィールド設置関数
	戻り値 : FIELD_CHIP*
	引数 :
	CHIP_ID id
	FIELD_TYPE type,				:設置フィールドタイプ
	FIELD_DIRECTION fdirection		:方向
=====================================================================*/
FIELD_CHIP* SetField(CHIP_ID id, FIELD_TYPE type, FIELD_DIRECTION fdirection);

/*=====================================================================
フィールド削除関数
=====================================================================*/
void DeleteField(FIELD_CHIP* pData);

/*=====================================================================
プレイヤーに接しているフィールドチャンク取得関数
=====================================================================*/
FIELD_CHIP* GetFieldOnPlayer();

/*=====================================================================
ID算出取得関数
=====================================================================*/
CHIP_ID GetChipID(short x, short z);

/*=====================================================================
基本ベクトル取得関数
=====================================================================*/
Vec3 GetFieldVector(FIELD_DIRECTION fdir);

/*=====================================================================
基本ベクトル取得関数
=====================================================================*/
CHIP_ID GetFieldIDVector(FIELD_DIRECTION fdir);

/*=====================================================================
ID加算関数
=====================================================================*/
CHIP_ID AddFieldID(CHIP_ID id1, CHIP_ID id2);

/*=====================================================================
タイプ別クリエイトカウンタ取得関数
=====================================================================*/
int GetCountCreateFieldType(FIELD_TYPE type);

/*=====================================================================
フィールド方向加算関数
=====================================================================*/
FIELD_DIRECTION AddFieldDirection(FIELD_DIRECTION fdir, int add);

/*=====================================================================
CHIP検索関数
=====================================================================*/
FIELD_CHIP* SearchChipID(CHIP_ID id);

// フィールド基本関数群
void InitField();
void UninitField();
void ResetField();
void UpdateField();
void DrawField();