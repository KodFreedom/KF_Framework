//--------------------------------------------------------------------------------
//	ゲームオブジェクト生成関数
//　gameObjectSpawner.h
//	Author : Xu Wenjie
//	Date   : 2017-08-20
//--------------------------------------------------------------------------------
#pragma once

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
	static GameObject* CreateField(const string& stageName);
	static GameObject* CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	static GameObject* CreateXModel(const string& modelName, const Vector3& position, const Vector3& rotation, const Vector3& scale);
	static GameObject* CreateModel(const string& modelName, const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	static GameObject* CreateGoal(const Vector3& position);
	static GameObjectActor* CreatePlayer(const string &actorPath, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);
	static GameObjectActor* CreateEnemy(const string &actorPath, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);

#if defined(_DEBUG) || defined(EDITOR)
	static GameObject* CreateStageEditor(GameObject* fieldEditor);
	static GameObject* CreateFieldEditor(void);
#endif // _DEBUG	

private:
	GameObjectSpawner() {}
	~GameObjectSpawner() {}

	static GameObject* createChildNode(Transform* parent, FILE* filePointer);
	static GameObject* createChildMesh(Transform* parent, const string& meshName);
};