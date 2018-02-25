//--------------------------------------------------------------------------------
//�@shadow_map_system.cpp
//  render objects to shadow map
//  �V���h�E�}�b�v�������_�����O����V�X�e��
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shadow_map_system.h"
#include "../main_system.h"
#include "../shader/shader_manager.h"
#include "render_system.h"
#include "../component/renderer/mesh_renderer_3d.h"
#include "../component/renderer/mesh_renderer_3d_skin.h"
#include "../component/transform/transform.h"
#include "../kf_utility/kf_utility.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  renderer��o�^����
//--------------------------------------------------------------------------------
void ShadowMapSystem::Register(MeshRenderer3d* renderer)
{
    renderers_array_[kBasicShadowMapShader].push_back(renderer);
}

//--------------------------------------------------------------------------------
//  renderer��o�^����
//--------------------------------------------------------------------------------
void ShadowMapSystem::Register(MeshRenderer3dSkin* renderer)
{
    renderers_array_[kBasicSkinShadowMapShader].push_back(renderer);
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void ShadowMapSystem::Render(void)
{
    // Set light
    // �ړ�
    Vector3 look_at = target_ ? target_->GetPosition() : Vector3::kZero;
    const Vector3& position = look_at + offset_;
    
    // View�s��
    const Vector3& direction = (look_at - position).Normalized();
    const Vector3& right = (Vector3::kUp * direction).Normalized();
    const Vector3& up = (direction * right).Normalized();

    const Vector3& negative_eye = position * -1.0f;
    const Matrix44& view_transpose = Matrix44(
        right.x_, right.y_, right.z_, right.Dot(negative_eye),
        up.x_, up.y_, up.z_, up.Dot(negative_eye),
        direction.x_, direction.y_, direction.z_, direction.Dot(negative_eye),
        0.0f, 0.0f, 0.0f, 1.0f);
    view_ = view_transpose.Transpose();

    // Projection�s��
    projection_ = Matrix44::OrthographicLeftHand(-range_, range_, -range_, range_, near_, far_);

    // Render
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    device_->SetRenderTarget(0, shadow_map_surface_);
    device_->SetDepthStencilSurface(depth_stencil_surface_);
    device_->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);
    if (device_->BeginScene() >= 0)
    {
        auto& render_system = MainSystem::Instance().GetRenderSystem();
        auto& shader_manager = MainSystem::Instance().GetShaderManager();
        for (int count = 0; count < static_cast<int>(kShadowMapShaderMax); ++count)
        {
            if (renderers_array_[count].empty()) continue;
            const auto& current_type = static_cast<ShadowMapShaderType>(count);
            shader_manager.Set(current_type);
            for (auto iterator = renderers_array_[count].begin(); iterator != renderers_array_[count].end();)
            {
                shader_manager.SetConstantTable(current_type, **iterator);
                (*iterator)->RenderBy(render_system);
                iterator = renderers_array_[count].erase(iterator);
            }
            shader_manager.Reset(current_type);
        }
        device_->EndScene();
    }
    else
    {
        Clear();
    }
#endif
}

//--------------------------------------------------------------------------------
//  �N���A����
//--------------------------------------------------------------------------------
void ShadowMapSystem::Clear(void)
{
    for (auto& renderers : renderers_array_)
    {
        renderers.clear();
    }
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ShadowMapSystem::Init(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    // �V���h�E�}�b�v�Ƃ��Ďg�p����e�N�X�`���̍쐬
    HRESULT hr = device_->CreateTexture(static_cast<UINT>(kShadowMapWidth)
        , static_cast<UINT>(kShadowMapHeight)
        , 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &shadow_map_,NULL);
    assert(hr == S_OK);

    // �e�N�X�`�����烌���_�����O�^�[�Q�b�g�ɂ���T�[�t�F�[�X�̎擾
    hr = shadow_map_->GetSurfaceLevel(0, &shadow_map_surface_);
    assert(hr == S_OK);

    // �e�N�X�`���ւ̃����_�����O�Ɏg���[�x�o�b�t�@�̍쐬
    hr = device_->CreateDepthStencilSurface(static_cast<UINT>(kShadowMapWidth)
        , static_cast<UINT>(kShadowMapHeight)
        , D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &depth_stencil_surface_, NULL);
#endif
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void ShadowMapSystem::Uninit(void)
{
    Clear();
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    SAFE_RELEASE(shadow_map_);
    SAFE_RELEASE(shadow_map_surface_);
#endif
}