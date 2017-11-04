//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@playerBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "manager.h"
#include "inputManager.h"
#include "modeDemo.h"
#include "camera.h"
#include "gameObjectActor.h"
#include "collider.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerBehaviorComponent::CPlayerBehaviorComponent(GameObject* const pGameObj, CActorBehaviorComponent& actor)
	: CBehaviorComponent(pGameObj)
	, m_actor(actor)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CPlayerBehaviorComponent::Init(void)
{
	m_actor.SetLifeMax(100.0f);
	m_actor.SetLifeNow(100.0f);
	m_actor.SetJumpSpeed(20.0f);
	m_actor.SetTurnSpeedMin(2.0f * KF_PI * DELTA_TIME);
	m_actor.SetTurnSpeedMax(4.0f * KF_PI * DELTA_TIME);
	m_actor.SetMoveSpeed(10.0f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Update(void)
{
	auto pInput = Main::GetManager()->GetInputManager();
	auto vAxis = Vector2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = Main::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(Vector3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.X + vCamForward * vAxis.Y;
	CKFMath::VecNormalize(vMove);
	auto bJump = pInput->GetKeyTrigger(Input::Jump);
	auto bAttack = pInput->GetKeyTrigger(Input::Attack);
	m_actor.Act(vMove, bJump, bAttack);;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::LateUpdate(void)
{
	if (m_actor.GetLifeNow() <= 0.0f)
	{
		auto pMode = Main::GetManager()->GetMode();
		auto pModeDemo = dynamic_cast<ModeDemo*>(pMode);
		pModeDemo->EndMode(true);
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnTrigger(Collider& colliderThis, Collider& collider)
{
	if (collider.GetGameObject()->GetTag()._Equal("Enemy"))
	{//����`�F�b�N
		if (collider.GetTag()._Equal("weapon") && colliderThis.GetTag()._Equal("body"))
		{
#ifdef _DEBUG
			Main::GetManager()->GetDebugManager()->DisplayScroll(GetGameObject()->GetName() + " is hurted by" + collider.GetGameObject()->GetParentName() + "!");
#endif
			m_actor.Hit(5.0f);
		}
	}

	if (collider.GetGameObject()->GetTag()._Equal("Goal"))
	{
		if (colliderThis.GetTag()._Equal("body"))
		{
			auto pMode = Main::GetManager()->GetMode();
			auto pModeDemo = dynamic_cast<ModeDemo*>(pMode);
			pModeDemo->EndMode(false);
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnCollision(CollisionInfo& collisionInfo)
{

}