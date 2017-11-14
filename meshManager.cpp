//--------------------------------------------------------------------------------
//	メッシュマネージャ
//　meshManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "meshManager.h"
#include "KF_Utility.h"
#include "rendererManager.h"
#include "renderSystem.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
MeshManager* MeshManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：Use
//  関数説明：メッシュの追加
//	引数：	meshName：メッシュの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::Use(const string& meshName)
{
	auto iterator = meshes.find(meshName);
	if (iterator != meshes.end()) 
	{// すでに存在してる
		++iterator->second.UserNumber;
		return;
	}

	// メッシュの作成
	MeshInfo newMesh;
	if (meshName.find(".mesh") != string::npos) newMesh = loadFromMesh(meshName);
	else if (meshName.find(".x") != string::npos) newMesh = loadFromXFile(meshName);
	else if (meshName._Equal("cube")) newMesh = createCube();
	else if (meshName._Equal("sphere")) newMesh = createSphere();
	else if (meshName._Equal("skyBox")) newMesh = createSkyBox();
	else
	{
		assert("wrong type!!");
		return;
	}
	meshes.emplace(meshName, newMesh);
}

//--------------------------------------------------------------------------------
//	関数名：Use
//  関数説明：メッシュの追加
//	引数：	meshName：メッシュの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::Use(const string& meshName, const DrawType& type, const vector<VERTEX_3D>& vertexes, const vector<int>& indexes)
{
	auto iterator = meshes.find(meshName);
	if (iterator != meshes.end())
	{// すでに存在してる
		++iterator->second.UserNumber;
		return;
	}

	// メッシュの作成
	MeshInfo newMesh = createMesh(type, vertexes, indexes);
	meshes.emplace(meshName, newMesh);
}

