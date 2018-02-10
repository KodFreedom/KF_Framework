//--------------------------------------------------------------------------------
//  �v���C���[UI�R���g���[��
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"
#include "KF_Math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Transform;
class ActorParameter;

//--------------------------------------------------------------------------------
//  �N���X�錾
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
    //  ����������
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
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
    //  ���[�j���O�Q�[�W�̍X�V
    //--------------------------------------------------------------------------------
    void UpdateWarningGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  ���C�t�Q�[�W�̍X�V
    //--------------------------------------------------------------------------------
    void UpdateLifeGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    Transform* warning_gauge_;
    Transform* life_gauge_;
    float      warning_flash_speed_;
    float      life_flash_speed_;
};