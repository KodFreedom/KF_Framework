//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�����֐�
//�@gameObjectSpawner.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include <cereal/archives/binary.hpp>
using namespace cereal;
#include "kf_math.h"
using namespace kodfreedom;
#include "renderer_manager.h"
#include "game_object_manager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObject;
class GameObjectActor;
class Transform;
class Animator;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class GameObjectSpawner
{
public:
	static GameObject* CreateSkyBox(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	static GameObject* CreateField(const String& name);
	static GameObject* CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	static GameObject* CreateXModel(const String& name, const Vector3& position, const Vector3& rotation, const Vector3& scale);
	static GameObject* CreateModel(const String& name, const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	static GameObject* CreateGoal(const Vector3& position);
    static GameObject* CreateBasicPolygon2d(const Vector3& scale, const Layer& layer = kDefaultLayer, const String& material_name = String(), const ShaderType& shader_type = kDefault2dShader, const RenderPriority& render_priority = k2d, const float& rotation = 0.0f, const Vector3& position = Vector3::kZero);
    static GameObject* CreateFlashButton2d(const float flash_speed, const Vector3& scale, const Layer& layer = kDefaultLayer, const String& material_name = String(), const ShaderType& shader_type = kDefault2dShader, const RenderPriority& render_priority = k2d, const float& rotation = 0.0f, const Vector3& position = Vector3::kZero);
	static GameObjectActor* CreatePlayer(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);
	static GameObjectActor* CreateEnemy(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);

#if defined(EDITOR)
	static GameObject* CreateEditor(void);
#endif // EDITOR	

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	GameObjectSpawner() {}
	GameObjectSpawner(const GameObjectSpawner& value) {}
	GameObjectSpawner& operator=(const GameObjectSpawner& value) {}
	~GameObjectSpawner() {}

	//--------------------------------------------------------------------------------
	//	�֐����FCreateChildNode
	//  �֐������F���f���t�@�C������Q�[���I�u�W�F�N�g�쐬
	//	�����F	parent�F�t�@�C���̖��O 
	//			archive : �t�@�C���X�g���[��
	//			animator : �A�j���[�^�[�iskinmesh�̂ݎg���j
	//	�߂�l�FGameObject*
	//--------------------------------------------------------------------------------
	static GameObject* CreateChildNode(Transform* parent, BinaryInputArchive& archive, Animator* animator = nullptr);
};