//--------------------------------------------------------------------------------
//	移動状態
//　moveStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "attackStatus.h"
#include "jumpStatus.h"
#include "moveStatus.h"
#include "normalStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
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
void CMoveStatus::Update(CActorBehaviorComponent& actor)
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
		//CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		//CKFVec3 vForwardCamera = pCamera->GetVecLook();
		//CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		//if (fRot != 0.0f)
		//{//操作より行く方向を回転する
		//	CKFMtx44 mtxYaw;
		//	CKFMath::MtxRotAxis(mtxYaw, vUp, fRot);
		//	CKFMath::Vec3TransformNormal(vForwardNext, mtxYaw);
		//}

		//CKFMath::VecNormalize(vForwardNext);

		//回転
		actor.Turn(pActor);

		//移動設定
		actor.Move(pActor);
	}
	else
	{
		actor.ChangeStatus(new CNormalStatus);
	}

	//ジャンプ
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		actor.ChangeStatus(new CJumpStatus);
		return;
	}

	//攻撃
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		actor.ChangeStatus(new CAttackStatus);
		return;
	}
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CMoveStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}