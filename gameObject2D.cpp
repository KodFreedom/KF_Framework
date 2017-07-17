//--------------------------------------------------------------------------------
//	ゲームオブジェクト2D
//　gameObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject2D.h"
#include "2DMeshComponent.h"
#include "2DDrawComponent.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  オブジェクトの生成
//--------------------------------------------------------------------------------
CGameObject2D* CGameObject2D::Create(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vSize, const string& strTexName)
{
	//生成
	CGameObject2D* pObj = new CGameObject2D;

	//コンポネント
	C2DMeshComponent* pMesh = new C2DMeshComponent(pObj);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C2DDrawComponent(pMesh, pObj);
	pObj->m_pDraw->SetTexName(strTexName);

	//パラメーター
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vSize);
	pTrans->SetScaleNext(vSize);
	pTrans->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}