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
#include "manager.h"
#include "meshManager.h"
#include "mesh.h"
#include "camera.h"
#include "KF_Utility.h"
#include "renderManager.h"
#include "renderer.h"

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
	auto itr = meshes.find(meshName);
	if (itr != meshes.end()) 
	{// すでに存在してる
		++itr->second.UserNumber;
		return;
	}

	// メッシュの作成
	MeshInfo meshInfo;
	if (meshName.find(".mesh") != string::npos) meshInfo = loadFromMesh(meshName);
	else if (meshName.find(".x") != string::npos) meshInfo = loadFromXFile(meshName);
	else if (meshName._Equal("cube")) meshInfo = createCube();
	else if (meshName._Equal("sphere")) meshInfo = createSphere();
	else if (meshName._Equal("skyBox")) meshInfo = createSkyBox();
	else
	{
		assert("wrong type!!");
		return;
	}
	meshes.emplace(meshName, meshInfo);
}

//--------------------------------------------------------------------------------
//	関数名：Disuse
//  関数説明：メッシュの破棄
//	引数：	meshName：メッシュの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::Disuse(const string& meshName)
{
	auto itr = meshes.find(meshName);
	--itr->second.UserNumber;
	if (itr->second.UserNumber == 0)
	{// 誰も使ってないので破棄する
		delete itr->second.CurrentMesh;
		meshes.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//	関数名：CreateEditorField
//  関数説明：エディタ用フィールドの作成
//	引数：	blockXNumber：X方向のブロック数
//			blockZNumber：Z方向のブロック数
//			blockSize：ブロックのサイズ
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::CreateEditorField(const int blockXNumber, const int blockZNumber, const Vector2& blockSize)
{
	auto itr = meshes.find("field");
	if (itr != meshes.end())
	{ //すでに存在してる
		return;
	}

	auto mesh = new Mesh;
	mesh->VertexNumber = (blockXNumber + 1) * (blockZNumber + 1);
	mesh->IndexNumber = ((blockXNumber + 1) * 2 + 2) * blockZNumber - 1;
	mesh->PolygonNumber = (blockXNumber + 2) * 2 * blockZNumber - 4;
	if (!createBuffer(mesh)) { return; }

#ifdef USING_DIRECTX
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	mesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto& startPosition = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	for (int countZ = 0; countZ < blockZNumber + 1; ++countZ)
	{
		for (int countX = 0; countX < blockXNumber + 1; ++countX)
		{
			pVtx[countZ * (blockXNumber + 1) + countX].Position = startPosition
				+ Vector3(countX * blockSize.X, 0.0f, -countZ * blockSize.Y);
			pVtx[countZ * (blockXNumber + 1) + countX].UV = Vector2(countX * 1.0f / (float)blockXNumber, countZ * 1.0f / (float)blockXNumber);
			pVtx[countZ * (blockXNumber + 1) + countX].Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[countZ * (blockXNumber + 1) + countX].Normal = Vector3(0.0f, 1.0f, 0.0f);
		}
	}

	//仮想アドレス解放
	mesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	mesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int countZ = 0; countZ < blockZNumber; ++countZ)
	{
		for (int countX = 0; countX < (blockXNumber + 1) * 2 + 2; ++countX)
		{
			if (countX < (blockXNumber + 1) * 2)
			{
				pIdx[countZ * ((blockXNumber + 1) * 2 + 2) + countX] = countX / 2 + (countX + 1) % 2 * (blockXNumber + 1) + countZ * (blockXNumber + 1);
			}
			else if (countZ * ((blockXNumber + 1) * 2 + 2) + countX < (((blockXNumber + 1) * 2 + 2) * blockZNumber - 1))//縮退ポリゴン
			{
				pIdx[countZ * ((blockXNumber + 1) * 2 + 2) + countX] = (countX - 1) / 2 % (blockXNumber + 1) + countX % 2 * 2 * (blockXNumber + 1) + countZ * (blockXNumber + 1);
			}
		}
	}

	mesh->m_pIdxBuffer->Unlock();
#endif

	//メッシュの作成
	MeshInfo info;
	info.CurrentMesh = mesh;

	//保存
	meshes.emplace("field", info);
}

//--------------------------------------------------------------------------------
//	関数名：UpdateEditorField
//  関数説明：エディタ用フィールドデータの更新
//	引数：	vertexes：頂点データ
//			choosenIndexes：選択された頂点のインデックス
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::UpdateEditorField(const vector<Vector3>& vertexes, const list<int>& choosenIndexes)
{
#ifdef USING_DIRECTX

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	auto mesh = GetMeshBy("field");
	mesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto color = Color(1.0f);
	for (int count = 0; count < (int)vertexes.size(); ++count)
	{
		pVtx[count].Position = vertexes[count];
		pVtx[count].Color = color;
	}

	//Choosen Color
	int blockXNumber = 100;
	int blockZNumber = 100;
	for (auto index : choosenIndexes)
	{
		pVtx[index].Color = Color::Red;

		//法線計算
		int countZ = index / (blockZNumber + 1);
		int countX = index - countZ * (blockZNumber + 1);
		Vector3 normal;
		Vector3 positionThis = pVtx[index].Position;
		Vector3 positionLeft;
		Vector3 positionRight;
		Vector3 positionTop;
		Vector3 positionBottom;

		if (countX == 0)
		{
			positionLeft = positionThis;
			positionRight = pVtx[countZ * (blockXNumber + 1) + countX + 1].Position;
		}
		else if (countX < blockXNumber)
		{
			positionLeft = pVtx[countZ * (blockXNumber + 1) + countX - 1].Position;
			positionRight = pVtx[countZ * (blockXNumber + 1) + countX + 1].Position;
		}
		else
		{
			positionLeft = pVtx[countZ * (blockXNumber + 1) + countX - 1].Position;
			positionRight = positionThis;
		}

		if (countZ == 0)
		{
			positionTop = positionThis;
			positionBottom = pVtx[(countZ + 1) * (blockXNumber + 1) + countX].Position;
		}
		else if (countZ < blockZNumber)
		{
			positionTop = pVtx[(countZ - 1) * (blockXNumber + 1) + countX].Position;
			positionBottom = pVtx[(countZ + 1) * (blockXNumber + 1) + countX].Position;
		}
		else
		{
			positionTop = pVtx[(countZ - 1) * (blockXNumber + 1) + countX].Position;
			positionBottom = positionThis;
		}
		normal = (positionRight - positionLeft) * (positionBottom - positionTop);
		pVtx[index].Normal = normal.Normalized();
	}

	//仮想アドレス解放
	mesh->m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//	関数名：SaveEditorFieldAs
//  関数説明：エディタ用フィールドデータの保存
//	引数：	fileName：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void MeshManager::SaveEditorFieldAs(const string& fileName)
{
#ifdef USING_DIRECTX
	auto mesh = GetMeshBy("field");

	//file open
	string filePath = "data/MESH/" + fileName + ".mesh";
	FILE *filePointer;
	fopen_s(&filePointer, filePath.c_str(), "wb");

	//DrawType
	int drawType = (int)TriangleStrip;
	fwrite(&drawType, sizeof(int), 1, filePointer);

	//NumVtx
	fwrite(&mesh->VertexNumber, sizeof(int), 1, filePointer);

	//NumIdx
	fwrite(&mesh->IndexNumber, sizeof(int), 1, filePointer);

	//NumPolygon
	fwrite(&mesh->PolygonNumber, sizeof(int), 1, filePointer);

	//Vtx&Idx
	//頂点データ
	VERTEX_3D* pVtx;
	mesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//色を白に戻す
	unsigned long white = Color::White;
	for (int count = 0; count < mesh->VertexNumber; ++count)
	{
		pVtx[count].Color = white;
	}

	//保存
	fwrite(pVtx, sizeof(VERTEX_3D), mesh->VertexNumber, filePointer);
	mesh->m_pVtxBuffer->Unlock();
	
	//インデックス
	WORD* pIdx;
	mesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), mesh->IndexNumber, filePointer);
	mesh->m_pIdxBuffer->Unlock();
	
	//Texture
	int nTexSize = 0;
	fwrite(&nTexSize, sizeof(int), 1, filePointer);

	//RenderPriority
	auto rp = RP_3D;
	fwrite(&rp, sizeof(rp), 1, filePointer);

	//RenderState
	auto rs = RS_LIGHTON_CULLFACEON_MUL;
	fwrite(&rs, sizeof(rs), 1, filePointer);

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
	for (auto itr = meshes.begin(); itr != meshes.end();)
	{
		SAFE_RELEASE(itr->second.CurrentMesh->m_pVtxBuffer);
		SAFE_RELEASE(itr->second.CurrentMesh->m_pIdxBuffer);
		itr = meshes.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//	関数名：loadFromMesh
//  関数説明：メッシュファイルからデータの読込
//	引数：	filePath：ファイルの名前 
//	戻り値：MESH
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::loadFromMesh(const string& filePath)
{
	string filePath = "data/MESH/" + filePath;
	MeshInfo info;
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "rb");

	if (!filePointer) 
	{
		assert("failed to open file!!");
		return info;
	}
	
	info.CurrentMesh = new Mesh;

	//DrawType
	fread_s(&info.CurrentMesh->CurrentType, sizeof(int), sizeof(int), 1, filePointer);

	//NumVtx
	fread_s(&info.CurrentMesh->VertexNumber, sizeof(int), sizeof(int), 1, filePointer);

	//NumIdx
	fread_s(&info.CurrentMesh->IndexNumber, sizeof(int), sizeof(int), 1, filePointer);

	//NumPolygon
	fread_s(&info.CurrentMesh->PolygonNumber, sizeof(int), sizeof(int), 1, filePointer);

#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	//頂点データ
	VERTEX_3D* pVtx;
	info.CurrentMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fread_s(pVtx, sizeof(VERTEX_3D) * info.CurrentMesh->VertexNumber, sizeof(VERTEX_3D), info.CurrentMesh->VertexNumber, filePointer);
	info.CurrentMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	info.CurrentMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fread_s(pIdx, sizeof(WORD) * info.CurrentMesh->IndexNumber, sizeof(WORD), info.CurrentMesh->IndexNumber, filePointer);
	info.CurrentMesh->m_pIdxBuffer->Unlock();
#endif

	//Texture
	int stringSize;
	fread_s(&stringSize, sizeof(int), sizeof(int), 1, filePointer);
	info.CurrentRenderInfo.TextureName.resize(stringSize);
	fread_s(&info.CurrentRenderInfo.TextureName[0], stringSize, sizeof(char), stringSize, filePointer);

	//Render Priority
	fread_s(&info.CurrentRenderInfo.CurrentPriority, sizeof(RenderPriority), sizeof(RenderPriority), 1, filePointer);

	//Render State
	fread_s(&info.CurrentRenderInfo.CurrentState, sizeof(RenderState), sizeof(RenderState), 1, filePointer);

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

	info.CurrentMesh = new Mesh;
	info.CurrentMesh->CurrentType = DrawType::TriangleList;
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
			ss >> info.CurrentMesh->PolygonNumber;

			//インデックスの読込
			info.CurrentMesh->VertexNumber =
				info.CurrentMesh->IndexNumber = info.CurrentMesh->PolygonNumber * 3;
			vertexIndexes.resize(info.CurrentMesh->IndexNumber);
			for (int count = 0; count < info.CurrentMesh->PolygonNumber; ++count)
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
			if (normalIndexNumber != info.CurrentMesh->PolygonNumber)
			{//エラー
				MessageBox(NULL,"MeshManager:法線数と頂点数が違う！", "エラー", MB_OK | MB_ICONWARNING);
			}

			//法線インデックスの読込
			normalIndexes.resize(normalIndexNumber * 3);
			for (int count = 0; count < info.CurrentMesh->PolygonNumber; ++count)
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
	
#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	info.CurrentMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		pVtx[count].Position = vertexes[vertexIndexes[count]];
		pVtx[count].Normal = normals[normalIndexes[count]];
		pVtx[count].UV = uvs[vertexIndexes[count]];
		pVtx[count].Color = colors[colorIndexes[count]];
	}

	//仮想アドレス解放
	info.CurrentMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	info.CurrentMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		pIdx[count] = count;
	}

	info.CurrentMesh->m_pIdxBuffer->Unlock();
