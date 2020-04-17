/**********************************************************************
[[フェーズ:Titleプログラム(Phase_Title.cpp)]]
	作者：奥田　真規

	フェーズ：Phase_Titleに関するプログラム
***********************************************************************/
#include"../Core/main.h"	
#include "../Core/fade.h"
#include "../Core/input.h"
#include "../Core/sound.h"
#include "../Core/camera.h"
#include "../Core/debugproc.h"

#include "Phase_Title.h"				
#include "Phase_GameTackle1.h"				// 次のフェーズ
#include "../Title/effect.h"				// タイトルエフェクト
#include "../Title/player.h"				// プレイヤー
#include "../Title/bottons.h"				// ボタン
#include "../Title/select.h"				// 選択処理
//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

// 画面遷移基本関数群をまとめておく
static PHASE_FUNC	g_PhaseFunc = { InitTitle,UninitTitle,UpdateTitle,DrawTitle };
static MySound		g_soundBGM;
static MySound		g_soundSelect;		// 選択音




static struct {
	VERTEX_2D  vtx[NUM_VERTEX];	// ロゴ頂点
	Texture    tex;				// テクスチャ
	float	   col;				// 色
}g_Logo;				// ロゴワーク


/*=====================================================================
Title更新関数
=====================================================================*/
void UpdateTitle()
{
	PrintDebugProc("タイトルフェーズ");


	// 後ろにある四角形エフェクトの更新
	UpdateTitleEffect();

	if (GetFade() == FADE_NONE)
	{ // フェードが発生していない場合に実行

		// プレイヤーの更新
		GetPlayerFunc()->Update();

		if (GetPlayerPosition()->z<=-105)
		{	// プレイヤーが指定位置にいる場合

			// ロゴの更新
			g_Logo.col += 0.02f;
			Set2DVertexColor(g_Logo.vtx, Color(g_Logo.col, g_Logo.col, g_Logo.col, g_Logo.col));

			// 選択の処理とそのエフェクトの更新
			GetSelectFunc()->Update();

			// ボタンの更新
			GetBottonsFunc()->Update();
		}
	}

}

/*=====================================================================
Title描画関数
=====================================================================*/
void DrawTitle()
{
	DWORD	d3drslightning;
	D3DDEVICE;

	// ライティングの設定
	pDevice->GetRenderState(D3DRS_LIGHTING, &d3drslightning);	
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// プレイヤーの描画
	GetPlayerFunc()->Draw();

	DrawTitleEffect();				// 周囲に舞っているエフェクトの描画

	// 元のライティング状態に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, d3drslightning);

	// ボタンの描画
	GetBottonsFunc()->Draw();

	// 選択エフェクトの描画処理
	GetSelectFunc()->Draw();

	// ロゴの描画
	pDevice->SetTexture(0, g_Logo.tex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_Logo.vtx, sizeof(VERTEX_2D));

}

/*=====================================================================
Title初期化関数
	戻り値 : void
	引数 :
	bool isFirst		true:リソース読み込み系を含めた初期化処理を行う
							　※ この処理がある場合はmain.cpp内の		  ※
							  ※ InitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の初期化処理を行う
=====================================================================*/
void InitTitle(bool isFirst)
{
	if (isFirst == true)
	{
		//---------------------------------------------------------------------
		//	リソース読み込み処理(Create???,Load???,シリーズ)
		//---------------------------------------------------------------------
		D3DDEVICE;

		// タイトルロゴのテクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleLogo_000.png", &g_Logo.tex);


		// BGM,SEの読み込み
		g_soundBGM		= MySoundCreate("data/BGM/Title.wav");
		g_soundSelect	= MySoundCreate("data/SE/select.wav");
		MySoundSetVolume(g_soundSelect, 3.0f);



		InitTitleEffect(true);

		// プレイヤーのリソース読み込み
		GetPlayerFunc()->Init(true);

		// ボタンのリソース読み込み
		GetBottonsFunc()->Init(true);

		// 選択エフェクトのリソース読み込み
		GetSelectFunc()->Init(true);

		return;
	}


	InitTitleEffect(false);
	

	MySoundSetMasterVolume(0.1f);
	//---------------------------------------------------------------------
	//	グローバル変数等のステータス書き換え処理
	//---------------------------------------------------------------------

	// カメラ
#if 1
	GetCamera()->pos	= Vec3(0.0f, 0.0f, -200.0f);	
	GetCamera()->at		= Vec3(-100.0f, 0.0f, 0);
#else
	GetCamera()->pos	= Vec3(0.0f, 0.0f, 200.0f);
	GetCamera()->at		= Vec3(200.0f, 0.0f, -200.0f);
#endif

	// プレイヤー初期化
	GetPlayerFunc()->Init(false);

	// ボタン初期化
	GetBottonsFunc()->Init(false);

	// 選択エフェクとの描画
	GetSelectFunc()->Init(false);

	MySoundPlayEternal(g_soundBGM);	// 永遠再生
	


	// ロゴ頂点の設置
	MakeNormal2DVertex(0, g_Logo.vtx, &Vec3(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3.0f, 0), &Vec2(350, 120));
	Set2DVertexColor(g_Logo.vtx, Color(1.0f, 1.0f, 1.0f, 0.0f));
	g_Logo.col = 0.0f;


}

/*=====================================================================
Title終了化関数
	戻り値 : void
	引数 :
	bool isEnd			true:リソース開放系を含めた終了化処理を行う
							　※ この処理がある場合はmain.cpp内の		    ※
							  ※ UninitGameResource関数に引数trueで書くこと ※

						false;リソース開放系以外の終了化処理を行う	
=====================================================================*/
void UninitTitle(bool isEnd)
{

//---------------------------------------------------------------------
//	その他の終了処理
//---------------------------------------------------------------------

	MySoundStop(g_soundBGM);	// 停止
	MySoundStop(g_soundSelect);

	GetPlayerFunc()->Uninit(false);
	UninitTitleEffect(false);



	if (isEnd == false)
	{
		return;
	}

	//---------------------------------------------------------------------
	//	リソース開放処理
	//---------------------------------------------------------------------


	// サウンドの開放
	MySoundDeleteAuto(&g_soundBGM);
	MySoundDeleteAuto(&g_soundSelect);

	SAFE_RELEASE(g_Logo.tex);

	UninitTitleEffect(true);
	

}


/*=====================================================================
Title基本関数群取得関数
=====================================================================*/
PHASE_FUNC* GetPhaseTitleFunc()
{
	return &g_PhaseFunc;
}

