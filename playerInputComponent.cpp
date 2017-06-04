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
#include "inputDX.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DPhysicsComponent.h"

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
	CPhysicsComponent* pPhysics = m_pGameObj->GetPhysicsComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pPhysics->GetType() != CPhysicsComponent::PSY_3D) { return; }

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

	if (bMove)
	{
		//移動設定
		C3DPhysicsComponent *p3DPhysics = (C3DPhysicsComponent*)pPhysics;
		p3DPhysics->MovePosByRot(c_fSpeed, fRot);

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