//--------------------------------------------------------------------------------
//�@shadow_map_system.cpp
//  render objects to shadow map
//	�V���h�E�}�b�v�������_�����O����V�X�e��
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shadow_map_system.h"
#include "main.h"
#include "main_system.h"
#include "shader_manager.h"
#include "render_system.h"
#include "mesh_renderer_3d.h"
#include "mesh_renderer_3d_skin.h"

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
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	device_->SetRenderTarget(0, shadow_map_surface_);
	device_->SetDepthStencilSurface(depth_stencil_surface_);
	device_->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);
	if (device_->BeginScene() >= 0)
	{
		const auto render_system = MainSystem::Instance()->GetRenderSystem();
		const auto shader_manager = MainSystem::Instance()->GetShaderManager();
		for (int count = 0; count < static_cast<int>(kShadowMapShaderMax); ++count)
		{
			if (renderers_array_[count].empty()) continue;
			const auto& current_type = static_cast<ShadowMapShaderType>(count);
			shader_manager->Set(current_type);
			for (auto iterator = renderers_array_[count].begin(); iterator != renderers_array_[count].end();)
			{
				shader_manager->SetConstantTable(current_type, **iterator);
				(*iterator)->RenderBy(*render_system);
				iterator = renderers_array_[count].erase(iterator);
			}
			shader_manager->Reset(current_type);
		}
		device_->EndScene();
	}
	else
	{
		for (int count = 0; count < static_cast<int>(kShadowMapShaderMax); ++count)
		{
			renderers_array_[count].clear();
		}
	}
#endif
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
		, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &shadow_map_,NULL);
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
	for (auto& renderers : renderers_array_)
	{
		renderers.clear();
	}
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	SAFE_RELEASE(shadow_map_);
	SAFE_RELEASE(shadow_map_surface_);
#endif
}
