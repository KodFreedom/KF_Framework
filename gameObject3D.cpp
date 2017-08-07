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
#include "3DMeshComponent.h"
#include "modelMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "sphereColliderComponent.h"
#include "OBBColliderComponent.h"
#include "fieldColliderComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D(const GOM::PRIORITY& pri, const OBJ_TYPE& type) : CGameObject(pri, type)
{
}

//--------------------------------------------------------------------------------
//  SkyBox��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("skyBox");
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pObj->m_pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);
	pObj->m_pDraw->SetTexName("skybox000.jpg");

	//�p�����[�^�[
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  MeshField��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateField(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pObj->m_pDraw->SetTexName("RockCliff.jpg");
	CFieldColliderComponent* pCollider = new CFieldColliderComponent(pObj, 100, 100, CKFVec2(2.0f, 2.0f));
	pObj->AddCollider(pCollider);

	//�p�����[�^�[
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("cube");
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pObj->m_pDraw->SetTexName("nomal_cube.jpg");
	COBBColliderComponent* pCollider = new COBBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, vScale.m_fX * 0.5f);
	pObj->AddCollider(pCollider);
	pObj->m_pRigidbody = new C3DRigidbodyComponent(pObj);

	//�p�����[�^�[
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  ���f����������
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateModel(const CMOM::MODEL_NAME &modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	////�R���|�l���g
	//CModelMeshComponent* pMesh = new CModelMeshComponent(pObj);
	//pMesh->SetModelName(modelName);
	//pObj->m_pMesh = pMesh;
	//pObj->m_pDraw = new CModelMeshDrawComponent(pObj);

	////�p�����[�^�[
	//CTransformComponent* pTrans = pObj->GetTransformComponent();
	//pTrans->SetPos(vPos);
	//pTrans->SetPosNext(vPos);
	//pTrans->SetScale(vScale);
	//pTrans->SetScaleNext(vScale);
	//pTrans->RotByEuler(vRot);

	////������
	//pObj->Init();

	return pObj;
}