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
#include "animatorComponent.h"
#include "actorBehaviorComponent.h"
#include "playerBehaviorComponent.h"
#include "enemyBehaviorComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"
#include "AABBColliderComponent.h"
#include "playerUIObject.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOM::PRIORITY& pri)
	: CGameObject(pri)
	, m_pAnimator(nullptr)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool CGameObjectActor::Init(void)
{
	CGameObject::Init();
	m_pAnimator->Init();
	return true;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void CGameObjectActor::Uninit(void)
{
	CGameObject::Uninit();
	m_pAnimator->Release();
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void CGameObjectActor::LateUpdate(void)
{
	CGameObject::LateUpdate();
	m_pAnimator->Update();
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const string &modelPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObjectActor(GOM::PRI_3D);

	//Tag
	pObj->SetTag("Player");

	//�R���|�l���g
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;
	pObj->m_pAnimator = new CAnimatorComponent(pObj, modelPath);
	auto pAb = new CActorBehaviorComponent(pObj, *pRb, pObj->m_pAnimator);
	auto pPb = new CPlayerBehaviorComponent(pObj, *pAb);
	pObj->m_listpBehavior.push_back(pAb);
	pObj->m_listpBehavior.push_back(pPb);
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, CKFVec3(0.3f * vScale.m_fX, 0.6f * vScale.m_fY, 0.3f * vScale.m_fZ));
	pCollider->SetOffset(CKFVec3(0.0f, 0.6f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	//UI
	//CPlayerUIObject::Create(pPb);

	return pObj;
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreateEnemy(const string &modelPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObjectActor(GOM::PRI_3D);

	//Tag
	pObj->SetTag("Enemy");

	//�R���|�l���g
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;
	pObj->m_pAnimator = new CAnimatorComponent(pObj, modelPath);
	
	//�r�w�C�r�A
	auto pAb = new CActorBehaviorComponent(pObj, *pRb, pObj->m_pAnimator);
	auto pEb = new CEnemyBehaviorComponent(pObj, *pAb);
	pObj->m_listpBehavior.push_back(pAb);
	pObj->m_listpBehavior.push_back(pEb);
	
	//�R���C�_�[
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, CKFVec3(0.3f * vScale.m_fX, 0.6f * vScale.m_fY, 0.3f * vScale.m_fZ));
	pCollider->SetOffset(CKFVec3(0.0f, 0.6f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);
	auto pDetector = new CSphereColliderComponent(pObj, CS::DYNAMIC, 6.0f);
	pDetector->SetTrigger(true);
	pDetector->SetTag("detector");
	pObj->AddCollider(pDetector);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}