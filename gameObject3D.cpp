//--------------------------------------------------------------------------------
//
//�@gameObject3D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject3D.h"
#include "skyBoxMeshComponent.h"
#include "fieldMeshComponent.h"
#include "cubeMeshComponent.h"
#include "modelMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "modelMeshDrawComponent.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D() : CGameObject(GOM::PRI_3D)
	, m_vForward(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(-1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(-1.0f, 0.0f, 0.0f))
{
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D(const GOM::PRIORITY &pri) : CGameObject(pri)
	, m_vForward(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(-1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(-1.0f, 0.0f, 0.0f))
{
}

//--------------------------------------------------------------------------------
//  �p�����[�^�[��������
//--------------------------------------------------------------------------------
void CGameObject3D::SwapParam(void)
{
	CGameObject::SwapParam();
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//  SkyBox��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateSkyBox(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	CSkyBoxMeshComponent *pMesh = new CSkyBoxMeshComponent;
	pMesh->SetTexName(CTM::TEX_DEMO_SKY);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;
	pObj->m_pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  MeshField��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateField(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	CFieldMeshComponent *pMesh = new CFieldMeshComponent(nNumBlockX, nNumBlockZ, vBlockSize);
	pMesh->SetTexName(CTM::TEX_DEMO_ROAD);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateCube(const CKFVec3 &vSize, const CKFColor &cColor, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	CCubeMeshComponent *pMesh = new CCubeMeshComponent(vSize, cColor);
	pMesh->SetTexName(CTM::TEX_DEMO_POLYGON);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  ���f����������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateModel(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	CModelMeshComponent *pMesh = new CModelMeshComponent();
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CModelMeshDrawComponent;

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//������
	pObj->Init();

	return pObj;
}