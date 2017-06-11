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
#include "inputDX.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
	CKeyboardDX* pKeyboard = GetManager()->GetKeyboard();

	//コンポネント
	CRigidbodyComponent* pRB = m_pGameObj->GetRigidbodyComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pRB->GetType() != CRigidbodyComponent::RB_3D) { return; }

	bool bMove = false;		//移動フラッグ
	bool bInverse = false;	//後ろキーフラッグ
	bool bFb = false;		//前後移動フラッグ
	float fRot = 0.0f;		//回転情報

	//移動
	if (pKeyboard->GetKeyPress(DIK_W))
	{
		bMove = true;
		bFb = true;
	}

	if (pKeyboard->GetKeyPress(DIK_S))
	{
		bMove = true;
		bFb = true;
		bInverse = true;
		fRot += KF_PI;
	}

	if (pKeyboard->GetKeyPress(DIK_A))
	{
		bMove = true;

		float fRotLR = -KF_PI * 0.5f;

		if (bInverse) { fRotLR = -fRotLR; }
		if (bFb) { fRotLR *= 0.5f; }

		fRot += fRotLR;
	}

	if (pKeyboard->GetKeyPress(DIK_D))
	{
		bMove = true;

		float fRotLR = KF_PI * 0.5f;

		if (bInverse) { fRotLR = -fRotLR; }
		if (bFb) { fRotLR *= 0.5f; }

		fRot += fRotLR;
	}

	//回転計算
	CKFVec3 vUp = m_pGameObj->GetUpNext();
	CKFVec3 vForward = m_pGameObj->GetForwardNext();
	CKFVec3 vRight = m_pGameObj->GetRightNext();

	//カメラ向きからプレイヤーの次の向きを算出する
	CCamera* pCamera = GetManager()->GetMode()->GetCamera();
	CKFVec3 vForwardCamera = pCamera->GetVecLook();
	CKFVec3 vForwardFinal = (vUp * vForwardCamera) * vUp;

	if (fRot != 0.0f)
	{//操作より行く方向を回転する
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, fRot);
		CKFMath::Vec3TransformNormal(&vForwardFinal, mtxYaw);
	}

	CKFMath::VecNormalize(&vForwardFinal);
	CKFVec3 vForwardNext = CKFMath::LerpNormal(vForward, vForwardFinal, 0.2f);
	m_pGameObj->RotByForward(vForwardNext);

	if (bMove)
	{
		//移動設定
		C3DRigidbodyComponent *p3DRB = (C3DRigidbodyComponent*)pRB;
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
		//モーション設定
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_NEUTAL);
		}
	}
}