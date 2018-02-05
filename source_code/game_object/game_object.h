//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�X�[�p�[�N���X
//�@game_object.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "transform.h"
#include "behavior.h"
#include "mesh_renderer.h"
#include "rigidbody.h"
#include "collider.h"
#include "game_object_manager.h"
#include "kf_utility.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class GameObject
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    GameObject(const Layer& layer = kDefaultLayer);
    ~GameObject() {}
    
    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    virtual bool Init(void);

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    virtual void Update(void)
    {
        if (!is_active_) return;
        for (auto& pair : behaviors_) pair.second->Update();
        rigidbody_->Update();
        for (auto collider : colliders_) collider->Update();
    }

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    virtual void LateUpdate(void)
    {
        if (!is_active_) return;
        rigidbody_->LateUpdate();
        for (auto& pair : behaviors_) { pair.second->LateUpdate(); }
        transform_->UpdateMatrix();
        for (auto& pair : renderers_) { pair.second->Update(); }
    }

    //--------------------------------------------------------------------------------
    //  �����[�X����
    //--------------------------------------------------------------------------------
    void Release(void)
    {
        Uninit();
        MY_DELETE this;
    }

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    const auto      GetTransform(void) const { return transform_; }
    Behavior*       GetBehaviorBy(const String& name) const
    {
        auto iterator = behaviors_.find(name);
        if (behaviors_.end() == iterator) { return nullptr; }
        return iterator->second;
    }
    const auto&     GetBehaviors(void) const { return behaviors_; }
    const auto      GetRigidbody(void) const { return rigidbody_; }
    MeshRenderer*   GetRendererBy(const MeshRendererType& type) const 
    {
        auto iterator = renderers_.find(type);
        if (renderers_.end() == iterator) { return nullptr; }
        return iterator->second;
    }
    const auto&     GetRenderers(void) const { return renderers_; }
    const auto&     GetColliders(void) const { return colliders_; }
    list<Collider*> GetCollidersFromChildren(void) const;
    const String&   GetName(void) const { return name_; }
    const String&   GetParentName(void) const
    {
        auto parent = transform_->GetParent();
        if (!parent) return name_;
        return parent->GetGameObject().GetParentName();
    }
    const String&   GetTag(void) const { return tag_; }

    //--------------------------------------------------------------------------------
    //  �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetRigidbody(Rigidbody* const value)
    {
        if (!value) return;

        //Release
        SAFE_RELEASE(rigidbody_);

        //Set
        rigidbody_ = value;
        rigidbody_->Init();
    }
    void SetActive(const bool& value);
    void SetAlive(const bool& value);
    void SetName(const String& value) { name_ = value; }
    void SetTag(const String& value) { tag_ = value; }
    void SetTagToAllChildren(const String& value);

    //--------------------------------------------------------------------------------
    //  ������t���O�̎擾
    //--------------------------------------------------------------------------------
    const bool& IsAlive(void) const { return is_alive_; }

    //--------------------------------------------------------------------------------
    //  �A�N�e�B�u�̎擾
    //--------------------------------------------------------------------------------
    const bool& IsActive(void) const { return is_active_; }

    //--------------------------------------------------------------------------------
    //  �r�w�r�A�̒ǉ�
    //--------------------------------------------------------------------------------
    void AddBehavior(Behavior* const behavior)
    {
        assert(behavior);
        behaviors_.emplace(behavior->GetName(), behavior);
    }
    
    //--------------------------------------------------------------------------------
    //  �R���C�_�[�̒ǉ�
    //--------------------------------------------------------------------------------
    void AddCollider(Collider* const collider) 
    {
        assert(collider);
        colliders_.push_back(collider);
    }

    //--------------------------------------------------------------------------------
    //  �����_���[�̒ǉ�
    //--------------------------------------------------------------------------------
    void AddRenderer(MeshRenderer* const renderer) 
    {
        assert(renderer);
        renderers_.emplace(renderer->GetType(), renderer);
    }

    //--------------------------------------------------------------------------------
    //  check if this equal value / ���l���菈��
    //  value : �l
    //  return�Fbool
    //--------------------------------------------------------------------------------
    bool operator==(const GameObject& value) const
    {
        return this == &value;
    }

protected:
    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    virtual void Uninit(void)
    {
        transform_->Release();
        rigidbody_->Release();
        for (auto iterator = behaviors_.begin(); iterator != behaviors_.end();)
        {
            iterator->second->Release();
            iterator = behaviors_.erase(iterator);
        }
        for (auto iterator = colliders_.begin(); iterator != colliders_.end();)
        {
            (*iterator)->Release();
            iterator = colliders_.erase(iterator);
        }
        for (auto iterator = renderers_.begin(); iterator != renderers_.end();)
        {
            iterator->second->Release();
            iterator = renderers_.erase(iterator);
        }
    }

    //--------------------------------------------------------------------------------
    //  �R���|�l���g
    //--------------------------------------------------------------------------------
    Transform*                                          transform_;
    unordered_map<String, Behavior*>                    behaviors_;
    Rigidbody*                                          rigidbody_;
    list<Collider*>                                     colliders_;
    unordered_multimap<MeshRendererType, MeshRenderer*> renderers_;

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    bool   is_active_;
    bool   is_alive_;
    String name_;
    String tag_;
    Layer  layer_;

private:
    //--------------------------------------------------------------------------------
    //  �֐���`
    //--------------------------------------------------------------------------------
    GameObject(GameObject&) {}
};