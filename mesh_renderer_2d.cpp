//--------------------------------------------------------------------------------
//  2d���b�V���`��R���|�l���g
//  mesh_renderer_2d.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_2d.h"
#include "render_system.h"
#include "main_system.h"

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer2d::Update(void)
{
    MainSystem::Instance()->GetRendererManager()->Register(this);
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void MeshRenderer2d::RenderBy(const RenderSystem& render_system)
{
    render_system.Render2dMesh(mesh_name_);
}