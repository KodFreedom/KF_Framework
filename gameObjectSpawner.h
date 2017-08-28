//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�����֐�
//�@gameObjectSpawner.h
//	Author : Xu Wenjie
//	Date   : 2017-08-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject;
class CTransformComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectSpawner
{
public:
	static CGameObject* CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateField(const string& strStageName);
	static CGameObject* CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject* CreateModel(const string& strFilePath, const CKFVec3& vPos, const CKFQuaternion& qRot, const CKFVec3& vScale);
	static CGameObject* CreateGoal(const CKFVec3& vPos);

	//Editor
	static CGameObject* CreateEditorController(CGameObject* pFieldEditor);
	static CGameObject* CreateEditorField(void);

private:
	CGameObjectSpawner() {}
	~CGameObjectSpawner() {}

	static CGameObject* createChildMesh(CTransformComponent* pParent, FILE* pFile);
	static CGameObject* createMesh(const string& strMeshName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
};