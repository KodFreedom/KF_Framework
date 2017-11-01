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
#include "renderManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CMesh;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CMeshManager
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct RENDER_INFO
	{
		string			strTex;
		RENDER_PRIORITY renderPriority;
		RENDER_STATE	renderState;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CMeshManager() { m_umMesh.clear(); }
	~CMeshManager() {}

	void	Release(void) { UnloadAll(); delete this; }
	void	UnloadAll(void);
	void	UseMesh(const string& strName);
	void	DisuseMesh(const string& strName);
	auto	GetRenderInfo(const string& strName) const { return m_umMesh.at(strName).renderInfo; }

	//Editor用
	void	CreateEditorField(const int nNumBlockX, const int nNumBlockZ, const Vector2& vBlockSize);
	void	UpdateEditorField(const vector<Vector3>& vecVtx, const list<int>& listChoosenIdx);
	void	SaveEditorFieldAs(const string& strFileName);

	//Get関数
	CMesh*	GetMesh(const string& strName) { return m_umMesh.at(strName).pMesh; }

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MESH
	{
		MESH()
			: usNumUsers(1)
			, pMesh(nullptr)
		{
			renderInfo.strTex.clear();
			renderInfo.renderPriority = RP_3D;
			renderInfo.renderState = RS_LIGHTON_CULLFACEON_MUL;
		}
		unsigned short	usNumUsers;
		CMesh*			pMesh;
		RENDER_INFO		renderInfo;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	MESH	loadFromMesh(const string& strFileName);
	MESH	loadFromXFile(const string& strPath);
	MESH	createCube(void);
	MESH	createSphere(void);
	MESH	createSkyBox(void);
	bool	createBuffer(CMesh* pMesh);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MESH> m_umMesh;
};