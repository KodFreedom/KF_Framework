//--------------------------------------------------------------------------------
//	3dスキンメッシュ描画コンポネント
//　mesh_renderer_3d_skin.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d_skin.h"
#include "render_system.h"
#include "animator.h"
#include "main_system.h"
#include "shadow_map_system.h"

//--------------------------------------------------------------------------------
//  更新処理
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
//  描画処理
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::RenderBy(const RenderSystem& render_system)
{
	render_system.Render3dSkin(mesh_name_);
}