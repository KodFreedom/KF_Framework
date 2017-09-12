//--------------------------------------------------------------------------------
//	3Dメッシュ描画コンポネント
//　3DMeshDrawComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DMeshDrawComponent.h"
#include "3DMeshComponent.h"
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
void C3DMeshDrawComponent::Draw(void)
{
	const auto pMeshInfo = CMain::GetManager()->GetMeshManager()->GetMesh(c_pMesh->GetMeshName());

	//マトリクス算出
	auto mtxWorld = m_pGameObj->GetTransformComponent()->GetMatrix();

#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	//RenderState設定
	m_pRenderState->SetRenderState();

	//マトリックス設定
	D3DXMATRIX mtx = mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		pMeshInfo->m_pVtxBuffer,//頂点バッファ
		0,						//オフセット（開始位置）
		sizeof(VERTEX_3D));		//ストライド量

	// 頂点インデックスの設定
	pDevice->SetIndices(pMeshInfo->m_pIdxBuffer);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	auto pTexture = CMain::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
	pDevice->SetTexture(0, pTexture);

	// マテリアルの設定
	D3DMATERIAL9 mat = CMain::GetManager()->GetMaterialManager()->GetMaterial(m_usMatID);
	pDevice->SetMaterial(&mat);

	//プリミティブ描画
	pDevice->DrawIndexedPrimitive(
		(_D3DPRIMITIVETYPE)pMeshInfo->m_drawType,
		0,
		0,
		pMeshInfo->m_nNumVtx,
		0,
		pMeshInfo->m_nNumPolygon);

	//RenderState戻す
	m_pRenderState->ResetRenderState();
#endif
}
