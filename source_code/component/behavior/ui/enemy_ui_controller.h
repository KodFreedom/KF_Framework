//--------------------------------------------------------------------------------
//  エネミーUIコントローラ
//  enemy_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../behavior.h"
#include "../../../kf_utility/kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class EnemyUiController : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    EnemyUiController(GameObject& owner);
    ~EnemyUiController() {}

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
    static const Vector3 kLifeGaugeOffset;
    static const float   kSquareDisplayDistance;

    //--------------------------------------------------------------------------------
    //  UIの数を敵に合わせる
    //--------------------------------------------------------------------------------
    void AdjustUiNumbers(const int difference);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    list<GameObject*> enemy_uis_;
};