//========================================
// 
// �T�E���h����
// Author : AKIRA TANAKA
// |
// Arrange : ���� ����
// 
//========================================
// *** sound.cpp ***
//========================================
#include <stdio.h>
#include "sound.h"
#include "setting.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �`�����N�̃`�F�b�N
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

// �`�����N�f�[�^�̓ǂݍ���
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//****************************************
// �O���[�o���ϐ�
//****************************************
IXAudio2				*g_pXAudio2			// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
						= NULL;				//
IXAudio2MasteringVoice	*g_pMasteringVoice	// �}�X�^�[�{�C�X
						= NULL;				//
IXAudio2SourceVoice		*g_apSourceVoice	// �\�[�X�{�C�X
						[SOUND_LABEL_MAX]	//
						= {};				//
BYTE					*g_apDataAudio		// �I�[�f�B�I�f�[�^
						[SOUND_LABEL_MAX]	//
						= {};				//
DWORD					g_aSizeAudio		//
						[SOUND_LABEL_MAX]	// �I�[�f�B�I�f�[�^�T�C�Y
						= {};				//
float					g_aSoundVolume		// ����
						[SOUND_TYPE_MAX]	//
						= { 1.0f,1.0f };	//
float					g_aSettingVolume	// �ݒ�̉���
						[SOUND_TYPE_MAX];	//

