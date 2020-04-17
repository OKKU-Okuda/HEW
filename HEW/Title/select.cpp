/**********************************************************************
[[Selectプログラム(Select.cpp)]]
	作者：奥田　真規

	タイトルボタン選択プログラム
***********************************************************************/
#include "../Core/fade.h"

#include "../Phase/Phase_GameTackle1.h"
#include "../Phase/Phase_Title.h"

#include "select.h"	
#include "bottons.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

#define SIZE_SELECTEFFECT		(Vec2(350,90))

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
static void SetSelectEffectActive();
static void SetSelectEffectPassive();

// 基本関数群
static void InitSelect(bool isFirst);
static void UninitSelect(bool isEnd);
static void UpdateSelect();

static void DrawSelectEffect();

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static OBJ_FUNC g_Func = { InitSelect,UninitSelect,UpdateSelect,NoFunction };

// セレクトエフェクト
static NormalFunc	g_updatefunc;		// エフェクト更新関数
static VERTEX_2D	g_vtx[NUM_VERTEX];	// ロゴ頂点
static Texture		g_tex;				// テクスチャ
static DWORD		g_cnt_show;			// 表示カウント


static DWORD		g_Select;			// ボタン

/*=====================================================================
Select更新関数
=====================================================================*/
void UpdateSelect()
{

	// 上下の選択
	if (GetKeyboardTrigger(DIK_UP))
	{
		g_Select = (g_Select - 1) % MAX_TITLESELECT;
		//MySoundPlayOnce(g_soundSelect);
	//	SetTitle3DRot(true);
		SetSelectEffectActive();
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_Select = (g_Select + 1) % MAX_TITLESELECT;
		//MySoundPlayOnce(g_soundSelect);
	//	SetTitle3DRot(false);
	//	SetSelectEffect();
		SetSelectEffectActive();
	}

	// 選択ボタンによって変わる処理
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		switch (g_Select)
		{
		case SELECT_STRAT:	// タックル1フェーズに移行
			GoNextPhase(GetPhaseGameTackle1Func());
			break;

		case SELECT_RANKING:
		case SELECT_CONFIG:
			GoNextPhase(GetPhaseTitleFunc());
			break;

		case SELECT_EXIT:	// ゲームの終了
			DestroyWindow(GetHandle());
			break;

		default:			// エラー出力
			MessageBox(0, 0, 0, 0);
			break;
		}
	}

	// エフェクトのテクスチャ更新処理
	g_updatefunc();


}

/*=====================================================================
Select描画関数
=====================================================================*/
void DrawSelectEffect()
{
	// エフェクトの描画
	Draw2DVertex(g_tex, g_vtx);
}

/*=====================================================================
Select初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitSelect(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// セレクトエフェクトのテクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/select_effect.png", &g_tex);
		return;
	}

	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// エフェクト頂点の設置
	MakeNormal2DVertex(0, g_vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &SIZE_SELECTEFFECT);

	SetSelectEffectPassive();

	g_Select = SELECT_STRAT;

}

/*=====================================================================
Select終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitSelect(bool isEnd)
{
	//---------------------------------------------------------------------
	//	その他の終了処理
	//---------------------------------------------------------------------

	if (isEnd == true)
	{
		return;
	}
	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------

	SAFE_RELEASE(g_tex);
}

/*=====================================================================
Select基本関数群取得関数
=====================================================================*/
OBJ_FUNC* GetSelectFunc()
{
	return &g_Func;
}

/*=====================================================================
選択番地取得関数
=====================================================================*/
DWORD GetSelect()
{
	return g_Select;
}

/*=====================================================================
選択エフェクトパッシブ関数(cpp_func)
=====================================================================*/
void UpdateSelectEffect()
{
	// 2フレームに1度テクスチャ座標が更新される
	Set2DTexPos(g_vtx, 1, 7, 0, g_cnt_show++ / 2);

	if (g_cnt_show % 15 == 0)
	{	// 一定以上進んだ場合は非表示にする
		SetSelectEffectPassive();
	}
}

/*=====================================================================
選択エフェクトアクティブ関数(cpp_func)
=====================================================================*/
void SetSelectEffectActive()
{
	g_Func.Draw		= DrawSelectEffect;
	g_updatefunc	= UpdateSelectEffect;

	g_cnt_show = 0ul;

	// 位置の設定
	Set2DVerTex(g_vtx, GetTitleBottonPosition(), &SIZE_SELECTEFFECT);
	Set2DVertexColor(g_vtx, *GetTitleBottonColor());
}

/*=====================================================================
選択エフェクトパッシブ関数(cpp_func)
=====================================================================*/
void SetSelectEffectPassive()
{
	g_Func.Draw		= NoFunction;
	g_updatefunc	= NoFunction;
}