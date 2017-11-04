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
#include "sphereCollider.h"
#include "OBBCollider.h"
#include "AABBCollider.h"
#include "fieldCollider.h"
#include "3DRigidbodyComponent.h"

#if defined(_DEBUG) || defined(EDITOR)
#include "fieldEditorBehaviorComponent.h"
#include "modelEditorBehaviorComponent.h"
#include "editorControllerBehaviorComponent.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  SkyBox��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateSkyBox(const Vector3& Position, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new GameObject;

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName("skyBox");
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetRenderState(RS_LIGHTOFF_CULLFACEON_MUL);
	pRender->SetTexName("skybox000.jpg");
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
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
GameObject* GameObjectSpawner::CreateField(const string& strStageName)
{
	auto pObj = new GameObject;
	string strFieldName = strStageName + "Field";

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName(strFieldName + ".mesh");
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("demoField.jpg");
	pObj->SetRenderComponent(pRender);
	auto pCollider = new FieldCollider(pObj, strFieldName);
	pObj->AddCollider(pCollider);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateCube(const Vector3& Position, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new GameObject;

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName("cube");
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("nomal_cube.jpg");
	pObj->SetRenderComponent(pRender);
	auto pCollider = new AABBCollider(pObj, DYNAMIC, vScale * 0.5f);
	//OBBCollider* pCollider = new OBBCollider(pObj, DYNAMIC, vScale * 0.5f);
	//SphereCollider* pCollider = new SphereCollider(pObj, DYNAMIC, vScale.X * 0.5f);
	pObj->AddCollider(pCollider);
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->SetRigidbodyComponent(pRb);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
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
GameObject* GameObjectSpawner::CreateXModel(const string& strPath, const Vector3& Position, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new GameObject;

	//Name
	auto& strName = Utility::GetFileName(strPath);
	pObj->SetName(strName);

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName(strPath);
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
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
GameObject* GameObjectSpawner::CreateGoal(const Vector3& Position)
{
	auto pObj = new GameObject;

	//Tag
	pObj->SetTag("Goal");

	//�R���C�_�[
	auto pCollider = new SphereCollider(pObj, STATIC, 2.0f);
	//pCollider->SetOffset(Vector3(0.0f));
	pCollider->SetTrigger(true);
	pCollider->SetTag("Goal");
	pObj->AddCollider(pCollider);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);

	//������
	pObj->Init();
	return pObj;
}

//--------------------------------------------------------------------------------
//	�֐����FCreateModel
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	filePath�F�t�@�C���̖��O 
//			Position
//			qRot
//			vScale
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateModel(const string& filePath, const Vector3& Position, const Quaternion& qRot, const Vector3& vScale)
{
	string strName, strType;
	Utility::AnalyzeFilePath(filePath, strName, strType);
	if (!strType._Equal("model")) { return nullptr; }
	
	//Model�t�@�C���̊J��
	string strPath = "data/MODEL/" + filePath;
	FILE *filePointer;
	fopen_s(&filePointer, strPath.c_str(), "rb");

	auto pObj = createChildNode(nullptr, filePointer);

	//Name
	pObj->SetName(strName);

	//Trans
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->SetRot(qRot);
	pTrans->SetRotNext(qRot);

	fclose(filePointer);

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
//			filePointer
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::createChildNode(CTransformComponent* pParent, FILE* filePointer)
{
	auto pObj = new GameObject;

	//Node��
	int nNodeNameSize;
	fread_s(&nNodeNameSize, sizeof(int), sizeof(int), 1, filePointer);
	string strNodeName;
	strNodeName.resize(nNodeNameSize);
	fread_s(&strNodeName[0], nNodeNameSize, sizeof(char), nNodeNameSize, filePointer);
	pObj->SetName(strNodeName);

	//Offset
	Vector3 Position, vRot, vScale;
	fread_s(&Position, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	fread_s(&vRot, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	fread_s(&vScale, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	auto pTrans = pObj->GetTransformComponent();
	if (pParent) { pTrans->RegisterParent(pParent, Position, vRot); }

	//Collider
	int nNumCollider = 0;
	fread_s(&nNumCollider, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < nNumCollider; ++count)
	{
		int nColType = 0;
		Vector3 vColPos, vColRot, vColScale;
		fread_s(&nColType, sizeof(int), sizeof(int), 1, filePointer);
		fread_s(&vColPos, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
		fread_s(&vColRot, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
		fread_s(&vColScale, sizeof(Vector3), sizeof(Vector3), 1, filePointer);

		Collider* pCollider = nullptr;
		switch ((ColliderType)nColType)
		{
		case Sphere:
			pCollider = new SphereCollider(pObj, STATIC, vColScale.X);
			break;
		case AABB:
			pCollider = new AABBCollider(pObj, STATIC, vColScale * 0.5f);
			break;
		case OBB:
			pCollider = new OBBCollider(pObj, STATIC, vColScale * 0.5f);
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
	fread_s(&nNumTexture, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < nNumTexture; ++count)
	{
		int nNameSize = 0;
		fread_s(&nNameSize, sizeof(int), sizeof(int), 1, filePointer);
		string texture;
		texture.resize(nNameSize);
		fread_s(&texture[0], nNameSize, sizeof(char), nNameSize, filePointer);
		//Mesh���œǂݍ��ނ̂ł����͕����Ă���
	}

	//Mesh
	int nNumMesh = 0;
	fread_s(&nNumMesh, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < nNumMesh; ++count)
	{
		//Mesh Name
		int nMeshNameSize = 0;
		fread_s(&nMeshNameSize, sizeof(int), sizeof(int), 1, filePointer);
		string strMeshName;
		strMeshName.resize(nMeshNameSize);
		fread_s(&strMeshName[0], nMeshNameSize, sizeof(char), nMeshNameSize, filePointer);

		//Check Type
		string strName, strType;
		Utility::AnalyzeFilePath(strMeshName, strName, strType);
		if (strType._Equal("mesh"))
		{//���Ȃ�
			auto pChildMesh = createChildMesh(pTrans, strMeshName);
		}
		else if (strType._Equal("oneSkinMesh"))
		{//�����X�L�[�����b�V��
			MessageBox(NULL, "oneSkinMesh���Ή�", "GameObjectSpawner::createChildNode", MB_OK | MB_ICONWARNING);
		}
	}

	//Child
	int nNumChild = 0;
	fread_s(&nNumChild, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < nNumChild; ++count)
	{
		auto pChild = createChildNode(pTrans, filePointer);
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
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::createChildMesh(CTransformComponent* pParent, const string& strMeshName)
{
	auto pObj = new GameObject;

	//Name
	auto& strName = Utility::GetFileName(strMeshName);
	pObj->SetName(strName);

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName(strMeshName);
	pObj->SetMeshComponent(mesh);
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
#if defined(_DEBUG) || defined(EDITOR)
GameObject* GameObjectSpawner::CreateEditorController(GameObject* pFieldEditor)
{
	auto pObj = new GameObject;

	//�R���|�l���g
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName("data/MODEL/target.x");
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pObj->SetRenderComponent(pRender);
	auto pMEBehavior = new ModelEditorBehaviorComponent(pObj);
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
GameObject* GameObjectSpawner::CreateEditorField(void)
{
	auto pObj = new GameObject;

	//�R���|�l���g
	auto pBehavior = new CFieldEditorBehaviorComponent(pObj);
	pObj->AddBehavior(pBehavior);
	auto mesh = new MeshComponent(pObj);
	mesh->SetMeshName("field");
	pObj->SetMeshComponent(mesh);
	auto pRender = new C3DMeshRenderComponent(pObj);
	pRender->SetTexName("editorField.jpg");
	pObj->SetRenderComponent(pRender);

	//�p�����[�^�[
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(Vector3(0.0f));
	pTrans->SetPosNext(Vector3(0.0f));
	pTrans->SetScale(Vector3(1.0f));
	pTrans->SetScaleNext(Vector3(1.0f));
	pTrans->RotByEuler(Vector3(0.0f));

	//������
	pObj->Init();
	return pObj;
}
#endif // _DEBUG