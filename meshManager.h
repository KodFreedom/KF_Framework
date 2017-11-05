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
class MeshInfo;

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct RenderInfo
{
	string		TextureName;
	Lighting	CurrentLighting;
	CullMode	CurrentCullMode;
	Synthesis	CurrentSynthesis;
	FillMode	CurrentFillMode;
	Alpha		CurrentAlpha;
	Fog			CurrentFog;
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
	static auto Create(void)
	{
		if (instance) return instance;
		instance = new MeshManager;
		instance->init();
		return instance;
	}
	static void	Release(void) { SAFE_UNINIT(instance); }
	static auto	Instance(void) { return instance; }
	
	void		Use(const string& meshName);
	void		Disuse(const string& meshName);
	
	//Editor用
	void		CreateEditorField(const int blockXNumber, const int blockZNumber, const Vector2& blockSize);
	void		UpdateEditorField(const vector<Vector3>& vertexes, const list<int>& choosenIndexes);
	void		SaveEditorFieldAs(const string& fileName);

	//Get関数
	auto		GetRenderInfoBy(const string& meshName) const { return meshes.at(meshName).CurrentRenderInfo; }
	auto		GetMeshBy(const string& meshName) { return meshes.at(meshName).CurrentMesh; }

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MeshStruct
	{
		MeshStruct()
			: UserNumber(1)
			, CurrentMesh(nullptr)
		{
			CurrentRenderInfo.TextureName.clear();
			CurrentRenderInfo.CurrentLighting = Lighting::On;
			CurrentRenderInfo.CurrentCullMode = CullMode::CCW;
			CurrentRenderInfo.CurrentSynthesis = Synthesis::Multiplication;
			CurrentRenderInfo.CurrentFillMode = FillMode::Solid;
			CurrentRenderInfo.CurrentAlpha = Alpha::None;
			CurrentRenderInfo.CurrentFog = Fog::On;
		}
		unsigned short	UserNumber;
		MeshInfo*		CurrentMesh;
		RenderInfo		CurrentRenderInfo;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	MeshManager() { meshes.clear(); }
	~MeshManager() {}
	void		init(void) {};
	void		uninit(void);
	MeshStruct	loadFromMesh(const string& filePath);
	MeshStruct	loadFromXFile(const string& filePath);
	MeshStruct	createCube(void);
	MeshStruct	createSphere(void);
	MeshStruct	createSkyBox(void);
	bool		createBuffer(MeshInfo* mesh);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MeshStruct> meshes;
	static MeshManager*			instance;
};