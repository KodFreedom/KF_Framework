//--------------------------------------------------------------------------------
//　sound_manager.cpp
//  manage the se,bgm's save, load
//	サウンド管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_manager.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
SoundManager::Paramater SoundManager::paramaters_[eSoundMax] =
{
	{ L"data/bgm/game.wav", -1 },	// BGM
};

//--------------------------------------------------------------------------------
//  指定したサウンドを鳴らす
//--------------------------------------------------------------------------------
void SoundManager::Play(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audio_sizes_[label];
	buffer.pAudioData = audio_datas_[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = paramaters_[label].count_loop;

	// 状態取得
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		source_voices_[label]->Stop(0);

		// オーディオバッファの削除
		source_voices_[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	source_voices_[label]->SubmitSourceBuffer(&buffer);

	// 再生
	source_voices_[label]->Start(0);
}

//--------------------------------------------------------------------------------
//  指定したサウンドを止める
//--------------------------------------------------------------------------------
void SoundManager::Stop(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		source_voices_[label]->Stop(0);

		// オーディオバッファの削除
		source_voices_[label]->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
//  指定したサウンドが終わってるかをチェック
//--------------------------------------------------------------------------------
bool SoundManager::IsOver(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
//  指定したサウンドがなってるかをチェック
//--------------------------------------------------------------------------------
bool SoundManager::IsPlaying(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  全てのサウンドを止まる
//--------------------------------------------------------------------------------
void SoundManager::StopAll(void)
{
	for (int count = 0; count < eSoundMax; count++)
	{
		if (source_voices_[count])
		{
			// 一時停止
			source_voices_[count]->Stop(0);
		}
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool SoundManager::Init(void)
{
	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	HRESULT hr = XAudio2Create(&instance_xaudio2_, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"XAudio2オブジェクトの作成に失敗！", L"警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	// マスターボイスの生成
	hr = instance_xaudio2_->CreateMasteringVoice(&mastering_voice_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"マスターボイスの生成に失敗！", L"警告！", MB_ICONWARNING);

		SAFE_RELEASE(instance_xaudio2_);

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	// サウンドデータの初期化
	for (int count = 0; count < eSoundMax; ++count)
	{
		HANDLE file;
		DWORD chunk_size = 0;
		DWORD chunk_position = 0;
		DWORD file_type;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		file = CreateFile(paramaters_[count].file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			//MessageBox(NULL, L"サウンドデータファイルの生成に失敗！(1)", L"警告！", MB_ICONWARNING);
			return false;
		}
		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			//MessageBox(NULL, L"サウンドデータファイルの生成に失敗！(2)", L"警告！", MB_ICONWARNING);
			return false;
		}

		// WAVEファイルのチェック
		if (!CheckChunk(file, 'FFIR', chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"WAVEファイルのチェックに失敗！(1)", L"警告！", MB_ICONWARNING);
			return false;
		}

		if (!ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position))
		{
			//MessageBox(NULL, L"WAVEファイルのチェックに失敗！(2)", L"警告！", MB_ICONWARNING);
			return false;
		}
		if (file_type != 'EVAW')
		{
			//MessageBox(NULL, L"WAVEファイルのチェックに失敗！(3)", L"警告！", MB_ICONWARNING);
			return false;
		}

		// フォーマットチェック
		if (!CheckChunk(file, ' tmf', chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"フォーマットチェックに失敗！(1)", L"警告！", MB_ICONWARNING);
			return false;
		}

		if (!ReadChunkData(file, &wfx, chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"フォーマットチェックに失敗！(2)", L"警告！", MB_ICONWARNING);
			return false;
		}

		// オーディオデータ読み込み
		if (!CheckChunk(file, 'atad', audio_sizes_[count], chunk_position))
		{
			//MessageBox(NULL, L"オーディオデータ読み込みに失敗！(1)", L"警告！", MB_ICONWARNING);
			return false;
		}

		audio_datas_[count] = (BYTE*)malloc(audio_sizes_[count]);
		if (!ReadChunkData(file, audio_datas_[count], audio_sizes_[count], chunk_position))
		{
			//MessageBox(NULL, L"オーディオデータ読み込みに失敗！(2)", L"警告！", MB_ICONWARNING);
			return false;
		}

		// ソースボイスの生成
		if (!instance_xaudio2_->CreateSourceVoice(&source_voices_[count], &(wfx.Format)))
		{
			//MessageBox(NULL, L"ソースボイスの生成に失敗！", L"警告！", MB_ICONWARNING);
			return false;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = audio_sizes_[count];
		buffer.pAudioData = audio_datas_[count];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = paramaters_[count].count_loop;

		// オーディオバッファの登録
		source_voices_[count]->SubmitSourceBuffer(&buffer);
	}

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void SoundManager::Uninit(void)
{
	for (int count = 0; count < eSoundMax; ++count)
	{
		if (source_voices_[count])
		{
			// 一時停止
			source_voices_[count]->Stop(0);

			// ソースボイスの破棄
			source_voices_[count]->DestroyVoice();
			source_voices_[count] = nullptr;

			// オーディオデータの開放
			free(audio_datas_[count]);
			audio_datas_[count] = nullptr;
		}
	}

	// マスターボイスの破棄
	mastering_voice_->DestroyVoice();
	mastering_voice_ = nullptr;

	// XAudio2オブジェクトの解放
	SAFE_RELEASE(instance_xaudio2_);

	// COMライブラリの終了処理
	CoUninitialize();
}

//--------------------------------------------------------------------------------
// チャンクのチェック
//--------------------------------------------------------------------------------
bool SoundManager::CheckChunk(HANDLE file, DWORD format, DWORD& chunk_size, DWORD& chunk_data_position)
{
	bool result = true;
	DWORD read;
	DWORD chunk_type;
	DWORD chunk_data_size;
	DWORD ffir_data_size = 0;
	DWORD file_type;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return false;
	}

	while (result)
	{
		if (ReadFile(file, &chunk_type, sizeof(DWORD), &read, NULL) == 0)
		{// チャンクの読み込み
			result = false;
		}

		if (ReadFile(file, &chunk_data_size, sizeof(DWORD), &read, NULL) == 0)
		{// チャンクデータの読み込み
			result = false;
		}

		switch (chunk_type)
		{
		case 'FFIR':
			ffir_data_size = chunk_data_size;
			chunk_data_size = 4;
			if (ReadFile(file, &file_type, sizeof(DWORD), &read, NULL) == 0)
			{// ファイルタイプの読み込み
				result = false;
			}
			break;

		default:
			if (SetFilePointer(file, chunk_data_size, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return false;
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunk_type == format)
		{
			chunk_size = chunk_data_size;
			chunk_data_position = offset;
			return true;
		}

		offset += chunk_data_size;
		if (bytesRead >= ffir_data_size)
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
// チャンクデータの読み込み
//--------------------------------------------------------------------------------
bool SoundManager::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
	DWORD read;

	if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}