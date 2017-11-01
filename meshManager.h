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
class Mesh;

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct RenderInfo
{
	string			TextureName;
	RenderPriority	CurrentPriority;
	RenderState		CurrentState;
};

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class MeshManager
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	static MeshManager* Create(void)
	{
		if (instance) return instance;
		instance = new MeshManager;
		instance->init();
		return instance;
	}
	static void			Release(void) { SAFE_UNINIT(instance); }
	static auto			Instance(void) { return instance; }
	
	void	Use(const string& meshName);
	void	Disuse(const string& meshName);
	
	//Editor用
	void	CreateEditorField(const int blockXNumber, const int blockZNumber, const Vector2& blockSize);
	void	UpdateEditorField(const vector<Vector3>& vertexes, const list<int>& choosenIndexes);
	void	SaveEditorFieldAs(const string& fileName);

	//Get関数
	auto	GetRenderInfoBy(const string& meshName) const { return meshes.at(meshName).CurrentRenderInfo; }
	auto	GetMeshBy(const string& meshName) { return meshes.at(meshName).CurrentMesh; }

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MeshInfo
	{
		MeshInfo()
			: UserNumber(1)
			, CurrentMesh(nullptr)
		{
			CurrentRenderInfo.TextureName.clear();
			CurrentRenderInfo.CurrentPriority = RP_3D;
			CurrentRenderInfo.CurrentState = RS_LIGHTON_CULLFACEON_MUL;
		}
		unsigned short	UserNumber;
		Mesh*			CurrentMesh;
		RenderInfo		CurrentRenderInfo;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	MeshManager() { meshes.clear(); }
	~MeshManager() {}
	void		init(void) {};
	void		uninit(void);
	MeshInfo	loadFromMesh(const string& filePath);
	MeshInfo	loadFromXFile(const string& filePath);
	MeshInfo	createCube(void);
	MeshInfo	createSphere(void);
	MeshInfo	createSkyBox(void);
	bool		createBuffer(Mesh* mesh);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MeshInfo> meshes;
	static MeshManager*			instance;
};