// �T�E���h�̏��
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data\\BGM\\seadenden\\The story begins.wav"         , "�^�C�g��BGM[000]"        , -1, 1.0f , SOUND_TYPE_BGM },	// �^�C�g��BGM			[000]
	{ "data\\BGM\\seadenden\\living star.wav"              , "�X�e�[�WBGM[000]"        , -1, 1.0f , SOUND_TYPE_BGM },	// �X�e�[�WBGM			[000]
	{ "data\\BGM\\seadenden\\meteor dance.wav"             , "�X�e�[�WBGM[001]"        , -1, 1.0f , SOUND_TYPE_BGM },	// �X�e�[�WBGM			[001]
	{ "data\\BGM\\seadenden\\Mastermind.wav"               , "�{�XBGM[000]"            , -1, 1.0f , SOUND_TYPE_BGM },	// �{�XBGM				[000]
	{ "data\\BGM\\seadenden\\Defense system.wav"           , "�{�XBGM[001]"            , -1, 1.0f , SOUND_TYPE_BGM },	// �{�XBGM				[001]
	{ "data\\SE\\seadenden\\You take care of the rest.wav" , "�Q�[���N���ASE[000]"     , 0 , 1.0f , SOUND_TYPE_SE  },	// �Q�[���N���ASE		[000]
	{ "data\\SE\\seadenden\\Regeneration cannot.wav"       , "�Q�[���I�[�o�[SE[000]"   , 0 , 1.0f , SOUND_TYPE_SE  },	// �Q�[���I�[�o�[SE		[000]
	{ "data\\SE\\seadenden\\metallic clank2.wav"           , "�I��SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �I��SE				[000]
	{ "data\\SE\\seadenden\\beep1.wav"                     , "�I��SE[001]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �I��SE				[001]
	{ "data\\SE\\seadenden\\clink1.wav"                    , "����SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// ����SE				[000]
	{ "data\\SE\\seadenden\\beep5.wav"                     , "����SE[001]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// ����SE				[001]
	{ "data\\SE\\seadenden\\beep4.wav"                     , "�폜SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �폜SE				[000]
	{ "data\\SE\\seadenden\\beep5.wav"                     , "�|�[�Y�J�nSE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// �|�[�Y�J�nSE			[000]
	{ "data\\SE\\seadenden\\beep4.wav"                     , "�|�[�Y�I��SE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// �|�[�Y�I��SE			[000]
	{ "data\\SE\\seadenden\\system14.wav"                  , "�J�E���g�_�E��SE[000]"   , 0 , 1.0f , SOUND_TYPE_SE  },	// �J�E���g�_�E��SE		[000]
	{ "data\\SE\\seadenden\\beep1.wav"                     , "�X�R�A�J�E���gSE[000]"   , 0 , 0.5f , SOUND_TYPE_SE  },	// �X�R�A�J�E���gSE		[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "�e�L�X�g�J�E���gSE[000]" , 0 , 0.4f , SOUND_TYPE_SE  },	// �e�L�X�g�J�E���gSE	[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "�����L���O�Z�b�gSE[000]" , 0 , 1.0f , SOUND_TYPE_SE  },	// �����L���O�Z�b�gSE	[000]
	{ "data\\SE\\seadenden\\damage3.wav"                   , "�_���[�WSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// �_���[�WSE			[000]
	{ "data\\SE\\seadenden\\damage4.wav"                   , "�_���[�WSE[001]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// �_���[�WSE			[001]
	{ "data\\SE\\seadenden\\bell3.wav"                     , "�{�[�i�XSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// �{�[�i�XSE			[000]
	{ "data\\SE\\seadenden\\Accumulate2.wav"               , "�x��SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �x��SE				[000]
	{ "data\\SE\\seadenden\\shoot19.wav"                   , "�V���b�gSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// �V���b�gSE			[000]
	{ "data\\SE\\seadenden\\missile2.wav"                  , "�V���b�gSE[001]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// �V���b�gSE			[001]
	{ "data\\SE\\seadenden\\shoot1.wav"                    , "�V���b�gSE[002]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// �V���b�gSE			[002]
	{ "data\\SE\\seadenden\\shoot2.wav"                    , "�V���b�gSE[003]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// �V���b�gSE			[003]
	{ "data\\SE\\seadenden\\shoot12.wav"                   , "�V���b�gSE[004]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// �V���b�gSE			[004]
	{ "data\\SE\\seadenden\\overthrow5.wav"                , "����SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// ����SE				[000]
	{ "data\\SE\\seadenden\\explosion10.wav"               , "����SE[001]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// ����SE				[001]
	{ "data\\SE\\seadenden\\explosion2.wav"                , "����SE[002]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// ����SE				[002]
	{ "data\\SE\\seadenden\\explosion3.wav"                , "����SE[003]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// ����SE				[003]
	{ "data\\SE\\seadenden\\explosion10.wav"               , "����SE[004]"             , 0 , 0.2f , SOUND_TYPE_SE  },	// ����SE				[004]
	{ "data\\SE\\seadenden\\flame4.wav"                    , "����SE[005]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// ����SE				[005]
	{ "data\\SE\\seadenden\\shoot22.wav"                   , "�g�USE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �g�USE				[000]
	{ "data\\SE\\seadenden\\get6.wav"                      , "���b�N�I��SE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// ���b�N�I��SE			[000]
	{ "data\\SE\\seadenden\\open4.wav"                     , "�J��SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �J��SE				[000]
	{ "data\\SE\\seadenden\\metallic clank3.wav"           , "����SE[000]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// ����SE				[000]
	{ "data\\SE\\seadenden\\don1.wav"                      , "�Փ�SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �Փ�SE				[000]
	{ "data\\SE\\seadenden\\explosion9.wav"                , "�o��SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// �o��SE				[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "���nSE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// ���nSE				[000]
	{ "data\\SE\\seadenden\\pup7.wav"                      , "�l��SE[000]"             , 0 , 1.5f , SOUND_TYPE_SE  },	// �l��SE				[000]
};

//========== *** �T�E���h�̏����擾 ***
SOUNDINFO *GetSoundInfo(void) 
{
	return g_aSoundInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// CheckChunk�֐� - �`�����N�̃`�F�b�N -
//========================================
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

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//========================================
// ReadChunkData�֐� - �`�����N�f�[�^�̓ǂݍ��� -
//========================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadSound�֐� - �T�E���h�̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel)
{
	SOUNDINFO	*pSoundInfo	// �T�E���h�̏��̃|�C���^
		= GetSoundInfo();

	if (strcmp(aString, "����") == 0)
	{// �T�E���h�������̎��A
		// �T�E���h��-1�ɐݒ�
		*pLabel = (SOUND_LABEL)-1;
	}
	else
	{// �T�E���h�������łȂ����A
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++, pSoundInfo++)
		{
			if (strcmp(aString, pSoundInfo->pLoadName) == 0)
			{// �ǂݍ��ݖ�����v�������A
				// �T�E���h����
				*pLabel = (SOUND_LABEL)nCntSound;
				break;
			}
		}
	}
}

//========================================
// InitSound�֐� - �T�E���h�̏��������� -
//========================================
HRESULT InitSound(HWND hWnd)
{
	// �ݒ艹�ʂ�ǂݍ���
	g_aSettingVolume[SOUND_TYPE_BGM] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	g_aSettingVolume[SOUND_TYPE_SE] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;

	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//========================================
// UninitSound�֐� - �T�E���h�̏I������ -
//========================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//========================================
// PlaySound�֐� - �Z�O�����g�Đ�(�Đ����Ȃ��~) -
//========================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// ���ʂ�ݒ�
	g_apSourceVoice[label]->SetVolume(g_aSoundInfo[label].fVolume * g_aSoundVolume[g_aSoundInfo[label].type] * g_aSettingVolume[g_aSoundInfo[label].type]);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//========================================
// StopSound - �Z�O�����g��~(���x���w��) -
//========================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//========================================
// StopSound - �Z�O�����g��~(�S��) -
//========================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//========================================
// SoundTypeVolumeChange�֐� - ��ޖ��ɉ��ʕύX -
//========================================
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume)
{
	// ���ʂ𐧌�
	if (fVolume <= 0.0f) { fVolume = 0.0f; }
	if (fVolume >= 1.0f) { fVolume = 1.0f; }

	// ���ʂ�ݒ�
	g_aSoundVolume[type] = fVolume;

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// �T�E���h�̎�ނ���v�������A
			// ���ʂ�ݒ�
			g_apSourceVoice[nCntSound]->SetVolume(g_aSoundInfo[nCntSound].fVolume * g_aSoundVolume[type] * g_aSettingVolume[type]);
		}
	}
}

//========================================
// SetSettingVolume�֐� - �ݒ艹�ʂ�ݒ� -
//========================================
void SetSettingVolume(SOUND_TYPE type) 
{
	// ��ނɉ������ݒ艹�ʂ��擾
	if (type == SOUND_TYPE_BGM) {
		g_aSettingVolume[type] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	}
	else if (type == SOUND_TYPE_SE) {
		g_aSettingVolume[type] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;
	}
	
	// ��ޖ��ɉ��ʂ�K�p
	SoundTypeVolumeChange(type, g_aSoundVolume[type]);
}

//========================================
// StopSoundType�֐� - ��ޖ��ɍĐ���~ -
//========================================
void StopSoundType(SOUND_TYPE type)
{
	XAUDIO2_VOICE_STATE xa2state;
	
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// �T�E���h�̎�ނ���v�������A
			// ��Ԏ擾
			g_apSourceVoice[nCntSound]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{// �Đ���
				// �ꎞ��~
				g_apSourceVoice[nCntSound]->Stop(0);

				// �I�[�f�B�I�o�b�t�@�̍폜
				g_apSourceVoice[nCntSound]->FlushSourceBuffers();
			}
		}
	}
}
