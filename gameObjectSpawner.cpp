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
#include "manager.h"
#include "meshManager.h"
#include "renderManager.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "MeshComponent.h"
#include "3DMeshRenderComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "modelEditorBehaviorComponent.h"
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
	auto pObj = new CGameObject;

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName("skyBox");
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetRenderState(RS_LIGHTOFF_CULLFACEON_MUL);
	pRender->SetTexName("skybox000.jpg");
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
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
CGameObject* CGameObjectSpawner::CreateField(const string& strStageName)
{
	auto pObj = new CGameObject;
	string strFieldName = strStageName + "Field";

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName(strFieldName + ".mesh");
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("demoField.jpg");
	pObj->SetRenderComponent(pRender);
	auto pCollider = new CFieldColliderComponent(pObj, strFieldName);
	pObj->AddCollider(pCollider);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject;

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName("cube");
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("nomal_cube.jpg");
	pObj->SetRenderComponent(pRender);
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//COBBColliderComponent* pCollider = new COBBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, vScale.m_fX * 0.5f);
	pObj->AddCollider(pCollider);
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->SetRigidbodyComponent(pRb);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
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
	auto pObj = new CGameObject;

	//Name
	auto& strName = CKFUtility::GetFileName(strPath);
	pObj->SetName(strName);

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName(strPath);
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
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
CGameObject* CGameObjectSpawner::CreateGoal(const CKFVec3& vPos)
{
	auto pObj = new CGameObject;

	//Tag
	pObj->SetTag("Goal");

	//�R���C�_�[
	auto pCollider = new CAABBColliderComponent(pObj, CS::STATIC, CKFVec3(8.5f, 6.0f, 9.25f));
	pCollider->SetOffset(CKFVec3(0.55f, 5.55f, 0.0f));
	pCollider->SetTrigger(true);
	pCollider->SetTag("Goal");
	pObj->AddCollider(pCollider);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//	�֐����FCreateModel
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	strFilePath�F�t�@�C���̖��O 
//			vPos
//			qRot
//			vScale
//	�߂�l�FCGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateModel(const string& strFilePath, const CKFVec3& vPos, const CKFQuaternion& qRot, const CKFVec3& vScale)
{
	string strName, strType;
	CKFUtility::AnalyzeFilePath(strFilePath, strName, strType);
	if (!strType._Equal("model")) { return nullptr; }
	
	//Model�t�@�C���̊J��
	string strPath = "data/MODEL/" + strFilePath;
	FILE *pFile;
	fopen_s(&pFile, strPath.c_str(), "rb");

	auto pObj = createChildNode(nullptr, pFile);

	//Name
	pObj->SetName(strName);

	//Trans
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->SetRot(qRot);
	pTrans->SetRotNext(qRot);

	fclose(pFile);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FcreateChildNode
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	pParent�F�t�@�C���̖��O 
//			pFile
//	�߂�l�FCGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::createChildNode(CTransformComponent* pParent, FILE* pFile)
{
	auto pObj = new CGameObject;

	//Node��
	int nNodeNameSize;
	fread_s(&nNodeNameSize, sizeof(int), sizeof(int), 1, pFile);
	string strNodeName;
	strNodeName.resize(nNodeNameSize);
	fread_s(&strNodeName[0], nNodeNameSize, sizeof(char), nNodeNameSize, pFile);
	pObj->SetName(strNodeName);

	//Offset
	CKFVec3 vPos, vRot, vScale;
	fread_s(&vPos, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);
	fread_s(&vRot, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);
	fread_s(&vScale, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);
	auto pTrans = pObj->GetTransformComponent();
	if (pParent) { pTrans->RegisterParent(pParent, vPos, vRot); }

	//Collider
	int nNumCollider = 0;
	fread_s(&nNumCollider, sizeof(int), sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumCollider; ++nCnt)
	{
		int nColType = 0;
		CKFVec3 vColPos, vColRot, vColScale;
		fread_s(&nColType, sizeof(int), sizeof(int), 1, pFile);
		fread_s(&vColPos, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);
		fread_s(&vColRot, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);
		fread_s(&vColScale, sizeof(CKFVec3), sizeof(CKFVec3), 1, pFile);

		CColliderComponent* pCollider = nullptr;
		switch ((CS::COL_TYPE)nColType)
		{
		case CS::COL_SPHERE:
			pCollider = new CSphereColliderComponent(pObj, CS::STATIC, vColScale.m_fX);
			break;
		case CS::COL_AABB:
			pCollider = new CAABBColliderComponent(pObj, CS::STATIC, vColScale * 0.5f);
			break;
		case CS::COL_OBB:
			pCollider = new COBBColliderComponent(pObj, CS::STATIC, vColScale * 0.5f);
			break;
		default:
			break;
		}

		if (!pCollider) { continue; }
		pCollider->SetOffset(vColPos, vColRot);
		pObj->AddCollider(pCollider);
	}

	//Texture
	int nNumTexture = 0;
	fread_s(&nNumTexture, sizeof(int), sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumTexture; ++nCnt)
	{
		int nNameSize = 0;
		fread_s(&nNameSize, sizeof(int), sizeof(int), 1, pFile);
		string strTexName;
		strTexName.resize(nNameSize);
		fread_s(&strTexName[0], nNameSize, sizeof(char), nNameSize, pFile);
		//Mesh���œǂݍ��ނ̂ł����͕����Ă���
	}

	//Mesh
	int nNumMesh = 0;
	fread_s(&nNumMesh, sizeof(int), sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumMesh; ++nCnt)
	{
		//Mesh Name
		int nMeshNameSize = 0;
		fread_s(&nMeshNameSize, sizeof(int), sizeof(int), 1, pFile);
		string strMeshName;
		strMeshName.resize(nMeshNameSize);
		fread_s(&strMeshName[0], nMeshNameSize, sizeof(char), nMeshNameSize, pFile);

		//Check Type
		string strName, strType;
		CKFUtility::AnalyzeFilePath(strMeshName, strName, strType);
		if (strType._Equal("mesh"))
		{//���Ȃ�
			auto pChildMesh = createChildMesh(pTrans, strMeshName);
		}
		else if (strType._Equal("oneSkinMesh"))
		{//�����X�L�[�����b�V��
			MessageBox(NULL, "oneSkinMesh���Ή�", "CGameObjectSpawner::createChildNode", MB_OK | MB_ICONWARNING);
		}
	}

	//Child
	int nNumChild = 0;
	fread_s(&nNumChild, sizeof(int), sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumChild; ++nCnt)
	{
		auto pChild = createChildNode(pTrans, pFile);
	}

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//	�֐����FcreateChildMesh
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	pParent�F�t�@�C���̖��O 
//			strMeshName
//	�߂�l�FCGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::createChildMesh(CTransformComponent* pParent, const string& strMeshName)
{
	auto pObj = new CGameObject;

	//Name
	auto& strName = CKFUtility::GetFileName(strMeshName);
	pObj->SetName(strName);

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName(strMeshName);
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->RegisterParent(pParent);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorController(CGameObject* pFieldEditor)
{
	auto pObj = new CGameObject;

	//�R���|�l���g
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName("data/MODEL/target.x");
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pObj->SetRenderComponent(pRender);
	auto pMEBehavior = new CModelEditorBehaviorComponent(pObj);
	auto pBehavior = new CEditorControllerBehaviorComponent(pObj);
	pBehavior->SetFieldEditor(pFieldEditor);
	pBehavior->SetModelEditor(pMEBehavior);
	pObj->AddBehavior(pBehavior);
	pObj->AddBehavior(pMEBehavior);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorField(void)
{
	auto pObj = new CGameObject;

	//�R���|�l���g
	auto pBehavior = new CFieldEditorBehaviorComponent(pObj);
	pObj->AddBehavior(pBehavior);
	auto pMesh = new CMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("editorField.jpg");
	pObj->SetRenderComponent(pRender);

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