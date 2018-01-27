//--------------------------------------------------------------------------------
//�@sound_system.cpp
//  manage the se,bgm's save, load
//  �T�E���h�Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_system.h"
#include "kf_utility.h"
#ifdef _DEBUG
#include "main_system.h"
#include "debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
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
//  �j������
//--------------------------------------------------------------------------------
void SoundSystem::Release(void)
{
    is_running_ = false;

    // �X���b�h�̔j��
    if (thread_ && thread_->joinable())
    {
        thread_->join();
        MY_DELETE thread_;
        thread_ = nullptr;
    }

    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���I����Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool SoundSystem::IsOver(const SoundEffectLabel label) const
{
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[label]->GetState(&xa2state);
    return xa2state.BuffersQueued == 0;
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���Ȃ��Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool SoundSystem::IsPlaying(const SoundEffectLabel label) const
{
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[label]->GetState(&xa2state);
    return xa2state.BuffersQueued != 0;
}

//--------------------------------------------------------------------------------
//  �S�ẴT�E���h���~�܂�
//--------------------------------------------------------------------------------
void SoundSystem::StopAll(void)
{
    for (int count = 0; count < kSoundEffectMax; count++)
    {
        if (se_source_voices_[count])
        {
            // �ꎞ��~
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
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
SoundSystem::SoundSystem()
{
    
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void SoundSystem::Init(void)
{
    // �X���b�h�̐���
    thread_ = MY_NEW thread(&SoundSystem::Run, this);
}

//--------------------------------------------------------------------------------
//  XAudio����������
//--------------------------------------------------------------------------------
bool SoundSystem::InitXAudio(void)
{
    // COM���C�u�����̏�����
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2�I�u�W�F�N�g�̍쐬
    HRESULT hr = XAudio2Create(&xaudio2_instance_, 0);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", L"�x���I", MB_ICONWARNING);
        CoUninitialize(); // COM���C�u�����̏I������
        return false;
    }

    // �}�X�^�[�{�C�X�̐���
    hr = xaudio2_instance_->CreateMasteringVoice(&mastering_voice_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"�}�X�^�[�{�C�X�̐����Ɏ��s�I", L"�x���I", MB_ICONWARNING);
        SAFE_RELEASE(xaudio2_instance_);
        CoUninitialize(); // COM���C�u�����̏I������
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
//  �T�E���h�f�[�^�t�@�C���̓Ǎ�
//--------------------------------------------------------------------------------
void SoundSystem::LoadSoundEffectData(void)
{
    // �T�E���h�f�[�^�̏�����
    for (int count = 0; count < kSoundEffectMax; ++count)
    {
        HANDLE file;
        DWORD chunk_size = 0;
        DWORD chunk_position = 0;
        DWORD file_type;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // �o�b�t�@�̃N���A
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // �T�E���h�f�[�^�t�@�C���̐���
        file = CreateFile(se_infos_[count].file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CreateFile���s�I", MB_ICONWARNING);
            continue;
        }

        if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// �t�@�C���|�C���^��擪�Ɉړ�
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"SetFilePointer���s�I", MB_ICONWARNING);
            continue;
        }

        // WAVE�t�@�C���̃`�F�b�N
        if (!CheckChunk(file, 'FFIR', chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk FFIR���s�I", MB_ICONWARNING);
            continue;
        }

        if (!ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
            continue;
        }

        if (file_type != 'EVAW')
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"file_type != 'EVAW'�I", MB_ICONWARNING);
            continue;
        }

        // �t�H�[�}�b�g�`�F�b�N
        if (!CheckChunk(file, ' tmf', chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk tmf���s�I", MB_ICONWARNING);
            continue;
        }

        if (!ReadChunkData(file, &wfx, chunk_size, chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
            continue;
        }

        // �I�[�f�B�I�f�[�^�ǂݍ���
        if (!CheckChunk(file, 'atad', se_sizes_[count], chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CheckChunk atad���s�I", MB_ICONWARNING);
            continue;
        }

        se_datas_[count] = (BYTE*)malloc(se_sizes_[count]);
        if (!ReadChunkData(file, se_datas_[count], se_sizes_[count], chunk_position))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"ReadChunkData���s�I", MB_ICONWARNING);
            continue;
        }

        // �\�[�X�{�C�X�̐���
        if (FAILED(xaudio2_instance_->CreateSourceVoice(&se_source_voices_[count], &(wfx.Format))))
        {
            MessageBox(NULL, se_infos_[count].file_path.c_str(), L"CreateSourceVoice���s�I", MB_ICONWARNING);
            continue;
        }

        // �o�b�t�@�̒l�ݒ�
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = se_sizes_[count];
        buffer.pAudioData = se_datas_[count];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = se_infos_[count].count_loop;

        // �I�[�f�B�I�o�b�t�@�̓o�^
        se_source_voices_[count]->SubmitSourceBuffer(&buffer);
    }
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void SoundSystem::Uninit(void)
{
    for (int count = 0; count < kSoundEffectMax; ++count)
    {
        if (se_source_voices_[count])
        {
            // �ꎞ��~
            se_source_voices_[count]->Stop(0);

            // �\�[�X�{�C�X�̔j��
            se_source_voices_[count]->DestroyVoice();
            se_source_voices_[count] = nullptr;

            // �I�[�f�B�I�f�[�^�̊J��
            free(se_datas_[count]);
            se_datas_[count] = nullptr;
        }
    }

    // �}�X�^�[�{�C�X�̔j��
    mastering_voice_->DestroyVoice();
    mastering_voice_ = nullptr;

    // XAudio2�I�u�W�F�N�g�̉��
    SAFE_RELEASE(xaudio2_instance_);

    // COM���C�u�����̏I������
    CoUninitialize();
}

//--------------------------------------------------------------------------------
//  �}���`�X���b�h����
//--------------------------------------------------------------------------------
void SoundSystem::Run(void)
{
    // ������
    if (!InitXAudio()) return;
    LoadSoundEffectData();

    while (is_running_)
    {
        PlaySe();
        StopSe();
    }

    // �I������
    Uninit();
}

//--------------------------------------------------------------------------------
//  SE�̖炷����
//--------------------------------------------------------------------------------
void SoundSystem::PlaySe(void)
{
    if (se_play_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    SoundEffectLabel current_task = se_play_tasks_.front();
    se_play_tasks_.pop();

    // �o�b�t�@�̒l�ݒ�
    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = se_sizes_[current_task];
    buffer.pAudioData = se_datas_[current_task];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = se_infos_[current_task].count_loop;

    // ��Ԏ擾
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[current_task]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ����Ȃ�ꎞ��~
        se_source_voices_[current_task]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        se_source_voices_[current_task]->FlushSourceBuffers();
    }

    // �I�[�f�B�I�o�b�t�@�̓o�^
    se_source_voices_[current_task]->SubmitSourceBuffer(&buffer);

    // �Đ�
    se_source_voices_[current_task]->Start(0);

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Play " + se_infos_[current_task].file_path);
#endif // _DEBUG

}

//--------------------------------------------------------------------------------
//  SE�̎~�߂鏈��
//--------------------------------------------------------------------------------
void SoundSystem::StopSe(void)
{
    if (se_stop_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    SoundEffectLabel current_task = se_stop_tasks_.front();
    se_stop_tasks_.pop();

    // ��Ԏ擾
    XAUDIO2_VOICE_STATE xa2state;
    se_source_voices_[current_task]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ����Ȃ�ꎞ��~
        se_source_voices_[current_task]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        se_source_voices_[current_task]->FlushSourceBuffers();
    }

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Stop " + se_infos_[current_task].file_path);
#endif // _DEBUG
}

//--------------------------------------------------------------------------------
// �`�����N�̃`�F�b�N
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
bool SoundSystem::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
    DWORD read;

    if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// �t�@�C���|�C���^���w��ʒu�܂ňړ�
        return false;
    }

    if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
    {// �f�[�^�̓ǂݍ���
        return false;
    }

    return true;
}