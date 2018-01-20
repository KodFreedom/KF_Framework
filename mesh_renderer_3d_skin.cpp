//--------------------------------------------------------------------------------
//	3d�X�L�����b�V���`��R���|�l���g
//�@mesh_renderer_3d_skin.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d_skin.h"
#include "render_system.h"
#include "animator.h"
#include "main_system.h"
#include "shadow_map_system.h"

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::Update(void)
{
	MeshRenderer::Update();
	if (is_cast_shadow_ && is_registered_)
	{
		MainSystem::Instance()->GetShadowMapSystem()->Register(this);
	}
}


//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::RenderBy(const RenderSystem& render_system)
{
	render_system.Render3dSkin(mesh_name_);
}