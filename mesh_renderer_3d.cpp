//--------------------------------------------------------------------------------
//  3dメッシュ描画コンポネント
//　mesh_renderer_3d_skin.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d.h"
#include "render_system.h"
#include "main_system.h"
#include "shadow_map_system.h"

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MeshRenderer3d::Update(void)
{
    MeshRenderer::Update();
    if (is_cast_shadow_ && is_registered_)
    {
        MainSystem::Instance()->GetShadowMapSystem()->Register(this);
    }
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void MeshRenderer3d::RenderBy(const RenderSystem& render_system)
{
    render_system.Render3dMesh(mesh_name_);
}