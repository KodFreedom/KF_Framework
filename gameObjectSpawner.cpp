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
#include "meshManager.h"
#include "rendererManager.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "gameObjectActor.h"
#include "meshRenderer.h"
#include "sphereCollider.h"
#include "OBBCollider.h"
#include "AABBCollider.h"
#include "fieldCollider.h"
#include "rigidbody3D.h"
#include "animator.h"
#include "playerController.h"
#include "enemyController.h"
#include "materialManager.h"

#if defined(_DEBUG) || defined(EDITOR)
#include "fieldEditor.h"
#include "modelEditor.h"
#include "editorController.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  SkyBox��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateSkyBox(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = new GameObject;

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName("skyBox");
	renderer->SetRenderState(RenderStateType::RS_NoLight_NoFog);
#ifdef _DEBUG
	MaterialManager::Instance()->CreateMaterialFile("sky", "skybox000.jpg");
#endif // DEBUG
	renderer->SetMaterialName("sky");
	result->AddRenderer(renderer);

	//�p�����[�^�[
	auto transform = result->GetTransform();;
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  MeshField��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateField(const string& stageName)
{
	auto result = new GameObject;
	string fieldName = stageName + "Field";

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName(fieldName + ".mesh");
#ifdef _DEBUG
	MaterialManager::Instance()->CreateMaterialFile("field", "demoField.jpg");
#endif // DEBUG
	renderer->SetMaterialName("field");
	result->AddRenderer(renderer);
	result->AddCollider(new FieldCollider(result, fieldName));

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = new GameObject;

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName("cube");
#ifdef _DEBUG
	MaterialManager::Instance()->CreateMaterialFile("cube", "nomal_cube.jpg");
#endif // DEBUG
	renderer->SetMaterialName("cube");
	result->AddRenderer(renderer);
	result->AddCollider(new OBBCollider(result, CM_Dynamic, scale * 0.5f));
	result->SetRigidbody(new Rigidbody3D(result));

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateXModel(const string& modelName, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = new GameObject;

	//Name
	auto& fileInfo = Utility::AnalyzeFilePath(modelName);
	result->SetName(fileInfo.Name);

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName(modelName);
	result->AddRenderer(renderer);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateGoal(const Vector3& position)
{
	auto result = new GameObject;

	//Tag
	result->SetTag("Goal");

	//�R���C�_�[
	auto collider = new SphereCollider(result, CM_Static, 2.0f);
	collider->SetTrigger(true);
	collider->SetTag("Goal");
	result->AddCollider(collider);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FCreateModel
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	filePath�F�t�@�C���̖��O 
//			Position
//			rotation
//			scale
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateModel(const string& modelName, const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	auto& fileInfo = Utility::AnalyzeFilePath(modelName);
	if (!fileInfo.Type._Equal("model")) return nullptr;
	
	//Model�t�@�C���̊J��
	string strPath = "data/MODEL/" + modelName;
	FILE *filePointer;
	fopen_s(&filePointer, strPath.c_str(), "rb");

	auto result = createChildNode(nullptr, filePointer);

	//Name
	result->SetName(fileInfo.Name);

	//Trans
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	fclose(filePointer);

	//������
	result->Init();
	return result;
}


//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreatePlayer(const string &actorPath, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = new GameObjectActor;

	//Tag
	result->SetTag("Player");

	//�R���|�l���g
	auto rigidbody = new Rigidbody3D(result);
	result->SetRigidbody(rigidbody);
	result->SetAnimator(new Animator(result, actorPath));
	result->AddBehavior(new PlayerController(result, *rigidbody));
	auto collider = new SphereCollider(result, CM_Dynamic, 0.6f);
	collider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	collider->SetTag("body");
	result->AddCollider(collider);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	//������
	result->Init();

	//UI
	//PlayerUI::Create(pPb);

	return result;
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreateEnemy(const string &actorPath, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = new GameObjectActor;

	//Tag
	result->SetTag("Enemy");

	//�R���|�l���g
	auto rigidbody = new Rigidbody3D(result);
	result->SetRigidbody(rigidbody);
	result->SetAnimator(new Animator(result, actorPath));
	result->AddBehavior(new EnemyController(result, *rigidbody));

	//�R���C�_�[
	auto collider = new SphereCollider(result, CM_Dynamic, 0.6f);
	collider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	collider->SetTag("body");
	result->AddCollider(collider);
	auto detector = new SphereCollider(result, CM_Dynamic, 6.0f);
	detector->SetTrigger(true);
	detector->SetTag("detector");
	result->AddCollider(detector);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetNextPosition(position);
	transform->SetNextScale(scale);
	transform->SetNextRotation(rotation);

	//������
	result->Init();
	return result;
}

#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  CreateStageEditor
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateStageEditor(GameObject* fieldEditor)
{
	auto result = new GameObject;

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName("data/MODEL/target.x");
	result->AddRenderer(renderer);
	auto modelEditor = new ModelEditor(result);
	auto editorController = new EditorController(result);
	editorController->SetFieldEditor(fieldEditor);
	editorController->SetModelEditor(modelEditor);
	result->AddBehavior(editorController);
	result->AddBehavior(modelEditor);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateFieldEditor(void)
{
	auto result = new GameObject;

	//�R���|�l���g
	auto pBehavior = new FieldEditor(result);
	result->AddBehavior(pBehavior);
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName("field");
#ifdef _DEBUG
	MaterialManager::Instance()->CreateMaterialFile("editorField", "editorField.jpg");
#endif // DEBUG
	renderer->SetMaterialName("editorField");
	result->AddRenderer(renderer);

	//������
	result->Init();
	return result;
}
#endif // _DEBUG

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FcreateChildNode
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	parent�F�t�@�C���̖��O 
//			filePointer
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::createChildNode(Transform* parent, FILE* filePointer)
{
	auto result = new GameObject;

	//Node��
	int nodeNameSize;
	fread_s(&nodeNameSize, sizeof(int), sizeof(int), 1, filePointer);
	string nodeName;
	nodeName.resize(nodeNameSize);
	fread_s(&nodeName[0], nodeNameSize, sizeof(char), nodeNameSize, filePointer);
	result->SetName(nodeName);

	//Offset
	Vector3 position, rotation, scale;
	fread_s(&position, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	fread_s(&rotation, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	fread_s(&scale, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
	auto transform = result->GetTransform();
	if (parent) transform->RegisterParent(parent, position, rotation);

	//Collider
	int colliderNumber = 0;
	fread_s(&colliderNumber, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < colliderNumber; ++count)
	{
		int colliderType = 0;
		Vector3 colliderPositon, colliderRotation, colliderScale;
		fread_s(&colliderType, sizeof(int), sizeof(int), 1, filePointer);
		fread_s(&colliderPositon, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
		fread_s(&colliderRotation, sizeof(Vector3), sizeof(Vector3), 1, filePointer);
		fread_s(&colliderScale, sizeof(Vector3), sizeof(Vector3), 1, filePointer);

		Collider* collider = nullptr;
		switch ((ColliderType)colliderType)
		{
		case CT_Sphere:
			collider = new SphereCollider(result, CM_Static, colliderScale.X);
			break;
		case CT_AABB:
			collider = new AABBCollider(result, CM_Static, colliderScale * 0.5f);
			break;
		case CT_OBB:
			collider = new OBBCollider(result, CM_Static, colliderScale * 0.5f);
			break;
		default:
			break;
		}

		if (!collider) continue;
		collider->SetOffset(colliderPositon, colliderRotation);
		result->AddCollider(collider);
	}

	//Material
	int materialNameSize = 0;
	fread_s(&materialNameSize, sizeof(int), sizeof(int), 1, filePointer);
	string materialName;
	materialName.resize(materialNameSize);
	fread_s(&materialName[0], materialNameSize, sizeof(char), materialNameSize, filePointer);

	//Mesh
	int meshNumber = 0;
	fread_s(&meshNumber, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < meshNumber; ++count)
	{
		//Mesh Name
		int meshNameSize = 0;
		fread_s(&meshNameSize, sizeof(int), sizeof(int), 1, filePointer);
		string meshName;
		meshName.resize(meshNameSize);
		fread_s(&meshName[0], meshNameSize, sizeof(char), meshNameSize, filePointer);

		//Check Type
		auto& fileInfo = Utility::AnalyzeFilePath(meshName);
		if (fileInfo.Type._Equal("mesh"))
		{//���Ȃ�
			auto childMesh = createChildMesh(transform, meshName);
		}
		else if (fileInfo.Type._Equal("oneSkinMesh"))
		{//�����X�L�[�����b�V��
			MessageBox(NULL, "oneSkinMesh���Ή�", "GameObjectSpawner::createChildNode", MB_OK | MB_ICONWARNING);
		}
	}

	//Child
	int childNumber = 0;
	fread_s(&childNumber, sizeof(int), sizeof(int), 1, filePointer);
	for (int count = 0; count < childNumber; ++count)
	{
		auto child = createChildNode(transform, filePointer);
	}

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FcreateChildMesh
//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
//	�����F	parent�F�t�@�C���̖��O 
//			meshName
//	�߂�l�FGameObject*
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::createChildMesh(Transform* parent, const string& meshName)
{
	auto result = new GameObject;

	//Name
	auto& fileInfo = Utility::AnalyzeFilePath(meshName);
	result->SetName(fileInfo.Name);

	//�R���|�l���g
	auto renderer = new MeshRenderer(result);
	renderer->SetMeshName(meshName);
	result->AddRenderer(renderer);

	//�p�����[�^�[
	result->GetTransform()->RegisterParent(parent);

	//������
	result->Init();
	return result;
}