//--------------------------------------------------------------------------------
//�@sound_system.h
//  manage the se,bgm's save, load
//  �T�E���h�Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <XAudio2.h>
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class WaveSe;
class WaveBgm;

//--------------------------------------------------------------------------------
//  �񋓌^��`
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
//  �T�E���h�Ǘ��҃N���X
//--------------------------------------------------------------------------------
class SoundSystem final
{
public:
    //--------------------------------------------------------------------------------
    //  ��������
    //  return : SoundSystem*
    //--------------------------------------------------------------------------------
    static SoundSystem* Create(void)
    {
        auto instance = MY_NEW SoundSystem();
        instance->Init();
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  �w�肵���T�E���h��炷
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
    //  �w�肵���T�E���h���~�߂�
    //  label : sound effect�̃��x��
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
    //  �w�肵���T�E���h���I����Ă邩���`�F�b�N
    //  label : sound effect�̃��x��
    //--------------------------------------------------------------------------------
    bool IsOver(const SoundEffectLabel label) const;

    //--------------------------------------------------------------------------------
    //  �w�肵���T�E���h���Ȃ��Ă邩���`�F�b�N
    //  label : sound effect�̃��x��
    //--------------------------------------------------------------------------------
    bool IsPlaying(const SoundEffectLabel label) const;

    //--------------------------------------------------------------------------------
    //  �S�ẴT�E���h���~�܂�
    //--------------------------------------------------------------------------------
    void StopAll(void);

private:
    //--------------------------------------------------------------------------------
    //�@�\���̒�`
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
    //    int                  primary = 0;      // ���ݍĐ����̃o�b�t�@
    //    int                  secondary = 1;    // �V�����f�[�^�̏������݉\�ȃo�b�t�@
    //    DWORD                write_cursor = 0; // �������݃J�[�\��
    //};

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    SoundSystem();
    SoundSystem(const SoundSystem& value) {}
    SoundSystem& operator=(const SoundSystem& value) {}
    ~SoundSystem() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    bool InitXAudio(void);

    //--------------------------------------------------------------------------------
    //  �T�E���h�f�[�^�t�@�C���̓Ǎ�
    //--------------------------------------------------------------------------------
    void LoadSoundData(void);

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  �}���`�X���b�h����
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  SE�̖炷����
    //--------------------------------------------------------------------------------
    void PlaySe(void);

    //--------------------------------------------------------------------------------
    //  SE�̎~�߂鏈��
    //--------------------------------------------------------------------------------
    void StopSe(void);

    //--------------------------------------------------------------------------------
    //  Bgm�̖炷����
    //--------------------------------------------------------------------------------
    void PlayBgm(void);

    //--------------------------------------------------------------------------------
    //  Bgm�̎~�߂鏈��
    //--------------------------------------------------------------------------------
    void StopBgm(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    bool                     is_running_ = true;
    thread*                  thread_ = nullptr;
    IXAudio2*                xaudio2_instance_ = nullptr;
    IXAudio2MasteringVoice*  mastering_voice_ = nullptr;
    mutex                    mutex_;        // �r������

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