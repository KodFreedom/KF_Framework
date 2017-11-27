//--------------------------------------------------------------------------------
//�@renderer_manager.cpp
//  render the registered renderers
//	�����_���[�Ǘ���
//	Author : �����^(KodFreedom)
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
//  �X�V����
//--------------------------------------------------------------------------------
void RendererManager::Update(void)
{
	//TODO: Z sort
}

//--------------------------------------------------------------------------------
//  �`�揈��
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
//  �o�^����
//--------------------------------------------------------------------------------
void RendererManager::Register(MeshRenderer* renderer)
{
	renderers_arrays_[renderer->GetRenderPriority()][renderer->GetRenderState()].push_back(renderer);
}