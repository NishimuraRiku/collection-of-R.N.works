//========================================
// 
// �T�E���h����
// Author:AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
// *** sound.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// �}�N����`
//****************************************
// �T�E���h�̃f�[�^�t�@�C���̑��΃p�X
#define SOUND_DATA_FILE_PATH "data\\SOUND_LIST.txt"
// �T�E���h�̍ő吔
#define SOUND_MAX (64)
// �T�E���h�̍Đ��ő吔
#define SOUND_PLAY_MAX (8)

//****************************************
// �\���̒�`
//****************************************
// �T�E���h�̊Ǘ����
typedef struct
{
	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2 *pXAudio2 = NULL;
	// �}�X�^�[�{�C�X
	IXAudio2MasteringVoice *pMasteringVoice = NULL;
	// �\�[�X�{�C�X
	IXAudio2SourceVoice *apSourceVoice[SOUND_MAX][SOUND_PLAY_MAX] = {};
	// �I�[�f�B�I�f�[�^
	BYTE *apDataAudio[SOUND_MAX] = {};
	// �I�[�f�B�I�f�[�^�T�C�Y
	DWORD aSizeAudio[SOUND_MAX] = {};
	// ���ޖ��̉���
	float aSoundVolume[SOUND_TYPE_MAX] = { 1.0f,1.0f };
	// ���ޖ��̐ݒ�̉���
	float aSettingVolume[SOUND_TYPE_MAX];
	// �T�E���h���Đ����Ă��邩�t���O
	bool aPlaySound[SOUND_MAX][SOUND_PLAY_MAX] = { false };
}SoundControl;

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
// �T�E���h�̏��
static SOUNDINFO g_aSoundInfo[SOUND_MAX];
// �T�E���h�̐�
static int g_nSoundNum;
// �T�E���h�̊Ǘ����
static SoundControl g_soundControl;

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
// Author:AKIRA TANAKA
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
// Author:AKIRA TANAKA
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

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================

