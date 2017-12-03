//--------------------------------------------------------------------------------
//	2dメッシュ描画コンポネント
//　mesh_renderer_2d.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_renderer.h"

//--------------------------------------------------------------------------------
//  クラス宣言
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
	//  描画処理
	//--------------------------------------------------------------------------------
	void RenderBy(const RenderSystem& render_system) override;
};