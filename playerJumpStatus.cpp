//--------------------------------------------------------------------------------
//	プレイヤージャンプステータス
//　playerJumpStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-24
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerJumpStatus.h"
#include "playerMoveStatus.h"
#include "playerNormalStatus.h"
#include "playerAttackStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
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
void CPlayerJumpStatus::Update(CActorBehaviorComponent& actor)
{
	//ジャンプ
	if (m_bFirst)
	{
		m_bFirst = false;
		CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
		CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
		actor.Jump(pActor);
		return;
	}

	//移動
	CInputManager* pInput = GetManager()->GetInputManager();
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
		actor.Move();
	}

	//着陸シたらノーマルステータスに戻す
	if (actor.m_pRigidbody->IsOnGround())
	{
		actor.ChangeStatus(new CPlayerNormalStatus);
	}
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CPlayerJumpStatus::LateUpdate(CActorBehaviorComponent& actor)
{
	
}