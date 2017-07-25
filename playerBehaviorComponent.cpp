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
#include "manager.h"
#include "mode.h"
#include "gameObjectActor.h"
#include "colliderComponent.h"
#include "playerNormalStatus.h"
#include "status.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CPlayerBehaviorComponent::CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody)
	: CActorBehaviorComponent(pGameObj, pRigidbody)
	, m_pStatus(nullptr)
	, m_usCntWhosYourDaddy(0)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CPlayerBehaviorComponent::Init(void)
{
	if (!m_pStatus) { m_pStatus = new CPlayerNormalStatus; }
	m_fMovementSpeed = 0.075f;
	m_fJumpForce = 20.0f;
	m_fTurnRate = 0.2f;
	m_fLifeMax = 100.0f;
	m_fLifeNow = 100.0f;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Uninit(void)
{
	CActorBehaviorComponent::Uninit();

	if (m_pStatus)
	{
		delete m_pStatus;
		m_pStatus = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Update(void)
{
	CActorBehaviorComponent::Update();
	m_pStatus->Update(*this);
	if (m_usCntWhosYourDaddy > 0) { m_usCntWhosYourDaddy--; }
	/*if (m_usCntWhosYourDaddy) { m_usCntWhosYourDaddy--; }
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();
	bool bCanControl = true;

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) 
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !m_pAttackCollider)
		{
			m_pAttackCollider = new CSphereColliderComponent(m_pGameObj, CM::DYNAMIC, CKFVec3(0.0f, 0.6f, 2.1f), 0.9f);
			m_pAttackCollider->SetTag("weapon");
			m_pAttackCollider->SetTrigger(true);
		}
		bCanControl = false; 
	}

	if (!bCanControl) { return; }

	if (m_pAttackCollider)
	{
		m_pGameObj->DeleteCollider(m_pAttackCollider);
		m_pAttackCollider->Release();
		m_pAttackCollider = nullptr;
	}

	//移動
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;
		CTransformComponent* pTrans = m_pGameObj->GetTransformComponent();

		//回転計算
		CKFVec3 vUp = pTrans->GetUpNext();
		CKFVec3 vForward = pTrans->GetForwardNext();
		CKFVec3 vRight = pTrans->GetRightNext();

		//カメラ向きを算出する
		CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		CKFVec3 vForwardCamera = pCamera->GetVecLook();
		CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		if (fRot != 0.0f)
		{//操作より行く方向を回転する
			CKFMtx44 mtxYaw;
			CKFMath::MtxRotAxis(mtxYaw, vUp, fRot);
			CKFMath::Vec3TransformNormal(vForwardNext, mtxYaw);
		}

		CKFMath::VecNormalize(vForwardNext);
		vForwardNext = CKFMath::LerpNormal(vForward, vForwardNext, 0.2f);
		pTrans->RotByForward(vForwardNext);

		//移動設定
		c_pRigidbody->MovePos(vForwardNext * c_fSpeed);

		//移動モーション設定
		pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
	}
	else
	{
		//ニュートラルモーション設定
		pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
	}

	//ジャンプ
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP) && c_pRigidbody->IsOnGround())
	{
		//上方向にジャンプ
		c_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
		c_pRigidbody->SetOnGround(false);

		//ジャンプモーション設定
		pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
	}

	//攻撃
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		//ジャンプモーション設定
		pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
	}*/
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::LateUpdate(void)
{
	CActorBehaviorComponent::LateUpdate();
	m_pStatus->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetObjType() == CGameObject::OT_ENEMY)
	{//武器チェック
		if (!m_usCntWhosYourDaddy && collider.GetTag() == "weapon" && colliderThis.GetTag() == "body")
		{
			m_fLifeNow -= 10.0f;
			if (m_fLifeNow <= 0.0f)
			{
				m_fLifeNow = 0.0f;
				GetManager()->GetMode()->EndMode();
			}
			m_usCntWhosYourDaddy = 30;
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnCollision(CColliderComponent& colliderThis, CCollisionInfo& collisionInfo)
{

}

//--------------------------------------------------------------------------------
//  状態更新
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::ChangeStatus(CStatus* const pStatus)
{
	if (!pStatus) { return; }
	if (m_pStatus) { delete m_pStatus; }
	m_pStatus = pStatus;
}