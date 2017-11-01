//--------------------------------------------------------------------------------
//	3Dメッシュ描画コンポネント
//　3DMeshRenderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DMeshRenderComponent.h"
#include "meshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "mesh.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void C3DMeshRenderComponent::Render(void)
{
	auto mesh = m_pGameObj->GetMeshComponent();
	const auto meshInfo = Main::GetManager()->GetMeshManager()->GetMesh(mesh->GetMeshName());

	//マトリクス算出
	auto mtxWorld = m_pGameObj->GetTransformComponent()->GetMatrix();

#ifdef USING_DIRECTX
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	
	//マトリックス設定
	D3DXMATRIX mtx = mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		meshInfo->m_pVtxBuffer,//頂点バッファ
		0,						//オフセット（開始位置）
		sizeof(VERTEX_3D));		//ストライド量

	// 頂点インデックスの設定
	pDevice->SetIndices(meshInfo->m_pIdxBuffer);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	auto pTexture = Main::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
	pDevice->SetTexture(0, pTexture);

	// マテリアルの設定
	D3DMATERIAL9 mat = Main::GetManager()->GetMaterialManager()->GetMaterial(m_usMatID);
	pDevice->SetMaterial(&mat);

	//プリミティブ描画
	pDevice->DrawIndexedPrimitive(
		(_D3DPRIMITIVETYPE)meshInfo->CurrentType,
		0,
		0,
		meshInfo->VertexNumber,
		0,
		meshInfo->PolygonNumber);
#endif
}
