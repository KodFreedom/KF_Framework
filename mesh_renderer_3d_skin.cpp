//--------------------------------------------------------------------------------
//	3d�X�L�����b�V���`��R���|�l���g
//�@mesh_renderer_3d_skin.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d_skin.h"
#include "render_system.h"

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::RenderBy(const RenderSystem& render_system)
{
	render_system.Render3dSkin(mesh_name_);
}