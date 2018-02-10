//--------------------------------------------------------------------------------
//  プレイヤーUIコントローラ
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"
#include "KF_Math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Transform;
class ActorParameter;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerUiController : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    PlayerUiController(GameObject& owner);
    ~PlayerUiController() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static const Vector3 kLifeGaugeSize;
    static const Vector3 kLifeGaugeLeftTop;
    static const Vector3 kCoverSize;
    static const Vector3 kCoverLeftTop;
    static const Vector3 kButtonSize;
    static const Vector3 kButtonALeftTop;
    static const Vector3 kButtonBLeftTop;
    static const Vector3 kButtonXLeftTop;
    static const Vector3 kButtonLbLeftTop;
    static const Vector3 kButtonTextSize;
    static const Vector3 kButtonTextALeftTop;
    static const Vector3 kButtonTextBLeftTop;
    static const Vector3 kButtonTextXLeftTop;
    static const Vector3 kButtonTextLbLeftTop;

    //--------------------------------------------------------------------------------
    //  ワーニングゲージの更新
    //--------------------------------------------------------------------------------
    void UpdateWarningGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  ライフゲージの更新
    //--------------------------------------------------------------------------------
    void UpdateLifeGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Transform* warning_gauge_;
    Transform* life_gauge_;
    float      warning_flash_speed_;
    float      life_flash_speed_;
};