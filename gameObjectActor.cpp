//--------------------------------------------------------------------------------
//	アクター
//　gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectActor.h"
#include "playerInputComponent.h"
#include "3DPhysicsComponent.h"
#include "actorMeshComponent.h"
#include "actorMeshDrawComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor() : CGameObject3D()
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOM::PRIORITY &pri) : CGameObject3D(pri)
{

}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor;

	//コンポネント
	pObj->m_pInput = new CPlayerInputComponent;
	pObj->m_pPhysics = new C3DPhysicsComponent;
	CActorMeshComponent* pMesh = new CActorMeshComponent;
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CActorMeshDrawComponent;

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//初期化
	pObj->Init();

	return pObj;
}