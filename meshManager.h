//--------------------------------------------------------------------------------
//	���b�V���}�l�[�W��
//�@meshManager.h
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CMesh;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CMeshManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	CMeshManager() { m_umMesh.clear(); }
	~CMeshManager() {}

	void	Release(void) { UnloadAll(); delete this; }
	void	UnloadAll(void);
	void	UseMesh(const string& strName);
	void	UseMesh(const string& strFileName, string& texName);
	void	DisuseMesh(const string& strName);

	//Editor�p
	void	CreateEditorField(const int nNumBlockX, const int nNumBlockZ, const CKFVec2& vBlockSize);
	void	UpdateEditorField(const vector<CKFVec3>& vecVtx, const list<int>& listChoosenIdx);
	void	SaveEditorFieldAs(const string& strFileName);

	//Get�֐�
	CMesh*	GetMesh(const string& strName) { return m_umMesh.at(strName).pMesh; }

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MESH
	{
		unsigned short	usNumUsers;
		//list<CMesh*>	listpMesh;
		string			strTex;
		CMesh*			pMesh;
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	CMesh*	createMesh(const string& strName);
	CMesh*	loadFromMesh(const string& strFileName, string& strTexName);
	CMesh*	loadFromXFile(const string& strPath, string& strTexName);
	void	createCube(CMesh* pMesh);
	void	createSphere(CMesh* pMesh);
	void	createSkyBox(CMesh* pMesh);
	void	loadField(const string& strFileName, CMesh* pMesh);
	bool	createBuffer(CMesh* pMesh);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<string, MESH> m_umMesh;
};