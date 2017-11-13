//--------------------------------------------------------------------------------
//	描画用マネージャ
//　renderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "rendererManager.h"
#include "meshRenderer.h"
#include "gameObject.h"
#include "transform.h"
#include "renderSystem.h"
#include "renderState.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
RendererManager* RendererManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void RendererManager::Update(void)
{
	//Todo : Z sort
}

//--------------------------------------------------------------------------------
//  Render
//--------------------------------------------------------------------------------
void RendererManager::Render(void)
{
	auto renderSystem = RenderSystem::Instance();
	for (int countPriority = 0; countPriority < RP_Max; ++countPriority)
	{
		if (countPriority == RP_AlphaTest)
		{
			renderSystem->SetRenderState(Alpha::A_AlphaTest);
		}

		for (int countState = 0; countState < RS_Max; ++countState)
		{
			auto& renderers = renderersArrays[countPriority][countState];
			if (renderers.empty()) continue;

			renderStates[RS_Max]->Set();
			for (auto iterator = renderers.begin(); iterator != renderers.end();)
			{
				renderSystem->Render(
					(*iterator)->GetMeshName(),
					(*iterator)->GetMaterialName(),
					(*iterator)->GetGameObject()->GetTransform()->GetCurrentWorldMatrix());
				iterator = renderers.erase(iterator);
			}
			renderStates[RS_Max]->Reset();
		}

		if (countPriority == RP_AlphaTest)
		{
			renderSystem->SetRenderState(Alpha::A_None);
		}
	}
}

//--------------------------------------------------------------------------------
//  配列クリア処理
//--------------------------------------------------------------------------------
void RendererManager::Clear(void)
{
	for (auto& renderersArray : renderersArrays)
	{
		for (auto& renderers : renderersArray)
		{
			renderers.clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  登録処理
//--------------------------------------------------------------------------------
void RendererManager::Register(MeshRenderer* renderer)
{
	renderersArrays[renderer->GetRenderPriority()][renderer->GetRenderState()].push_back(renderer);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
RendererManager::RendererManager()
{
	for (auto& renderState : renderStates)
	{
		renderState = nullptr;
	}
	Clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void RendererManager::init(void)
{
	renderStates[RS_Default] = new DefaultRenderState;
	renderStates[RS_NoLight_NoFog] = new NoLightNoFog;
	renderStates[RS_NoCullMode] = new NoCullMode;
	renderStates[RS_NoLight_NoFog_NoCullMode] = new NoLightNoFogNoCullMode;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void RendererManager::uninit(void)
{
	for (auto& renderState : renderStates)
	{
		SAFE_DELETE(renderState);
	}
	Clear();
}