//--------------------------------------------------------------------------------
//@renderer_manager.cpp
//  render the registered renderers
//	ƒŒƒ“ƒ_ƒ‰[ŠÇ—ŽÒ
//	Author : ™•¶ž^(KodFreedom)
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
//  XVˆ—
//--------------------------------------------------------------------------------
void RendererManager::Update(void)
{
	//TODO: Z sort
}

//--------------------------------------------------------------------------------
//  •`‰æˆ—
//--------------------------------------------------------------------------------
void RendererManager::Render(void)
{
	auto render_system = MainSystem::Instance()->GetRenderSystem();
	auto shader_manager = MainSystem::Instance()->GetShaderManager();
	for (int count_priority = 0; count_priority < static_cast<int>(kPriorityMax); ++count_priority)
	{
		if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
		{
			MainSystem::Instance()->GetRenderSystem()->SetRenderState(AlphaMode::kAlphaTest);
		}

		for (int count_shader = 0; count_shader < static_cast<int>(kShaderMax); ++count_shader)
		{
			auto& renderers = renderers_arrays_[count_priority][count_shader];
			if (renderers.empty()) continue;
			shader_manager->Set(static_cast<ShaderType>(count_shader));
			for (auto iterator = renderers.begin(); iterator != renderers.end();)
			{
				shader_manager->SetConstantTable(**iterator);
				render_system->Render((*iterator)->GetMeshName());
				iterator = renderers.erase(iterator);
			}
			shader_manager->Reset(static_cast<ShaderType>(count_shader));
		}

		if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
		{
			MainSystem::Instance()->GetRenderSystem()->SetRenderState(AlphaMode::kAlphaNone);
		}
	}
}

//--------------------------------------------------------------------------------
//  “o˜^ˆ—
//--------------------------------------------------------------------------------
void RendererManager::Register(MeshRenderer* renderer)
{
	renderers_arrays_[renderer->GetRenderPriority()][renderer->GetShaderType()].push_back(renderer);
}