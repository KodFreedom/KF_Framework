//--------------------------------------------------------------------------------
//	アクター描画コンポネント
//　actorMeshDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorMeshDrawComponent.h"
#include "actorMeshComponent.h"
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
void CActorMeshDrawComponent::Draw(CGameObject &gameObj, const CMeshComponent &meshComponent)
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

	CActorMeshComponent* pMesh = (CActorMeshComponent*)&meshComponent;
	const CTM::TEX_NAME& texName = pMesh->GetTexName();
	const CMM::MATERIAL& matType = pMesh->GetMatType();
	const CMOM::MODEL_NAME& modelName = pMesh->GetModelName();
	CActorMeshComponent::ACTOR_MOTION_INFO& info = pMesh->GetMotionInfo();

	//モデルの取得
	CModel* pModel = GetManager()->GetModelManager()->GetModel(modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;

	//RenderState設定
	m_pRenderState->SetRenderState();

	pModelActor->Draw(mtxWorld, info.vectorPartsInfo, matType, texName);

	//RenderState戻す
	m_pRenderState->ResetRenderState();
}
