//--------------------------------------------------------------------------------
//	ゲームオブジェクト生成関数
//　gameObjectSpawner.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObject;
class GameObjectActor;
class Transform;

//--------------------------------------------------------------------------------
//  クラス宣言
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
	static GameObjectActor* CreatePlayer(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);
	static GameObjectActor* CreateEnemy(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);

#if defined(EDITOR)
	static GameObject* CreateStageEditor(GameObject* field_editor);
	static GameObject* CreateFieldEditor(void);
#endif // EDITOR	

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	GameObjectSpawner() {}
	GameObjectSpawner(const GameObjectSpawner& value) {}
	GameObjectSpawner& operator=(const GameObjectSpawner& value) {}
	~GameObjectSpawner() {}

	static GameObject* CreateChildNode(Transform* parent, BinaryInputArchive& archive);
	static GameObject* CreateChildMesh(Transform* parent, const String& mesh_name);
};