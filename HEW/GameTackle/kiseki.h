//=============================================================================
//
// 剣の軌跡処理 [kiseki.h]
// Author : GP11A292 24 高橋　翔輝
//
//=============================================================================
#ifndef _KISEKI_H_
#define _KISEKI_H_


//**************************************
// 種類
//**************************************
enum
{
	KISEKITYPE_BULLET_PLAYER = 0,	// プレイヤーの弾の爆発
	KISEKITYPE_BULLET_ENEMY,			// 敵の弾の爆発
	KISEKITYPE_MAX
};
// 多分実行したら画面死ぬのでヨロピクミン。でも実行します
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKiseki(int type);
void UninitKiseki(void);
void UpdateKiseki(void);
void DrawKiseki(void);

int SetKiseki(D3DXVECTOR3 nemoto1, D3DXVECTOR3 nemoto2, D3DXVECTOR3 sentan1, D3DXVECTOR3 sentan2, int nType,int col_type);

#endif
