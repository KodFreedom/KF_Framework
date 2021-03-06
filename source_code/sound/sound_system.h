//--------------------------------------------------------------------------------
//　sound_system.h
//  manage the se,bgm's save, load
//  サウンド管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <XAudio2.h>
#include "../common_setting.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class WaveSe;
class WaveBgm;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum SoundEffectLabel
{
    kSubmitSe,
    kZombieBeatSe,
    kZombieWarningSe,
    kZombieDeathSe,
    kBlockSe,
    kSordAttackSe,
    kAttackVoice1Se,
    kAttackVoice2Se,
    kAttackVoice3Se,
    kBeginVoiceSe,
    kDamageVoice1Se,
    kDamageVoice2Se,
    kDeathVoiceSe,
    kGuardVoiceSe,
    kPinchVoiceSe,
    kCursorSe,
    kSeMax,
};

enum BackgroundMusicLabel
{
    kTitleBgm,
    kGameBgm,
    kResultBgm,
    kBgmMax
};

//--------------------------------------------------------------------------------
//  サウンド管理者クラス
//--------------------------------------------------------------------------------
class SoundSystem final
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : SoundSystem*
    //--------------------------------------------------------------------------------
    static SoundSystem* Create(void)
    {
        auto instance = MY_NEW SoundSystem();
        instance->Init();
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  指定したサウンドを鳴らす
    //--------------------------------------------------------------------------------
    void Play(const SoundEffectLabel label)
    {
        lock_guard<mutex> lock(mutex_);
        se_play_tasks_.push(label);
    }
    void Play(const BackgroundMusicLabel label)
    {
        lock_guard<mutex> lock(mutex_);
        bgm_play_tasks_.push(label);
    }

    //--------------------------------------------------------------------------------
    //  指定したサウンドを止める
    //  label : sound effectのラベル
    //--------------------------------------------------------------------------------
    void Stop(const SoundEffectLabel label)
    {
        lock_guard<mutex> lock(mutex_);
        se_stop_tasks_.push(label);
    }
    void Stop(const BackgroundMusicLabel label)
    {
        lock_guard<mutex> lock(mutex_);
        bgm_stop_tasks_.push(label);
    }

    //--------------------------------------------------------------------------------
    //  指定したサウンドが終わってるかをチェック
    //  label : sound effectのラベル
    //--------------------------------------------------------------------------------
    bool IsOver(const SoundEffectLabel label) const;

    //--------------------------------------------------------------------------------
    //  指定したサウンドがなってるかをチェック
    //  label : sound effectのラベル
    //--------------------------------------------------------------------------------
    bool IsPlaying(const SoundEffectLabel label) const;

    //--------------------------------------------------------------------------------
    //  全てのサウンドを止まる
    //--------------------------------------------------------------------------------
    void StopAll(void);

private:
    //--------------------------------------------------------------------------------
    //　構造体定義
    //--------------------------------------------------------------------------------
    struct SoundInfo
    {
        String file_path;
        int    count_loop;
    };

    //struct SoundEffect
    //{
    //    IXAudio2SourceVoice* source_voice;
    //    BYTE*                data;
    //    DWORD                size;
    //};
    //
    //struct BackgroundMusic
    //{
    //    IXAudio2SourceVoice* source_voice = nullptr;
    //    vector<BYTE>         datas[2];
    //    int                  primary = 0;      // 現在再生中のバッファ
    //    int                  secondary = 1;    // 新しいデータの書き込み可能なバッファ
    //    DWORD                write_cursor = 0; // 書き込みカーソル
    //};

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    SoundSystem();
    SoundSystem(const SoundSystem& value) {}
    SoundSystem& operator=(const SoundSystem& value) {}
    ~SoundSystem() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    bool InitXAudio(void);

    //--------------------------------------------------------------------------------
    //  サウンドデータファイルの読込
    //--------------------------------------------------------------------------------
    void LoadSoundData(void);

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  マルチスレッド処理
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  SEの鳴らす処理
    //--------------------------------------------------------------------------------
    void PlaySe(void);

    //--------------------------------------------------------------------------------
    //  SEの止める処理
    //--------------------------------------------------------------------------------
    void StopSe(void);

    //--------------------------------------------------------------------------------
    //  Bgmの鳴らす処理
    //--------------------------------------------------------------------------------
    void PlayBgm(void);

    //--------------------------------------------------------------------------------
    //  Bgmの止める処理
    //--------------------------------------------------------------------------------
    void StopBgm(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    bool                     is_running_ = true;
    thread*                  thread_ = nullptr;
    IXAudio2*                xaudio2_instance_ = nullptr;
    IXAudio2MasteringVoice*  mastering_voice_ = nullptr;
    mutex                    mutex_;        // 排他制御

    WaveSe*                  sound_effects_[kSeMax] = { 0 };
    queue<SoundEffectLabel>  se_play_tasks_;
    queue<SoundEffectLabel>  se_stop_tasks_;
    static SoundInfo         se_infos_[kSeMax];

    WaveBgm*                     background_musics_[kBgmMax];
    BackgroundMusicLabel         current_bgm_ = kBgmMax;
    queue<BackgroundMusicLabel>  bgm_play_tasks_;
    queue<BackgroundMusicLabel>  bgm_stop_tasks_;
    static SoundInfo             bgm_infos_[kBgmMax];
};