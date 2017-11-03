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
#include "sphereCollider.h"
#include "AABBCollider.h"
#include "playerUI.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
GameObjectActor::GameObjectActor(const Layer& layer)
	: GameObject(layer)
	, m_pAnimator(nullptr)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool GameObjectActor::Init(void)
{
	GameObject::Init();
	m_pAnimator->Init();
	return true;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void GameObjectActor::LateUpdate(void)
{
	GameObject::LateUpdate();
	m_pAnimator->Update();
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectActor::CreatePlayer(const string &modelPath, const Vector3& Position, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new GameObjectActor;

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
	//auto pCollider = new AABBCollider(pObj, DYNAMIC, Vector3(0.3f * vScale.X, 0.6f * vScale.Y, 0.3f * vScale.Z));
	auto pCollider = new SphereCollider(pObj, DYNAMIC, 0.6f);
	pCollider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	//UI
	//PlayerUI::Create(pPb);

	return pObj;
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectActor::CreateEnemy(const string &modelPath, const Vector3& Position, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new GameObjectActor;

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
	auto pCollider = new AABBCollider(pObj, DYNAMIC, Vector3(0.3f * vScale.X, 0.6f * vScale.Y, 0.3f * vScale.Z));
	pCollider->SetOffset(Vector3(0.0f, 0.6f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);
	auto pDetector = new SphereCollider(pObj, DYNAMIC, 6.0f);
	pDetector->SetTrigger(true);
	pDetector->SetTag("detector");
	pObj->AddCollider(pDetector);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void GameObjectActor::uninit(void)
{
	GameObject::uninit();
	SAFE_RELEASE(m_pAnimator);
}