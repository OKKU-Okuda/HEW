//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : 奥田 真規
//
//=============================================================================
#include "sound.h"
#include "MyList.h"	// リストの使用
#include <tchar.h>

#define VOLUME_DEFAULT	(0.8f)	// マスタボイスの音量(0~2f)
//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************

// サウンドデータ構造体
typedef struct {
	WAVEFORMATEX	wfx;				// サウンドファイルの詳細情報
	DWORD			SoundDataSize;		// データサイズ
	BYTE*			pSoundData;			// サウンドデータの先頭アドレス(通常この構造体の直下に配置される)
	MyList			SourceList;			// ソースボイスのリスト（参照カウンタを含む[0で自身も消す]）
}SOUND_DATA;

// ソースサウンド管理構造体
typedef struct {
	IXAudio2SourceVoice		*pSourceVoice;	// ソースボイスオブジェクト
	SOUND_DATA				*pTargetSound;	// 対象にしたサウンドデータへのアドレス
	FLOAT					Volume;			// 音量
	FLOAT					Pitch;			// ピッチ
	bool					isPlay;			// 再生中かどうか
}SOURCE_SOUND;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SourceStopSound(SOURCE_SOUND* pt);
void SourcePlaySound(SOURCE_SOUND* pt, DWORD cntLoop);
SOURCE_SOUND*	   CreateSourceVoice(SOUND_DATA* Sound_Data);
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static IXAudio2*				g_pXAudio2 = NULL;				// XAudio2オブジェクトへのインターフェイス
static IXAudio2MasteringVoice*	g_pMasteringVoice = NULL;		// マスターボイス
static MyList					g_DataList;						// サウンドデータを管理するリスト

static float					g_MainVolume;					// 総サウンド音量の設定

/*=====================================================================
 サウンド情報取得関数
		MySoundGetVolume	:任意のサウンド一つの音量を取得する
		MySoundGetPitch		:任意のサウンド一つの速度を取得する
		MySoundGetPlaying	:任氏のサウンド一つの再生状況を取得する
	戻り値；void
	引数：MySound:サウンド機能
		　float	:???
===================================================================== */
float MySoundGetVolume(MySound sound)
{
	SOURCE_SOUND* sound_pt = (SOURCE_SOUND*)sound;
	return 		sound_pt->Volume;
}

float MySoundGetPitch(MySound sound)
{
	SOURCE_SOUND* sound_pt = (SOURCE_SOUND*)sound;
	return 		sound_pt->Pitch;
}

bool MySoundGetPlaying(MySound sound)
{
	SOURCE_SOUND* sound_pt = (SOURCE_SOUND*)sound;
	return 		sound_pt->isPlay;
}

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
void MySoundSetVolume(MySound sound, float Volume)
{
	SOURCE_SOUND* sound_pt = (SOURCE_SOUND*)sound;

	SAFE_NUMBER(Volume, 0.0f, 5.0f);

	if (sound_pt->Volume != Volume)
	{
		sound_pt->Volume = Volume;
		sound_pt->pSourceVoice->SetVolume(Volume);
	}
}

void MySoundSetVolumeAuto(MySound sound, float Volume)
{
	SOUND_DATA*				Sound_pt;
	SOURCE_SOUND*			source_pt;

	if (sound == NULL)return;
	source_pt = (SOURCE_SOUND*)sound;

	Sound_pt = source_pt->pTargetSound;								// ターゲットの取得

	// 所属ソースボイスを巡回して開放
	MyListResetIterator(Sound_pt->SourceList, true);
	while (MyListLoop(Sound_pt->SourceList, (void**)&source_pt))
	{
		MySoundSetVolume(source_pt, Volume);
	}
}

void MySoundSetPitch(MySound sound,float Pitch)
{
	SOURCE_SOUND* sound_pt = (SOURCE_SOUND*)sound;

	SAFE_NUMBER(Pitch, XAUDIO2_MIN_FREQ_RATIO, 2.0f);

	if (sound_pt->Pitch != Pitch)
	{
		sound_pt->Pitch = Pitch;
		sound_pt->pSourceVoice->SetFrequencyRatio(Pitch);
	}
}

void MySoundSetPitchAuto(MySound sound, float Pitch)
{
	SOUND_DATA*				Sound_pt;
	SOURCE_SOUND*			source_pt;

	if (sound == NULL)return;
	source_pt = (SOURCE_SOUND*)sound;

	Sound_pt = source_pt->pTargetSound;								// ターゲットの取得

	// 所属ソースボイスを巡回して開放
	MyListResetIterator(Sound_pt->SourceList, true);
	while (MyListLoop(Sound_pt->SourceList, (void**)&source_pt))
	{
		MySoundSetPitch(source_pt, Pitch);
	}
}


/*=====================================================================
 サウンド停止関数
		MySoundStop		:指定サウンド一つを止める
		MySoundStopAll	:全サウンド停止
	戻り値；void
	引数：MySound:サウンド機能
===================================================================== */
void MySoundStop(MySound sound)
{
	SOURCE_SOUND*		 source_pt = (SOURCE_SOUND*)sound;

	SourceStopSound(source_pt);
}

