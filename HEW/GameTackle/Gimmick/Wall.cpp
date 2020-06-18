/**********************************************************************
[[GIMMICKの壁プログラム(Gimmick/Wall.cpp)]]
	作者：奥田　真規

	壁を4つのキーで壊すプログラム
***********************************************************************/
#include "../../Core/main.h"
#include "../../Phase/Phase_GameTackle1.h"

#include "../field.h"
#include "Wall.h"
#include "Gimmick.h"

#include "../effect.h"
#include "../player.h"
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define NUM_POOL		(30)			// 保管領域数
#define Wk				GIMMICK_WALL*		// オブジェクトワークポインタ
#define var(name,arg)	Wk name = (Wk)arg;	// 親構造体アドレスの変換ﾏｸﾛ

#define RANGE_INPUTSTART		(750.0f)			// 入力受付範囲(プレイヤーとの距離)
#define RANGE_INPUTEND			(75.0f)				// 入力終了範囲	(同上)
#define RANGE_DOWN				(30.0f)				// 扉が沈没するまでの範囲

#define WALLSIZE_Z		(20.0f)
#define EFFECT_SIZE		(40.0f)
#define MAX_DOWNVOLUME	(5.0f)		// 壁が落ちるときの最大音量
#define MIN_DOWNVOLUME	(0.5f)		// 同上最小音量
#define DOWN_VOLUME		(0.15f)		// 減衰音量
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// 壁GIMMICK状態列挙
enum WALL_STATE {
	WSTATE_WAIT,
	WSTATE_FAILEDQTE,
	WSTATE_SUCCESSQTE,
};

// 壁GIMMICK構造体
typedef struct {
	GIMMICK_HEADER	Head;			// ヘッダ

	Matrix			WldMat;			// 壁のワールドマトリックス
	Vec3			Pos;			// 3次元座標(フィールド(chip)座標)
	Vec3			WldPos;			// 上記ワールド座標
	//Vec3			Rot;			// 回転

	GIMICKWALL_QTE	Type;			// QTEボタンの種類
	WALL_STATE		State;			// ボタンの状態
	float			lenZSuc;		// QTE成功したときのplayerz値との距離
	bool			isSoundPlaying;	// サウンドを流すときに

}GIMMICK_WALL;

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
static GIMMICK_WALL g_aGimmick[NUM_POOL];
static Texture		g_tex[MAX_GIMICKWALLQTE];
static Mesh			g_meshWall;	
static MySound		g_seWalldowm;	// 壁が終われる音
static MySound		g_seWallFailed;	// GIMMICKがミスった音
/*=====================================================================
壁初期化関数
=====================================================================*/
GIMMICK_HEADER *GetWallMemory(FIELD_CHIP* pChip, Vec3* pos, GIMICKWALL_QTE Type)
{
	for (int i = 0; i < NUM_POOL; i++)
	{// 未使用箇所を捜索

		float Roty = pChip->Dir*(D3DX_PI / 2);			// 回転軸の決定

		if (g_aGimmick[i].Head.isActive == true)
		{
			continue;
		}

		// ゼロクリア
		ZeroMemory(&g_aGimmick[i], sizeof(GIMMICK_WALL));

		g_aGimmick[i].Head.isActive = true;
		g_aGimmick[i].Head.Type		= GTYPE_WALL;
		g_aGimmick[i].Head.pFunc	= &g_Func;

		if (Type >= MAX_GIMICKWALLQTE || Type < 0)
		{// 引数のチェック
			Type = (GIMICKWALL_QTE)(rand() % MAX_GIMICKWALLQTE);
		}

		// z軸のみ引数を使用
		g_aGimmick[i].Pos = *pos;
		g_aGimmick[i].Pos.y = 0.0f;
		g_aGimmick[i].Pos.x = 0.0f;

		g_aGimmick[i].State = WSTATE_WAIT;
		g_aGimmick[i].Type = Type;
		
		// 位置からワールド座標を求める
		D3DXVec3TransformCoord(&g_aGimmick[i].WldPos, &g_aGimmick[i].Pos, &pChip->WldMat);
		GetMatrix(&g_aGimmick[i].WldMat, &g_aGimmick[i].WldPos, &Vec3(0, Roty, 0));

		return (GIMMICK_HEADER *)&g_aGimmick[i];
	}

	return NULL;	// 取得できない場合
}

