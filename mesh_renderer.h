//--------------------------------------------------------------------------------
//	���b�V���`��R���|�l���g
//�@mesh_renderer.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "renderer_manager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class RenderSystem;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MeshRenderer(GameObject& owner, const RenderPriority& priority, const ShaderType& shader_type)
		: Component(owner)
		, priority_(priority)
		, shader_type_(shader_type)
		, is_cast_shadow_(false) {}
	~MeshRenderer() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	virtual bool Init(void) override;

	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	virtual void Uninit(void) override;

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	virtual void Update(void);

	//--------------------------------------------------------------------------------
	//  �`�揈��
	//--------------------------------------------------------------------------------
	virtual void RenderBy(const RenderSystem& render_system) = 0;

	//Set�֐�
	void SetMesh(const String& name);
	void SetMaterial(const String& name);
	void SetRenderPriority(const RenderPriority& value) { priority_ = value; }
	void SetShaderType(const ShaderType& value) { shader_type_ = value; }
	void SetCastShadowFlag(const bool& value) { is_cast_shadow_ = value; }

	//Get�֐�
	const auto& GetMeshName(void) const { return mesh_name_; }
	const auto& GetMaterialName(void) const { return material_name_; }
	const auto& GetRenderPriority(void) const { return priority_; }
	const auto& GetShaderType(void) const { return shader_type_; }

protected:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	String          mesh_name_;
	String          material_name_;
	RenderPriority  priority_;
	ShaderType      shader_type_;
	bool            is_cast_shadow_;
};