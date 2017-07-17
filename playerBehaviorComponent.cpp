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
#include "camera.h"
#include "inputManager.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"
#include "colliderComponent.h"
#include "KF_CollisionSystem.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CPlayerBehaviorComponent::CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed, const float& fJumpForce)
	: CBehaviorComponent(pGameObj)
	, c_pRigidbody(pRigidbody)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
	, m_fLifeNow(c_fLifeMax)
{}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Update(void)
{
	m_fLifeNow -= 0.1f;
	m_fLifeNow = m_fLifeNow < 0.0f ? 0.0f : m_fLifeNow;
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();
	bool bCanControl = true;

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) { bCanControl = false; }

	if (bCanControl)
	{
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
		}
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnTrigger(const CColliderComponent& collider)
{

}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnCollision(const CCollisionInfo& collisionInfo)
{

}