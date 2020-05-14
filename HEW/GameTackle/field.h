/**********************************************************************
[[fieldヘッダー(field.h)]]
	作者：奥田　真規

	field.cppに関するヘッダー
***********************************************************************/
#pragma once

#define MAX_FIELD		(50)		// フィールドプール最大数
#define MAX_GIMMICK		(5)			// フィールド毎ギミック数

// 1フィールドの縦横サイズ
#define FIELDCHIP_WIDTH		(1000.0f)
#define FIELDCHIP_HEIGHT	(FIELDCHIP_WIDTH)

// 道のサイズ
#define FIELDROAD_X		(100.0f)
#define FIELDROAD_Y		(10.0f)	

#define PLAYER_POSX		(FIELDCHIP_WIDTH/2)
#define PLAYER_POSZ		(FIELDCHIP_HEIGHT/2)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// フィールドタイプ
enum FIELD_TYPE {
	FTYPE_ROAD,			// 一本道
	FTYPE_CLIFFR,		// 右半分崖の道
	FTYPE_CLIFFL,		// 左半分崖の道
	FTYPE_JUMP,			// イッツアジャンプワールド
	FTYPE_TURNLR,		// 左右分岐点の道
	FTYPE_TURNR,		// 右に曲がる道
	FTYPE_TURNL,		// 左に曲がる道

	MAX_FIELDTYPE		/* 最大数取得 */
};

// フィールド遷移
enum FIELD_STATE {
	FSTATE_NONE,		// 使用してない状態

	FSTATE_READY,		// 生成済み(プレイヤーの直線状にある)
	FSTATE_CURVE,		// 生成済み(プレイヤーの直線状にはない）

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
	bool(*CheckHit)(FIELD_CHIP*);
	void(*Update)(FIELD_CHIP*);
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
typedef struct FIELD_CHIP{
	FIELD_TYPE		Type;		// 道の種類(一応すぐ参照できるように保管)
	FIELD_STATE		State;		// 状態保管
	CHIP_ID			ID;			// CHIP場所から位置を判別するID
	FIELD_OBJFUNC*	pFunc;		// 道ごとに異なる更新関数等取得関数ポインタ

	Matrix			WldMat;		// 当フィールドのワールド行列

}FIELD_CHIP;

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// フィールド基本関数群
void InitField();
void UninitField();
void ResetField();
void UpdateField();
void DrawField();