void MySoundStopAll()
{
	SOUND_DATA*			 data_adr;

	// サウンドデータの巡回
	MyListResetIterator(g_DataList, true);
	while (MyListLoop(g_DataList, (void**)&data_adr))
	{
		SOURCE_SOUND*		 sourse_adr;

		// サウンドデータ直下のソースボイス巡回
		MyListResetIterator(data_adr->SourceList, true);
		while (MyListLoop(data_adr->SourceList, (void**)&sourse_adr))
		{
			SourceStopSound(sourse_adr);
		}
	}
}

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
void MySoundPlayOnce(MySound sound)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// サウンド再生
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, 0);
}

void MySoundPlayEternal(MySound sound)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// サウンド再生
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, XAUDIO2_LOOP_INFINITE);
}

void MySoundPlay(MySound sound, DWORD cntLoop)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// サウンド再生
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, cntLoop);
}

void MySoundPlayTemporary(MySound sound)
{
	XAUDIO2_VOICE_STATE xa2state;
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	sound_pt = (SOURCE_SOUND*)sound;

	// 状態取得
	sound_pt->pSourceVoice->GetState(&xa2state);

	if (xa2state.pCurrentBufferContext != NULL)
	{// キューにバッファが存在する場合
		sound_pt->pSourceVoice->Start(0);
		sound_pt->isPlay = true;
	}
	else
	{// 存在しない場合
		MySoundPlayOnce(sound);
	}
}

