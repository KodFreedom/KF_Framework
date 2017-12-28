//--------------------------------------------------------------------------------
//�@shadow_map_system.h
//  render objects to shadow map
//    �V���h�E�}�b�v�������_�����O����V�X�e��
//    Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MeshRenderer;
class MeshRenderer3d;
class MeshRenderer3dSkin;
class Transform;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class ShadowMapSystem
{
#ifdef _DEBUG
    friend class DebugObserver;
#endif

public:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kShadowMapWidth = 1024.0f;
    static constexpr float kShadowMapHeight = 1024.0f;

    //--------------------------------------------------------------------------------
    //  ��������
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static ShadowMapSystem* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW ShadowMapSystem(device);
        instance->Init();
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Release(void) { Uninit(); }

    //--------------------------------------------------------------------------------
    //  �`�揈��
    //--------------------------------------------------------------------------------
    void Render(void);

    //--------------------------------------------------------------------------------
    //  renderer��o�^����
    //  renderer : �����_���[
    //--------------------------------------------------------------------------------
    void Register(MeshRenderer3d* renderer);

    //--------------------------------------------------------------------------------
    //  renderer��o�^����
    //  renderer : �����_���[
    //--------------------------------------------------------------------------------
    void Register(MeshRenderer3dSkin* renderer);

    //--------------------------------------------------------------------------------
    //  �N���A����
    //--------------------------------------------------------------------------------
    void Clear(void);

    //--------------------------------------------------------------------------------
    //  Set�֐�
    //--------------------------------------------------------------------------------
    void SetTarget(Transform* target) { target_ = target; }

    //--------------------------------------------------------------------------------
    //  Get�֐�
    //--------------------------------------------------------------------------------
    const Matrix44& GetLightView(void) const { return view_; }
    const Matrix44& GetLightProjection(void) const { return projection_; }
    const float& GetBias(void) const { return bias_; }

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  �V���h�E�}�b�v�̃|�C���^���擾
    //  return : const LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    const LPDIRECT3DTEXTURE9 GetShadowMap(void) const
    {
        return shadow_map_;
    }
#endif

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    ShadowMapSystem(const LPDIRECT3DDEVICE9 device)
        : device_(device)
        , shadow_map_(nullptr)
        , shadow_map_surface_(nullptr)
        , offset_(Vector3(20.0f, 80.0f, -20.0f))
        , range_(20.0f) 
        , near_(0.0f)
        , far_(200.0f)
        , bias_(0.00001f)
        , target_(nullptr) {}
    ShadowMapSystem() : device_(nullptr) {}
    ShadowMapSystem(const ShadowMapSystem& value) : device_(nullptr) {}
#else
    ShadowMapSystem() {}
    ShadowMapSystem(const ShadowMapSystem& value) {}
#endif
    ShadowMapSystem& operator=(const ShadowMapSystem& value) {}
    ~ShadowMapSystem() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    list<MeshRenderer*> renderers_array_[kShadowMapShaderMax]; // �V���h�E��\�����郌���_���[
    Vector3 offset_;
    float range_;
    float near_;
    float far_;
    float bias_;
    Matrix44 view_;
    Matrix44 projection_;
    Transform* target_;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    LPDIRECT3DTEXTURE9 shadow_map_;
    LPDIRECT3DSURFACE9 shadow_map_surface_;
    LPDIRECT3DSURFACE9 depth_stencil_surface_;
    const LPDIRECT3DDEVICE9 device_; // directx9�̃f�B�o�C�X
#endif
};