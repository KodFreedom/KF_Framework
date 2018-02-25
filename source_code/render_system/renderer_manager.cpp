//--------------------------------------------------------------------------------
//@renderer_manager.cpp
//  render the registered renderers
//  ƒŒƒ“ƒ_ƒ‰[ŠÇ—Ò
//  Author : ™•¶^(KodFreedom)
//--------------------------------------------------------------------------------
#include "renderer_manager.h"
#include "../main_system.h"
#include "render_system.h"
#include "../component/renderer/mesh_renderer.h"
#include "../game_object/game_object.h"
#include "../component/transform/transform.h"
#include "../shader/shader_manager.h"

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
    auto& render_system = MainSystem::Instance().GetRenderSystem();
    auto& shader_manager = MainSystem::Instance().GetShaderManager();
    for (int count_priority = 0; count_priority < static_cast<int>(kPriorityMax); ++count_priority)
    {
        if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
        {// enable alpha test
            render_system.SetRenderState(AlphaMode::kAlphaTest);
        }

        for (int count_shader = 0; count_shader < static_cast<int>(kShaderMax); ++count_shader)
        {
            auto& renderers = renderers_arrays_[count_priority][count_shader];
            if (renderers.empty()) continue;

            const ShaderType& current_type = static_cast<ShaderType>(count_shader);
            shader_manager.Set(current_type);

            for (auto iterator = renderers.begin(); iterator != renderers.end();)
            {// render
                shader_manager.SetConstantTable(current_type, **iterator);
                (*iterator)->RenderBy(render_system);
                iterator = renderers.erase(iterator);
            }

            shader_manager.Reset(current_type);
        }

        if (RenderPriority::kUseAlphaTest == static_cast<RenderPriority>(count_priority))
        {// disable alpha test
            render_system.SetRenderState(AlphaMode::kAlphaNone);
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