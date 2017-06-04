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
#include "textureManager.h"
#include "materialManager.h"

#ifdef USING_DIRECTX9
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
	//チェックMeshComponentのタイプ
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pMesh->GetType() != CMeshComponent::MESH_3D) { return; }
	C3DMeshComponent* p3DMesh = (C3DMeshComponent*)pMesh;
	const C3DMeshComponent::MESH3D& meshInfo = p3DMesh->GetMeshInfo();

	//マトリクス算出
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//単位行列に初期化
	CKFMath::MtxIdentity(&mtxWorld);

	//回転(Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
	mtxWorld *= mtxRot;

	//平行移動
	CKFMath::MtxTranslation(&mtxPos, m_pGameObj->GetPos());
	mtxWorld *= mtxPos;

	m_pGameObj->SetMatrix(mtxWorld);

#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//RenderState設定
	m_pRenderState->SetRenderState();

	//マトリックス設定
	D3DXMATRIX mtx = mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		meshInfo.pVtxBuffer,	//頂点バッファ
		0,						//オフセット（開始位置）
		sizeof(VERTEX_3D));		//ストライド量

	// 頂点インデックスの設定
	pDevice->SetIndices(meshInfo.pIdxBuffer);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(p3DMesh->GetTexName());
	pDevice->SetTexture(0, pTexture);

	// マテリアルの設定
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(p3DMesh->GetMatType());
	pDevice->SetMaterial(&mat);

	//プリミティブ描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		meshInfo.nNumVtx,
		0,
		meshInfo.nNumPolygon);

	//RenderState戻す
	m_pRenderState->ResetRenderState();
#endif
}
