//--------------------------------------------------------------------------------
//�@wave_file.cpp
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "wave_file.h"

//--------------------------------------------------------------------------------
//
//  Wave file
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void WaveFile::Release(void)
{
    if (source_voice_)
    {
        // �ꎞ��~
        source_voice_->Stop(0);

        // �\�[�X�{�C�X�̔j��
        source_voice_->DestroyVoice();
        source_voice_ = nullptr;
    }

    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  �~�߂鏈��
//--------------------------------------------------------------------------------
void WaveFile::Stop(void)
{
    // ��Ԏ擾
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    if (state.BuffersQueued != 0)
    {// �Đ����Ȃ�ꎞ��~
        source_voice_->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        source_voice_->FlushSourceBuffers();
    }
}

//--------------------------------------------------------------------------------
//  �I����Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool WaveFile::IsOver(void) const
{
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    return state.BuffersQueued == 0;
}

//--------------------------------------------------------------------------------
//  �Ȃ��Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool WaveFile::IsPlaying(void) const
{
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);
    return state.BuffersQueued != 0;
}

//--------------------------------------------------------------------------------
//  ���[�h����
//--------------------------------------------------------------------------------
bool WaveFile::Load(IXAudio2& xaudio, const String& file_path)
{
    // �T�E���h�f�[�^�t�@�C���̐���
    file_ = CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (file_ == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, file_path.c_str(), L"CreateFile���s�I", MB_ICONWARNING);
        return false;
    }

    // �t�@�C���|�C���^��擪�Ɉړ�
    if (SetFilePointer(file_, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        MessageBox(NULL, file_path.c_str(), L"SetFilePointer���s�I", MB_ICONWARNING);
        return false;
    }

    // WAVE�t�@�C���̃`�F�b�N
    DWORD chunk_size = 0;
    DWORD chunk_position = 0;
    if (!CheckChunk(file_, 'FFIR', chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk FFIR���s�I", MB_ICONWARNING);
        return false;
    }

    // FileType�̃`�F�b�N
    DWORD file_type;
    if (!ReadChunkData(file_, &file_type, sizeof(file_type), chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
        return false;
    }

    if (file_type != 'EVAW')
    {
        MessageBox(NULL, file_path.c_str(), L"file_type != 'EVAW'�I", MB_ICONWARNING);
        return false;
    }

    // �t�H�[�}�b�g�`�F�b�N
    if (!CheckChunk(file_, ' tmf', chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk tmf���s�I", MB_ICONWARNING);
        return false;
    }

    WAVEFORMATEXTENSIBLE wfx;
    memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
    if (!ReadChunkData(file_, &wfx, chunk_size, chunk_position))
    {
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
        return false;
    }
    bytes_per_second_ = wfx.Format.nAvgBytesPerSec;

    // �I�[�f�B�I�T�C�Y�ǂݍ���
    if (!CheckChunk(file_, 'atad', size_, data_origin_))
    {
        MessageBox(NULL, file_path.c_str(), L"CheckChunk atad���s�I", MB_ICONWARNING);
        return false;
    }

    // �\�[�X�{�C�X�̐���
    if (FAILED(xaudio.CreateSourceVoice(&source_voice_, &(wfx.Format))))
    {
        MessageBox(NULL, file_path.c_str(), L"CreateSourceVoice���s�I", MB_ICONWARNING);
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
// �`�����N�̃`�F�b�N
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
    {// �t�@�C���|�C���^��擪�Ɉړ�
        return false;
    }

    while (result)
    {
        if (ReadFile(file, &chunk_type, sizeof(DWORD), &read, NULL) == 0)
        {// �`�����N�̓ǂݍ���
            result = false;
        }

        if (ReadFile(file, &chunk_data_size, sizeof(DWORD), &read, NULL) == 0)
        {// �`�����N�f�[�^�̓ǂݍ���
            result = false;
        }

        switch (chunk_type)
        {
        case 'FFIR':
            ffir_data_size = chunk_data_size;
            chunk_data_size = 4;
            if (ReadFile(file, &file_type, sizeof(DWORD), &read, NULL) == 0)
            {// �t�@�C���^�C�v�̓ǂݍ���
                result = false;
            }
            break;

        default:
            if (SetFilePointer(file, chunk_data_size, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//--------------------------------------------------------------------------------
DWORD WaveFile::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
    if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// �t�@�C���|�C���^���w��ʒu�܂ňړ�
        return 0;
    }

    DWORD read;
    if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
    {// �f�[�^�̓ǂݍ���
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
//  ��������
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
//  �j������
//--------------------------------------------------------------------------------
void WaveSe::Release(void)
{
    data_.clear();
    WaveFile::Release();
}

//--------------------------------------------------------------------------------
//  �炷����
//--------------------------------------------------------------------------------
void WaveSe::Start(void)
{
    // �ꎞ��~
    Stop();

    // �o�b�t�@�̒l�ݒ�
    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = size_;
    buffer.pAudioData = &data_[0];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = count_loop_;

    // �I�[�f�B�I�o�b�t�@�̓o�^
    source_voice_->SubmitSourceBuffer(&buffer);

    // �Đ�
    source_voice_->Start(0);
}

//--------------------------------------------------------------------------------
//  ���[�h����
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
        MessageBox(NULL, file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
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
//  ��������
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
//  �j������
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
//  �炷����
//--------------------------------------------------------------------------------
void WaveBgm::Start(void)
{
    Stop();
    write_cursor_ = data_origin_;
    AddNextBuffer();
    source_voice_->Start(0);
}

//--------------------------------------------------------------------------------
//  �|�[�����O����
//--------------------------------------------------------------------------------
void WaveBgm::Polling(void)
{
    //��Ԃ��擾
    XAUDIO2_VOICE_STATE state;
    source_voice_->GetState(&state);

    //�Đ��L���[�ɏ�ɂQ�̃o�b�t�@�𗭂߂Ă���
    if (state.BuffersQueued < 2)
    {
        if (write_cursor_ >= size_)
        {
            if (count_loop_ == -1)
            {// ���[�v����Ȃ�擪�ɖ߂�
                write_cursor_ = data_origin_;
            }
            else
            {// ���[�v���Ȃ��Ȃ�Đ��I��
                return;
            }
        }

        //�o�b�t�@�Ƀf�[�^����������ŁA�Đ��L���[�ɒǉ�
        AddNextBuffer();
    }
}

//--------------------------------------------------------------------------------
//  ���[�h����
//--------------------------------------------------------------------------------
bool WaveBgm::Load(IXAudio2& xaudio, const String& file_path)
{
    if (!WaveFile::Load(xaudio, file_path))
    {
        return false;
    }

    // �P�b�̃o�b�t�@���Q�p��
    for (auto& data : datas_)
    {
        data.resize(bytes_per_second_);
    }

    // �J�n�ʒu��ݒ�
    write_cursor_ = data_origin_;

    return true;
}

//--------------------------------------------------------------------------------
//  �v���C�}���ƃZ�J���_���̃t���b�v
//--------------------------------------------------------------------------------
void WaveBgm::AddNextBuffer(void)
{
    // secndary�Ƀf�[�^����������ŁA�������݃J�[�\����i�߂�
    DWORD read = ReadChunkData(file_, &datas_[secondary_][0], datas_[secondary_].size(), write_cursor_);
    if (read == 0)
    {
        MessageBox(NULL, L"AddNextBuffer", L"���s�I", MB_ICONWARNING);
        return;
    }

    write_cursor_ += read;

    // SourceVoice�Ƀf�[�^�𑗐M
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = read;
    buffer.pAudioData = &datas_[secondary_][0];
    if (size_ <= write_cursor_)
    {
        buffer.Flags = XAUDIO2_END_OF_STREAM;
    }
    source_voice_->SubmitSourceBuffer(&buffer);

    //primary��secondary�̓���ւ�
    Flip();
}

//--------------------------------------------------------------------------------
//  �v���C�}���ƃZ�J���_���̃t���b�v
//--------------------------------------------------------------------------------
void WaveBgm::Flip(void)
{
    swap(primary_, secondary_);
}