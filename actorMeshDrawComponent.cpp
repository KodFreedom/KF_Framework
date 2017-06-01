//--------------------------------------------------------------------------------
//	�A�N�^�[�`��R���|�l���g
//�@actorMeshDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
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
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CActorMeshDrawComponent::Draw(CGameObject &gameObj, const CMeshComponent &meshComponent)
{
	//�}�g���N�X�Z�o
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(&mtxWorld);

	//��](Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, gameObj.GetRot());
	mtxWorld *= mtxRot;

	//���s�ړ�
	CKFMath::MtxTranslation(&mtxPos, gameObj.GetPos());
	mtxWorld *= mtxPos;

	gameObj.SetMatrix(mtxWorld);

	CActorMeshComponent* pMesh = (CActorMeshComponent*)&meshComponent;
	const CTM::TEX_NAME& texName = pMesh->GetTexName();
	const CMM::MATERIAL& matType = pMesh->GetMatType();
	const CMOM::MODEL_NAME& modelName = pMesh->GetModelName();
	CActorMeshComponent::ACTOR_MOTION_INFO& info = pMesh->GetMotionInfo();

	//���f���̎擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;

	//RenderState�ݒ�
	m_pRenderState->SetRenderState();

	pModelActor->Draw(mtxWorld, info.vectorPartsInfo, matType, texName);

	//RenderState�߂�
	m_pRenderState->ResetRenderState();
}
