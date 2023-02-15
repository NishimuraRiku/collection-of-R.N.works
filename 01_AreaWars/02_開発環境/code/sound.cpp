//========================================
// 
// サウンド処理
// Author : AKIRA TANAKA
// |
// Arrange : 西村 吏功
// 
//========================================
// *** sound.cpp ***
//========================================
#include <stdio.h>
#include "sound.h"
#include "setting.h"

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
						= NULL;				//
IXAudio2MasteringVoice	*g_pMasteringVoice	// マスターボイス
						= NULL;				//
IXAudio2SourceVoice		*g_apSourceVoice	// ソースボイス
						[SOUND_LABEL_MAX]	//
						= {};				//
BYTE					*g_apDataAudio		// オーディオデータ
						[SOUND_LABEL_MAX]	//
						= {};				//
DWORD					g_aSizeAudio		//
						[SOUND_LABEL_MAX]	// オーディオデータサイズ
						= {};				//
float					g_aSoundVolume		// 音量
						[SOUND_TYPE_MAX]	//
						= { 1.0f,1.0f };	//
float					g_aSettingVolume	// 設定の音量
						[SOUND_TYPE_MAX];	//

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data\\BGM\\seadenden\\The story begins.wav"         , "タイトルBGM[000]"        , -1, 1.0f , SOUND_TYPE_BGM },	// タイトルBGM			[000]
	{ "data\\BGM\\seadenden\\living star.wav"              , "ステージBGM[000]"        , -1, 1.0f , SOUND_TYPE_BGM },	// ステージBGM			[000]
	{ "data\\BGM\\seadenden\\meteor dance.wav"             , "ステージBGM[001]"        , -1, 1.0f , SOUND_TYPE_BGM },	// ステージBGM			[001]
	{ "data\\BGM\\seadenden\\Mastermind.wav"               , "ボスBGM[000]"            , -1, 1.0f , SOUND_TYPE_BGM },	// ボスBGM				[000]
	{ "data\\BGM\\seadenden\\Defense system.wav"           , "ボスBGM[001]"            , -1, 1.0f , SOUND_TYPE_BGM },	// ボスBGM				[001]
	{ "data\\SE\\seadenden\\You take care of the rest.wav" , "ゲームクリアSE[000]"     , 0 , 1.0f , SOUND_TYPE_SE  },	// ゲームクリアSE		[000]
	{ "data\\SE\\seadenden\\Regeneration cannot.wav"       , "ゲームオーバーSE[000]"   , 0 , 1.0f , SOUND_TYPE_SE  },	// ゲームオーバーSE		[000]
	{ "data\\SE\\seadenden\\metallic clank2.wav"           , "選択SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 選択SE				[000]
	{ "data\\SE\\seadenden\\beep1.wav"                     , "選択SE[001]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 選択SE				[001]
	{ "data\\SE\\seadenden\\clink1.wav"                    , "決定SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 決定SE				[000]
	{ "data\\SE\\seadenden\\beep5.wav"                     , "決定SE[001]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 決定SE				[001]
	{ "data\\SE\\seadenden\\beep4.wav"                     , "削除SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 削除SE				[000]
	{ "data\\SE\\seadenden\\beep5.wav"                     , "ポーズ開始SE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// ポーズ開始SE			[000]
	{ "data\\SE\\seadenden\\beep4.wav"                     , "ポーズ終了SE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// ポーズ終了SE			[000]
	{ "data\\SE\\seadenden\\system14.wav"                  , "カウントダウンSE[000]"   , 0 , 1.0f , SOUND_TYPE_SE  },	// カウントダウンSE		[000]
	{ "data\\SE\\seadenden\\beep1.wav"                     , "スコアカウントSE[000]"   , 0 , 0.5f , SOUND_TYPE_SE  },	// スコアカウントSE		[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "テキストカウントSE[000]" , 0 , 0.4f , SOUND_TYPE_SE  },	// テキストカウントSE	[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "ランキングセットSE[000]" , 0 , 1.0f , SOUND_TYPE_SE  },	// ランキングセットSE	[000]
	{ "data\\SE\\seadenden\\damage3.wav"                   , "ダメージSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// ダメージSE			[000]
	{ "data\\SE\\seadenden\\damage4.wav"                   , "ダメージSE[001]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// ダメージSE			[001]
	{ "data\\SE\\seadenden\\bell3.wav"                     , "ボーナスSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// ボーナスSE			[000]
	{ "data\\SE\\seadenden\\Accumulate2.wav"               , "警告SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 警告SE				[000]
	{ "data\\SE\\seadenden\\shoot19.wav"                   , "ショットSE[000]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// ショットSE			[000]
	{ "data\\SE\\seadenden\\missile2.wav"                  , "ショットSE[001]"         , 0 , 1.0f , SOUND_TYPE_SE  },	// ショットSE			[001]
	{ "data\\SE\\seadenden\\shoot1.wav"                    , "ショットSE[002]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// ショットSE			[002]
	{ "data\\SE\\seadenden\\shoot2.wav"                    , "ショットSE[003]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// ショットSE			[003]
	{ "data\\SE\\seadenden\\shoot12.wav"                   , "ショットSE[004]"         , 0 , 1.4f , SOUND_TYPE_SE  },	// ショットSE			[004]
	{ "data\\SE\\seadenden\\overthrow5.wav"                , "爆発SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 爆発SE				[000]
	{ "data\\SE\\seadenden\\explosion10.wav"               , "爆発SE[001]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// 爆発SE				[001]
	{ "data\\SE\\seadenden\\explosion2.wav"                , "爆発SE[002]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// 爆発SE				[002]
	{ "data\\SE\\seadenden\\explosion3.wav"                , "爆発SE[003]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// 爆発SE				[003]
	{ "data\\SE\\seadenden\\explosion10.wav"               , "爆発SE[004]"             , 0 , 0.2f , SOUND_TYPE_SE  },	// 爆発SE				[004]
	{ "data\\SE\\seadenden\\flame4.wav"                    , "爆発SE[005]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 爆発SE				[005]
	{ "data\\SE\\seadenden\\shoot22.wav"                   , "拡散SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 拡散SE				[000]
	{ "data\\SE\\seadenden\\get6.wav"                      , "ロックオンSE[000]"       , 0 , 1.0f , SOUND_TYPE_SE  },	// ロックオンSE			[000]
	{ "data\\SE\\seadenden\\open4.wav"                     , "開くSE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 開くSE				[000]
	{ "data\\SE\\seadenden\\metallic clank3.wav"           , "金属SE[000]"             , 0 , 0.8f , SOUND_TYPE_SE  },	// 金属SE				[000]
	{ "data\\SE\\seadenden\\don1.wav"                      , "衝突SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 衝突SE				[000]
	{ "data\\SE\\seadenden\\explosion9.wav"                , "出撃SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 出撃SE				[000]
	{ "data\\SE\\seadenden\\Landing2.wav"                  , "着地SE[000]"             , 0 , 1.0f , SOUND_TYPE_SE  },	// 着地SE				[000]
	{ "data\\SE\\seadenden\\pup7.wav"                      , "獲得SE[000]"             , 0 , 1.5f , SOUND_TYPE_SE  },	// 獲得SE				[000]
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
// StringLoadSound関数 - サウンドの種類を文字列から読み込む -
//========================================
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel)
{
	SOUNDINFO	*pSoundInfo	// サウンドの情報のポインタ
		= GetSoundInfo();

	if (strcmp(aString, "無し") == 0)
	{// サウンドが無しの時、
		// サウンドを-1に設定
		*pLabel = (SOUND_LABEL)-1;
	}
	else
	{// サウンドが無しでない時、
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++, pSoundInfo++)
		{
			if (strcmp(aString, pSoundInfo->pLoadName) == 0)
			{// 読み込み名が一致した時、
				// サウンドを代入
				*pLabel = (SOUND_LABEL)nCntSound;
				break;
			}
		}
	}
}

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
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
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
