//--------------------------------------------------------------------------------
//
//　gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
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
CNullInputComponent		CGameObject::s_nullInput;
CNullRigidbodyComponent	CGameObject::s_nullRigidbody;
//CNullColliderComponent	CGameObject::s_nullCollision;
CNullMeshComponent		CGameObject::s_nullMesh;
CNullDrawComponent		CGameObject::s_nullDraw;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ(for null object)
//--------------------------------------------------------------------------------
CGameObject::CGameObject()
	: m_pri(GOM::PRI_MAX)
	, m_bActive(false)
	, m_bAlive(false)
	, m_pInput(&s_nullInput)
	, m_pRigidbody(&s_nullRigidbody)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
	, m_vPos(CKFVec3(0.0f))
	, m_vPosNext(CKFVec3(0.0f))
	, m_vScale(CKFVec3(0.0f))
	, m_vScaleNext(CKFVec3(0.0f))
	, m_vForward(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(1.0f, 0.0f, 0.0f))
{
	m_apCollider.clear();
	CKFMath::MtxIdentity(&m_mtxThis);
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject::CGameObject(const GOM::PRIORITY &pri)
	: m_pri(pri)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pInput(&s_nullInput)
	, m_pRigidbody(&s_nullRigidbody)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
	, m_vPos(CKFVec3(0.0f))
	, m_vPosNext(CKFVec3(0.0f))
	, m_vScale(CKFVec3(0.0f))
	, m_vScaleNext(CKFVec3(0.0f))
	, m_vForward(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(1.0f, 0.0f, 0.0f))
{
	m_apCollider.clear();
	CKFMath::MtxIdentity(&m_mtxThis);
	GetManager()->GetGameObjectManager()->SaveGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void CGameObject::Release(void)
{
	GetManager()->GetGameObjectManager()->ReleaseGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//	回転行列の取得
//--------------------------------------------------------------------------------
CKFMtx44 CGameObject::GetMatrixRot(void)
{
	m_mtxRot.m_af[0][0] = m_vRight.m_fX;
	m_mtxRot.m_af[0][1] = m_vRight.m_fY;
	m_mtxRot.m_af[0][2] = m_vRight.m_fZ;
	m_mtxRot.m_af[1][0] = m_vUp.m_fX;
	m_mtxRot.m_af[1][1] = m_vUp.m_fY;
	m_mtxRot.m_af[1][2] = m_vUp.m_fZ;
	m_mtxRot.m_af[2][0] = m_vForward.m_fX;
	m_mtxRot.m_af[2][1] = m_vForward.m_fY;
	m_mtxRot.m_af[2][2] = m_vForward.m_fZ;

	return m_mtxRot;
}

//--------------------------------------------------------------------------------
//  上方向により回転
//--------------------------------------------------------------------------------
void CGameObject::RotByUp(const CKFVec3& vUp)
{
	m_vUpNext = vUp;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	CKFMath::VecNormalize(&m_vUpNext);
	CKFMath::VecNormalize(&m_vForwardNext);
	CKFMath::VecNormalize(&m_vRightNext);
}

//--------------------------------------------------------------------------------
//  前方向により回転
//--------------------------------------------------------------------------------
void CGameObject::RotByForward(const CKFVec3& vForward)
{
	m_vForwardNext = vForward;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	CKFMath::VecNormalize(&m_vUpNext);
	CKFMath::VecNormalize(&m_vForwardNext);
	CKFMath::VecNormalize(&m_vRightNext);
}

//--------------------------------------------------------------------------------
//  右方向により回転
//--------------------------------------------------------------------------------
void CGameObject::RotByRight(const CKFVec3& vRight)
{
	m_vRightNext = vRight;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	CKFMath::VecNormalize(&m_vUpNext);
	CKFMath::VecNormalize(&m_vForwardNext);
	CKFMath::VecNormalize(&m_vRightNext);
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CGameObject::SwapParam(void)
{
	m_vPos = m_vPosNext;
	//m_vRot = m_vRotNext;
	m_vScale = m_vScaleNext;
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//  ZXY回転
//--------------------------------------------------------------------------------
void CGameObject::RotByEuler(const CKFVec3& vRot)
{
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, vRot);
	CKFMath::Vec3TransformNormal(&m_vUpNext, mtxRot);
	CKFMath::Vec3TransformNormal(&m_vForwardNext, mtxRot);
	CKFMath::Vec3TransformNormal(&m_vRightNext, mtxRot);
}

//--------------------------------------------------------------------------------
//  X軸(vVecRight)回転
//--------------------------------------------------------------------------------
void CGameObject::RotByPitch(const float& fRadian)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(&mtxPitch, m_vRightNext, fRadian);
	CKFMath::Vec3TransformNormal(&m_vUpNext, mtxPitch);
	CKFMath::Vec3TransformNormal(&m_vForwardNext, mtxPitch);
}

//--------------------------------------------------------------------------------
//  Y軸(vVecUp)回転
//--------------------------------------------------------------------------------
void CGameObject::RotByYaw(const float& fRadian)
{
	CKFMtx44 mtxYaw;
	CKFMath::MtxRotAxis(&mtxYaw, m_vUpNext, fRadian);
	CKFMath::Vec3TransformNormal(&m_vRightNext, mtxYaw);
	CKFMath::Vec3TransformNormal(&m_vForwardNext, mtxYaw);
}

//--------------------------------------------------------------------------------
//  Z軸(vLook)回転
//--------------------------------------------------------------------------------
void CGameObject::RotByRoll(const float& fRadian)
{
	CKFMtx44 mtxRoll;
	CKFMath::MtxRotAxis(&mtxRoll, m_vForwardNext, fRadian);
	CKFMath::Vec3TransformNormal(&m_vForwardNext, mtxRoll);
	CKFMath::Vec3TransformNormal(&m_vRightNext, mtxRoll);
}

//--------------------------------------------------------------------------------
//  法線の描画
//--------------------------------------------------------------------------------
#ifdef _DEBUG
void CGameObject::DrawNormal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	D3DXMATRIX mtx, mtxPos;
	D3DXMatrixIdentity(&mtx);
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	VERTEX_3D vtx[2];
	vtx[0].vPos = m_vPos;
	vtx[0].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	vtx[1].vPos = m_vPos + m_vUp * 2.0f;
	vtx[1].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//プリミティブ描画
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST,//プリミティブの種類
		1,//プリミティブの数
		&vtx[0],//頂点データの先頭アドレス
		sizeof(VERTEX_3D)//一個分のサイズ
	);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif