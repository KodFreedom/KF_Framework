//--------------------------------------------------------------------------------
//　fade_system.h
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Mode;

//--------------------------------------------------------------------------------
//  フェイドシステムクラス
//--------------------------------------------------------------------------------
class FadeSystem
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : FadeSystem*
    //--------------------------------------------------------------------------------
    static FadeSystem* Create(void);

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);
    
    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  次のモードにフェードする
    //  next_mode：次のモード
    //  fade_time：フェードの時間（秒数）
    //--------------------------------------------------------------------------------
    void FadeTo(Mode* next_mode, const float fade_time = 1.0f);

private:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum FadeState
    {
        kFadeNone = 0,
        kFadeIn,
        kFadeOut,
        kFadeWaitIn,
        kFadeWait,
        kFadeWaitOut,
    };

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    FadeSystem() {}
    FadeSystem(const FadeSystem& value) {}
    FadeSystem& operator=(const FadeSystem& value) {}
    ~FadeSystem() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  フェイドイン処理
    //--------------------------------------------------------------------------------
    void FadeIn(void);

    //--------------------------------------------------------------------------------
    //  フェードアウト処理
    //--------------------------------------------------------------------------------
    void FadeOut(void);

    //--------------------------------------------------------------------------------
    //  フェード待ち入る処理
    //--------------------------------------------------------------------------------
    void FadeWaitIn(void);

    //--------------------------------------------------------------------------------
    //  フェード待ち処理
    //--------------------------------------------------------------------------------
    void FadeWait(void);

    //--------------------------------------------------------------------------------
    //  フェード待ち出る処理
    //--------------------------------------------------------------------------------
    void FadeWaitOut(void);

    //--------------------------------------------------------------------------------
    //  マテリアルのアルファ値更新処理
    //--------------------------------------------------------------------------------
    void UpdateAlpha(const String& material_name, const float& alpha);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    FadeState current_state_ = kFadeWait;
    Mode*     next_mode_ = nullptr;
    float     fade_time_ = 1.0f;
    float     wait_fade_time_ = 0.5f;
    float     time_counter_ = 1.0f;
    float     wait_time_counter_ = 1.0f;
};