//--------------------------------------------------------------------------------
//	�A�N�^�[
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectActor.h"
#include "playerInputComponent.h"
#include "3DRigidbodyComponent.h"
#include "actorMeshComponent.h"
#include "actorMeshDrawComponent.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor() : CGameObject3D()
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOM::PRIORITY& pri) : CGameObject3D(pri)
{

}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor;

	//�R���|�l���g
	pObj->m_pInput = new CPlayerInputComponent(pObj);
	pObj->m_pCollider = new CSphereColliderComponent(pObj, CM::DYNAMIC, 0.0f);
	pObj->m_pRigidbody = new C3DRigidbodyComponent(pObj);
	CActorMeshComponent* pMesh = new CActorMeshComponent(pObj);
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CActorMeshDrawComponent(pObj);

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//������
	pObj->Init();

	return pObj;
}