//--------------------------------------------------------------------------------
//	関数名：Disuse
//  関数説明：メッシュの破棄
//	引数：	meshName：メッシュの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::Disuse(const string& meshName)
{
	auto iterator = meshes.find(meshName);
	if (meshes.end() == iterator) return;
	if (--iterator->second.UserNumber <= 0)
	{// 誰も使ってないので破棄する
		delete iterator->second.Pointer;
		meshes.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	関数名：Update
//  関数説明：メッシュの更新処理
//	引数：	meshName：メッシュの名前
//			vertexes
//			indexesToUpdate
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::Update(const string& meshName, const vector<VERTEX_3D>& vertexes, const list<int>& indexesToUpdate)
{
	auto iterator = meshes.find(meshName);
	if (meshes.end() == iterator) return;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	VERTEX_3D* vertexPointer;
	iterator->second.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	for (auto index : indexesToUpdate)
	{
		vertexPointer[index] = vertexes[index];
	}
	iterator->second.Pointer->VertexBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//	関数名：SaveMesh
//  関数説明：メッシュの保存
//	引数：	meshName：メッシュの名前
//			fileName：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::SaveMesh(const string& meshName, const string& fileName)
{
	auto iterator = meshes.find(meshName);
	if (meshes.end() == iterator) return;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	auto pointer = iterator->second.Pointer;

	//file open
	string filePath = "data/MESH/" + fileName + ".mesh";
	FILE *filePointer;
	fopen_s(&filePointer, filePath.c_str(), "wb");
	
	//DrawType
	fwrite(&pointer->CurrentType, sizeof(DrawType), 1, filePointer);
	
	//NumVtx
	fwrite(&pointer->VertexNumber, sizeof(int), 1, filePointer);
	
	//NumIdx
	fwrite(&pointer->IndexNumber, sizeof(int), 1, filePointer);
	
	//NumPolygon
	fwrite(&pointer->PolygonNumber, sizeof(int), 1, filePointer);
	
	//Vtx&Idx
	//頂点データ
	VERTEX_3D* vertexPointer;
	pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	fwrite(vertexPointer, sizeof(VERTEX_3D), pointer->VertexNumber, filePointer);
	pointer->VertexBuffer->Unlock();
	
	//インデックス
	WORD* indexPointer;
	pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);
	fwrite(indexPointer, sizeof(WORD), pointer->IndexNumber, filePointer);
	pointer->IndexBuffer->Unlock();

	fclose(filePointer);
#endif
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MeshManager::uninit(void)
{
	for (auto iterator = meshes.begin(); iterator != meshes.end();)
	{
		delete iterator->second.Pointer;
		iterator = meshes.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	関数名：loadFromMesh
//  関数説明：メッシュファイルからデータの読込
//	引数：	fileName：ファイルの名前 
//	戻り値：MESH
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::loadFromMesh(const string& fileName)
{
	string filePath = "data/MESH/" + fileName;
	MeshInfo info;
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "rb");

	if (!filePointer) 
	{
		assert("failed to open file!!");
		return info;
	}
	
	info.Pointer = new Mesh;

	//DrawType
	fread_s(&info.Pointer->CurrentType, sizeof(int), sizeof(int), 1, filePointer);

	//NumVtx
	fread_s(&info.Pointer->VertexNumber, sizeof(int), sizeof(int), 1, filePointer);

	//NumIdx
	fread_s(&info.Pointer->IndexNumber, sizeof(int), sizeof(int), 1, filePointer);

	//NumPolygon
	fread_s(&info.Pointer->PolygonNumber, sizeof(int), sizeof(int), 1, filePointer);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!createBuffer(info.Pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

	//頂点データ
	//vector<VERTEX_3D> testV;
	//testV.resize(info.Pointer->VertexNumber);
	//fread_s(&testV[0], sizeof(VERTEX_3D) * info.Pointer->VertexNumber, sizeof(VERTEX_3D), info.Pointer->VertexNumber, filePointer);

	VERTEX_3D* vertexPointer;
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	fread_s(vertexPointer, sizeof(VERTEX_3D) * info.Pointer->VertexNumber, sizeof(VERTEX_3D), info.Pointer->VertexNumber, filePointer);
	info.Pointer->VertexBuffer->Unlock();

	//インデックス
	//vector<WORD> testI;
	//testI.resize(info.Pointer->IndexNumber);
	//fread_s(&testI[0], sizeof(WORD) * info.Pointer->IndexNumber, sizeof(WORD), info.Pointer->IndexNumber, filePointer);

	WORD *indexPointer;
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);
	fread_s(indexPointer, sizeof(WORD) * info.Pointer->IndexNumber, sizeof(WORD), info.Pointer->IndexNumber, filePointer);
	info.Pointer->IndexBuffer->Unlock();
#endif

	////Texture
	//int stringSize;
	//fread_s(&stringSize, sizeof(int), sizeof(int), 1, filePointer);
	//info.CurrentRenderInfo.TextureName.resize(stringSize);
	//fread_s(&info.CurrentRenderInfo.TextureName[0], stringSize, sizeof(char), stringSize, filePointer);

	////Render Priority
	////fread_s(&info.CurrentRenderInfo.CurrentPriority, sizeof(RenderPriority), sizeof(RenderPriority), 1, filePointer);

	////Render State
	////fread_s(&info.CurrentRenderInfo.CurrentState, sizeof(RenderState), sizeof(RenderState), 1, filePointer);

	////Lighting
	//fread_s(&info.CurrentRenderInfo.CurrentLighting, sizeof(Lighting), sizeof(Lighting), 1, filePointer);

	////CullMode
	//fread_s(&info.CurrentRenderInfo.CurrentCullMode, sizeof(CullMode), sizeof(CullMode), 1, filePointer);

	////Synthesis
	//fread_s(&info.CurrentRenderInfo.CurrentSynthesis, sizeof(Synthesis), sizeof(Synthesis), 1, filePointer);

	////FillMode
	//fread_s(&info.CurrentRenderInfo.CurrentFillMode, sizeof(FillMode), sizeof(FillMode), 1, filePointer);

	////Alpha
	//fread_s(&info.CurrentRenderInfo.CurrentAlpha, sizeof(Alpha), sizeof(Alpha), 1, filePointer);

	////Fog
	//fread_s(&info.CurrentRenderInfo.CurrentFog, sizeof(Fog), sizeof(Fog), 1, filePointer);

	fclose(filePointer);

	return info;
}

//--------------------------------------------------------------------------------
//	関数名：loadFromXFile
//  関数説明：Xファイルからデータの読込
//	引数：	filePath：ファイルのパス
//	戻り値：MESH
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::loadFromXFile(const string& filePath)
{
	MeshInfo info;
	FILE* filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "r");

	if (!filePointer)
	{
		assert("failed to open file!!");
		return info;
	}

	info.Pointer = new Mesh;
	info.Pointer->CurrentType = DrawType::TriangleList;
	vector<Vector3>	vertexes;
	vector<Vector3>	normals;
	vector<Vector2>	uvs;
	vector<Color>	colors;
	vector<int>		vertexIndexes;
	vector<int>		normalIndexes;
	vector<int>		colorIndexes;

	string buffer;
	while (Utility::GetStringUntilToken(filePointer, "\n", buffer) >= 0)
	{
		if (buffer.compare("Mesh {") == 0)
		{
			//頂点数の読込
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int vertexNumber;
			ss << buffer;
			ss >> vertexNumber;

			//頂点データの読込
			vertexes.resize(vertexNumber);
			for (int count = 0; count < vertexNumber; ++count)
			{
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].X;
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].Y;
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].Z;
				Utility::GetStringUntilToken(filePointer, "\n", buffer);
			}

			//ポリゴン数の読込
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			ss << buffer;
			ss >> info.Pointer->PolygonNumber;

			//インデックスの読込
			info.Pointer->VertexNumber =
				info.Pointer->IndexNumber = info.Pointer->PolygonNumber * 3;
			vertexIndexes.resize(info.Pointer->IndexNumber);
			for (int count = 0; count < info.Pointer->PolygonNumber; ++count)
			{
				//3;を飛ばす
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				Utility::GetStringUntilToken(filePointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexIndexes[count * 3];
				Utility::GetStringUntilToken(filePointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexIndexes[count * 3 + 1];
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexIndexes[count * 3 + 2];
				Utility::GetStringUntilToken(filePointer, "\n", buffer);
			}
		}

		if (buffer.compare(" MeshMaterialList {") == 0)
		{
			//Material数
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int colorNumber;
			ss << buffer;
			ss >> colorNumber;
			colors.resize(colorNumber);

			//インデックス数
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			int polygonNumber;
			ss << buffer;
			ss >> polygonNumber;
			colorIndexes.resize(polygonNumber * 3);

			//インデックスの読み込み
			for (int count = 0; count < polygonNumber; ++count)
			{
				Utility::GetStringUntilToken(filePointer, ",;", buffer);
				ss.clear();
				ss << buffer;
				ss >> colorIndexes[count * 3];
				colorIndexes[count * 3 + 1] = 
					colorIndexes[count * 3 + 2] =
					colorIndexes[count * 3];
			}

			//マテリアルの読み込み
			for (int count = 0; count < colorNumber; ++count)
			{
				while (Utility::GetStringUntilToken(filePointer, "\n", buffer) >= 0)
				{
					if (buffer.compare("  Material {") == 0)
					{
						//R
						Utility::GetStringUntilToken(filePointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].R;

						//G
						Utility::GetStringUntilToken(filePointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].G;

						//B
						Utility::GetStringUntilToken(filePointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].B;

						//A
						Utility::GetStringUntilToken(filePointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].A;

						break;
					}
				}
			}
		}

		if (buffer.compare(" MeshNormals {") == 0)
		{
			//法線数の読込
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int normalNumber;
			ss << buffer;
			ss >> normalNumber;

			//法線データの読込
			normals.resize(normalNumber);
			for (int count = 0; count < normalNumber; ++count)
			{
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].X;
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].Y;
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].Z;
				Utility::GetStringUntilToken(filePointer, "\n", buffer);
			}

			//法線インデックス数を読込(頂点数と同じのため)
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			int normalIndexNumber;
			ss << buffer;
			ss >> normalIndexNumber;
			if (normalIndexNumber != info.Pointer->PolygonNumber)
			{//エラー
				MessageBox(NULL,"MeshManager:法線数と頂点数が違う！", "エラー", MB_OK | MB_ICONWARNING);
			}

			//法線インデックスの読込
			normalIndexes.resize(normalIndexNumber * 3);
			for (int count = 0; count < info.Pointer->PolygonNumber; ++count)
			{
				//3;を飛ばす
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				Utility::GetStringUntilToken(filePointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> normalIndexes[count * 3];
				Utility::GetStringUntilToken(filePointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> normalIndexes[count * 3 + 1];
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normalIndexes[count * 3 + 2];
				Utility::GetStringUntilToken(filePointer, "\n", buffer);
			}
		}

		if (buffer.compare(" MeshTextureCoords {") == 0)
		{
			//UV数の読込
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int uvNumber;
			ss << buffer;
			ss >> uvNumber;
			if (uvNumber != vertexes.size())
			{//エラー
				MessageBox(NULL, "MeshManager:UV数と頂点数が違う！", "エラー", MB_OK | MB_ICONWARNING);
			}

			//UVデータの読込
			uvs.resize(uvNumber);
			for (int count = 0; count < uvNumber; ++count)
			{
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> uvs[count].X;
				Utility::GetStringUntilToken(filePointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> uvs[count].Y;
				Utility::GetStringUntilToken(filePointer, "\n", buffer);
			}
		}
	}

	fclose(filePointer);
	
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!createBuffer(info.Pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *vertexPointer;

	//頂点バッファをロックして、仮想アドレスを取得する
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);

	for (int count = 0; count < info.Pointer->IndexNumber; ++count)
	{
		vertexPointer[count].Position = vertexes[vertexIndexes[count]];
		vertexPointer[count].Normal = normals[normalIndexes[count]];
		vertexPointer[count].UV = uvs[vertexIndexes[count]];
		vertexPointer[count].Color = colors[colorIndexes[count]];
	}

	//仮想アドレス解放
	info.Pointer->VertexBuffer->Unlock();

	//インデックス
	WORD *indexPointer;
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);

	for (int count = 0; count < info.Pointer->IndexNumber; ++count)
	{
		indexPointer[count] = count;
	}

	info.Pointer->IndexBuffer->Unlock();
#endif

	return info;
}

//--------------------------------------------------------------------------------
//  Cubeの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::createCube(void)
{
	MeshInfo info;
	info.Pointer = new Mesh;
	info.Pointer->VertexNumber = 6 * 4;
	info.Pointer->IndexNumber = 6 * 4 + 5 * 2;
	info.Pointer->PolygonNumber = 6 * 2 + 5 * 4;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!createBuffer(info.Pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

	// 仮想アドレスを取得するためのポインタ
	VERTEX_3D *vertexPointer;

	// 頂点バッファをロックして、仮想アドレスを取得する
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	auto& halfSize = Vector3::One * 0.5f;
	unsigned long white = Color::White;
	int countVertex = 0;

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			-halfSize.Z);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Back;
		++countVertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			halfSize.Y,
			halfSize.Z - (count / 2) * halfSize.Z * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Up;
		++countVertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-halfSize.X,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			halfSize.Z - (count % 2) * halfSize.Z * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Left;

		++countVertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			-halfSize.Y,
			-halfSize.Z + (count / 2) * halfSize.Z * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Down;
		++countVertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			halfSize.X,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			-halfSize.Z + (count % 2) * halfSize.Z * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Right;
		++countVertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			halfSize.X - (count % 2) * halfSize.X * 2.0f,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			halfSize.Z);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Forward;
		++countVertex;
	}

	// 仮想アドレス解放
	info.Pointer->VertexBuffer->Unlock();

	// インデックス
	WORD *indexPointer;
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);

	for (int count = 0; count < info.Pointer->IndexNumber; ++count)
	{
		if (count % 6 < 4)
		{
			indexPointer[count] = (WORD)((count / 6) * 4 + (count % 6) % 4);
		}
		else
		{// 縮退
			indexPointer[count] = (WORD)((count / 6) * 4 + (count % 2) + 3);
		}
	}

	info.Pointer->IndexBuffer->Unlock();

	// Modelファイルの保存
	string fileName = "data/MODEL/cube.model";
	FILE *filePointer;

	// file open
	fopen_s(&filePointer, fileName.c_str(), "wb");

	// Node名
	string nodeName = "cube";
	int stringSize = (int)nodeName.size();
	fwrite(&stringSize, sizeof(int), 1, filePointer);
	fwrite(&nodeName[0], sizeof(char), stringSize, filePointer);

	// Offset
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filePointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filePointer);
	fwrite(&Vector3::One, sizeof(Vector3), 1, filePointer);

	// Collider
	int colliderNumber = 1;
	fwrite(&colliderNumber, sizeof(int), 1, filePointer);

	int type = 1;
	fwrite(&type, sizeof(int), 1, filePointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filePointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filePointer);
	fwrite(&Vector3::One, sizeof(Vector3), 1, filePointer);

	// Texture
	int textureNumber = 1;
	fwrite(&textureNumber, sizeof(int), 1, filePointer);
	string textureName = "nomal_cube.jpg";
	stringSize = textureName.size();
	fwrite(&stringSize, sizeof(int), 1, filePointer);
	fwrite(&textureName[0], sizeof(char), stringSize, filePointer);

	// Mesh
	int meshNumber = 1;
	fwrite(&meshNumber, sizeof(int), 1, filePointer);
	string meshName = nodeName + ".mesh";
	stringSize = (int)meshName.size();
	fwrite(&stringSize, sizeof(int), 1, filePointer);
	fwrite(&meshName[0], sizeof(char), stringSize, filePointer);

	fclose(filePointer);

	// Mesh
	fileName = "data/MESH/cube.mesh";

	// file open
	fopen_s(&filePointer, fileName.c_str(), "wb");

	// DrawType
	int drawType = (int)info.Pointer->CurrentType;
	fwrite(&drawType, sizeof(int), 1, filePointer);

	// NumVtx
	fwrite(&info.Pointer->VertexNumber, sizeof(int), 1, filePointer);

	// NumIdx
	fwrite(&info.Pointer->IndexNumber, sizeof(int), 1, filePointer);

	// NumPolygon
	fwrite(&info.Pointer->PolygonNumber, sizeof(int), 1, filePointer);

	// Vtx&Idx
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	fwrite(vertexPointer, sizeof(VERTEX_3D), info.Pointer->VertexNumber, filePointer);
	info.Pointer->VertexBuffer->Unlock();
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);
	fwrite(indexPointer, sizeof(WORD), info.Pointer->IndexNumber, filePointer);
	info.Pointer->IndexBuffer->Unlock();

	// Texture
	textureName = "nomal_cube.jpg";
	stringSize = textureName.size();
	fwrite(&stringSize, sizeof(int), 1, filePointer);
	fwrite(&textureName[0], sizeof(char), stringSize, filePointer);

	//Lighting
	auto lighting = Lighting_On;
	fwrite(&lighting, sizeof(lighting), 1, filePointer);

	//CullMode
	auto cullMode = Cull_CCW;
	fwrite(&cullMode, sizeof(cullMode), 1, filePointer);

	//Synthesis
	auto synthesis = S_Multiplication;
	fwrite(&synthesis, sizeof(synthesis), 1, filePointer);

	//FillMode
	auto fillMode = Fill_Solid;
	fwrite(&fillMode, sizeof(fillMode), 1, filePointer);

	//Alpha
	auto alpha = A_None;
	fwrite(&alpha, sizeof(alpha), 1, filePointer);

	//Fog
	auto fog = Fog_On;
	fwrite(&fog, sizeof(fog), 1, filePointer);

	fclose(filePointer);
