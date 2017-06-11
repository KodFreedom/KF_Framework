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
CGameObject2D* CGameObject2D::Create(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vSize, const CTM::TEX_NAME& texName)
{
	//生成
	CGameObject2D* pObj = new CGameObject2D;

	//コンポネント
	C2DMeshComponent* pMesh = new C2DMeshComponent(pObj);
	pMesh->SetTexName(CTM::TEX_TEST);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C2DDrawComponent(pObj);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vSize;
	pObj->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}