//--------------------------------------------------------------------------------
//  2dメッシュ描画コンポネント
//  mesh_renderer_2d.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_2d.h"
#include "render_system.h"
#include "main_system.h"

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MeshRenderer2d::Update(void)
{
    MainSystem::Instance()->GetRendererManager()->Register(this);
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void MeshRenderer2d::RenderBy(const RenderSystem& render_system)
{
    render_system.Render2dMesh(mesh_name_);
}