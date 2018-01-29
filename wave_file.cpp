//--------------------------------------------------------------------------------
//　wave_file.cpp
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "wave_file.h"

//--------------------------------------------------------------------------------
//
//  Wave file
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void WaveFile::Release(void)
{
    if (source_voice_)
    {
        // 一時停止
        source_voice_->Stop(0);

        // ソースボイスの破棄
        source_voice_->DestroyVoice();
        source_voice_ = nullptr;
    }

    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  止める処理
//--------------------------------------------------------------------------------
void WaveFile::Stop(void)
{
    // 状態取得
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    if (state.BuffersQueued != 0)
    {// 再生中なら一時停止
        source_voice_->Stop(0);

        // オーディオバッファの削除
        source_voice_->FlushSourceBuffers();
    }
}

//--------------------------------------------------------------------------------
//  終わってるかをチェック
//--------------------------------------------------------------------------------
bool WaveFile::IsOver(void) const
{
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    return state.BuffersQueued == 0;
}

//--------------------------------------------------------------------------------
//  なってるかをチェック
//--------------------------------------------------------------------------------
bool WaveFile::IsPlaying(void) const
{
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    return state.BuffersQueued != 0;
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool WaveFile::Load(IXAudio2& xaudio, const String& file_path)
{
    // サウンドデータファイルの生成
    file_ = CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (file_ == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, file_path.c_str(), L"CreateFile失敗！", MB_ICONWARNING);
        return false;
    }

    // ファイルポインタを先頭に移動
    if (SetFilePointer(file_, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        MessageBox(NULL, file_path.c_str(), L"SetFilePointer失敗！", MB_ICONWARNING);
        return false;
    }

    // WAVEファイルのチェック
    DWORD chunk_size = 0;
    DWORD chunk_position = 0;
    if (!CheckChunk(file_, 'FFIR', chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk FFIR失敗！", MB_ICONWARNING);
        return false;
    }

    // FileTypeのチェック
    DWORD file_type;
    if (!ReadChunkData(file_, &file_type, sizeof(file_type), chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
        return false;
    }

    if (file_type != 'EVAW')
    {
        MessageBox(NULL, file_path.c_str(), L"file_type != 'EVAW'！", MB_ICONWARNING);
        return false;
    }

    // フォーマットチェック
    if (!CheckChunk(file_, ' tmf', chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk tmf失敗！", MB_ICONWARNING);
        return false;
    }

    WAVEFORMATEXTENSIBLE wfx;
    memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
    if (!ReadChunkData(file_, &wfx, chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
        return false;
    }
    bytes_per_second_ = wfx.Format.nAvgBytesPerSec;

    // オーディオサイズ読み込み
    if (!CheckChunk(file_, 'atad', size_, data_origin_))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk atad失敗！", MB_ICONWARNING);
        return false;
    }

    // ソースボイスの生成
    if (FAILED(xaudio.CreateSourceVoice(&source_voice_, &(wfx.Format))))
    {
        MessageBox(NULL, file_path.c_str(), L"CreateSourceVoice失敗！", MB_ICONWARNING);
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
// チャンクのチェック
//--------------------------------------------------------------------------------
bool WaveFile::CheckChunk(HANDLE file, DWORD format, DWORD& chunk_size, DWORD& chunk_data_position)
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
DWORD WaveFile::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
    if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return 0;
    }

    DWORD read;
    if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
    {// データの読み込み
        return 0;
    }

    return read;
}

//--------------------------------------------------------------------------------
//
//  Wave se
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
WaveSe* WaveSe::Create(IXAudio2& xaudio, const String& file_path, const int count_loop)
{
    WaveSe* result = MY_NEW WaveSe;
    result->count_loop_ = count_loop;

    if (!result->Load(xaudio, file_path))
    {
        result->Release();
        MY_DELETE result;
        return nullptr;
    }
    
    return result;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void WaveSe::Release(void)
{
    data_.clear();
    WaveFile::Release();
}

//--------------------------------------------------------------------------------
//  鳴らす処理
//--------------------------------------------------------------------------------
void WaveSe::Start(void)
{
    // 一時停止
    Stop();

    // バッファの値設定
    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = size_;
    buffer.pAudioData = &data_[0];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = count_loop_;

    // オーディオバッファの登録
    source_voice_->SubmitSourceBuffer(&buffer);

    // 再生
    source_voice_->Start(0);
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool WaveSe::Load(IXAudio2& xaudio, const String& file_path)
{
    if (!WaveFile::Load(xaudio, file_path))
    {
        return false;
    }

    data_.resize(size_);
    if (!ReadChunkData(file_, &data_[0], size_, data_origin_))
    {
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData失敗！", MB_ICONWARNING);
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
//
//  Wave bgm
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
WaveBgm* WaveBgm::Create(IXAudio2& xaudio, const String& file_path, const int count_loop)
{
    WaveBgm* result = MY_NEW WaveBgm;
    result->count_loop_ = count_loop;

    if (!result->Load(xaudio, file_path))
    {
        result->Release();
        MY_DELETE result;
        return nullptr;
    }

    return result;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void WaveBgm::Release(void)
{
    for (auto& data : datas_)
    {
        data.clear();
    }
    WaveFile::Release();
}

//--------------------------------------------------------------------------------
//  鳴らす処理
//--------------------------------------------------------------------------------
void WaveBgm::Start(void)
{
    Stop();
    write_cursor_ = data_origin_;
    AddNextBuffer();
    source_voice_->Start(0);
}

//--------------------------------------------------------------------------------
//  ポーリング処理
//--------------------------------------------------------------------------------
void WaveBgm::Polling(void)
{
    //状態を取得
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);

    //再生キューに常に２つのバッファを溜めておく
    if (state.BuffersQueued < 2)
    {
        if (write_cursor_ >= size_)
        {
            if (count_loop_ == -1)
            {// ループするなら先頭に戻す
                write_cursor_ = data_origin_;
            }
            else
            {// ループしないなら再生終了
                return;
            }
        }

        //バッファにデータを書き込んで、再生キューに追加
        AddNextBuffer();
    }
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool WaveBgm::Load(IXAudio2& xaudio, const String& file_path)
{
    if (!WaveFile::Load(xaudio, file_path))
    {
        return false;
    }

    // １秒のバッファを２つ用意
    for (auto& data : datas_)
    {
        data.resize(bytes_per_second_);
    }

    // 開始位置を設定
    write_cursor_ = data_origin_;

    return true;
}

//--------------------------------------------------------------------------------
//  プライマリとセカンダリのフリップ
//--------------------------------------------------------------------------------
void WaveBgm::AddNextBuffer(void)
{
    // secndaryにデータを書き込んで、書き込みカーソルを進める
    DWORD read = ReadChunkData(file_, &datas_[secondary_][0], datas_[secondary_].size(), write_cursor_);
    if (read == 0)
    {
        MessageBox(NULL, L"AddNextBuffer", L"失敗！", MB_ICONWARNING);
        return;
    }

    write_cursor_ += read;

    // SourceVoiceにデータを送信
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = read;
    buffer.pAudioData = &datas_[secondary_][0];
    if (size_ <= write_cursor_)
    {
        buffer.Flags = XAUDIO2_END_OF_STREAM;
    }
    source_voice_->SubmitSourceBuffer(&buffer);

    //primaryとsecondaryの入れ替え
    Flip();
}

//--------------------------------------------------------------------------------
//  プライマリとセカンダリのフリップ
//--------------------------------------------------------------------------------
void WaveBgm::Flip(void)
{
    swap(primary_, secondary_);
}