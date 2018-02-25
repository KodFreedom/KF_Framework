//--------------------------------------------------------------------------------
//�@sound_system.cpp
//  manage the se,bgm's save, load
//  �T�E���h�Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_system.h"
#include "wave_file.h"
#include "../kf_utility/kf_utility.h"
#ifdef _DEBUG
#include "../main_system.h"
#include "../observer/debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
SoundSystem::SoundInfo SoundSystem::se_infos_[kSeMax] =
{
    { L"data/se/submit.wav", 0 }, // kSubmitSe
    { L"data/se/zombie_beat.wav", 0 }, // kZombieBeat
    { L"data/se/zombie_warning.wav", 0 }, // kZombieWarning
    { L"data/se/zombie_death.wav", 0 }, // kZombieDeath
    { L"data/se/block.wav", 0 }, // kBlockSe,
    { L"data/se/sord_attack.wav", 0 }, // kSordAttackSe,
    { L"data/se/attack_voice_1.wav", 0 }, // kAttackVoice1Se,
    { L"data/se/attack_voice_2.wav", 0 }, // kAttackVoice2Se,
    { L"data/se/attack_voice_3.wav", 0 }, // kAttackVoice3Se,
    { L"data/se/begin_voice.wav", 0 }, // kBeginVoiceSe,
    { L"data/se/damage_voice_1.wav", 0 }, // kDamageVoice1Se,
    { L"data/se/damage_voice_2.wav", 0 }, // kDamageVoice2Se,
    { L"data/se/death_voice.wav", 0 }, // kDeathVoiceSe,
    { L"data/se/guard_voice.wav", 0 }, // kGuardVoiceSe,
    { L"data/se/pinch_voice.wav", 0 }, // kPinchVoiceSe,
    { L"data/se/cursor.wav", 0 }, // kCursorSe,
};

SoundSystem::SoundInfo SoundSystem::bgm_infos_[kBgmMax] =
{
    { L"data/bgm/title.wav", -1 }, // kTitleBgm
    { L"data/bgm/game.wav", -1 }, // kGameBgm
    { L"data/bgm/result.wav", -1 }, // kResultBgm
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
    return sound_effects_[label]->IsOver();
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���Ȃ��Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool SoundSystem::IsPlaying(const SoundEffectLabel label) const
{
    return sound_effects_[label]->IsPlaying();
}

//--------------------------------------------------------------------------------
//  �S�ẴT�E���h���~�܂�
//--------------------------------------------------------------------------------
void SoundSystem::StopAll(void)
{
    for (auto& sound_effect : sound_effects_)
    {
        sound_effect->Stop();
    }

    for (auto& background_music : background_musics_)
    {
        background_music->Stop();
    }
    current_bgm_ = kBgmMax;
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
void SoundSystem::LoadSoundData(void)
{
    for (int count = 0; count < kSeMax; ++count)
    {
        sound_effects_[count] = WaveSe::Create(*xaudio2_instance_, se_infos_[count].file_path, se_infos_[count].count_loop);
        assert(sound_effects_[count]);
    }

    for (int count = 0; count < kBgmMax; ++count)
    {
        background_musics_[count] = WaveBgm::Create(*xaudio2_instance_, bgm_infos_[count].file_path, bgm_infos_[count].count_loop);
        assert(background_musics_[count]);
    }
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void SoundSystem::Uninit(void)
{
    for (auto& sound_effect : sound_effects_)
    {
        SAFE_RELEASE(sound_effect);
    }

    for (auto& background_music : background_musics_)
    {
        SAFE_RELEASE(background_music);
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
    LoadSoundData();

    while (is_running_)
    {
        PlaySe();
        StopSe();
        PlayBgm();
        StopBgm();
    }

    // �I������
    Uninit();
}

//--------------------------------------------------------------------------------
//  SE�̖炷����
//--------------------------------------------------------------------------------
void SoundSystem::PlaySe(void)
{
    lock_guard<mutex> lock(mutex_);
    if (se_play_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    SoundEffectLabel current_task = se_play_tasks_.front();
    se_play_tasks_.pop();

    sound_effects_[current_task]->Start();

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Play " + se_infos_[current_task].file_path);
#endif // _DEBUG

}

//--------------------------------------------------------------------------------
//  SE�̎~�߂鏈��
//--------------------------------------------------------------------------------
void SoundSystem::StopSe(void)
{
    lock_guard<mutex> lock(mutex_);
    if (se_stop_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    SoundEffectLabel current_task = se_stop_tasks_.front();
    se_stop_tasks_.pop();

    sound_effects_[current_task]->Stop();

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Stop " + se_infos_[current_task].file_path);
#endif // _DEBUG
}

//--------------------------------------------------------------------------------
//  Bgm�̖炷����
//--------------------------------------------------------------------------------
void SoundSystem::PlayBgm(void)
{
    // ����bgm���|�[�����O����
    if (current_bgm_ != kBgmMax)
    {
        background_musics_[current_bgm_]->Polling();
    }

    // bgm�̐؂�ւ�
    lock_guard<mutex> lock(mutex_);
    if (bgm_play_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    BackgroundMusicLabel current_task = bgm_play_tasks_.front();
    bgm_play_tasks_.pop();

    // ����bgm�Ɠ����Ȃ珈�����Ȃ�
    if (current_bgm_ == current_task) return;

    // ����bgm���~�߂�
    if (current_bgm_ != kBgmMax)
    {
        background_musics_[current_bgm_]->Stop();
    }

    current_bgm_ = current_task;
    background_musics_[current_bgm_]->Start();
}

//--------------------------------------------------------------------------------
//  Bgm�̎~�߂鏈��
//--------------------------------------------------------------------------------
void SoundSystem::StopBgm(void)
{
    lock_guard<mutex> lock(mutex_);
    if (bgm_stop_tasks_.empty()) return;

    // ��Ԗڂ̃^�X�N�����s����
    BackgroundMusicLabel current_task = bgm_stop_tasks_.front();
    bgm_stop_tasks_.pop();

    if (current_bgm_ == current_task)
    {
        current_bgm_ = kBgmMax;
    }
    background_musics_[current_task]->Stop();
    

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Stop " + bgm_infos_[current_task].file_path);
#endif // _DEBUG
}