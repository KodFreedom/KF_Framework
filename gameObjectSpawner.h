//--------------------------------------------------------------------------------
//	ゲームオブジェクト生成関数
//　gameObjectSpawner.h
//	Author : Xu Wenjie
//	Date   : 2017-08-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObject;
class CTransformComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class GameObjectSpawner
{
public:
	static GameObject* CreateSkyBox(const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static GameObject* CreateField(const string& strStageName);
	static GameObject* CreateCube(const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static GameObject* CreateXModel(const string& strPath, const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static GameObject* CreateModel(const string& filePath, const Vector3& Position, const Quaternion& qRot, const Vector3& vScale);
	static GameObject* CreateGoal(const Vector3& Position);

#if defined(_DEBUG) || defined(EDITOR)
	static GameObject* CreateEditorController(GameObject* pFieldEditor);
	static GameObject* CreateEditorField(void);
#endif // _DEBUG	

private:
	GameObjectSpawner() {}
	~GameObjectSpawner() {}

	static GameObject* createChildNode(CTransformComponent* pParent, FILE* filePointer);
	static GameObject* createChildMesh(CTransformComponent* pParent, const string& strMeshName);
};