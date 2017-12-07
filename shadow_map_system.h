//--------------------------------------------------------------------------------
//�@shadow_map_system.h
//  render objects to shadow map
//	�V���h�E�}�b�v�������_�����O����V�X�e��
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MeshRenderer;
class MeshRenderer3d;
class MeshRenderer3dSkin;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class ShadowMapSystem
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static constexpr float kShadowMapWidth = 2048.0f;
	static constexpr float kShadowMapHeight = 2048.0f;

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
	ShadowMapSystem(const LPDIRECT3DDEVICE9 device) : device_(device)
	, shadow_map_(nullptr), shadow_map_surface_(nullptr) {}
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
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DTEXTURE9 shadow_map_;
	LPDIRECT3DSURFACE9 shadow_map_surface_;
	LPDIRECT3DSURFACE9 depth_stencil_surface_;
	const LPDIRECT3DDEVICE9 device_; // directx9�̃f�B�o�C�X
#endif
};