//--------------------------------------------------------------------------------
//　renderer_manager.cpp
//  render the registered renderers
//	レンダラー管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "renderer_manager.h"
#include "main_system.h"
#include "render_system.h"
#include "mesh_renderer.h"
#include "game_object.h"
#include "transform.h"
#include "shader_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void RendererManager::Update(void)
{
	//TODO: Z sort
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void RendererManager::Render(void)
{
	const auto render_system = MainSystem::Instance()->GetRenderSystem();
	const auto shader_manager = MainSystem::Instance()->GetShaderManager();
	for (int count_priority = 0; count_priority < static_cast<int>(kPriorityMax); ++count_priority)
	{
		if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
		{// enable alpha test
			MainSystem::Instance()->GetRenderSystem()->SetRenderState(AlphaMode::kAlphaTest);
		}

		for (int count_shader = 0; count_shader < static_cast<int>(kShaderMax); ++count_shader)
		{
			auto& renderers = renderers_arrays_[count_priority][count_shader];
			if (renderers.empty()) continue;

			shader_manager->Set(static_cast<ShaderType>(count_shader));

			for (auto iterator = renderers.begin(); iterator != renderers.end();)
			{// render
				shader_manager->SetConstantTable(**iterator);
				(*iterator)->RenderBy(*render_system);
				iterator = renderers.erase(iterator);
			}

			shader_manager->Reset(static_cast<ShaderType>(count_shader));
		}

		if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
		{// disable alpha test
			MainSystem::Instance()->GetRenderSystem()->SetRenderState(AlphaMode::kAlphaNone);
		}
	}
}

//--------------------------------------------------------------------------------
//  登録処理
//--------------------------------------------------------------------------------
void RendererManager::Register(MeshRenderer* renderer)
{
	renderers_arrays_[renderer->GetRenderPriority()][renderer->GetShaderType()].push_back(renderer);
}