//========================================
// InitSound�֐� - �T�E���h�̏��������� -
// Author:AKIRA TANAKA
// Arrange:RIKU NISHIMURA
//========================================
HRESULT InitSound(HWND hWnd)
{
	// �ݒ艹�ʂ�ǂݍ���
	g_soundControl.aSettingVolume[SOUND_TYPE_BGM] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	g_soundControl.aSettingVolume[SOUND_TYPE_SE] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;

	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_soundControl.pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = g_soundControl.pXAudio2->CreateMasteringVoice(&g_soundControl.pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (g_soundControl.pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_soundControl.pXAudio2->Release();
			g_soundControl.pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
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
		hFile = CreateFile(g_aSoundInfo[nCntSound].aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			char aString[TXT_MAX] = "";
			sprintf(aString, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1) (�J�E���g%d)", nCntSound);
			MessageBox(hWnd, aString, "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_soundControl.aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_soundControl.apDataAudio[nCntSound] = (BYTE*)malloc(g_soundControl.aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_soundControl.apDataAudio[nCntSound], g_soundControl.aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
		{
			// �\�[�X�{�C�X�̐���
			hr = g_soundControl.pXAudio2->CreateSourceVoice(&g_soundControl.apSourceVoice[nCntSound][nCntPlay], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �I�[�f�B�I�o�b�t�@�̓o�^
			g_soundControl.apSourceVoice[nCntSound][nCntPlay]->SubmitSourceBuffer(&buffer);
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_soundControl.aSizeAudio[nCntSound];
		buffer.pAudioData = g_soundControl.apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//========================================
// UninitSound�֐� - �T�E���h�̏I������ -
// Author:AKIRA TANAKA
//========================================
void UninitSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
		{
			if (g_soundControl.apSourceVoice[nCntSound][nCntPlay] != NULL)
			{
				// �ꎞ��~
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->Stop(0);

				// �\�[�X�{�C�X�̔j��
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->DestroyVoice();
				g_soundControl.apSourceVoice[nCntSound][nCntPlay] = NULL;
			}

			// �I�[�f�B�I�f�[�^�̊J��
			free(g_soundControl.apDataAudio[nCntSound]);
			g_soundControl.apDataAudio[nCntSound] = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	g_soundControl.pMasteringVoice->DestroyVoice();
	g_soundControl.pMasteringVoice = NULL;

	if (g_soundControl.pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_soundControl.pXAudio2->Release();
		g_soundControl.pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//========================================
// UpdateSound�֐� - �T�E���h�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateSound(void)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
		{
			// ��Ԏ擾
			g_soundControl.apSourceVoice[nCntSound][nCntPlay]->GetState(&xa2state);

			if ((g_soundControl.aPlaySound[nCntSound][nCntPlay]) && (xa2state.BuffersQueued == 0))
			{// �Đ����t���O���^ & �Đ����łȂ����A
				// �Đ����t���O���U�ɂ���
				g_soundControl.aPlaySound[nCntSound][nCntPlay] = false;

				// �ꎞ��~
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->Stop(0);

				// �I�[�f�B�I�o�b�t�@�̍폜
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->FlushSourceBuffers();
			}
		}
	}
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadSound�֐� - �T�E���h�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadSound(void)
{
	// �T�E���h�̏��̃|�C���^
	SOUNDINFO *pSoundInfo = g_aSoundInfo;
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX] = "";	// �f�[�^�����p
	
	// �T�E���h����������
	g_nSoundNum = 0;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(SOUND_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))
		{// �ǂݍ��݂��I��
			fclose(pFile); break;
		}
		if (!strcmp(aDataSearch, "SOUND"))
		{
			fscanf(pFile, "%s", &pSoundInfo->aFileName);	// �t�@�C����
			fscanf(pFile, "%d", &pSoundInfo->nCntLoop);		// ���[�v�J�E���g
			fscanf(pFile, "%f", &pSoundInfo->fVolume);		// ����
			fscanf(pFile, "%d", &pSoundInfo->type);			// ���
			pSoundInfo++;	// �|�C���^��i�߂�
			g_nSoundNum++;	// �T�E���h�������Z
		}
	}
}

//============================================================
//--------------------| *** ���� *** |------------------------
//============================================================

//========================================
// PlaySound�֐� - �Z�O�����g�Đ�(�Đ����Ȃ��~) -
// Author:AKIRA TANAKA
// Arrange:RIKU NISHIMURA
//========================================
HRESULT PlaySound(int nSoundIdx)
{
	if (nSoundIdx == -1)
	{// �T�E���h�ԍ���-1�̎��A
		return S_OK;	// �������I������
	}

	if (g_aSoundInfo[nSoundIdx].type == SOUND_TYPE_BGM)
	{// �T�E���h�̎�ނ�BGM���������A
		StopSoundType(SOUND_TYPE_BGM);	// BGM���ꊇ��~����
	}

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_soundControl.aSizeAudio[nSoundIdx];
	buffer.pAudioData = g_soundControl.apDataAudio[nSoundIdx];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[nSoundIdx].nCntLoop;

	for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
	{
		if (g_soundControl.aPlaySound[nSoundIdx][nCntPlay]) 
		{// �Đ����̎��A������܂�Ԃ�
			continue;
		}

		// ��Ԏ擾
		g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->GetState(&xa2state);

		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			// �ꎞ��~
			g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->SubmitSourceBuffer(&buffer);

		// ���ʂ�ݒ�
		g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->SetVolume(g_aSoundInfo[nSoundIdx].fVolume * g_soundControl.aSoundVolume[g_aSoundInfo[nSoundIdx].type] * g_soundControl.aSettingVolume[g_aSoundInfo[nSoundIdx].type]);

		// �Đ�
		g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->Start(0);
	}

	return S_OK;
}

//========================================
// StopSound - �Z�O�����g��~(���x���w��) -
// Author:AKIRA TANAKA
//========================================
void StopSound(int nSoundIdx)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
	{
		// ��Ԏ擾
		g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			// �ꎞ��~
			g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_soundControl.apSourceVoice[nSoundIdx][nCntPlay]->FlushSourceBuffers();
		}
	}
}

//========================================
// StopSound - �Z�O�����g��~(�S��) -
// Author:AKIRA TANAKA
//========================================
void StopSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
		{
			if (g_soundControl.apSourceVoice[nCntSound][nCntPlay] != NULL)
			{
				// �ꎞ��~
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->Stop(0);
			}
		}
	}
}

//========================================
// SoundTypeVolumeChange�֐� - ��ޖ��ɉ��ʕύX -
// Author:RIKU NISHIMURA
//===================s=====================
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume)
{
	// ���ʂ𐧌�
	if (fVolume <= 0.0f)
	{
		fVolume = 0.0f;
	}
	else if (fVolume >= 1.0f)
	{
		fVolume = 1.0f;
	}

	// ���ʂ�ݒ�
	g_soundControl.aSoundVolume[type] = fVolume;

	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// �T�E���h�̎�ނ���v�������A
			for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
			{
				// ���ʂ�ݒ�
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->SetVolume(g_aSoundInfo[nCntSound].fVolume * g_soundControl.aSoundVolume[type] * g_soundControl.aSettingVolume[type]);
			}
		}
	}
}

//========================================
// SetSettingVolume�֐� - �ݒ艹�ʂ�ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetSettingVolume(SOUND_TYPE type)
{
	// ��ނɉ������ݒ艹�ʂ��擾
	if (type == SOUND_TYPE_BGM)
	{
		g_soundControl.aSettingVolume[type] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	}
	else if (type == SOUND_TYPE_SE)
	{
		g_soundControl.aSettingVolume[type] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;
	}

	// ��ޖ��ɉ��ʂ�K�p
	SoundTypeVolumeChange(type, g_soundControl.aSoundVolume[type]);
}

//========================================
// StopSoundType�֐� - ��ޖ��ɍĐ���~ -
// Author:RIKU NISHIMURA
//========================================
void StopSoundType(SOUND_TYPE type)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// �T�E���h�̎�ނ���v�������A
			for (int nCntPlay = 0; nCntPlay < SOUND_PLAY_MAX; nCntPlay++)
			{
				// ��Ԏ擾
				g_soundControl.apSourceVoice[nCntSound][nCntPlay]->GetState(&xa2state);

				if (xa2state.BuffersQueued != 0)
				{// �Đ���
					// �ꎞ��~
					g_soundControl.apSourceVoice[nCntSound][nCntPlay]->Stop(0);

					// �I�[�f�B�I�o�b�t�@�̍폜
					g_soundControl.apSourceVoice[nCntSound][nCntPlay]->FlushSourceBuffers();
				}
			}
		}
	}
}