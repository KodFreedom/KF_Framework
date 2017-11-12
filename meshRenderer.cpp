//--------------------------------------------------------------------------------
//	描画コンポネント
//　MeshRenderer.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshRenderer.h"
#include "gameObject.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "rendererManager.h"

//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool MeshRenderer::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
	if (!meshName.empty())
	{
		MeshManager::Instance()->Disuse(meshName);
		meshName.clear();
	}

	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}

	if (!materialName.empty())
	{
		MaterialManager::Instance()->Disuse(meshName);
		materialName.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	RendererManager::Instance()->Register(this);
}

//--------------------------------------------------------------------------------
//  メッシュの設定
//--------------------------------------------------------------------------------
void MeshRenderer::SetMeshName(const string& name)
{
	if (!meshName.empty())
	{
		MeshManager::Instance()->Disuse(meshName);
		meshName.clear();
	}

	meshName = name;
	MeshManager::Instance()->Use(name);
}

//--------------------------------------------------------------------------------
//  テクスチャの設定
//--------------------------------------------------------------------------------
void MeshRenderer::SetTextureName(const string& name)
{
	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}

	textureName = name;
	TextureManager::Instance()->Use(name);
}

//--------------------------------------------------------------------------------
//  マテリアルの設定
//--------------------------------------------------------------------------------
void MeshRenderer::SetMaterialName(const string& name)
{
	if (!materialName.empty())
	{
		MaterialManager::Instance()->Disuse(materialName);
		materialName.clear();
	}

	materialName = name;
	MaterialManager::Instance()->Use(name);
}