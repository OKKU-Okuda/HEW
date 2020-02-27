//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : ���c �^�K
//
//=============================================================================
#include "sound.h"
#include "MyList.h"	// ���X�g�̎g�p
#include <tchar.h>

#define VOLUME_DEFAULT	(0.8f)	// �}�X�^�{�C�X�̉���(0~2f)
//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************

// �T�E���h�f�[�^�\����
typedef struct {
	WAVEFORMATEX	wfx;				// �T�E���h�t�@�C���̏ڍ׏��
	DWORD			SoundDataSize;		// �f�[�^�T�C�Y
	BYTE*			pSoundData;			// �T�E���h�f�[�^�̐擪�A�h���X(�ʏ킱�̍\���̂̒����ɔz�u�����)
	MyList			SourceList;			// �\�[�X�{�C�X�̃��X�g�i�Q�ƃJ�E���^���܂�[0�Ŏ��g������]�j
}SOUND_DATA;

// �\�[�X�T�E���h�Ǘ��\����
typedef struct {
	IXAudio2SourceVoice		*pSourceVoice;	// �\�[�X�{�C�X�I�u�W�F�N�g
	SOUND_DATA				*pTargetSound;	// �Ώۂɂ����T�E���h�f�[�^�ւ̃A�h���X
	FLOAT					Volume;			// ����
	FLOAT					Pitch;			// �s�b�`
	bool					isPlay;			// �Đ������ǂ���
}SOURCE_SOUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SourceStopSound(SOURCE_SOUND* pt);
void SourcePlaySound(SOURCE_SOUND* pt, DWORD cntLoop);
SOURCE_SOUND*	   CreateSourceVoice(SOUND_DATA* Sound_Data);
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static IXAudio2*				g_pXAudio2 = NULL;				// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
static IXAudio2MasteringVoice*	g_pMasteringVoice = NULL;		// �}�X�^�[�{�C�X
static MyList					g_DataList;						// �T�E���h�f�[�^���Ǘ����郊�X�g

static float					g_MainVolume;					// ���T�E���h���ʂ̐ݒ�

/*=====================================================================
 �T�E���h���擾�֐�
		MySoundGetVolume	:�C�ӂ̃T�E���h��̉��ʂ��擾����
		MySoundGetPitch		:�C�ӂ̃T�E���h��̑��x���擾����
		MySoundGetPlaying	:�C���̃T�E���h��̍Đ��󋵂��擾����
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
		�@float	:???
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
 �T�E���h�ݒ�֐�
		MySoundSetVolume	:�C�ӂ̃T�E���h��̉��ʂ𒲐�����
		MySoundSetVolumeAuto:���n���T�E���h�̉��ʂ𒲐�����
		MySoundSetPitch		:�C�ӂ̃T�E���h��̑��x�𒲐�����
		MySoundSetPitchAuto	:���n���T�E���h�̉��ʂ𒲐�����
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
		�@float	:???
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

	Sound_pt = source_pt->pTargetSound;								// �^�[�Q�b�g�̎擾

	// �����\�[�X�{�C�X�����񂵂ĊJ��
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

	Sound_pt = source_pt->pTargetSound;								// �^�[�Q�b�g�̎擾

	// �����\�[�X�{�C�X�����񂵂ĊJ��
	MyListResetIterator(Sound_pt->SourceList, true);
	while (MyListLoop(Sound_pt->SourceList, (void**)&source_pt))
	{
		MySoundSetPitch(source_pt, Pitch);
	}
}


/*=====================================================================
 �T�E���h��~�֐�
		MySoundStop		:�w��T�E���h����~�߂�
		MySoundStopAll	:�S�T�E���h��~
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
===================================================================== */
void MySoundStop(MySound sound)
{
	SOURCE_SOUND*		 source_pt = (SOURCE_SOUND*)sound;

	SourceStopSound(source_pt);
}

