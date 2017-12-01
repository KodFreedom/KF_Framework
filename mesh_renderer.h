//--------------------------------------------------------------------------------
//	メッシュ描画コンポネント
//　MeshRenderer.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "renderer_manager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
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
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual bool Init(void) override;

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	virtual void Uninit(void) override;

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	virtual void Update(void);

	//Set関数
	void SetMesh(const String& name);
	void SetMaterial(const String& name);
	void SetRenderPriority(const RenderPriority& value) { priority_ = value; }
	void SetShaderType(const ShaderType& value) { shader_type_ = value; }

	//Get関数
	const auto& GetMeshName(void) const { return mesh_name_; }
	const auto& GetMaterialName(void) const { return material_name_; }
	const auto& GetRenderPriority(void) const { return priority_; }
	const auto& GetShaderType(void) const { return shader_type_; }

protected:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	String          mesh_name_;
	String          material_name_;
	RenderPriority  priority_;
	ShaderType      shader_type_;
};