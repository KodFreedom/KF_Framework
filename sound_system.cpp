//--------------------------------------------------------------------------------
//　sound_system.cpp
//  manage the se,bgm's save, load
//  サウンド管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_system.h"
#include "kf_utility.h"
#ifdef _DEBUG
#include "main_system.h"
#include "debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
SoundSystem::SoundEffectInfo SoundSystem::se_infos_[kSoundEffectMax] =
{
    { L"data/se/submit.wav", 0 },    // kSubmitSoundEffect
};

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void SoundSystem::Release(void)
{
    is_running_ = false;

    // スレッドの破棄
    if (thread_ && thread_->joinable())
    {
        thread_->join();
        MY_DELETE thread_;
        thread_ = nullptr;
    }

    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  指定したサウンドが終わってるかをチェック
//--------------------------------------------------------------------------------
bool SoundSystem::IsOver(const SoundEffectLabel label) const
{
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[label]->GetState(&xa2state);
    return xa2state.BuffersQueued == 0;
}

//--------------------------------------------------------------------------------
//  指定したサウンドがなってるかをチェック
//--------------------------------------------------------------------------------
bool SoundSystem::IsPlaying(const SoundEffectLabel label) const
{
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[label]->GetState(&xa2state);
    return xa2state.BuffersQueued != 0;
}

//--------------------------------------------------------------------------------
//  全てのサウンドを止まる
//--------------------------------------------------------------------------------
void SoundSystem::StopAll(void)
{
    for (int count = 0; count < kSoundEffectMax; count++)
    {
        if (se_source_voices_[count])
        {
            // 一時停止
            se_source_voices_[count]->Stop(0);
        }
    }
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
SoundSystem::SoundSystem()
{
    
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void SoundSystem::Init(void)
{
    // スレッドの生成
    thread_ = MY_NEW thread(&SoundSystem::Run, this);
}

//--------------------------------------------------------------------------------
//  XAudio初期化処理
//--------------------------------------------------------------------------------
bool SoundSystem::InitXAudio(void)
{
    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    HRESULT hr = XAudio2Create(&xaudio2_instance_, 0);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"XAudio2オブジェクトの作成に失敗！", L"警告！", MB_ICONWARNING);
        CoUninitialize(); // COMライブラリの終了処理
        return false;
    }

    // マスターボイスの生成
    hr = xaudio2_instance_->CreateMasteringVoice(&mastering_voice_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"マスターボイスの生成に失敗！", L"警告！", MB_ICONWARNING);
        SAFE_RELEASE(xaudio2_instance_);
        CoUninitialize(); // COMライブラリの終了処理
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
//  サウンドデータファイルの読込
//--------------------------------------------------------------------------------
void SoundSystem::LoadSoundEffectData(void)
{
    // サウンドデータの初期化
    for (int count = 0; count < kSoundEffectMax; ++count)
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
        file = CreateFile(se_infos_[count].file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CreateFile失敗！", MB_ICONWARNING);
            continue;
        }

        if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"SetFilePointer失敗！", MB_ICONWARNING);
            continue;
        }

        // WAVEファイルのチェック
        if (!CheckChunk(file, 'FFIR', chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk FFIR失敗！", MB_ICONWARNING);
            continue;
        }

        if (!ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
            continue;
        }

        if (file_type != 'EVAW')
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"file_type != 'EVAW'！", MB_ICONWARNING);
            continue;
        }

        // フォーマットチェック
        if (!CheckChunk(file, ' tmf', chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk tmf失敗！", MB_ICONWARNING);
            continue;
        }

        if (!ReadChunkData(file, &wfx, chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
            continue;
        }

        // オーディオデータ読み込み
        if (!CheckChunk(file, 'atad', se_sizes_[count], chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk atad失敗！", MB_ICONWARNING);
            continue;
        }

        se_datas_[count] = (BYTE*)malloc(se_sizes_[count]);
        if (!ReadChunkData(file, se_datas_[count], se_sizes_[count], chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
            continue;
        }

        // ソースボイスの生成
        if (FAILED(xaudio2_instance_->CreateSourceVoice(&se_source_voices_[count], &(wfx.Format))))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CreateSourceVoice失敗！", MB_ICONWARNING);
            continue;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = se_sizes_[count];
        buffer.pAudioData = se_datas_[count];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = se_infos_[count].count_loop;

        // オーディオバッファの登録
        se_source_voices_[count]->SubmitSourceBuffer(&buffer);
    }
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void SoundSystem::Uninit(void)
{
    for (int count = 0; count < kSoundEffectMax; ++count)
    {
        if (se_source_voices_[count])
        {
            // 一時停止
            se_source_voices_[count]->Stop(0);

            // ソースボイスの破棄
            se_source_voices_[count]->DestroyVoice();
            se_source_voices_[count] = nullptr;

            // オーディオデータの開放
            free(se_datas_[count]);
            se_datas_[count] = nullptr;
        }
    }

    // マスターボイスの破棄
    mastering_voice_->DestroyVoice();
    mastering_voice_ = nullptr;

    // XAudio2オブジェクトの解放
    SAFE_RELEASE(xaudio2_instance_);

    // COMライブラリの終了処理
    CoUninitialize();
}

//--------------------------------------------------------------------------------
//  マルチスレッド処理
//--------------------------------------------------------------------------------
void SoundSystem::Run(void)
{
    // 初期化
    if (!InitXAudio()) return;
    LoadSoundEffectData();

    while (is_running_)
    {
        PlaySe();
        StopSe();
    }

    // 終了処理
    Uninit();
}

//--------------------------------------------------------------------------------
//  SEの鳴らす処理
//--------------------------------------------------------------------------------
void SoundSystem::PlaySe(void)
{
    if (se_play_tasks_.empty()) return;

    // 一番目のタスクを実行する
    SoundEffectLabel current_task = se_play_tasks_.front();
    se_play_tasks_.pop();

    // バッファの値設定
    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = se_sizes_[current_task];
    buffer.pAudioData = se_datas_[current_task];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = se_infos_[current_task].count_loop;

    // 状態取得
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[current_task]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中なら一時停止
        se_source_voices_[current_task]->Stop(0);

        // オーディオバッファの削除
        se_source_voices_[current_task]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    se_source_voices_[current_task]->SubmitSourceBuffer(&buffer);

    // 再生
    se_source_voices_[current_task]->Start(0);

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Play " + se_infos_[current_task].file_path);
#endif // _DEBUG

}

//--------------------------------------------------------------------------------
//  SEの止める処理
//--------------------------------------------------------------------------------
void SoundSystem::StopSe(void)
{
    if (se_stop_tasks_.empty()) return;

    // 一番目のタスクを実行する
    SoundEffectLabel current_task = se_stop_tasks_.front();
    se_stop_tasks_.pop();

    // 状態取得
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[current_task]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中なら一時停止
        se_source_voices_[current_task]->Stop(0);

        // オーディオバッファの削除
        se_source_voices_[current_task]->FlushSourceBuffers();
    }

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Stop " + se_infos_[current_task].file_path);
#endif // _DEBUG
}

//--------------------------------------------------------------------------------
// チャンクのチェック
//--------------------------------------------------------------------------------
bool SoundSystem::CheckChunk(HANDLE file, DWORD format, DWORD& chunk_size, DWORD& chunk_data_position)
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
bool SoundSystem::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
    DWORD read;

    if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return false;
    }

    if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
    {// データの読み込み
        return false;
    }

    return true;
}