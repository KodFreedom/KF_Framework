////--------------------------------------------------------------------------------
////	アクター描画コンポネント
////　actorMeshDrawComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-05-22
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  インクルードファイル
////--------------------------------------------------------------------------------
//#include "actorMeshDrawComponent.h"
//#include "actorMeshComponent.h"
//#include "transformComponent.h"
//#include "manager.h"
//#include "textureManager.h"
//#include "materialManager.h"
//#include "modelManager.h"
//#include "gameObject.h"
//#include "model.h"
//
////--------------------------------------------------------------------------------
////  クラス
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  描画処理
////--------------------------------------------------------------------------------
//void CActorMeshDrawComponent::Draw(void)
//{
//	//マトリクス算出
//	CKFMtx44 mtxWorld;
//	CKFMtx44 mtxRot;
//	CKFMtx44 mtxPos;
//
//	//単位行列に初期化
//	CKFMath::MtxIdentity(mtxWorld);
//
//	//回転(Y->X->Z)
//	//CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
//	mtxRot = m_pGameObj->GetTransformComponent()->GetMatrixRot();
//	mtxWorld *= mtxRot;
//
//	//平行移動
//	CKFMath::MtxTranslation(mtxPos, m_pGameObj->GetTransformComponent()->GetPos());
//	mtxWorld *= mtxPos;
//
//	m_pGameObj->GetTransformComponent()->SetMatrix(mtxWorld);
//
//	const CMOM::MODEL_NAME& modelName = c_pMesh->GetModelName();
//	CActorMeshComponent::ACTOR_MOTION_INFO& info = c_pMesh->GetMotionInfo();
//
//	//モデルの取得
//	CModel* pModel = GetManager()->GetModelManager()->GetModel(modelName);
//	if (pModel == NULL) { return; }
//	CModelActorX* pModelActor = (CModelActorX*)pModel;
//
//	//RenderState設定
//	m_pRenderState->SetRenderState();
//
//	pModelActor->Draw(mtxWorld, info.vectorPartsInfo, m_matType, m_strTexName);
//
//	//RenderState戻す
//	m_pRenderState->ResetRenderState();
//}
