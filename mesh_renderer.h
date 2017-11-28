//--------------------------------------------------------------------------------
//	���b�V���`��R���|�l���g
//�@MeshRenderer.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "renderer_manager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MeshRenderer(GameObject& owner)
		: Component(owner)
		, priority_(kDefaultPriority)
		, shader_type_(kDefaultShader) {}
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

	//Set�֐�
	void SetMesh(const String& name);
	void SetMaterial(const String& name);
	void SetRenderPriority(const RenderPriority& value) { priority_ = value; }
	void SetShaderType(const ShaderType& value) { shader_type_ = value; }

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
};