#endif

	return info;
}

//--------------------------------------------------------------------------------
//  Sphereの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::createSphere(void)
{
	MeshInfo info;
	assert("未実装!!");
	return info;
}

//--------------------------------------------------------------------------------
//  SkyBoxの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::createSkyBox(void)
{
	MeshInfo info;
	info.Pointer = new Mesh;
	info.Pointer->VertexNumber = 6 * 4;
	info.Pointer->IndexNumber = 6 * 4 + 5 * 2;
	info.Pointer->PolygonNumber = 6 * 2 + 5 * 4;

	if (!createBuffer(info.Pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	// 仮想アドレスを取得するためのポインタ
	VERTEX_3D *vertexPointer;

	// 頂点バッファをロックして、仮想アドレスを取得する
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	auto camera = CameraManager::Instance()->GetMainCamera();
	float length = camera ? (float)camera->GetFar() * 0.5f : 500.0f;
	unsigned long white = Color::White;
	int countVertex = 0;
	float uvTweens = 1.0f / 1024.0f;	//隙間を無くすためにUVを1px縮める

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			-length);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Forward;
		++countVertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			length,
			length - (count / 2) * length * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Down;
		++countVertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-length,
			length - (count / 2) * length * 2.0f,
			-length + (count % 2) * length * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + 0.5f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Right;
		++countVertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			-length,
			-length + (count / 2) * length * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 2.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Up;
		++countVertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			length,
			length - (count / 2) * length * 2.0f,
			length - (count % 2) * length * 2.0f);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Left;
		++countVertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		vertexPointer[countVertex].Position = Vector3(
			-length + (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			length);
		vertexPointer[countVertex].UV = Vector2((count % 2) * 0.25f + 0.75f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		vertexPointer[countVertex].Color = white;
		vertexPointer[countVertex].Normal = Vector3::Back;
		++countVertex;
	}

	//仮想アドレス解放
	info.Pointer->VertexBuffer->Unlock();

	//インデックス
	WORD *indexPointer;
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);

	for (int count = 0; count < 6 * 4 + 5 * 2; ++count)
	{
		if (count % 6 < 4)
		{
			indexPointer[count] = (count / 6) * 4 + (count % 6) % 4;
		}
		else
		{//縮退
			indexPointer[count] = (count / 6) * 4 + (count % 2) + 3;
		}
	}

	info.Pointer->IndexBuffer->Unlock();
#endif
	
	//Render State
	//info.CurrentRenderInfo.CurrentLighting = Lighting_Off;
	//info.CurrentRenderInfo.CurrentCullMode = Cull_CCW;
	//info.CurrentRenderInfo.CurrentSynthesis = S_Multiplication;
	//info.CurrentRenderInfo.CurrentFillMode = Fill_Solid;
	//info.CurrentRenderInfo.CurrentAlpha = A_None;
	//info.CurrentRenderInfo.CurrentFog = Fog_Off;

	return info;
}

