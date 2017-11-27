//--------------------------------------------------------------------------------
//@renderer_manager.cpp
//  render the registered renderers
//	ƒŒƒ“ƒ_ƒ‰[ŠÇ—ŽÒ
//	Author : ™•¶ž^(KodFreedom)
//--------------------------------------------------------------------------------
#include "renderer_manager.h"
#include "main_system.h"
#include "render_system.h"
#include "meshRenderer.h"
#include "gameObject.h"
#include "transform.h"

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
	for (int count_priority = 0; count_priority < static_cast<int>(kPriorityMax); ++count_priority)
	{
		for (int count_shader = 0; count_shader < static_cast<int>(kShaderMax); ++count_shader)
		{
			auto& renderers = renderers_arrays_[count_priority][count_shader];
			if (renderers.empty()) continue;

			//renderStates[count_shader]->Set();
			for (auto iterator = renderers.begin(); iterator != renderers.end();)
			{
				render_system->Render((*iterator)->GetMeshName());
				iterator = renderers.erase(iterator);
			}
			//renderStates[count_shader]->Reset();
		}
	}
}

//--------------------------------------------------------------------------------
//  “o˜^ˆ—
//--------------------------------------------------------------------------------
void RendererManager::Register(MeshRenderer* renderer)
{
	renderers_arrays_[renderer->GetRenderPriority()][renderer->GetRenderState()].push_back(renderer);
}