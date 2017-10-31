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
class CGameObject;
class CTransformComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectSpawner
{
public:
	static CGameObject* CreateSkyBox(const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static CGameObject* CreateField(const string& strStageName);
	static CGameObject* CreateCube(const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static CGameObject* CreateXModel(const string& strPath, const Vector3& Position, const Vector3& vRot, const Vector3& vScale);
	static CGameObject* CreateModel(const string& strFilePath, const Vector3& Position, const Quaternion& qRot, const Vector3& vScale);
	static CGameObject* CreateGoal(const Vector3& Position);

#ifdef _DEBUG
	static CGameObject* CreateEditorController(CGameObject* pFieldEditor);
	static CGameObject* CreateEditorField(void);
#endif // _DEBUG	

private:
	CGameObjectSpawner() {}
	~CGameObjectSpawner() {}

	static CGameObject* createChildNode(CTransformComponent* pParent, FILE* pFile);
	static CGameObject* createChildMesh(CTransformComponent* pParent, const string& strMeshName);
};