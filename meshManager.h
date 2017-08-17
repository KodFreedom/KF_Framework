//--------------------------------------------------------------------------------
//	メッシュマネージャ
//　meshManager.h
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CMesh;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CMeshManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CMeshManager() { m_umMesh.clear(); }
	~CMeshManager() {}

	void	Release(void) { UnloadAll(); delete this; }
	void	UnloadAll(void);
	void	UseMesh(const string& strName);
	void	UseMesh(const string& strName, string& texName);
	void	DisuseMesh(const string& strName);

	//Get関数
	CMesh*	GetMesh(const string& strName) { return m_umMesh.at(strName).pMesh; }

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MESH
	{
		unsigned short	usNumUsers;
		//list<CMesh*>	listpMesh;
		CMesh*			pMesh;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CMesh*	createMesh(const string& strName);
	CMesh*	createMesh(const string& strName, string& texName);
	void	createXFile(const string& strName, CMesh* pMesh, string& texName);
	void	createCube(CMesh* pMesh);
	void	createSphere(CMesh* pMesh);
	void	createSkyBox(CMesh* pMesh);
	bool	createBuffer(CMesh* pMesh);

	//一時採用
	void	createField(CMesh* pMesh);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MESH> m_umMesh;
};