//--------------------------------------------------------------------------------
//	3dスキンメッシュ描画コンポネント
//　mesh_renderer_3d_skin.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d_skin.h"
#include "render_system.h"

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::RenderBy(const RenderSystem& render_system)
{
	render_system.Render3dSkin(mesh_name_);
}