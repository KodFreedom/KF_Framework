//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�����֐�
//�@gameObjectSpawner.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "3DMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "editorControllerBehaviorComponent.h"
#include "sphereColliderComponent.h"
#include "OBBColliderComponent.h"
#include "AABBColliderComponent.h"
#include "fieldColliderComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  SkyBox��������
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("skyBox");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);
	pDraw->SetTexName("skybox000.jpg");
	pObj->SetDrawComponent(pDraw);

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
CGameObject* CGameObjectSpawner::CreateField(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("RockCliff.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pCollider = new CFieldColliderComponent(pObj, 200, 200, CKFVec2(10.0f, 10.0f));
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
CGameObject* CGameObjectSpawner::CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("cube");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("nomal_cube.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//COBBColliderComponent* pCollider = new COBBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, vScale.m_fX * 0.5f);
	pObj->AddCollider(pCollider);
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->SetRigidbodyComponent(pRb);

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
CGameObject* CGameObjectSpawner::CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	string strTexName;
	pMesh->SetMeshName(strPath, strTexName);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTexName);
	pObj->SetDrawComponent(pDraw);

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
//  �N���X
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorController(CGameObject* pFieldEditor)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	auto pMesh = new C3DMeshComponent(pObj);
	string strTex;
	pMesh->SetMeshName("data/MODEL/target.x", strTex);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTex);
	pObj->SetDrawComponent(pDraw);
	auto pBehavior = new CEditorControllerBehaviorComponent(pObj);
	pBehavior->SetFieldEditor(pFieldEditor);
	pObj->AddBehavior(pBehavior);

	//������
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorField(void)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//�R���|�l���g
	auto pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("RockCliff.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pBehavior = new CFieldEditorBehaviorComponent(pObj);
	pObj->AddBehavior(pBehavior);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(CKFVec3(0.0f));
	pTrans->SetPosNext(CKFVec3(0.0f));
	pTrans->SetScale(CKFVec3(1.0f));
	pTrans->SetScaleNext(CKFVec3(1.0f));
	pTrans->RotByEuler(CKFVec3(0.0f));

	//������
	pObj->Init();

	return pObj;
}