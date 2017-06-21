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

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CPlayerInputComponent::CPlayerInputComponent(CGameObject* const pGameObj, const float& fMoveSpeed, const float& fJumpForce)
	: CInputComponent(pGameObj)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
{}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
	//コンポネント
	CRigidbodyComponent* pRB = m_pGameObj->GetRigidbodyComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pRB->GetType() != CRigidbodyComponent::RB_3D) { return; }

	CInputManager* pInput = GetManager()->GetInputManager();
	C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;

	//移動
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());

	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;

		//回転計算
		CKFVec3 vUp = m_pGameObj->GetUpNext();
		CKFVec3 vForward = m_pGameObj->GetForwardNext();
		CKFVec3 vRight = m_pGameObj->GetRightNext();

		//カメラ向きを算出する
		CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		CKFVec3 vForwardCamera = pCamera->GetVecLook();
		CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		if (fRot != 0.0f)
		{//操作より行く方向を回転する
			CKFMtx44 mtxYaw;
			CKFMath::MtxRotAxis(&mtxYaw, vUp, fRot);
			CKFMath::Vec3TransformNormal(&vForwardNext, mtxYaw);
		}

		CKFMath::VecNormalize(&vForwardNext);
		vForwardNext = CKFMath::LerpNormal(vForward, vForwardNext, 0.2f);
		m_pGameObj->RotByForward(vForwardNext);

		//移動設定
		p3DRB->MovePos(vForwardNext * c_fSpeed);

		//移動モーション設定
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_MOVE);
		}
	}
	else
	{
		//ニュートラルモーション設定
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_NEUTAL);
		}
	}

	//ジャンプ
	//if (pKeyboard->GetKeyPress(DIK_SPACE) && p3DRB->IsOnGround())
	//{
	//	//上方向にジャンプ
	//	p3DRB->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
	//	p3DRB->SetOnGround(false);

	//	//ジャンプモーション設定
	//	if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
	//	{
	//		CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	//		pActor->SetMotionNext(CActorMeshComponent::MOTION_JUMP);
	//	}
	//}
}