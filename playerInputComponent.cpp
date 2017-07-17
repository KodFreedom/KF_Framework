//--------------------------------------------------------------------------------
//
//　playerInputComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerInputComponent.h"
#include "manager.h"
#include "mode.h"
#include "camera.h"
#include "inputManager.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CPlayerInputComponent::CPlayerInputComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed, const float& fJumpForce)
	: CInputComponent(pGameObj)
	, m_pRigidbody(pRigidbody)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
{}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
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
			m_pRigidbody->MovePos(vForwardNext * c_fSpeed);

			//移動モーション設定
			pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
		}
		else
		{
			//ニュートラルモーション設定
			pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
		}

		//ジャンプ
		if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP) && m_pRigidbody->IsOnGround())
		{
			//上方向にジャンプ
			m_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
			m_pRigidbody->SetOnGround(false);

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