/*=====================================================================
サウンド読み込み関数
	※ 読み込み失敗時nullが入る ※
	戻り値；MySound :サウンド機能
	引数：const char* filepath　:ファイルパス
=====================================================================*/
MySound MySoundCreate(const char* filepath)
{
	SOUND_DATA	Sound;
	SOUND_DATA*	Sound_pt;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	HANDLE hFile;

	ZeroMemory(&Sound, sizeof(SOUND_DATA));

	// サウンドデータファイルの生成
	hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(GetHandle(), "Xaudio2:音データフォルダの読み込みに失敗しました。", "警告！", MB_ICONWARNING);
		return NULL;
	}

	// WAVEファイルのチェック
	if (FAILED(CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return 0;
	}
	if (FAILED(ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition)))
	{
		MessageBox(GetHandle(), "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return 0;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(GetHandle(), "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return 0;
	}

	// フォーマットチェック
	if (FAILED(CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return 0;
	}
	if (FAILED(ReadChunkData(hFile, &Sound.wfx, dwChunkSize, dwChunkPosition)))
	{
		MessageBox(GetHandle(), "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return 0;
	}

	// オーディオデータ読み込み
	if (FAILED(CheckChunk(hFile, 'atad', (DWORD *)&Sound.SoundDataSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return 0;
	}

	// ここからサウンドデータのリストオブジェクト作成に入る
	Sound_pt = (SOUND_DATA*)MyListCreateObjectBottom(g_DataList, Sound.SoundDataSize);
	Sound.pSoundData = (BYTE*)&Sound_pt[1];						// サウンドのデータ先頭アドレスを直下に置く
	Sound.SourceList = MyListCreate(sizeof(SOURCE_SOUND), 0);	// リストの作成
	*Sound_pt = Sound;											// 確保したメモリ領域にコピー

	// 読み込み
	if (FAILED(ReadChunkData(hFile, (void *)Sound_pt->pSoundData, Sound_pt->SoundDataSize, dwChunkPosition)))
	{
		MessageBox(GetHandle(), "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return 0;
	}

	// ソースボイス作成
	return 	CreateSourceVoice(Sound_pt);
}

/*=====================================================================
 サウンド複製関数
	サウンドの複製を行う関数　同時に同じサウンドを使用したい場合などに複製する
	元のサウンド同様使い終わったら開放関数を呼ぶ
	戻り値；MySound	:複製されたものが返る
	引数：MySound:サウンド機能
===================================================================== */
MySound MySoundClone(MySound sound)
{
	SOURCE_SOUND*		 source_pt;

	if (sound == NULL)return NULL;
	source_pt = (SOURCE_SOUND*)sound;

	return CreateSourceVoice(source_pt->pTargetSound);
}

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
void MySoundDelete(MySound* sound)
{
	SOUND_DATA*				Sound_pt;
	SOURCE_SOUND*			source_pt;

	if (*sound == NULL)return;
	source_pt = (SOURCE_SOUND*)*sound;

	Sound_pt = source_pt->pTargetSound;								// ターゲットの取得

	SourceStopSound(source_pt);						// サウンドの停止
	source_pt->pSourceVoice->FlushSourceBuffers();	// オーディオバッファの削除

	source_pt->pSourceVoice->DestroyVoice();						// ソースボイスの開放
	MyListDeleteObject(Sound_pt->SourceList, (void**)&source_pt);	// ソースリストから削除
	
	if (Sound_pt->SourceList->numObj == 0)
	{// データ側の参照カウンタが０なのでサウンドデータも削除する
		MyListDelete(&Sound_pt->SourceList);
		MyListDeleteObject(g_DataList, (void**)&Sound_pt);
	}
}

void MySoundDeleteAuto(MySound* sound)
{
	SOUND_DATA*				Sound_pt;
	SOURCE_SOUND*			source_pt;

	if (*sound == NULL)return;
	source_pt = (SOURCE_SOUND*)*sound;

	Sound_pt = source_pt->pTargetSound;								// ターゲットの取得

	// 所属ソースボイスを巡回して開放
	MyListResetIterator(Sound_pt->SourceList, true);
	while (MyListLoop(Sound_pt->SourceList, (void**)&source_pt))
	{
		SourceStopSound(source_pt);						// サウンドの停止
		source_pt->pSourceVoice->FlushSourceBuffers();	// オーディオバッファの削除

		source_pt->pSourceVoice->DestroyVoice();						// ソースボイスの開放
		MyListDeleteObject(Sound_pt->SourceList, (void**)&source_pt);	// ソースリストから削除
	}

	MyListDelete(&Sound_pt->SourceList);
	MyListDeleteObject(g_DataList, (void**)&Sound_pt);
}

/*=====================================================================
サウンド音量設定関数
	戻り値；void
	引数：Volume->音量
=====================================================================*/
void MySoundSetMasterVolume(float Volume)
{
	if (g_MainVolume == Volume)return;

	SAFE_NUMBER(Volume, 0.0f, 2.0f);
	g_pMasteringVoice->SetVolume(Volume);
}

/*=====================================================================
初期化サウンド関数
	戻り値；HRESULT
	引数：void
=====================================================================*/
HRESULT InitSound(void)
{
	//==============================================================================
	//エラーの終了化はどちらにしろmain.cppのUninitに飛ぶので全部UninitSoundに任せる
	//==============================================================================

	// comシステムを使用可能に
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		//失敗したらエラーメッセージで終了
		MessageBox(GetHandle(), _T("Xaudio2:COMシステムの使用が不可です"), _T("エラー"), MB_ICONERROR);
		return E_FAIL;
	}

	g_pXAudio2 = NULL;

	// XAudio2のシステムを作成
	if (XAudio2Create(&g_pXAudio2, 0))
	{
		//失敗したらエラーメッセージで終了
		MessageBox(GetHandle(), _T("Xaudio2:オーディオシステムの作成に失敗しました"), _T("エラー"), MB_ICONERROR);
		g_pXAudio2 = NULL;
		return E_FAIL;
	}


	// マスターボイスの作成
	if (FAILED(g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice)))
	{
		//失敗したらエラーメッセージで終了
		MessageBox(GetHandle(), _T("Xaudio2:マスターボイスの作成に失敗しました（多分）"), _T("エラー"), MB_ICONERROR);
		return E_FAIL;
	}

	// マスターボイスの音量設定
	g_MainVolume = VOLUME_DEFAULT;
	g_pMasteringVoice->SetVolume(g_MainVolume);

	// データリストの作成
	g_DataList = MyListCreate(sizeof(SOUND_DATA), 16);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// ソースボイスとサウンドデータ破棄
	SOUND_DATA* sound_adr;
	MyListResetIterator(g_DataList, true);
	while (MyListLoop(g_DataList, (void**)&sound_adr))
	{
		SOURCE_SOUND* source_pt;
		source_pt = (SOURCE_SOUND*)MyListGetTopObject(sound_adr->SourceList);
		MySoundDeleteAuto((MySound*)&source_pt);
	}

	MyListDelete(&g_DataList);

	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// サウンドストップ(cpp_func)
//=============================================================================
void SourceStopSound(SOURCE_SOUND* pt)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pt->pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		pt->pSourceVoice->Stop(0);
		pt->isPlay = false;

	}
}

//=============================================================================
// サウンドプレイ(cpp_func)
//=============================================================================
void SourcePlaySound(SOURCE_SOUND* pt, DWORD cntLoop)
{
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = pt->pTargetSound->SoundDataSize;
	buffer.pAudioData = pt->pTargetSound->pSoundData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = cntLoop;

	// サウンドの確認
	SourceStopSound(pt);
	
	// オーディオバッファの削除
	pt->pSourceVoice->FlushSourceBuffers();

	// オーディオバッファの登録
	pt->pSourceVoice->SubmitSourceBuffer(&buffer);
	
	// 再生
	pt->pSourceVoice->Start(0);

	pt->isPlay = true;
}

//=============================================================================
// ソースボイスの作成(cpp_func)
//=============================================================================
SOURCE_SOUND* CreateSourceVoice(SOUND_DATA* Sound_Data)
{
	SOURCE_SOUND* Source_pt = (SOURCE_SOUND*)MyListCreateObjectBottom(Sound_Data->SourceList);

	// ソースボイス作成
	g_pXAudio2->CreateSourceVoice(&Source_pt->pSourceVoice, &Sound_Data->wfx);
	
	// 各パラメータ挿入
	Source_pt->Pitch = 1.0f;
	Source_pt->Volume = 1.0f;
	Source_pt->pTargetSound = Sound_Data;
	Source_pt->isPlay = false;
	return Source_pt;
}

//=============================================================================
// チャンクのチェック(cpp_func)
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み(cpp_func)
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

