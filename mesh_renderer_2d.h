//--------------------------------------------------------------------------------
//	2d���b�V���`��R���|�l���g
//�@mesh_renderer_2d.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_renderer.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer2d : public MeshRenderer
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MeshRenderer2d(GameObject& owner
		, const RenderPriority& priority = kDefaultPriority
		, const ShaderType& shader_type = kDefaultShader)
		: MeshRenderer(owner, priority, shader_type) {}
	~MeshRenderer2d() {}

	//--------------------------------------------------------------------------------
	//  �`�揈��
	//--------------------------------------------------------------------------------
	void RenderBy(const RenderSystem& render_system) override;
};