#endif

	return info;
}

//--------------------------------------------------------------------------------
//  Cubeの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::createCube(void)
{
	MeshInfo info;
	info.CurrentMesh = new Mesh;
	info.CurrentMesh->VertexNumber = 6 * 4;
	info.CurrentMesh->IndexNumber = 6 * 4 + 5 * 2;
	info.CurrentMesh->PolygonNumber = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	// 仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する
	info.CurrentMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto& halfSize = Vector3::One * 0.5f;
	unsigned long white = Color::White;
	int countVertex = 0;

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			-halfSize.Z);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Back;
		++countVertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			halfSize.Y,
			halfSize.Z - (count / 2) * halfSize.Z * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Up;
		++countVertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-halfSize.X,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			halfSize.Z - (count % 2) * halfSize.Z * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Left;

		++countVertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-halfSize.X + (count % 2) * halfSize.X * 2.0f,
			-halfSize.Y,
			-halfSize.Z + (count / 2) * halfSize.Z * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Down;
		++countVertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			halfSize.X,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			-halfSize.Z + (count % 2) * halfSize.Z * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Right;
		++countVertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			halfSize.X - (count % 2) * halfSize.X * 2.0f,
			halfSize.Y - (count / 2) * halfSize.Y * 2.0f,
			halfSize.Z);
		pVtx[countVertex].UV = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Forward;
		++countVertex;
	}

	// 仮想アドレス解放
	info.CurrentMesh->m_pVtxBuffer->Unlock();

	// インデックス
	WORD *pIdx;
	info.CurrentMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		if (count % 6 < 4)
		{
			pIdx[count] = (WORD)((count / 6) * 4 + (count % 6) % 4);
		}
		else
		{// 縮退
			pIdx[count] = (WORD)((count / 6) * 4 + (count % 2) + 3);
		}
	}

	info.CurrentMesh->m_pIdxBuffer->Unlock();

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
	int drawType = (int)info.CurrentMesh->CurrentType;
	fwrite(&drawType, sizeof(int), 1, filePointer);

	// NumVtx
	fwrite(&info.CurrentMesh->VertexNumber, sizeof(int), 1, filePointer);

	// NumIdx
	fwrite(&info.CurrentMesh->IndexNumber, sizeof(int), 1, filePointer);

	// NumPolygon
	fwrite(&info.CurrentMesh->PolygonNumber, sizeof(int), 1, filePointer);

	// Vtx&Idx
	info.CurrentMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fwrite(pVtx, sizeof(VERTEX_3D), info.CurrentMesh->VertexNumber, filePointer);
	info.CurrentMesh->m_pVtxBuffer->Unlock();
	info.CurrentMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), info.CurrentMesh->IndexNumber, filePointer);
	info.CurrentMesh->m_pIdxBuffer->Unlock();

	// Texture
	textureName = "nomal_cube.jpg";
	stringSize = textureName.size();
	fwrite(&stringSize, sizeof(int), 1, filePointer);
	fwrite(&textureName[0], sizeof(char), stringSize, filePointer);

	// RenderPriority
	auto rp = RP_3D;
	fwrite(&rp, sizeof(rp), 1, filePointer);

	// RenderState
	auto rs = RS_LIGHTON_CULLFACEON_MUL;
	fwrite(&rs, sizeof(rs), 1, filePointer);

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
	info.CurrentMesh = new Mesh;
	info.CurrentMesh->VertexNumber = 6 * 4;
	info.CurrentMesh->IndexNumber = 6 * 4 + 5 * 2;
	info.CurrentMesh->PolygonNumber = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	// 仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する
	info.CurrentMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	float length = (float)Camera::DEFAULT_FAR * 0.5f;
	unsigned long white = Color::White;
	int countVertex = 0;
	float uvTweens = 1.0f / 1024.0f;	//隙間を無くすためにUVを1px縮める

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			-length);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Forward;
		++countVertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			length,
			length - (count / 2) * length * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Down;
		++countVertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-length,
			length - (count / 2) * length * 2.0f,
			-length + (count % 2) * length * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + 0.5f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Right;
		++countVertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			length - (count % 2) * length * 2.0f,
			-length,
			-length + (count / 2) * length * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 2.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Up;
		++countVertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			length,
			length - (count / 2) * length * 2.0f,
			length - (count % 2) * length * 2.0f);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Left;
		++countVertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		pVtx[countVertex].Position = Vector3(
			-length + (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			length);
		pVtx[countVertex].UV = Vector2((count % 2) * 0.25f + 0.75f + uvTweens - (count % 2) * uvTweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uvTweens - (count / 2) * uvTweens * 2.0f);
		pVtx[countVertex].Color = white;
		pVtx[countVertex].Normal = Vector3::Back;
		++countVertex;
	}

	//仮想アドレス解放
	info.CurrentMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	info.CurrentMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < 6 * 4 + 5 * 2; ++count)
	{
		if (count % 6 < 4)
		{
			pIdx[count] = (count / 6) * 4 + (count % 6) % 4;
		}
		else
		{//縮退
			pIdx[count] = (count / 6) * 4 + (count % 2) + 3;
		}
	}

	info.CurrentMesh->m_pIdxBuffer->Unlock();
#endif
	
	//Render State
	info.CurrentRenderInfo.CurrentState = RS_LIGHTOFF_CULLFACEON_MUL;

	return info;
}

//--------------------------------------------------------------------------------
//  バッファの作成
//--------------------------------------------------------------------------------
bool MeshManager::createBuffer(Mesh* mesh)
{
#ifdef USING_DIRECTX
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * mesh->VertexNumber,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
		FVF_VERTEX_3D,							//書かなくても大丈夫
		D3DPOOL_MANAGED,						//メモリ管理方法(managed：デバイスにお任せ)
		&mesh->m_pVtxBuffer,					//頂点バッファのポインタ
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//インデックスバッファの作成
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * mesh->IndexNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->m_pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateIndexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}