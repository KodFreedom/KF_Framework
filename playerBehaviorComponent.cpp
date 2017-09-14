//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "manager.h"
#include "inputManager.h"
#include "modeDemo.h"
#include "camera.h"
#include "gameObjectActor.h"
#include "colliderComponent.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CPlayerBehaviorComponent::CPlayerBehaviorComponent(CGameObject* const pGameObj, CActorBehaviorComponent& actor)
	: CBehaviorComponent(pGameObj)
	, m_actor(actor)
{}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Update(void)
{
	auto pInput = CMain::GetManager()->GetInputManager();
	auto vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = CMain::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(CKFVec3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.m_fX + vCamForward * vAxis.m_fY;
	if (CKFMath::VecMagnitudeSquare(vMove) > 1.0f) { CKFMath::VecNormalize(vMove); }
	auto bJump = pInput->GetKeyTrigger(CInputManager::K_JUMP);
	auto bAttack = pInput->GetKeyTrigger(CInputManager::K_ATTACK);
	m_actor.Act(vMove, bJump, bAttack);;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::LateUpdate(void)
{
	if (m_actor.GetLifeNow() <= 0.0f)
	{
		auto pMode = CMain::GetManager()->GetMode();
		auto pModeDemo = dynamic_cast<CModeDemo*>(pMode);
		pModeDemo->EndMode(true);
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetTag()._Equal("Enemy"))
	{//武器チェック
		if (collider.GetTag()._Equal("weapon") && colliderThis.GetTag()._Equal("body"))
		{
#ifdef _DEBUG
			CMain::GetManager()->GetDebugManager()->DisplayScroll(GetGameObject()->GetName() + " is hurted by" + collider.GetGameObject()->GetParentName() + "!");
#endif
			m_actor.Hit(5.0f);
		}
	}

	if (collider.GetGameObject()->GetTag()._Equal("Goal"))
	{
		if (colliderThis.GetTag()._Equal("body"))
		{
			auto pMode = CMain::GetManager()->GetMode();
			auto pModeDemo = dynamic_cast<CModeDemo*>(pMode);
			pModeDemo->EndMode(false);
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnCollision(CCollisionInfo& collisionInfo)
{

}