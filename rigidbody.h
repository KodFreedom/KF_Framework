//--------------------------------------------------------------------------------
//  リジッドボディコンポネント
//　rigidbodyComponent.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"

//--------------------------------------------------------------------------------
//  リジッドボディコンポネントクラス
//--------------------------------------------------------------------------------
class Rigidbody : public Component
{
public:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum Type
    {
        kRigidbodyNull = 0,
        kRigidbody3D
    };

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Rigidbody(GameObject& owner, const Type& type)
        : Component(owner), type_(type) {}
    ~Rigidbody() {}

    virtual bool Init(void) override = 0;
    virtual void Uninit(void) override = 0;
    virtual void Update(void) = 0;
    virtual void LateUpdate(void) = 0;

    //Get関数
    const auto& GetType(void) const { return type_; }

protected:
    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Type type_;
};

//--------------------------------------------------------------------------------
//  ヌルリジッドボディコンポネント
//--------------------------------------------------------------------------------
class RigidbodyNull : public Rigidbody
{
public:
    RigidbodyNull(GameObject& owner)
        : Rigidbody(owner, kRigidbodyNull) {}
    ~RigidbodyNull() {}
    bool Init(void) override { return true; }
    void Uninit(void) override {}
    void Update(void) override {}
    void LateUpdate(void) override {}
}; 