void MySoundStopAll()
{
	SOUND_DATA*			 data_adr;

	// �T�E���h�f�[�^�̏���
	MyListResetIterator(g_DataList, true);
	while (MyListLoop(g_DataList, (void**)&data_adr))
	{
		SOURCE_SOUND*		 sourse_adr;

		// �T�E���h�f�[�^�����̃\�[�X�{�C�X����
		MyListResetIterator(data_adr->SourceList, true);
		while (MyListLoop(data_adr->SourceList, (void**)&sourse_adr))
		{
			SourceStopSound(sourse_adr);
		}
	}
}

/*=====================================================================
�T�E���h�Đ��֐�
	���̊֐��Ŏ��s����ƍŏ�����Đ������
	MySoundPlayOnce		1��Đ�
	MySoundPlayEternal�@�~�߂�֐����g�p����܂ōĐ�
	MySoundPlay			�C�Ӊ񐔕����[�v�Đ�
	MySoundPlayTemporary�~�߂��Ƃ��납��Đ�(�~�߂����ƂȂ��ꍇ��1��Đ�)
	�߂�l�Gvoid
	�����FMySound :�T�E���h�@�\
=====================================================================*/
void MySoundPlayOnce(MySound sound)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// �T�E���h�Đ�
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, 0);
}

void MySoundPlayEternal(MySound sound)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// �T�E���h�Đ�
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, XAUDIO2_LOOP_INFINITE);
}

void MySoundPlay(MySound sound, DWORD cntLoop)
{
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	// �T�E���h�Đ�
	sound_pt = (SOURCE_SOUND*)sound;
	SourcePlaySound(sound_pt, cntLoop);
}

void MySoundPlayTemporary(MySound sound)
{
	XAUDIO2_VOICE_STATE xa2state;
	SOURCE_SOUND* sound_pt;
	if (sound == NULL)return;

	sound_pt = (SOURCE_SOUND*)sound;

	// ��Ԏ擾
	sound_pt->pSourceVoice->GetState(&xa2state);

	if (xa2state.pCurrentBufferContext != NULL)
	{// �L���[�Ƀo�b�t�@�����݂���ꍇ
		sound_pt->pSourceVoice->Start(0);
		sound_pt->isPlay = true;
	}
	else
	{// ���݂��Ȃ��ꍇ
		MySoundPlayOnce(sound);
	}
}

/*=====================================================================
�T�E���h�ǂݍ��݊֐�
	�� �ǂݍ��ݎ��s��null������ ��
	�߂�l�GMySound :�T�E���h�@�\
	�����Fconst char* filepath�@:�t�@�C���p�X
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

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(GetHandle(), "Xaudio2:���f�[�^�t�H���_�̓ǂݍ��݂Ɏ��s���܂����B", "�x���I", MB_ICONWARNING);
		return NULL;
	}

	// WAVE�t�@�C���̃`�F�b�N
	if (FAILED(CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return 0;
	}
	if (FAILED(ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition)))
	{
		MessageBox(GetHandle(), "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return 0;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(GetHandle(), "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
		return 0;
	}

	// �t�H�[�}�b�g�`�F�b�N
	if (FAILED(CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return 0;
	}
	if (FAILED(ReadChunkData(hFile, &Sound.wfx, dwChunkSize, dwChunkPosition)))
	{
		MessageBox(GetHandle(), "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return 0;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	if (FAILED(CheckChunk(hFile, 'atad', (DWORD *)&Sound.SoundDataSize, &dwChunkPosition)))
	{
		MessageBox(GetHandle(), "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return 0;
	}

	// ��������T�E���h�f�[�^�̃��X�g�I�u�W�F�N�g�쐬�ɓ���
	Sound_pt = (SOUND_DATA*)MyListCreateObjectBottom(g_DataList, Sound.SoundDataSize);
	Sound.pSoundData = (BYTE*)&Sound_pt[1];						// �T�E���h�̃f�[�^�擪�A�h���X�𒼉��ɒu��
	Sound.SourceList = MyListCreate(sizeof(SOURCE_SOUND), 0);	// ���X�g�̍쐬
	*Sound_pt = Sound;											// �m�ۂ����������̈�ɃR�s�[

	// �ǂݍ���
	if (FAILED(ReadChunkData(hFile, (void *)Sound_pt->pSoundData, Sound_pt->SoundDataSize, dwChunkPosition)))
	{
		MessageBox(GetHandle(), "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return 0;
	}

	// �\�[�X�{�C�X�쐬
	return 	CreateSourceVoice(Sound_pt);
}

/*=====================================================================
 �T�E���h�����֐�
	�T�E���h�̕������s���֐��@�����ɓ����T�E���h���g�p�������ꍇ�Ȃǂɕ�������
	���̃T�E���h���l�g���I�������J���֐����Ă�
	�߂�l�GMySound	:�������ꂽ���̂��Ԃ�
	�����FMySound:�T�E���h�@�\
===================================================================== */
MySound MySoundClone(MySound sound)
{
	SOURCE_SOUND*		 source_pt;

	if (sound == NULL)return NULL;
	source_pt = (SOURCE_SOUND*)sound;

	return CreateSourceVoice(source_pt->pTargetSound);
}

