/**********************************************************************
[[Gimmickヘッダー(Gimmick.h)]]
	作者：奥田　真規

	Field/Gimmick/Gimmick.cppに関するヘッダー
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// GIMMICKタイプの指定
enum GIMMICK_TYPE {
	GTYPE_WALL,							// QTEの壁
	GTYPE_FORSLIDE,						// スライディングしないといけないやつ（丸太的な）

	MAX_GIMMICKTYPE
};

struct FIELD_CHIP;
struct GIMMICK_HEADER;
typedef struct {
	void(*Update)(GIMMICK_HEADER*, Vec3*);		// GIMMICK更新関数(第2引数はプレイヤーの位置)
	void(*Draw)(GIMMICK_HEADER*);				// GIMMICK描画関数
	void(*End)(GIMMICK_HEADER*);				// GIMMICK終了関数
}GIMMICK_FUNC;

typedef struct GIMMICK_HEADER{
	GIMMICK_FUNC*	pFunc;		// 個別関数
	GIMMICK_TYPE	Type;		// GIMMICKの種類
	bool			isActive;	// 起動しているか否か
}GIMMICK_HEADER;

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------


/*=====================================================================
GIMMICK設置関数
引数：
FIELD_CHIP* pChip,		:紐づけするフィールドCHIP
GIMMICK_TYPE Type,		:どのGIMMICKを出すか
Vec3* pPos,				:設置する場所(第1引数のCHIP座標）
DWORD option			:GIMMICKによって異なる引数
=====================================================================*/
void SetFieldGimmick(FIELD_CHIP* pChip, GIMMICK_TYPE Type, Vec3* pPos, DWORD option = 0);

/*=====================================================================
GIMMICK更新関数
=====================================================================*/
void FieldGimmickUpdate(FIELD_CHIP* pChip, Vec3* pos);

/*=====================================================================
GIMMICK描画関数
=====================================================================*/
void FieldGimmickDraw(FIELD_CHIP *pChip);

/*=====================================================================
フィールドアドレス指定のギミック削除関数
=====================================================================*/
void DeleteGimmickByFieldPtr(FIELD_CHIP* pChip);

// 基本関数群(field.cpp内に記述)
void InitFieldGimmick();
void UninitFieldGimmick();
void ResetFieldGimmick();
void EndFieldGimmick();