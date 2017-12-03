//--------------------------------------------------------------------------------
//	3d�X�L�����b�V���`��R���|�l���g
//�@mesh_renderer_3d_skin.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_renderer.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer3dSkin : public MeshRenderer
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MeshRenderer3dSkin(GameObject& owner
		, const RenderPriority& priority = kDefaultPriority
		, const ShaderType& shader_type = kDefaultShader)
		: MeshRenderer(owner, priority, shader_type) {}
	~MeshRenderer3dSkin() {}

	//--------------------------------------------------------------------------------
	//  �`�揈��
	//--------------------------------------------------------------------------------
	void RenderBy(const RenderSystem& render_system) override;
};