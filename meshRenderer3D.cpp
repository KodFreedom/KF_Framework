//--------------------------------------------------------------------------------
//	3Dメッシュ描画コンポネント
//　MeshRenderer3D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshRenderer3D.h"
#include "mesh.h"
#include "gameObject.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void MeshRenderer3D::Render(void)
{
	Renderer::Instance()->Render(
		owner->GetMesh()->GetMeshName(),
		textureName,
		materialID,
		owner->GetTransform()->GetCurrentWorldMatrix());
}
