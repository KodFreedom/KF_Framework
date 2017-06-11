//--------------------------------------------------------------------------------
//	モデルメッシュ描画コンポネント
//　modelMeshDrawComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "modelMeshDrawComponent.h"
#include "modelMeshComponent.h"
#include "manager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "modelManager.h"
#include "gameObject.h"
#include "model.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelMeshDrawComponent::Draw(void)
{
	//チェックMeshComponentのタイプ
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pMesh->GetType() != CMeshComponent::MESH_MODEL) { return; }
	CModelMeshComponent* pModelMesh = (CModelMeshComponent*)pMesh;

	//マトリクス算出
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//単位行列に初期化
	CKFMath::MtxIdentity(&mtxWorld);

	//回転(Y->X->Z)
	mtxRot = m_pGameObj->GetMatrixRot();
	//CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
	//mtxWorld *= mtxRot;
	mtxWorld *= mtxRot;

	//平行移動
	CKFMath::MtxTranslation(&mtxPos, m_pGameObj->GetPos());
	mtxWorld *= mtxPos;

	m_pGameObj->SetMatrix(mtxWorld);

	const CTM::TEX_NAME& texName = pModelMesh->GetTexName();
	const CMM::MATERIAL& matType = pModelMesh->GetMatType();
	CModel* pModel = GetManager()->GetModelManager()->GetModel(pModelMesh->GetModelName());
	if (pModel != NULL)
	{
		//RenderState設定
		m_pRenderState->SetRenderState();

		pModel->Draw(mtxWorld, matType, texName);

		//RenderState戻す
		m_pRenderState->ResetRenderState();
	}
}
