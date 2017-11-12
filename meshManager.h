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
#include "main.h"

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
enum DrawType
{
	PointList = 1,
	LineList = 2,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#else
enum DrawType
{
	PointList = 0,
	LineList = 1,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#endif

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct Mesh
{
	Mesh()
		: CurrentType(TriangleStrip)
		, VertexNumber(0)
		, IndexNumber(0)
		, PolygonNumber(0)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		, VertexBuffer(nullptr)
		, IndexBuffer(nullptr)
#endif
	{}
	~Mesh()
	{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);
#endif
	}

	DrawType CurrentType;
	int		 VertexNumber;
	int		 IndexNumber;
	int		 PolygonNumber;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DINDEXBUFFER9	IndexBuffer;
#endif
};

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class MeshManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static void	Create(const LPDIRECT3DDEVICE9 device)
	{
		if (instance) return;
		instance = new MeshManager;
		instance->device = device;
	}
#endif
	static void	Release(void) { SAFE_UNINIT(instance); }
	static auto	Instance(void) { return instance; }
	
	void	Use(const string& meshName);
	void	Use(const string& meshName, const DrawType& type, const vector<VERTEX_3D>& vertexes, const vector<int>& indexes);
	void	Disuse(const string& meshName);
	void	Update(const string& meshName, const vector<VERTEX_3D>& vertexes, const list<int>& indexesToUpdate);
	void	SaveMesh(const string& meshName, const string& fileName);
	Mesh*	GetMesh(const string& meshName)
	{
		auto iterator = meshes.find(meshName);
		if (meshes.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MeshInfo
	{
		MeshInfo() : UserNumber(1) , Pointer(nullptr) {}
		~MeshInfo() {}
		int		UserNumber;
		Mesh*	Pointer;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	MeshManager() { meshes.clear(); }
	~MeshManager() {}
	void		uninit(void);
	MeshInfo	loadFromMesh(const string& filePath);
	MeshInfo	loadFromXFile(const string& filePath);
	MeshInfo	createCube(void);
	MeshInfo	createSphere(void);
	MeshInfo	createSkyBox(void);
	MeshInfo	createMesh(const DrawType& type, const vector<VERTEX_3D>& vertexes, const vector<int>& indexes);
	bool		createBuffer(Mesh* mesh);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static MeshManager*	instance;
	unordered_map<string, MeshInfo> meshes;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DDEVICE9 device;
#endif
};