//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�����֐�
//�@gameObjectSpawner.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "game_object_spawner.h"
#include "game_object.h"
#include "game_object_actor.h"
#include "main_system.h"
#include "material_manager.h"
#include "mesh_manager.h"
#include "renderer_manager.h"
#include "mesh_renderer.h"
#include "sphere_collider.h"
#include "obb_collider.h"
#include "aabb_collider.h"
#include "field_collider.h"
#include "rigidbody3d.h"
#include "animator.h"
#include "actor_controller.h"

#if defined(EDITOR)
#include "field_editor.h"
#include "model_editor.h"
#include "editor_controller.h"
#endif // EDITOR

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
	auto result = MY_NEW GameObject;

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(L"skyBox");
	renderer->SetShaderType(ShaderType::kNoLightNoFog);
	renderer->SetMaterial(L"sky");
	result->AddRenderer(renderer);

	//�p�����[�^�[
	auto transform = result->GetTransform();;
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  MeshField��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateField(const String& name)
{
	auto result = MY_NEW GameObject;
	String field_name = name + L"Field";

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(field_name + L".mesh");
	renderer->SetMaterial(field_name);
	result->AddRenderer(renderer);
	result->AddCollider(MY_NEW FieldCollider(*result, field_name));

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = MY_NEW GameObject;

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(L"cube");
	renderer->SetMaterial(L"cube");
	result->AddRenderer(renderer);
	result->AddCollider(MY_NEW ObbCollider(*result, kDynamic, scale * 0.5f));
	result->SetRigidbody(MY_NEW Rigidbody3D(*result));

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateXModel(const String& name, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	auto result = MY_NEW GameObject;

	//Name
	auto& file_info = utility::AnalyzeFilePath(name);
	result->SetName(file_info.name);

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(name);
	result->AddRenderer(renderer);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  Cube��������
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateGoal(const Vector3& position)
{
	auto result = MY_NEW GameObject;

	//Tag
	result->SetTag(L"Goal");

	//�R���C�_�[
	auto collider = MY_NEW SphereCollider(*result, kStatic, 2.0f);
	collider->SetTrigger(true);
	collider->SetTag(L"Goal");
	result->AddCollider(collider);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetPosition(position);

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
GameObject* GameObjectSpawner::CreateModel(const String& name, const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	auto& file_info = utility::AnalyzeFilePath(name);
	if (!file_info.type._Equal(L"model")) return nullptr;
	
	//Model�t�@�C���̊J��
	String path = L"data/model/" + name;
	ifstream file(path);
	if (!file.is_open)
	{
		assert("failed to open");
		return nullptr;
	}
	BinaryInputArchive archive(file);
	auto result = CreateChildNode(nullptr, archive);

	//Name
	result->SetName(file_info.name);

	//Trans
	auto transform = result->GetTransform();
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);

	file.close();

	//������
	result->Init();
	return result;
}


//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreatePlayer(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
{
	auto result = MY_NEW GameObjectActor;

	//Tag
	result->SetTag(L"Player");

	//�R���|�l���g
	auto rigidbody = MY_NEW Rigidbody3D(*result);
	result->SetRigidbody(rigidbody);
	auto animator = result->GetAnimator();
	result->AddBehavior(MY_NEW ActorController(*result, *rigidbody, *animator));
	auto collider = MY_NEW SphereCollider(*result, kDynamic, 0.6f);
	collider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	collider->SetTag(L"body");
	result->AddCollider(collider);

	//�p�����[�^�[
	auto transform = result->GetTransform();
	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotation(rotation);

	//������
	result->Init();

	//UI
	//PlayerUI::Create(pPb);

	return result;
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreateEnemy(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
{
	auto result = MY_NEW GameObjectActor;

	////Tag
	//result->SetTag(L"Enemy");
	//
	////�R���|�l���g
	//auto rigidbody = MY_NEW Rigidbody3D(result);
	//result->SetRigidbody(rigidbody);
	//result->SetAnimator(MY_NEW Animator(result, actorPath));
	//result->AddBehavior(MY_NEW EnemyController(result, *rigidbody));
	//
	////�R���C�_�[
	//auto collider = MY_NEW SphereCollider(result, kDynamic, 0.6f);
	//collider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	//collider->SetTag("body");
	//result->AddCollider(collider);
	//auto detector = MY_NEW SphereCollider(result, kDynamic, 6.0f);
	//detector->SetTrigger(true);
	//detector->SetTag("detector");
	//result->AddCollider(detector);
	//
	////�p�����[�^�[
	//auto transform = result->GetTransform();
	//transform->SetPosition(position);
	//transform->SetScale(scale);
	//transform->SetRotation(rotation);
	//
	////������
	//result->Init();
	return result;
}

#if defined(EDITOR)
//--------------------------------------------------------------------------------
//  CreateStageEditor
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateStageEditor(GameObject* field_editor)
{
	auto result = MY_NEW GameObject;

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(L"data/model/target.x");
	result->AddRenderer(renderer);
	auto model_editor = MY_NEW ModelEditor(*result);
	auto editor_controller = MY_NEW EditorController(*result);
	editor_controller->SetFieldEditor(field_editor);
	editor_controller->SetModelEditor(model_editor);
	result->AddBehavior(editor_controller);
	result->AddBehavior(model_editor);

	//������
	result->Init();
	return result;
}

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateFieldEditor(void)
{
	auto result = MY_NEW GameObject;

	//�R���|�l���g
	auto pBehavior = MY_NEW FieldEditor(*result);
	result->AddBehavior(pBehavior);
	auto renderer = MY_NEW MeshRenderer(*result);
	renderer->SetMesh(L"field");
	renderer->SetMaterial(L"editorField");
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
GameObject* GameObjectSpawner::CreateChildNode(Transform* parent, BinaryInputArchive& archive)
{
	auto result = MY_NEW GameObject;

	//Node��
	size_t name_size;
	archive.loadBinary(&name_size, sizeof(name_size));
	string name;
	name.resize(name_size);
	archive.loadBinary(&name[0], name_size);
	result->SetName(String(name.begin(), name.end()));

	//Offset
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	archive.loadBinary(&position, sizeof(position));
	archive.loadBinary(&rotation, sizeof(rotation));
	archive.loadBinary(&scale, sizeof(scale));
	auto transform = result->GetTransform();
	if (parent) transform->RegisterParent(parent, position, rotation);

	//Collider
	int collider_number = 0;
	archive.loadBinary(&collider_number, sizeof(collider_number));
	for (int count = 0; count < collider_number; ++count)
	{
		int collider_type = 0;
		Vector3 collider_positon, collider_rotation, collider_scale;
		archive.loadBinary(&collider_type, sizeof(collider_type));
		archive.loadBinary(&collider_positon, sizeof(collider_positon));
		archive.loadBinary(&collider_rotation, sizeof(collider_rotation));
		archive.loadBinary(&collider_scale, sizeof(collider_scale));

		Collider* collider = nullptr;
		switch (static_cast<ColliderType>(collider_type))
		{
		case kSphere:
			collider = MY_NEW SphereCollider(*result, kStatic, collider_scale.X);
			break;
		case kAabb:
			collider = MY_NEW AabbCollider(*result, kStatic, collider_scale * 0.5f);
			break;
		case kObb:
			collider = MY_NEW ObbCollider(*result, kStatic, collider_scale * 0.5f);
			break;
		default:
			break;
		}

		if (!collider) continue;
		collider->SetOffset(collider_positon, collider_rotation);
		result->AddCollider(collider);
	}

	//Mesh
	int mesh_number = 0;
	archive.loadBinary(&mesh_number, sizeof(mesh_number));
	for (int count = 0; count < mesh_number; ++count)
	{
		//Mesh Name
		int mesh_name_size = 0;
		archive.loadBinary(&mesh_name_size, sizeof(mesh_name_size));
		string mesh_name;
		mesh_name.resize(mesh_name_size);
		archive.loadBinary(&mesh_name[0], mesh_name_size);
		String real_name = String(mesh_name.begin(), mesh_name.end());

		//Check Type
		auto& file_info = utility::AnalyzeFilePath(real_name);
		if (file_info.type._Equal(L"mesh") || file_info.type._Equal(L"x"))
		{//���Ȃ�
			auto childMesh = createChildMesh(transform, meshName);
		}
		else if (file_info.Type._Equal("oneSkinMesh"))
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
	auto result = MY_NEW GameObject;

	//Name
	auto& file_info = utility::AnalyzeFilePath(meshName);
	result->SetName(file_info.Name);

	//�R���|�l���g
	auto renderer = MY_NEW MeshRenderer(result);
	renderer->SetMesh(meshName);
	result->AddRenderer(renderer);

	//�p�����[�^�[
	result->GetTransform()->RegisterParent(parent);

	//������
	result->Init();
	return result;
}