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
void CModelMeshDrawComponent::Draw(CGameObject &gameObj, const CMeshComponent &meshComponent)
{
	//マトリクス算出
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//単位行列に初期化
	CKFMath::MtxIdentity(&mtxWorld);

	//回転(Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, gameObj.GetRot());
	mtxWorld *= mtxRot;

	//平行移動
	CKFMath::MtxTranslation(&mtxPos, gameObj.GetPos());
	mtxWorld *= mtxPos;

	gameObj.SetMatrix(mtxWorld);

	CModelMeshComponent* pMesh = (CModelMeshComponent*)&meshComponent;
	const CTM::TEX_NAME& texName = pMesh->GetTexName();
	const CMM::MATERIAL& matType = pMesh->GetMatType();
	CModel* pModel = GetManager()->GetModelManager()->GetModel(pMesh->GetModelName());
	if (pModel != NULL)
	{
		//RenderState設定
		m_pRenderState->SetRenderState();

		pModel->Draw(mtxWorld, matType, texName);

		//RenderState戻す
		m_pRenderState->ResetRenderState();
	}
}
