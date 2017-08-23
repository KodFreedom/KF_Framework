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

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectSpawner
{
public:
	static CGameObject* CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateField(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateModel(const string& strFilePath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateMesh(const string& strMeshName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);

	//Editor
	static CGameObject* CreateEditorController(CGameObject* pFieldEditor);
	static CGameObject* CreateEditorField(void);

private:
	CGameObjectSpawner() {}
	~CGameObjectSpawner() {}
};