//--------------------------------------------------------------------------------
//  Meshの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::createMesh(const DrawType& type, const vector<VERTEX_3D>& vertexes, const vector<int>& indexes)
{
	MeshInfo info;
	info.Pointer = new Mesh;
	info.Pointer->VertexNumber = (int)vertexes.size();
	info.Pointer->IndexNumber = (int)indexes.size();
	info.Pointer->PolygonNumber = info.Pointer->IndexNumber / getVertexNumberPerPolygon(type);

	if (!createBuffer(info.Pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//Vertex
	VERTEX_3D* vertexPointer;
	info.Pointer->VertexBuffer->Lock(0, 0, (void**)&vertexPointer, 0);
	for (int count = 0; count < info.Pointer->VertexNumber; ++count)
	{
		vertexPointer[count] = vertexes[count];
	}
	info.Pointer->VertexBuffer->Unlock();

	//インデックス
	WORD* indexPointer;
	info.Pointer->IndexBuffer->Lock(0, 0, (void**)&indexPointer, 0);
	for (int count = 0; count < info.Pointer->IndexNumber; ++count)
	{
		indexPointer[count] = indexes[count];
	}
	info.Pointer->IndexBuffer->Unlock();
#endif

	return info;
}

//--------------------------------------------------------------------------------
//  バッファの作成
//--------------------------------------------------------------------------------
bool MeshManager::createBuffer(Mesh* mesh)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//頂点バッファ
	HRESULT hr = device->CreateVertexBuffer(
		sizeof(VERTEX_3D) * mesh->VertexNumber,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
		FVF_VERTEX_3D,							//書かなくても大丈夫
		D3DPOOL_MANAGED,						//メモリ管理方法(managed：デバイスにお任せ)
		&mesh->VertexBuffer,					//頂点バッファのポインタ
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//インデックスバッファの作成
	hr = device->CreateIndexBuffer(
		sizeof(WORD) * mesh->IndexNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->IndexBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateIndexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  ドロータイプからポリゴンを構成する頂点数を取得する
//--------------------------------------------------------------------------------
int MeshManager::getVertexNumberPerPolygon(const DrawType& type)
{
	switch (type)
	{
	case DrawType::PointList:
		return 1;
	case DrawType::LineList:
		return 2;
	case DrawType::LineStrip:
		return 2;
	case DrawType::TriangleList:
		return 3;
	case DrawType::TriangleStrip:
		return 3;
	case DrawType::TriangleFan:
		return 3;
	default:
		return 0;
	}
}