/*=====================================================================
壁更新関数
=====================================================================*/
void UpdateGimmick(GIMMICK_HEADER* pHead, Vec3* pPos)
{
	var(pData, pHead);

	if (pData->State == WSTATE_WAIT && 
		pData->Pos.z - pPos->z < RANGE_INPUTSTART)
	{// 入力受付処理
		DWORD bitIpt = 0ul;								// トリガーされたものを格納したビット列
		DWORD bitType = 0x00000001 << pData->Type;		// 押されるべきビットを立てたビット列

		// ボタン押されたかのチェック
		if (GetKeyboardTrigger(DIK_1)|| IsButtonTriggered(0, BUTTON_A))
		{
			bitIpt |= 0x00000001 << GQTE_X;
		}

		if (GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_B))
		{
			bitIpt |= 0x00000001 << GQTE_Y;
		}

		//if (GetKeyboardTrigger(DIK_3))
		//{
		//	bitIpt |= 0x00000001 << GQTE_A;
		//}
		//
		//if (GetKeyboardTrigger(DIK_4))
		//{
		//	bitIpt |= 0x00000001 << GQTE_B;
		//}


		if (bitIpt)
		{// いづれかのボタンが押された場合の処理

			if (bitIpt & bitType)
			{// 指定ビットが押されている
				pData->State = WSTATE_SUCCESSQTE;
				pData->lenZSuc = pData->Pos.z - WALLSIZE_Z - pPos->z;

				pData->isSoundPlaying = true;

				// 再生
				MySoundSetVolume(g_seWalldowm, MIN_DOWNVOLUME);
				MySoundPlayOnce(g_seWalldowm);
				SetPlayerSE(PSE_WALLATTACK);
			}
			else
			{// 指定ビット以外が押されている
				pData->State = WSTATE_FAILEDQTE;
				MySoundPlayOnce(g_seWallFailed);
			}
		}
		else if (pData->Pos.z - pPos->z < RANGE_INPUTEND)
		{// 入力終了機関に入った場合も失敗扱い
			pData->State = WSTATE_FAILEDQTE;
			MySoundPlayOnce(g_seWallFailed);
		}

	}
	else if (pData->State == WSTATE_SUCCESSQTE)
	{// 入力成功演出処理

		float posY_rate = 1.f - ((pData->Pos.z - WALLSIZE_Z - pPos->z) / pData->lenZSuc);
		
		pData->WldMat.m[3][1] = -FIELDROAD_X * posY_rate;
		//Vec3 vec =  *GetPlayerPos()- Vec3(pData->WldMat.m[3]);
		//D3DXVec3Normalize(&vec, &vec);
		//pData->WldMat.m[3][0] = vec.x;
		//pData->WldMat.m[3][1] = vec.y;
		//pData->WldMat.m[3][2] = vec.z;

		if (posY_rate > 1.0f)
		{// 下がり切ったら音を下げる

			float volume_se = MySoundGetVolume(g_seWalldowm);

			MySoundSetVolume(g_seWalldowm, volume_se - DOWN_VOLUME);
		}
		else
		{
			// SEの音量調整
			float volume_se = MIN_DOWNVOLUME + (MAX_DOWNVOLUME - MIN_DOWNVOLUME)*posY_rate;

			MySoundSetVolume(g_seWalldowm, volume_se);

			if (GetCount() % 5 == 0)
			{
				// エフェクトを出す
				Vec3 pos = pData->WldPos;
		
				pos.x += ((rand() % 1000 - 500) / 1000.0f ) * WALLSIZE_Z*4;
				pos.z += ((rand() % 1000 - 500) / 1000.0f ) * WALLSIZE_Z*4;

				SetEffect(pos, EFFECT_SIZE, EFFECT_SIZE);
			}

		}

		if (GetCount() % 2 == 0)
		{// 指定フレーム毎にぐらぐら処理
			pData->WldMat.m[3][0] = pData->WldPos.x + (rand() % 1000 - 500) / 400.0f;
			pData->WldMat.m[3][2] = pData->WldPos.z + (rand() % 1000 - 500) / 400.0f;
		}

	}
	else if (pData->State == WSTATE_FAILEDQTE && pData->Pos.z - pPos->z < WALLSIZE_Z / 2)
	{// GAMEOVER処理
		GameTackle1End();
	}
}

/*=====================================================================
壁描画関数
=====================================================================*/
void DrawGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);
	D3DDEVICE;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pData->WldMat);

	pDevice->SetTexture(0, g_tex[pData->Type]);

	g_meshWall->DrawSubset(0);
}

/*=====================================================================
壁終了関数
=====================================================================*/
void EndGimmick(GIMMICK_HEADER* pHead)
{
	var(pData, pHead);

	if (pData->isSoundPlaying == true)
	{// 壁を壊したオブジェクトの場合は音を停止
	//	MySoundStop(g_seWalldowm);
	}
}

/*=====================================================================
壁初期化関数
=====================================================================*/
void InitFieldGimmickWall()
{
	D3DDEVICE;

	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));

	g_meshWall = Create3DBoxMesh(&Vec3(FIELDROAD_X, FIELDROAD_X, WALLSIZE_Z),
		&Vec3(0, FIELDROAD_X / 2, 0));

	int id = MessageBox(NULL, "テクスチャをパッド用に変更する？", "(´・ω・)", MB_YESNOCANCEL | MB_ICONQUESTION);

	if (id == IDYES)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall_TRY.png", &g_tex[0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall_SQA.png", &g_tex[1]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall1.png", &g_tex[0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall2.png", &g_tex[1]);
	}

	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall3.png", &g_tex[2]);
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gimmick_wall4.png", &g_tex[3]);

	g_seWalldowm = MySoundCreate("data/SE/WallDown.wav");
	g_seWallFailed = MySoundCreate("data/SE/WallFailed.wav");
	MySoundSetVolume(g_seWallFailed, 3.0f);
}

/*=====================================================================
壁終了化関数
=====================================================================*/
void UninitFieldGimmickWall()
{
	SAFE_RELEASE(g_meshWall);

	for (int i = 0; i < MAX_GIMMICKTYPE; i++)
	{
		SAFE_RELEASE(g_tex[i]);
	}

	MySoundDelete(&g_seWalldowm);
	MySoundDelete(&g_seWallFailed);
}

/*=====================================================================
壁ゲーム一時終了関数
=====================================================================*/
void AllEndFieldGimmickWall()
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
壁ゲームリセット関数
=====================================================================*/
void ResetFieldGimmickWall()
{
	// ゼロクリア
	ZeroMemory(g_aGimmick, sizeof(g_aGimmick));
}