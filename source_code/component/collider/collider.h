//--------------------------------------------------------------------------------
//  colliderコンポネント
//　collider.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "collision_system.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Behavior;
class CollisionInfo;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Collider : public Component
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Collider(GameObject& owner, const ColliderType& type, const ColliderMode& mode);
    ~Collider() {}

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    virtual bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    virtual void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    virtual void Update(void);

    //--------------------------------------------------------------------------------
    //  一時的に当たり判定処理から抜ける
    //--------------------------------------------------------------------------------
    void Sleep(void) override;

    //--------------------------------------------------------------------------------
    //  当たり判定処理に登録する
    //--------------------------------------------------------------------------------
    void Awake(void) override;

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(Collider& other);

    //--------------------------------------------------------------------------------
    //  コライダー衝突の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnCollision(CollisionInfo& info);
    
    //--------------------------------------------------------------------------------
    //  Set関数
    //--------------------------------------------------------------------------------
    void SetTrigger(const bool& value) { is_trigger_ = value; }
    void SetTag(const String& value) { tag_ = value; }
    void SetOffset(const Vector3& position, const Vector3& rotation = Vector3::kZero);
    void SetMode(const ColliderMode& mode);

    //--------------------------------------------------------------------------------
    //  Get関数
    //--------------------------------------------------------------------------------
    Vector3         GetLocalPosition(void) const { return Vector3(offset_.m30_, offset_.m31_, offset_.m32_); }
    Vector3         GetWorldPosition(void) const { return Vector3(world_.m30_, world_.m31_, world_.m32_); }
    const Matrix44& GetWorldMatrix(void) const { return world_; }
    const Matrix44& GetOffset(void) const { return offset_; }
    const auto&     GetType(void) const { return type_; }
    const auto&     GetMode(void) const { return mode_; }
    const bool&     IsTrigger(void) const { return is_trigger_; }
    const String&   GetTag(void) const { return tag_; }

    //--------------------------------------------------------------------------------
    //  observer関数
    //--------------------------------------------------------------------------------
    void Register(Behavior* observer);
    void Deregister(Behavior* observer);

protected:
    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    ColliderType    type_;
    ColliderMode    mode_;
    bool            is_trigger_;
    bool            is_registered_;
    Matrix44        offset_;
    Matrix44        world_;
    String          tag_;
    list<Behavior*> observers_;
};