//--------------------------------------------------------------------------------
//	3dメッシュ描画コンポネント
//　mesh_renderer_3d.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_renderer.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class MeshRenderer3d : public MeshRenderer
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MeshRenderer3d(GameObject& owner
		, const RenderPriority& priority = kDefaultPriority
		, const ShaderType& shader_type = kDefaultShader)
		: MeshRenderer(owner, priority, shader_type, kMeshRenderer3d) {}
	~MeshRenderer3d() {}

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void) override;

	//--------------------------------------------------------------------------------
	//  描画処理
	//--------------------------------------------------------------------------------
	void RenderBy(const RenderSystem& render_system) override;
};