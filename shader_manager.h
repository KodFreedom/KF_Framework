//--------------------------------------------------------------------------------
//�@shader_manager.h
//	�V�F�[�_�[�Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MeshRenderer;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
class ShaderDirectX9;
#endif

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
class ShaderManager
{
public:
	//--------------------------------------------------------------------------------
	//  ��������
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static ShaderManager* Create(const LPDIRECT3DDEVICE9 device)
	{
		auto instance = MY_NEW ShaderManager(device);
		instance->Init();
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  �V�F�[�_�[�̎g�p
	//--------------------------------------------------------------------------------
	void Set(const ShaderType& type);

	//--------------------------------------------------------------------------------
	//  �V�F�[�_�[�̒萔�e�[�u���̐ݒ�
	//--------------------------------------------------------------------------------
	void SetConstantTable(const MeshRenderer& renderer);

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ShaderManager(const LPDIRECT3DDEVICE9 device) : device_(device) {}
	ShaderManager() : device_(nullptr) {}
	ShaderManager(const ShaderManager& value) : device_(nullptr) {}
#else
	ShaderManager() {}
	ShaderManager(const ShaderManager& value) {}
#endif
	ShaderManager& operator=(const ShaderManager& value) {}
	~ShaderManager() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ShaderDirectX9* shaders_[ShaderType::kShaderMax];
	const LPDIRECT3DDEVICE9 device_; // directx9�̃f�B�o�C�X
#endif
};