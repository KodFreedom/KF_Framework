////--------------------------------------------------------------------------------
////	�A�N�^�[�`��R���|�l���g
////�@actorMeshDrawComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-05-22
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  �C���N���[�h�t�@�C��
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
////  �N���X
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  �`�揈��
////--------------------------------------------------------------------------------
//void CActorMeshDrawComponent::Draw(void)
//{
//	//�}�g���N�X�Z�o
//	CKFMtx44 mtxWorld;
//	CKFMtx44 mtxRot;
//	CKFMtx44 mtxPos;
//
//	//�P�ʍs��ɏ�����
//	CKFMath::MtxIdentity(mtxWorld);
//
//	//��](Y->X->Z)
//	//CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
//	mtxRot = m_pGameObj->GetTransformComponent()->GetMatrixRot();
//	mtxWorld *= mtxRot;
//
//	//���s�ړ�
//	CKFMath::MtxTranslation(mtxPos, m_pGameObj->GetTransformComponent()->GetPos());
//	mtxWorld *= mtxPos;
//
//	m_pGameObj->GetTransformComponent()->SetMatrix(mtxWorld);
//
//	const CMOM::MODEL_NAME& modelName = c_pMesh->GetModelName();
//	CActorMeshComponent::ACTOR_MOTION_INFO& info = c_pMesh->GetMotionInfo();
//
//	//���f���̎擾
//	CModel* pModel = GetManager()->GetModelManager()->GetModel(modelName);
//	if (pModel == NULL) { return; }
//	CModelActorX* pModelActor = (CModelActorX*)pModel;
//
//	//RenderState�ݒ�
//	m_pRenderState->SetRenderState();
//
//	pModelActor->Draw(mtxWorld, info.vectorPartsInfo, m_matType, m_strTexName);
//
//	//RenderState�߂�
//	m_pRenderState->ResetRenderState();
//}
