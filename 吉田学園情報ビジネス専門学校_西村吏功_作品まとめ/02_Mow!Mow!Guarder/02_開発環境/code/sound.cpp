//========================================
// 
// サウンド処理
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
// 
//========================================
// *** sound.cpp ***
//========================================
#include "sound.h"
#include "setting.h"
#include <stdio.h>

//****************************************
// プロトタイプ宣言
//****************************************
// チャンクのチェック
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

// チャンクデータの読み込み
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//****************************************
// グローバル変数
//****************************************
IXAudio2				*g_pXAudio2			// XAudio2オブジェクトへのインターフェイス
						= NULL;
IXAudio2MasteringVoice	*g_pMasteringVoice	// マスターボイス
						= NULL;
IXAudio2SourceVoice		*g_apSourceVoice	// ソースボイス
						[SOUND_LABEL_MAX]
						= {};
BYTE					*g_apDataAudio		// オーディオデータ
						[SOUND_LABEL_MAX]
						= {};
DWORD					g_aSizeAudio		// オーディオデータサイズ
						[SOUND_LABEL_MAX]
						= {};
float					g_aSoundVolume		// 音量
						[SOUND_TYPE_MAX]
						= { 1.0f,1.0f };
float					g_aSettingVolume	// 設定の音量
						[SOUND_TYPE_MAX];

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data\\SOUND\\BGM\\seadenden\\Absolute god.wav"       ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[000](0)
	{ "data\\SOUND\\SE\\seadenden\\damage3.wav"             ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :ダメージ	[000](1)
	{ "data\\SOUND\\SE\\seadenden\\damage4.wav"             ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :ダメージ	[001](2)
	{ "data\\SOUND\\SE\\seadenden\\damage1.wav"             ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :破壊		[000](3)
	{ "data\\SOUND\\SE\\seadenden\\shoot19.wav"             ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :ショット	[000](4)
	{ "data\\SOUND\\SE\\seadenden\\shoot1.wav"              ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :ショット	[001](5)
	{ "data\\SOUND\\SE\\seadenden\\touch8.wav"              ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :ショット	[002](6)
	{ "data\\SOUND\\SE\\seadenden\\overthrow5.wav"          ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :爆発		[000](7)
	{ "data\\SOUND\\SE\\seadenden\\explosion10.wav"         ,0 ,0.2f,SOUND_TYPE_SE  },	// SE  :爆発		[001](8)
	{ "data\\SOUND\\SE\\seadenden\\Landing2.wav"            ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :選択		[000](9)
	{ "data\\SOUND\\SE\\seadenden\\get4.wav"                ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :購入		[000](10)
	{ "data\\SOUND\\SE\\seadenden\\open4.wav"               ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :設置		[000](11)
	{ "data\\SOUND\\SE\\seadenden\\get3.wav"                ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :獲得		[000](12)
	{ "data\\SOUND\\SE\\seadenden\\explosion3.wav"          ,0 ,0.7f,SOUND_TYPE_SE  },	// SE  :爆発		[002](13)
	{ "data\\SOUND\\SE\\seadenden\\cut8.wav"                ,0 ,0.7f,SOUND_TYPE_SE  },	// SE  :斬撃		[000](14)
	{ "data\\SOUND\\SE\\seadenden\\walk2_0.wav"             ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :足音		[000](15)
	{ "data\\SOUND\\SE\\seadenden\\walk2_1.wav"             ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :足音		[001](16)
	{ "data\\SOUND\\SE\\seadenden\\bowwow.wav"              ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :鳴き声		[000](17)
	{ "data\\SOUND\\SE\\seadenden\\bowwow2.wav"             ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :鳴き声		[001](18)
	{ "data\\SOUND\\SE\\seadenden\\mow.wav"                 ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :鳴き声		[002](19)
	{ "data\\SOUND\\BGM\\seadenden\\Emergence of---.wav"    ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[001](20)
	{ "data\\SOUND\\SE\\seadenden\\siren1.wav"              ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :警告		[000](21)
	{ "data\\SOUND\\BGM\\seadenden\\Wind demon.wav"         ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[002](22)
	{ "data\\SOUND\\SE\\seadenden\\roar.wav"                ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :鳴き声		[003](23)
	{ "data\\SOUND\\SE\\seadenden\\graaa.wav"               ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :鳴き声		[004](24)
	{ "data\\SOUND\\SE\\seadenden\\clink1.wav"              ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :決定		[000](25)
	{ "data\\SOUND\\SE\\seadenden\\explosion11.wav"         ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :足音		[002](26)
	{ "data\\SOUND\\SE\\seadenden\\Landing1.wav"            ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :押下		[000](27)
	{ "data\\SOUND\\SE\\seadenden\\beep5.wav"               ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :決定		[001](28)
	{ "data\\SOUND\\BGM\\seadenden\\Unexplored space---.wav",-1,1.0f,SOUND_TYPE_BGM },	// BGM :タイトル	[000](29)
	{ "data\\SOUND\\SE\\seadenden\\Rotation1.wav"           ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :切り替え	[000](30)
	{ "data\\SOUND\\SE\\seadenden\\beep4.wav"               ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :ポーズ		[000](31)
	{ "data\\SOUND\\SE\\seadenden\\bang2.wav"               ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :足音		[003](32)
	{ "data\\SOUND\\SE\\seadenden\\katana1.wav"             ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :金属		[000](33)
	{ "data\\SOUND\\SE\\seadenden\\overthrow5.wav"          ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :衝撃		[000](34)
	{ "data\\SOUND\\BGM\\seadenden\\Half-destroyed---.wav"  ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[003](35)
	{ "data\\SOUND\\BGM\\seadenden\\Evaluation time.wav"    ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[004](36)
	{ "data\\SOUND\\BGM\\seadenden\\Flame demon.wav"        ,-1,1.0f,SOUND_TYPE_BGM },	// BGM :ステージ	[005](37)
	{ "data\\SOUND\\BGM\\seadenden\\You take care---.wav"   ,0 ,1.0f,SOUND_TYPE_BGM },	// BGM :クリア		[000](38)
	{ "data\\SOUND\\SE\\seadenden\\metallic clank3.wav"     ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :金属		[001](39)
	{ "data\\SOUND\\SE\\seadenden\\Electric shock2.wav"     ,0 ,0.5f,SOUND_TYPE_SE  },	// SE  :電気		[000](40)
	{ "data\\SOUND\\SE\\seadenden\\beep1.wav"               ,0 ,1.0f,SOUND_TYPE_SE  },	// SE  :カウント	[000](41)
}; 

//========== *** サウンドの情報を取得 ***
SOUNDINFO *GetSoundInfo(void)
{
	return g_aSoundInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// CheckChunk関数 - チャンクのチェック -
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
// ReadChunkData関数 - チャンクデータの読み込み -
//========================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSound関数 - サウンドの初期化処理 -
//========================================
HRESULT InitSound(HWND hWnd)
{
	// 設定音量を読み込む
	g_aSettingVolume[SOUND_TYPE_BGM] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	g_aSettingVolume[SOUND_TYPE_SE] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;

	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			char aString[TXT_MAX];
			sprintf(aString, "サウンドデータファイルの生成に失敗！(1) (カウント%d)", nCntSound);
			MessageBox(hWnd, aString, "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//========================================
// UninitSound関数 - サウンドの終了処理 -
//========================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//========================================
// PlaySound関数 - セグメント再生(再生中なら停止) -
//========================================
HRESULT PlaySound(SOUND_LABEL label)
{
	if (g_aSoundInfo[label].type == SOUND_TYPE_BGM) 
	{// サウンドの種類がBGMだった時、
		StopSoundType(SOUND_TYPE_BGM);	// BGMを一括停止する
	}

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 音量を設定
	g_apSourceVoice[label]->SetVolume(g_aSoundInfo[label].fVolume * g_aSoundVolume[g_aSoundInfo[label].type] * g_aSettingVolume[g_aSoundInfo[label].type]);

	// 再生
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//========================================
// StopSound - セグメント停止(ラベル指定) -
//========================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//========================================
// StopSound - セグメント停止(全て) -
//========================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//========================================
// SoundTypeVolumeChange関数 - 種類毎に音量変更 -
//========================================
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume)
{
	// 音量を制御
	if (fVolume <= 0.0f) { fVolume = 0.0f; }
	if (fVolume >= 1.0f) { fVolume = 1.0f; }

	// 音量を設定
	g_aSoundVolume[type] = fVolume;

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// サウンドの種類が一致した時、
			// 音量を設定
			g_apSourceVoice[nCntSound]->SetVolume(g_aSoundInfo[nCntSound].fVolume * g_aSoundVolume[type] * g_aSettingVolume[type]);
		}
	}
}

//========================================
// SoundVolumeChange関数 - 音量変更 -
//========================================
void SoundVolumeChange(SOUND_LABEL label, float fVolume)
{
	// 音量を制御
	if (fVolume <= 0.0f) { fVolume = 0.0f; }
	if (fVolume >= 1.0f) { fVolume = 1.0f; }

	// 音量のパラメーターを設定
	g_aSoundInfo[label].fVolume = fVolume;

	// 音量を設定
	g_apSourceVoice[label]->SetVolume(g_aSoundInfo[label].fVolume * g_aSoundVolume[g_aSoundInfo[label].type] * g_aSettingVolume[g_aSoundInfo[label].type]);
}

//========================================
// SetSettingVolume関数 - 設定音量を設定 -
//========================================
void SetSettingVolume(SOUND_TYPE type) 
{
	// 種類に応じた設定音量を取得
	if (type == SOUND_TYPE_BGM) {
		g_aSettingVolume[type] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	}
	else if (type == SOUND_TYPE_SE) {
		g_aSettingVolume[type] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;
	}
	
	// 種類毎に音量を適用
	SoundTypeVolumeChange(type, g_aSoundVolume[type]);
}

//========================================
// StopSoundType関数 - 種類毎に再生停止 -
//========================================
void StopSoundType(SOUND_TYPE type)
{
	XAUDIO2_VOICE_STATE xa2state;
	
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// サウンドの種類が一致した時、
			// 状態取得
			g_apSourceVoice[nCntSound]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{// 再生中
				// 一時停止
				g_apSourceVoice[nCntSound]->Stop(0);

				// オーディオバッファの削除
				g_apSourceVoice[nCntSound]->FlushSourceBuffers();
			}
		}
	}
}
