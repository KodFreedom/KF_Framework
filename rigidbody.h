//--------------------------------------------------------------------------------
//  ���W�b�h�{�f�B�R���|�l���g
//�@rigidbodyComponent.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"

//--------------------------------------------------------------------------------
//  ���W�b�h�{�f�B�R���|�l���g�N���X
//--------------------------------------------------------------------------------
class Rigidbody : public Component
{
public:
    //--------------------------------------------------------------------------------
    //  �񋓌^��`
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

    //Get�֐�
    const auto& GetType(void) const { return type_; }

protected:
    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    Type type_;
};

//--------------------------------------------------------------------------------
//  �k�����W�b�h�{�f�B�R���|�l���g
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
