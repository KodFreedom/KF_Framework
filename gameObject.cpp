//--------------------------------------------------------------------------------
//
//　gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "gameObject.h"

#ifdef _DEBUG
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
CNullRigidbodyComponent	CGameObject::s_nullRigidbody;
CNullMeshComponent		CGameObject::s_nullMesh;
CNullDrawComponent		CGameObject::s_nullDraw;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject::CGameObject(const GOM::PRIORITY &pri, const OBJ_TYPE& type)
	: m_pri(pri)
	, m_type(type)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pRigidbody(&s_nullRigidbody)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
{
	m_listpBehavior.clear();
	m_listpCollider.clear();
	m_pTransform = new CTransformComponent(this);
	CMain::GetManager()->GetGameObjectManager()->SaveGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void CGameObject::Release(void)
{
	CMain::GetManager()->GetGameObjectManager()->ReleaseGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  コライダーの追加
//--------------------------------------------------------------------------------
void CGameObject::AddBehavior(CBehaviorComponent* pBehavior)
{
	m_listpBehavior.push_back(pBehavior);
}

//--------------------------------------------------------------------------------
//  コライダーの追加
//--------------------------------------------------------------------------------
void CGameObject::AddCollider(CColliderComponent* pCollider)
{
	m_listpCollider.push_back(pCollider);
}

//--------------------------------------------------------------------------------
//  コライダーの削除
//--------------------------------------------------------------------------------
void CGameObject::DeleteCollider(CColliderComponent* pCollider)
{
	m_listpCollider.remove(pCollider);
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CGameObject::SwapParam(void)
{
	m_pTransform->SwapParam();
}

//--------------------------------------------------------------------------------
//  法線の描画
//--------------------------------------------------------------------------------
#ifdef _DEBUG
void CGameObject::DrawNormal(void)
{
	//LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	//D3DXMATRIX mtx, mtxPos;
	//D3DXMatrixIdentity(&mtx);
	//pDevice->SetTransform(D3DTS_WORLD, &mtx);

	//VERTEX_3D vtx[2];
	//vtx[0].vPos = m_pTransform->GetPos();
	//vtx[0].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	//vtx[1].vPos = m_pTransform->GetPos() + m_pTransform->GetUp() * 2.0f;
	//vtx[1].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	////プリミティブ描画
	//pDevice->DrawPrimitiveUP(D3DPT_LINELIST,//プリミティブの種類
	//	1,//プリミティブの数
	//	&vtx[0],//頂点データの先頭アドレス
	//	sizeof(VERTEX_3D)//一個分のサイズ
	//);
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif