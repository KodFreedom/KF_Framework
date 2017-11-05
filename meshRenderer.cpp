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
#include "meshComponent.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//	クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool MeshRenderer::Init(void)
{
	auto mesh = owner->GetMesh();
	auto& renderInfo = MeshManager::Instance()->GetRenderInfoBy(mesh->GetMeshName());
	if (!renderInfo.TextureName.empty()) Set(renderInfo.TextureName);
	lighting = renderInfo.CurrentLighting;
	cullMode = renderInfo.CurrentCullMode;
	synthesis = renderInfo.CurrentSynthesis;
	fillMode = renderInfo.CurrentFillMode;
	alpha = renderInfo.CurrentAlpha;
	fog = renderInfo.CurrentFog;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	if (CameraManager::Instance()->GetMainCamera()->
		IsInRange(owner->GetTransform()->GetCurrentPosition()))
	{
		RenderManager::Instance()->Register(this);
	}
}

//--------------------------------------------------------------------------------
//  テクスチャ設定
//--------------------------------------------------------------------------------
void MeshRenderer::Set(const string& texture)
{
	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}
	textureName = texture;
	TextureManager::Instance()->Use(textureName);
}