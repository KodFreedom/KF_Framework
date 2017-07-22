//--------------------------------------------------------------------------------
//	プレイやノーマルステータス
//　playerNormalStatus.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerNormalStatus.h"
#include "playerJumpStatus.h"
#include "playerAttackStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"
#include "mode.h"
#include "camera.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CPlayerNormalStatus::Update(CActorBehaviorComponent& actor)
{
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();

	//移動
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;
		CTransformComponent* pTrans = actor.m_pGameObj->GetTransformComponent();

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

		//回転
		actor.Turn(vForwardNext);

		//移動設定
		actor.Move(pActor);
	}
	else
	{
		actor.Stay(pActor);
	}

	//ジャンプ
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		actor.Jump(pActor);
		actor.ChangeStatus(new CPlayerJumpStatus);
		return;
	}

	//攻撃
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		actor.Attack(pActor);
		actor.ChangeStatus(new CPlayerAttackStatus);
		return;
	}
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CPlayerNormalStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}