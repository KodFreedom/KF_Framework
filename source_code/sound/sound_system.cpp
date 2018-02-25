//--------------------------------------------------------------------------------
//　sound_system.cpp
//  manage the se,bgm's save, load
//  サウンド管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_system.h"
#include "wave_file.h"
#include "../kf_utility/kf_utility.h"
#ifdef _DEBUG
#include "../main_system.h"
#include "../observer/debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数
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
    return sound_effects_[label]->IsOver();
}

//--------------------------------------------------------------------------------
//  指定したサウンドがなってるかをチェック
//--------------------------------------------------------------------------------
bool SoundSystem::IsPlaying(const SoundEffectLabel label) const
{
    return sound_effects_[label]->IsPlaying();
}

//--------------------------------------------------------------------------------
//  全てのサウンドを止まる
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
//  終了処理
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
    LoadSoundData();

    while (is_running_)
    {
        PlaySe();
        StopSe();
        PlayBgm();
        StopBgm();
    }

    // 終了処理
    Uninit();
}

//--------------------------------------------------------------------------------
//  SEの鳴らす処理
//--------------------------------------------------------------------------------
void SoundSystem::PlaySe(void)
{
    lock_guard<mutex> lock(mutex_);
    if (se_play_tasks_.empty()) return;

    // 一番目のタスクを実行する
    SoundEffectLabel current_task = se_play_tasks_.front();
    se_play_tasks_.pop();

    sound_effects_[current_task]->Start();

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Play " + se_infos_[current_task].file_path);
#endif // _DEBUG

}

//--------------------------------------------------------------------------------
//  SEの止める処理
//--------------------------------------------------------------------------------
void SoundSystem::StopSe(void)
{
    lock_guard<mutex> lock(mutex_);
    if (se_stop_tasks_.empty()) return;

    // 一番目のタスクを実行する
    SoundEffectLabel current_task = se_stop_tasks_.front();
    se_stop_tasks_.pop();

    sound_effects_[current_task]->Stop();

#ifdef _DEBUG
    MainSystem::Instance().GetDebugObserver().Display(L"Sound Thread : Stop " + se_infos_[current_task].file_path);
#endif // _DEBUG
}

//--------------------------------------------------------------------------------
//  Bgmの鳴らす処理
//--------------------------------------------------------------------------------
void SoundSystem::PlayBgm(void)
{
    // 今のbgmをポーリングする
    if (current_bgm_ != kBgmMax)
    {
        background_musics_[current_bgm_]->Polling();
    }

    // bgmの切り替え
    lock_guard<mutex> lock(mutex_);
    if (bgm_play_tasks_.empty()) return;

    // 一番目のタスクを実行する
    BackgroundMusicLabel current_task = bgm_play_tasks_.front();
    bgm_play_tasks_.pop();

    // 今のbgmと同じなら処理しない
    if (current_bgm_ == current_task) return;

    // 今のbgmを止める
    if (current_bgm_ != kBgmMax)
    {
        background_musics_[current_bgm_]->Stop();
    }

    current_bgm_ = current_task;
    background_musics_[current_bgm_]->Start();
}

//--------------------------------------------------------------------------------
//  Bgmの止める処理
//--------------------------------------------------------------------------------
void SoundSystem::StopBgm(void)
{
    lock_guard<mutex> lock(mutex_);
    if (bgm_stop_tasks_.empty()) return;

    // 一番目のタスクを実行する
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