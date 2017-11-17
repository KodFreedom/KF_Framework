//--------------------------------------------------------------------------------
//
//　soundManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "soundManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
SoundManager* SoundManager::instance = nullptr;
SoundManager::Paramater SoundManager::paramaters[SL_Max] =
{
	{ "data/BGM/gameBGM.wav", -1 },	// BGM
};

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
SoundManager::SoundManager()
	: instanceXAudio2(nullptr)
	, masteringVoice(nullptr)
{
	for (int count = 0; count < (int)SL_Max; ++count)
	{
		sourceVoices[count] = nullptr;
		audioDatas[count] = nullptr;
		audioSizes[count] = 0;
	}
}

//--------------------------------------------------------------------------------
//  読み込み処理
//--------------------------------------------------------------------------------
HRESULT	SoundManager::LoadAll(void)
{
	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	HRESULT hr = XAudio2Create(&instanceXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return hr;
	}

	// マスターボイスの生成
	hr = instanceXAudio2->CreateMasteringVoice(&masteringVoice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (instanceXAudio2)
		{
			// XAudio2オブジェクトの開放
			instanceXAudio2->Release();
			instanceXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return hr;
	}

	// サウンドデータの初期化
	for (int count = 0; count < SL_Max; ++count)
	{
		HANDLE file;
		DWORD chunkSize = 0;
		DWORD chunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		file = CreateFile(paramaters[count].FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(NULL, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = checkChunk(file, 'FFIR', &chunkSize, &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return hr;
		}
		hr = readChunkData(file, &filetype, sizeof(DWORD), chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return hr;
		}
		if (filetype != 'EVAW')
		{
			MessageBox(NULL, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return hr;
		}

		// フォーマットチェック
		hr = checkChunk(file, ' tmf', &chunkSize, &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return hr;
		}
		hr = readChunkData(file, &wfx, chunkSize, chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return hr;
		}

		// オーディオデータ読み込み
		hr = checkChunk(file, 'atad', &audioSizes[count], &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return hr;
		}
		audioDatas[count] = (BYTE*)malloc(audioSizes[count]);
		hr = readChunkData(file, audioDatas[count], audioSizes[count], chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return hr;
		}

		// ソースボイスの生成
		hr = instanceXAudio2->CreateSourceVoice(&sourceVoices[count], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(NULL, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return hr;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = audioSizes[count];
		buffer.pAudioData = audioDatas[count];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = paramaters[count].CountLoop;

		// オーディオバッファの登録
		sourceVoices[count]->SubmitSourceBuffer(&buffer);
	}

	return hr;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void SoundManager::UnloadAll(void)
{
	for (int count = 0; count < SL_Max; ++count)
	{
		if (sourceVoices[count])
		{
			// 一時停止
			sourceVoices[count]->Stop(0);

			// ソースボイスの破棄
			sourceVoices[count]->DestroyVoice();
			sourceVoices[count] = nullptr;

			// オーディオデータの開放
			free(audioDatas[count]);
			audioDatas[count] = nullptr;
		}
	}

	// マスターボイスの破棄
	masteringVoice->DestroyVoice();
	masteringVoice = nullptr;

	// XAudio2オブジェクトの解放
	SAFE_RELEASE(instanceXAudio2);

	// COMライブラリの終了処理
	CoUninitialize();
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void SoundManager::Play(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioSizes[label];
	buffer.pAudioData = audioDatas[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = paramaters[label].CountLoop;

	// 状態取得
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		sourceVoices[label]->Stop(0);

		// オーディオバッファの削除
		sourceVoices[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	sourceVoices[label]->SubmitSourceBuffer(&buffer);

	// 再生
	sourceVoices[label]->Start(0);
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void SoundManager::Stop(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		sourceVoices[label]->Stop(0);

		// オーディオバッファの削除
		sourceVoices[label]->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
bool SoundManager::IsOver(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
bool SoundManager::IsPlaying(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void SoundManager::StopAll(void)
{
	for (int count = 0; count < SL_Max; count++)
	{
		if (sourceVoices[count])
		{
			// 一時停止
			sourceVoices[count]->Stop(0);
		}
	}
}

//--------------------------------------------------------------------------------
// チャンクのチェック
//--------------------------------------------------------------------------------
HRESULT SoundManager::checkChunk(HANDLE file, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD read;
	DWORD chunkType;
	DWORD chunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD filetype;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(file, &chunkType, sizeof(DWORD), &read, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(file, &chunkDataSize, sizeof(DWORD), &read, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunkType)
		{
		case 'FFIR':
			RIFFDataSize = chunkDataSize;
			chunkDataSize = 4;
			if (ReadFile(file, &filetype, sizeof(DWORD), &read, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(file, chunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format)
		{
			*pChunkSize = chunkDataSize;
			*pChunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= RIFFDataSize)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
// チャンクデータの読み込み
//--------------------------------------------------------------------------------
HRESULT SoundManager::readChunkData(HANDLE file, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD read;

	if (SetFilePointer(file, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file, pBuffer, dwBuffersize, &read, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}