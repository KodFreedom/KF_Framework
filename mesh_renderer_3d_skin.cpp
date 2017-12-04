//--------------------------------------------------------------------------------
//	3d�X�L�����b�V���`��R���|�l���g
//�@mesh_renderer_3d_skin.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer_3d_skin.h"
#include "render_system.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void MeshRenderer3dSkin::RenderBy(const RenderSystem& render_system)
{
	render_system.Render3dSkin(mesh_name_);
}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  �e�N�X�`���̎擾
//--------------------------------------------------------------------------------
const LPDIRECT3DTEXTURE9 MeshRenderer3dSkin::GetBoneTexture(void) const
{
	return animator_.GetBoneTexture();
}
#endif