/*=====================================================================
 �T�E���h�폜�֐�
	�����Œ�~���s���̂ŃT�E���h��~�֐����ĂԕK�v�͂Ȃ�
	MySoundDelete		:�C�ӂ̃T�E���h���J��
	MySoundDeleteAuto	:�����T�E���h�f�[�^���g�p���Ă���T�E���h����C�ɊJ��
						 �� �����֐��ŃR�s�[�������̂��Ώ� ��
						 !!���̊֐����g�p��R�s�[�����T�E���h���J�����悤��
						 !!���Ȃ�����(�s���ȏꍇ��MySoundDelete�ł��ׂĂ��J������)
	�߂�l�Gvoid
	�����FMySound:�T�E���h�@�\
===================================================================== */
void MySoundDelete(MySound* sound)
{
	SOUND_DATA*				Sound_pt;
	SOURCE_SOUND*			source_pt;

	if (*sound == NULL)return;
	source_pt = (SOURCE_SOUND*)*sound;

	Sound_pt = source_pt->pTargetSound;								// �^�[�Q�b�g�̎擾

	SourceStopSound(source_pt);						// �T�E���h�̒�~
	source_pt->pSourceVoice->FlushSourceBuffers();	// �I�[�f�B�I�o�b�t�@�̍폜

	source_pt->pSourceVoice->DestroyVoice();						// �\�[�X�{�C�X�̊J��
	MyListDeleteObject(Sound_pt->SourceList, (void**)&source_pt);	// �\�[�X���X�g����폜
	
	if (Sound_pt->SourceList->numObj == 0)
	{// �f�[�^���̎Q�ƃJ�E���^���O�Ȃ̂ŃT�E���h�f�[�^���폜����
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

	Sound_pt = source_pt->pTargetSound;								// �^�[�Q�b�g�̎擾

	// �����\�[�X�{�C�X�����񂵂ĊJ��
	MyListResetIterator(Sound_pt->SourceList, true);
	while (MyListLoop(Sound_pt->SourceList, (void**)&source_pt))
	{
		SourceStopSound(source_pt);						// �T�E���h�̒�~
		source_pt->pSourceVoice->FlushSourceBuffers();	// �I�[�f�B�I�o�b�t�@�̍폜

		source_pt->pSourceVoice->DestroyVoice();						// �\�[�X�{�C�X�̊J��
		MyListDeleteObject(Sound_pt->SourceList, (void**)&source_pt);	// �\�[�X���X�g����폜
	}

	MyListDelete(&Sound_pt->SourceList);
	MyListDeleteObject(g_DataList, (void**)&Sound_pt);
}

/*=====================================================================
�T�E���h���ʐݒ�֐�
	�߂�l�Gvoid
	�����FVolume->����
=====================================================================*/
void MySoundSetMasterVolume(float Volume)
{
	if (g_MainVolume == Volume)return;

	SAFE_NUMBER(Volume, 0.0f, 2.0f);
	g_pMasteringVoice->SetVolume(Volume);
}

/*=====================================================================
�������T�E���h�֐�
	�߂�l�GHRESULT
	�����Fvoid
=====================================================================*/
HRESULT InitSound(void)
{
	//==============================================================================
	//�G���[�̏I�����͂ǂ���ɂ���main.cpp��Uninit�ɔ�Ԃ̂őS��UninitSound�ɔC����
	//==============================================================================

	// com�V�X�e�����g�p�\��
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		//���s������G���[���b�Z�[�W�ŏI��
		MessageBox(GetHandle(), _T("Xaudio2:COM�V�X�e���̎g�p���s�ł�"), _T("�G���["), MB_ICONERROR);
		return E_FAIL;
	}

	g_pXAudio2 = NULL;

	// XAudio2�̃V�X�e�����쐬
	if (XAudio2Create(&g_pXAudio2, 0))
	{
		//���s������G���[���b�Z�[�W�ŏI��
		MessageBox(GetHandle(), _T("Xaudio2:�I�[�f�B�I�V�X�e���̍쐬�Ɏ��s���܂���"), _T("�G���["), MB_ICONERROR);
		g_pXAudio2 = NULL;
		return E_FAIL;
	}


	// �}�X�^�[�{�C�X�̍쐬
	if (FAILED(g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice)))
	{
		//���s������G���[���b�Z�[�W�ŏI��
		MessageBox(GetHandle(), _T("Xaudio2:�}�X�^�[�{�C�X�̍쐬�Ɏ��s���܂����i�����j"), _T("�G���["), MB_ICONERROR);
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̉��ʐݒ�
	g_MainVolume = VOLUME_DEFAULT;
	g_pMasteringVoice->SetVolume(g_MainVolume);

	// �f�[�^���X�g�̍쐬
	g_DataList = MyListCreate(sizeof(SOUND_DATA), 16);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �\�[�X�{�C�X�ƃT�E���h�f�[�^�j��
	SOUND_DATA* sound_adr;
	MyListResetIterator(g_DataList, true);
	while (MyListLoop(g_DataList, (void**)&sound_adr))
	{
		SOURCE_SOUND* source_pt;
		source_pt = (SOURCE_SOUND*)MyListGetTopObject(sound_adr->SourceList);
		MySoundDeleteAuto((MySound*)&source_pt);
	}

	MyListDelete(&g_DataList);

	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �T�E���h�X�g�b�v(cpp_func)
//=============================================================================
void SourceStopSound(SOURCE_SOUND* pt)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	pt->pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		pt->pSourceVoice->Stop(0);
		pt->isPlay = false;

	}
}

//=============================================================================
// �T�E���h�v���C(cpp_func)
//=============================================================================
void SourcePlaySound(SOURCE_SOUND* pt, DWORD cntLoop)
{
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = pt->pTargetSound->SoundDataSize;
	buffer.pAudioData = pt->pTargetSound->pSoundData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = cntLoop;

	// �T�E���h�̊m�F
	SourceStopSound(pt);
	
	// �I�[�f�B�I�o�b�t�@�̍폜
	pt->pSourceVoice->FlushSourceBuffers();

	// �I�[�f�B�I�o�b�t�@�̓o�^
	pt->pSourceVoice->SubmitSourceBuffer(&buffer);
	
	// �Đ�
	pt->pSourceVoice->Start(0);

	pt->isPlay = true;
}

//=============================================================================
// �\�[�X�{�C�X�̍쐬(cpp_func)
//=============================================================================
SOURCE_SOUND* CreateSourceVoice(SOUND_DATA* Sound_Data)
{
	SOURCE_SOUND* Source_pt = (SOURCE_SOUND*)MyListCreateObjectBottom(Sound_Data->SourceList);

	// �\�[�X�{�C�X�쐬
	g_pXAudio2->CreateSourceVoice(&Source_pt->pSourceVoice, &Sound_Data->wfx);
	
	// �e�p�����[�^�}��
	Source_pt->Pitch = 1.0f;
	Source_pt->Volume = 1.0f;
	Source_pt->pTargetSound = Sound_Data;
	Source_pt->isPlay = false;
	return Source_pt;
}

//=============================================================================
// �`�����N�̃`�F�b�N(cpp_func)
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���(cpp_func)
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

