//--------------------------------------------------------------------------------
//　mesh_manager.h
//	メッシュ管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "render_system.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum MeshType // メッシュのタイプ
{
	k3dMesh = 0,
	k3dSkin,
    k2dMesh
};

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
enum DrawType // 描画タイプ
{
	kPointList = 1,
	kLineList = 2,
	kLineStrip = 3,
	kTriangleList = 4,
	kTriangleStrip = 5,
	kTriangleFan = 6,
};
#else
enum DrawType
{
	kPointList = 0,
	kLineList = 1,
	kLineStrip = 3,
	kTriangleList = 4,
	kTriangleStrip = 5,
	kTriangleFan = 6,
};
#endif

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
struct Mesh
{
	Mesh()
		: type(k3dMesh)
		, draw_type(kTriangleStrip)
		, vertex_number(0)
		, index_number(0)
		, polygon_number(0)
		, vertex_buffer(nullptr)
		, index_buffer(nullptr) {}
	~Mesh()
	{
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
	}

	MeshType type;
	DrawType draw_type;
	int vertex_number;
	int index_number;
	int polygon_number;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
	LPDIRECT3DINDEXBUFFER9	index_buffer;
};
#else
struct Mesh
{
	Mesh()
		: type(k3d)
		, draw_type(kTriangleStrip)
		, vertex_number(0)
		, index_number(0)
		, polygon_number(0) {}
	MeshType type;
	DrawType draw_type;
	int vertex_number;
	int index_number;
	int polygon_number;
};
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class MeshManager
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MeshManager*
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static MeshManager* Create(const LPDIRECT3DDEVICE9 device)
	{
		auto instance = MY_NEW MeshManager(device);
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Uninit();
        MY_DELETE this;
    }
	
	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュを使う
	//  すでにあるの場合ユーザーを1たす、ないの場合ファイルから読み込む
	//  mesh_name : メッシュ名
	//--------------------------------------------------------------------------------
	void Use(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュを使う
	//  すでにあるの場合ユーザーを1たす、ないの場合与えられた頂点で作る
	//  mesh_name : メッシュ名
	//  type : 描画タイプ
	//  vertexes : 頂点データ
	//  indexes : インデックスデータ
	//--------------------------------------------------------------------------------
	void Use(const String& mesh_name, const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number = -1);

	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュを使わない
	//  ユーザーが-1になる、0になった場合メモリから破棄する
	//  mesh_name : メッシュ名
	//--------------------------------------------------------------------------------
	void Disuse(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュを与えられた頂点とインデックスで更新する
	//  mesh_name : メッシュ名
	//  vertexes : 頂点データ
	//  indexes : インデックスデータ
	//--------------------------------------------------------------------------------
	void Update(const String& mesh_name, const vector<Vertex3d>& vertexes, const list<int>& indexes);
	
	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュをファイルに書き出す
	//  mesh_name : メッシュ名
	//  file_name : ファイル名
	//--------------------------------------------------------------------------------
	void SaveMeshToFile(const String& mesh_name, const String& file_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のメッシュのポインタを取得
	//  mesh_name : メッシュ名
	//  return : Mesh*
	//--------------------------------------------------------------------------------
	Mesh* GetMesh(const String& mesh_name) const
	{
		auto iterator = meshes_.find(mesh_name);
		if (meshes_.end() == iterator) return nullptr;
		return iterator->second.pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr int kSphereCircleNumber = 10;
	static constexpr int kSphereVertexesPerCircle = 10;

	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MeshInfo
	{
		MeshInfo() : user_number(1) , pointer(nullptr) {}
		int user_number;
		Mesh* pointer;
	};

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	MeshManager(const LPDIRECT3DDEVICE9 device) : device_(device) {}
	MeshManager() : device_(nullptr) {}
	MeshManager(const MeshManager& value) : device_(nullptr) {}
#else
	MeshManager() {}
	MeshManager(const MeshManager& value) {}
#endif
	MeshManager& operator=(const MeshManager& value) {}
	~MeshManager() {}

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  meshファイルからデータを読み込む
	//  mesh_name : メッシュ名
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromMesh(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  skinファイルからデータを読み込む
	//  skin_name : スキンメッシュ名
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromSkin(const String& skin_name);

	//--------------------------------------------------------------------------------
	//  xファイルからデータを読み込む
	//  mesh_name : メッシュ名
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromXFile(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  cubeメッシュを生成する
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateCube(void);

	//--------------------------------------------------------------------------------
	//  sphereメッシュを生成する
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateSphere(void);

	//--------------------------------------------------------------------------------
	//  skyboxメッシュを生成する
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateSkyBox(void);

    //--------------------------------------------------------------------------------
    //  Polygon2dを生成する
    //  return : MeshInfo
    //--------------------------------------------------------------------------------
    MeshInfo CreatePolygon2d(void);

	//--------------------------------------------------------------------------------
	//  与えられた頂点とインデックスでメッシュを生成する
	//  type : 描画タイプ
	//  vertexes : 頂点データ
	//  indexes : インデックスデータ
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateMesh(const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number);
	
    //--------------------------------------------------------------------------------
    //  create the 2d vertex and index buffer, return true if successed
    //  バーテックスとインデックスバッファの生成
    //  return : bool
    //--------------------------------------------------------------------------------
    bool CreateBuffer2d(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  create the 3d vertex and index buffer, return true if successed
	//  バーテックスとインデックスバッファの生成
	//  return : bool
	//--------------------------------------------------------------------------------
	bool CreateBuffer3d(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  create the 3dSkin vertex and index buffer, return true if successed
	//  バーテックスとインデックスバッファの生成
	//  return : bool
	//--------------------------------------------------------------------------------
	bool CreateBuffer3dSkin(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  DrawTypeによってポリゴンの頂点を返す
	//  return : int
	//--------------------------------------------------------------------------------
	int GetVertexNumberPerPolygon(const DrawType& type);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<String, MeshInfo> meshes_; // メッシュを保存するところ
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	const LPDIRECT3DDEVICE9 device_; // directx9のディバイス
#endif
};