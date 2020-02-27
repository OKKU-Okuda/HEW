//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 奥田 真規
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

typedef void* MySound;	// サウンドオブジェクトへのアドレス

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

/*=====================================================================
全体サウンド音量設定関数
	戻り値；void
	引数：Volume->音量
=====================================================================*/
void MySoundSetMasterVolume(float Volume);

/*=====================================================================
サウンド読み込み関数
	※ 読み込み失敗時nullが入る ※
	戻り値；MySound :サウンド機能
	引数：const char* filepath　:ファイルパス
=====================================================================*/
MySound MySoundCreate(const char* filepath);

/*=====================================================================
 サウンド複製関数
	サウンドの複製を行う関数　同時に同じサウンドを使用したい場合などに複製する
	元のサウンド同様使い終わったら開放関数を呼ぶ
	戻り値；MySound	:複製されたものが返る
	引数：MySound:サウンド機能
===================================================================== */
MySound MySoundClone(MySound sound);

/*=====================================================================
 サウンド削除関数
	内部で停止も行うのでサウンド停止関数を呼ぶ必要はない
	MySoundDelete		:任意のサウンドを開放
	MySoundDeleteAuto	:同じサウンドデータを使用しているサウンドを一気に開放
						 ※ 複製関数でコピーしたものが対象 ※
						 !!この関数を使用後コピーしたサウンドを開放しようと
						 !!しないこと(不安な場合はMySoundDeleteですべてを開放する)
	戻り値；void
	引数：MySound:サウンド機能
===================================================================== */
void MySoundDelete(MySound* sound);
void MySoundDeleteAuto(MySound* sound);

/*=====================================================================
サウンド再生関数
	この関数で実行すると最初から再生される
	MySoundPlayOnce		1回再生
	MySoundPlayEternal　止める関数を使用するまで再生
	MySoundPlay			任意回数分ループ再生
	MySoundPlayTemporary止めたところから再生(止めたことない場合は1回再生)
	戻り値；void
	引数：MySound :サウンド機能
=====================================================================*/
void MySoundPlayOnce(MySound sound);
void MySoundPlayEternal(MySound sound);
void MySoundPlay(MySound sound, DWORD cntLoop);
void MySoundPlayTemporary(MySound sound);

/*=====================================================================
 サウンド停止関数
		MySoundStop		:指定サウンド一つを止める
		MySoundStopAll	:全サウンド停止
	戻り値；void
	引数：MySound:サウンド機能
===================================================================== */
void MySoundStop(MySound sound);
void MySoundStopAll();

/*=====================================================================
 サウンド情報取得関数
		MySoundGetVolume	:任意のサウンド一つの音量を取得する
		MySoundGetPitch		:任意のサウンド一つの速度を取得する
		MySoundGetPlaying	:任氏のサウンド一つの再生状況を取得する
		MySoundGetNumGroup	:所属しているサウンドデータの配下数を取得する
	戻り値；void
	引数：MySound:サウンド機能
		　float	:???
===================================================================== */
float MySoundGetVolume(MySound sound);
float MySoundGetPitch(MySound sound);
bool MySoundGetPlaying(MySound sound);
DWORD MySoundGetNumGroup(MySound sound);

/*=====================================================================
 サウンド設定関数
		MySoundSetVolume	:任意のサウンド一つの音量を調整する
		MySoundSetVolumeAuto:同系統サウンドの音量を調整する
		MySoundSetPitch		:任意のサウンド一つの速度を調整する
		MySoundSetPitchAuto	:同系統サウンドの音量を調整する
	戻り値；void
	引数：MySound:サウンド機能
		　float	:???
===================================================================== */
void MySoundSetVolume(MySound sound, float Volume);
void MySoundSetVolumeAuto(MySound sound, float Volume);
void MySoundSetPitch(MySound sound, float Pitch);
void MySoundSetPitchAuto(MySound sound, float Pitch);

// サウンド基本関数群
HRESULT InitSound();
void UninitSound();

#endif
