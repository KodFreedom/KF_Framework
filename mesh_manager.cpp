//--------------------------------------------------------------------------------
//　mesh_manager.cpp
//	メッシュ管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_manager.h"
#include "kf_utility.h"
#include "main_system.h"
#include "renderer_manager.h"
#include "camera.h"
#include "camera_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  与えられた名前のメッシュを使う
//--------------------------------------------------------------------------------
void MeshManager::Use(const String& mesh_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() != iterator)
	{// すでに存在してる
		++iterator->second.user_number;
		return;
	}

	// メッシュの作成
	MeshInfo info;
	if (mesh_name.find(L".mesh") != String::npos) info = LoadFromMesh(mesh_name);
	else if (mesh_name.find(L".skin") != String::npos) info = LoadFromSkin(mesh_name);
	else if (mesh_name.find(L".x") != String::npos) info = LoadFromXFile(mesh_name);
	else if (mesh_name._Equal(L"cube")) info = CreateCube();
	else if (mesh_name._Equal(L"sphere")) info = CreateSphere();
	else if (mesh_name._Equal(L"skyBox")) info = CreateSkyBox();
    else if (mesh_name._Equal(L"polygon2d")) info = CreatePolygon2d();
	else
	{
		//throw::runtime_error("unsupport file type!!");
		return;
	}
	meshes_.emplace(mesh_name, info);
}

//--------------------------------------------------------------------------------
//  与えられた名前のメッシュを使う
//--------------------------------------------------------------------------------
void MeshManager::Use(const String& mesh_name, const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number)
{
	auto iterator = meshes_.find(mesh_name);
	if (iterator != meshes_.end())
	{// すでに存在してる
		++iterator->second.user_number;
		return;
	}

	// メッシュの作成
	MeshInfo info = CreateMesh(type, vertexes, indexes, polygon_number);
	meshes_.emplace(mesh_name, info);
}

//--------------------------------------------------------------------------------
//  与えられた名前のメッシュを使わない
//--------------------------------------------------------------------------------
void MeshManager::Disuse(const String& mesh_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;
	if (--iterator->second.user_number <= 0)
	{// 誰も使ってないので破棄する
		SAFE_DELETE(iterator->second.pointer);
		meshes_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  与えられた名前のメッシュを与えられた頂点とインデックスで更新する
//--------------------------------------------------------------------------------
void MeshManager::Update(const String& mesh_name, const vector<Vertex3d>& vertexes, const list<int>& indexes)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	Vertex3d* vertex_pointer;
	iterator->second.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	for (auto index : indexes)
	{
		vertex_pointer[index] = vertexes[index];
	}
	iterator->second.pointer->vertex_buffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  与えられた名前のメッシュをファイルに書き出す
//--------------------------------------------------------------------------------
void MeshManager::SaveMeshToFile(const String& mesh_name, const String& file_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;

	auto pointer = iterator->second.pointer;

	String& path = L"data/mesh/" + file_name + L".mesh";
	ofstream file(path, ios::binary);
	if (!file.is_open()) return;
	BinaryOutputArchive archive(file);

	archive.saveBinary(&pointer->draw_type, sizeof(pointer->draw_type));
	archive.saveBinary(&pointer->vertex_number, sizeof(pointer->vertex_number));
	archive.saveBinary(&pointer->index_number, sizeof(pointer->index_number));
	archive.saveBinary(&pointer->polygon_number, sizeof(pointer->polygon_number));

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//頂点データ
	Vertex3d* vertex_pointer;
	pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	archive.saveBinary(vertex_pointer, sizeof(Vertex3d) * pointer->vertex_number);
	pointer->vertex_buffer->Unlock();
	
	//インデックス
	WORD* index_pointer;
	pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	archive.saveBinary(index_pointer, sizeof(WORD) * pointer->index_number);
	pointer->index_buffer->Unlock();
#endif
	file.close();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MeshManager::Uninit(void)
{
	for (auto iterator = meshes_.begin(); iterator != meshes_.end();)
	{
		SAFE_DELETE(iterator->second.pointer);
		iterator = meshes_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  meshファイルからデータを読み込む
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadFromMesh(const String& mesh_name)
{
	MeshInfo info;
	String& path = L"data/mesh/" + mesh_name;
	ifstream file(path, ios::binary);
	if (!file.is_open())
	{
		assert(file.is_open());
		return info;
	}
	BinaryInputArchive archive(file);

	info.pointer = MY_NEW Mesh;
	info.pointer->type = k3dMesh;
	archive.loadBinary(&info.pointer->draw_type, sizeof(info.pointer->draw_type));
	archive.loadBinary(&info.pointer->vertex_number, sizeof(info.pointer->vertex_number));
	archive.loadBinary(&info.pointer->index_number, sizeof(info.pointer->index_number));
	archive.loadBinary(&info.pointer->polygon_number, sizeof(info.pointer->polygon_number));

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		file.close();
		return info;
	}

	//頂点データ
	Vertex3d* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	archive.loadBinary(vertex_pointer, sizeof(Vertex3d) * info.pointer->vertex_number);
	info.pointer->vertex_buffer->Unlock();

	//インデックス
	WORD* index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	archive.loadBinary(index_pointer, sizeof(WORD) * info.pointer->index_number);
	info.pointer->index_buffer->Unlock();
#endif
	file.close();
	return info;
}

//--------------------------------------------------------------------------------
//  skinファイルからデータを読み込む
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadFromSkin(const String& skin_name)
{
	MeshInfo info;
	String& path = L"data/skin/" + skin_name;
	ifstream file(path, ios::binary);
	if (!file.is_open())
	{
		assert(file.is_open());
		return info;
	}
	BinaryInputArchive archive(file);

	info.pointer = MY_NEW Mesh;
	info.pointer->type = k3dSkin;
	archive.loadBinary(&info.pointer->draw_type, sizeof(info.pointer->draw_type));
	archive.loadBinary(&info.pointer->vertex_number, sizeof(info.pointer->vertex_number));
	archive.loadBinary(&info.pointer->index_number, sizeof(info.pointer->index_number));
	archive.loadBinary(&info.pointer->polygon_number, sizeof(info.pointer->polygon_number));

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer3dSkin(info.pointer))
	{
		assert(info.pointer);
		file.close();
		return info;
	}

	//頂点データ
	Vertex3dSkin* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	archive.loadBinary(vertex_pointer, sizeof(Vertex3dSkin) * info.pointer->vertex_number);
	info.pointer->vertex_buffer->Unlock();

	//インデックス
	WORD* index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	archive.loadBinary(index_pointer, sizeof(WORD) * info.pointer->index_number);
	info.pointer->index_buffer->Unlock();
#endif
	file.close();
	return info;
}

//--------------------------------------------------------------------------------
//  xファイルからデータを読み込む
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadFromXFile(const String& mesh_name)
{
	MeshInfo info;
	FILE* filepointer;
	string path = string(mesh_name.begin(), mesh_name.end());
	fopen_s(&filepointer, path.c_str(), "r");
	if (!filepointer)
	{
		assert(filepointer);
		return info;
	}

	info.pointer = MY_NEW Mesh;
	info.pointer->draw_type = DrawType::kTriangleList;
	vector<Vector3>	vertexes;
	vector<Vector3>	normals;
	vector<Vector2>	uvs;
	vector<Color>	colors;
	vector<int>		vertex_indexes;
	vector<int>		normal_indexes;
	vector<int>		color_indexes;

	string buffer;
	while (Utility::GetStringUntilToken(filepointer, "\n", buffer) >= 0)
	{
		if (buffer.compare("Mesh {") == 0)
		{
			//頂点数の読込
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			stringstream ss;
			int vertex_number;
			ss << buffer;
			ss >> vertex_number;

			//頂点データの読込
			vertexes.resize(vertex_number);
			for (int count = 0; count < vertex_number; ++count)
			{
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].x_;
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].y_;
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertexes[count].z_;
				Utility::GetStringUntilToken(filepointer, "\n", buffer);
			}

			//ポリゴン数の読込
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			ss.clear();
			ss << buffer;
			ss >> info.pointer->polygon_number;

			//インデックスの読込
			info.pointer->vertex_number =
				info.pointer->index_number = info.pointer->polygon_number * 3;
			vertex_indexes.resize(info.pointer->index_number);
			for (int count = 0; count < info.pointer->polygon_number; ++count)
			{
				//3;を飛ばす
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				Utility::GetStringUntilToken(filepointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertex_indexes[count * 3];
				Utility::GetStringUntilToken(filepointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertex_indexes[count * 3 + 1];
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> vertex_indexes[count * 3 + 2];
				Utility::GetStringUntilToken(filepointer, "\n", buffer);
			}
		}

		if (buffer.compare(" MeshMaterialList {") == 0)
		{
			//Material数
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			stringstream ss;
			int color_number;
			ss << buffer;
			ss >> color_number;
			colors.resize(color_number);

			//インデックス数
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			ss.clear();
			int polygon_number;
			ss << buffer;
			ss >> polygon_number;
			color_indexes.resize(polygon_number * 3);

			//インデックスの読み込み
			for (int count = 0; count < polygon_number; ++count)
			{
				Utility::GetStringUntilToken(filepointer, ",;", buffer);
				ss.clear();
				ss << buffer;
				ss >> color_indexes[count * 3];
				color_indexes[count * 3 + 1] =
					color_indexes[count * 3 + 2] =
					color_indexes[count * 3];
			}

			//マテリアルの読み込み
			for (int count = 0; count < color_number; ++count)
			{
				while (Utility::GetStringUntilToken(filepointer, "\n", buffer) >= 0)
				{
					if (buffer.compare("  Material {") == 0)
					{
						//R
						Utility::GetStringUntilToken(filepointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].r_;

						//G
						Utility::GetStringUntilToken(filepointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].g_;

						//B
						Utility::GetStringUntilToken(filepointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].b_;

						//A
						Utility::GetStringUntilToken(filepointer, ";", buffer);
						ss.clear();
						ss << buffer;
						ss >> colors[count].a_;
						break;
					}
				}
			}
		}

		if (buffer.compare(" MeshNormals {") == 0)
		{
			//法線数の読込
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			stringstream ss;
			int normalNumber;
			ss << buffer;
			ss >> normalNumber;

			//法線データの読込
			normals.resize(normalNumber);
			for (int count = 0; count < normalNumber; ++count)
			{
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].x_;
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].y_;
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normals[count].z_;
				Utility::GetStringUntilToken(filepointer, "\n", buffer);
			}

			//法線インデックス数を読込(頂点数と同じのため)
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			ss.clear();
			int normal_index_number;
			ss << buffer;
			ss >> normal_index_number;
			if (normal_index_number != info.pointer->polygon_number)
			{//エラー
				MessageBox(NULL, L"MeshManager:法線数と頂点数が違う！", L"エラー", MB_OK | MB_ICONWARNING);
			}

			//法線インデックスの読込
			normal_indexes.resize(normal_index_number * 3);
			for (int count = 0; count < info.pointer->polygon_number; ++count)
			{
				//3;を飛ばす
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				Utility::GetStringUntilToken(filepointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> normal_indexes[count * 3];
				Utility::GetStringUntilToken(filepointer, ",", buffer);
				ss.clear();
				ss << buffer;
				ss >> normal_indexes[count * 3 + 1];
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> normal_indexes[count * 3 + 2];
				Utility::GetStringUntilToken(filepointer, "\n", buffer);
			}
		}

		if (buffer.compare(" MeshTextureCoords {") == 0)
		{
			//uv数の読込
			Utility::GetStringUntilToken(filepointer, ";", buffer);
			stringstream ss;
			int uv_number;
			ss << buffer;
			ss >> uv_number;
			if (uv_number != vertexes.size())
			{//エラー
				MessageBox(NULL, L"MeshManager:uv数と頂点数が違う！", L"エラー", MB_OK | MB_ICONWARNING);
			}

			//uvデータの読込
			uvs.resize(uv_number);
			for (int count = 0; count < uv_number; ++count)
			{
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> uvs[count].x_;
				Utility::GetStringUntilToken(filepointer, ";", buffer);
				ss.clear();
				ss << buffer;
				ss >> uvs[count].y_;
				Utility::GetStringUntilToken(filepointer, "\n", buffer);
			}
		}
	}
	fclose(filepointer);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		return info;
	}

	Vertex3d* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		vertex_pointer[count].position = vertexes[vertex_indexes[count]];
		vertex_pointer[count].normal = normals[normal_indexes[count]];
		vertex_pointer[count].uv = uvs[vertex_indexes[count]];
		vertex_pointer[count].color = colors[color_indexes[count]];
	}
	info.pointer->vertex_buffer->Unlock();

	WORD* index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		index_pointer[count] = count;
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  Cubeの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateCube(void)
{
	MeshInfo info;
	info.pointer = MY_NEW Mesh;
	info.pointer->vertex_number = 6 * 4;
	info.pointer->index_number = 6 * 4 + 5 * 2;
	info.pointer->polygon_number = 6 * 2 + 5 * 4;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		return info;
	}

	Vertex3d* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	auto& half_size = Vector3::kOne * 0.5f / 0.2f;
	int count_vertex = 0;

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			-half_size.z_);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kBack;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			half_size.y_,
			half_size.z_ - (count / 2) * half_size.z_ * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kUp;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-half_size.x_,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			half_size.z_ - (count % 2) * half_size.z_ * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kLeft;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			-half_size.y_,
			-half_size.z_ + (count / 2) * half_size.z_ * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kDown;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			half_size.x_,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			-half_size.z_ + (count % 2) * half_size.z_ * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kRight;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			half_size.x_ - (count % 2) * half_size.x_ * 2.0f,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			half_size.z_);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertex_pointer[count_vertex].normal = Vector3::kForward;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}
	info.pointer->vertex_buffer->Unlock();

	// インデックス
	WORD *index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);

	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		if (count % 6 < 4)
		{
			index_pointer[count] = (WORD)((count / 6) * 4 + (count % 6) % 4);
		}
		else
		{// 縮退
			index_pointer[count] = (WORD)((count / 6) * 4 + (count % 2) + 3);
		}
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  Sphereの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSphere(void)
{
	MeshInfo info;
	info.pointer = MY_NEW Mesh;
	info.pointer->vertex_number = (kSphereCircleNumber + 1) * (kSphereVertexesPerCircle + 1);
	info.pointer->index_number = kSphereCircleNumber * (kSphereCircleNumber * 2 + 2) + (kSphereVertexesPerCircle - 1) * 2;
	info.pointer->polygon_number = info.pointer->index_number - 2;

	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	Vertex3d* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	int vertex_counter = 0;
	for (int count_circle = 0; count_circle < kSphereCircleNumber + 1; count_circle++)
	{
		for (int count_vertex = 0; count_vertex < kSphereVertexesPerCircle + 1; count_vertex++)
		{
			float angle_xz = count_circle * (D3DX_PI / (kSphereCircleNumber - 1));
			float angle_y = count_vertex * (D3DX_PI * 2.0f / (kSphereVertexesPerCircle - 1));
			vertex_pointer->position = Vector3(
				sinf(angle_xz) * sinf(angle_y),
				cosf(angle_xz),
				sinf(angle_xz) * cosf(angle_y));
			vertex_pointer->normal = vertex_pointer->position.Normalized();
			vertex_pointer->color = Color::kWhite;
			vertex_pointer->uv = Vector2(
				static_cast<float>(count_vertex) / (kSphereVertexesPerCircle - 1),
				static_cast<float>(count_circle) / (kSphereCircleNumber - 1) * 2.0f);
			++vertex_counter;
			++vertex_pointer;
		}
	}
	info.pointer->vertex_buffer->Unlock();

	WORD* index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	for (int count_circle = 0; count_circle < kSphereCircleNumber; count_circle++)
	{
		if (count_circle != 0)
		{
			*index_pointer = (kSphereVertexesPerCircle + 1) * (count_circle + 1);
			++index_pointer;
		}
		for (int count_vertex = 0; count_vertex < kSphereVertexesPerCircle + 1; count_vertex++)
		{
			*index_pointer = (kSphereVertexesPerCircle + 1) * (count_circle + 1) + count_vertex;
			++index_pointer;
			*index_pointer = (kSphereVertexesPerCircle + 1) * count_circle + count_vertex;
			++index_pointer;
		}
		if (count_circle + 1 != kSphereCircleNumber)
		{
			*index_pointer = *(index_pointer - 1);
			++index_pointer;
		}
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  SkyBoxの作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSkyBox(void)
{
	MeshInfo info;
	info.pointer = MY_NEW Mesh;
	info.pointer->vertex_number = 6 * 4;
	info.pointer->index_number = 6 * 4 + 5 * 2;
	info.pointer->polygon_number = 6 * 2 + 5 * 4;

	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	Vertex3d *vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
	float length = camera ? camera->GetFar() * 0.5f : 500.0f;
	int count_vertex = 0;
	float uv_tweens = 1.0f / 1024.0f; //隙間を無くすためにuvを1px縮める

	// 後ろ
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			-length);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kForward;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 上
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			length,
			length - (count / 2) * length * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kDown;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 左
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-length,
			length - (count / 2) * length * 2.0f,
			-length + (count % 2) * length * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.5f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kRight;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 下
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			-length,
			-length + (count / 2) * length * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 2.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kUp;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 右
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			length,
			length - (count / 2) * length * 2.0f,
			length - (count % 2) * length * 2.0f);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kLeft;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}

	// 正面
	for (int count = 0; count < 4; ++count)
	{
		vertex_pointer[count_vertex].position = Vector3(
			-length + (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			length);
		vertex_pointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.75f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertex_pointer[count_vertex].normal = Vector3::kBack;
		vertex_pointer[count_vertex].color = Color::kWhite;
		++count_vertex;
	}
	info.pointer->vertex_buffer->Unlock();

	//インデックス
	WORD *index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		if (count % 6 < 4)
		{
			index_pointer[count] = (count / 6) * 4 + (count % 6) % 4;
		}
		else
		{//縮退
			index_pointer[count] = (count / 6) * 4 + (count % 2) + 3;
		}
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  Polygon2dを生成する
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreatePolygon2d(void)
{
    MeshInfo info;
    info.pointer = MY_NEW Mesh;
    info.pointer->type = MeshType::k2dMesh;
    info.pointer->vertex_number = 4;
    info.pointer->index_number = 4;
    info.pointer->polygon_number = 2;

    if (!CreateBuffer2d(info.pointer))
    {
        assert(info.pointer);
        return info;
    }

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    Vertex2d* vertex_pointer;
    info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
    vertex_pointer[0].position = Vector3(-0.5f, -0.5f, 0.0f);
    vertex_pointer[1].position = Vector3( 0.5f, -0.5f, 0.0f);
    vertex_pointer[2].position = Vector3(-0.5f,  0.5f, 0.0f);
    vertex_pointer[3].position = Vector3( 0.5f,  0.5f, 0.0f);
    vertex_pointer[0].color = Color::kWhite;
    vertex_pointer[1].color = Color::kWhite;
    vertex_pointer[2].color = Color::kWhite;
    vertex_pointer[3].color = Color::kWhite;
    vertex_pointer[0].uv = Vector2(0.0f, 0.0f);
    vertex_pointer[1].uv = Vector2(1.0f, 0.0f);
    vertex_pointer[2].uv = Vector2(0.0f, 1.0f);
    vertex_pointer[3].uv = Vector2(1.0f, 1.0f);
    info.pointer->vertex_buffer->Unlock();

    WORD* index_pointer;
    info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
    index_pointer[0] = 0;
    index_pointer[1] = 1;
    index_pointer[2] = 2;
    index_pointer[3] = 3;
    info.pointer->index_buffer->Unlock();
#endif
    return info;
}

//--------------------------------------------------------------------------------
//  与えられた頂点とインデックスでメッシュを生成する
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateMesh(const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number)
{
	MeshInfo info;
	info.pointer = MY_NEW Mesh;
	info.pointer->type = MeshType::k3dMesh;
	info.pointer->draw_type = type;
	info.pointer->vertex_number = (int)vertexes.size();
	info.pointer->index_number = (int)indexes.size();
	if (polygon_number == -1)
	{
		info.pointer->polygon_number = info.pointer->index_number / GetVertexNumberPerPolygon(type);
	}
	else
	{
		info.pointer->polygon_number = polygon_number;
	}

	if (!CreateBuffer3d(info.pointer))
	{
		assert(info.pointer);
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//Vertex
	Vertex3d* vertex_pointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertex_pointer, 0);
	memcpy_s(vertex_pointer, sizeof(Vertex3d) * info.pointer->vertex_number
		, &vertexes[0], sizeof(Vertex3d) * vertexes.size());
	info.pointer->vertex_buffer->Unlock();

	//インデックス
	WORD* index_pointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&index_pointer, 0);
	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		index_pointer[count] = static_cast<WORD>(indexes[count]);
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  バーテックスとインデックスバッファの生成
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer2d(Mesh* const mesh) const
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //頂点バッファ
    HRESULT hr = device_->CreateVertexBuffer(
        sizeof(Vertex2d) * mesh->vertex_number,
        D3DUSAGE_WRITEONLY,
        0,
        D3DPOOL_MANAGED,
        &mesh->vertex_buffer,
        NULL);

    if (FAILED(hr))
    {
        MessageBox(NULL, L"MeshManager : CreateBuffer2d ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
        return false;
    }

    //インデックスバッファの作成
    hr = device_->CreateIndexBuffer(
        sizeof(WORD) * mesh->index_number,
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &mesh->index_buffer,
        NULL);

    if (FAILED(hr))
    {
        MessageBox(NULL, L"MeshManager : Createindex_buffer ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
        return false;
    }
#endif
    return true;
}

//--------------------------------------------------------------------------------
//  バーテックスとインデックスバッファの生成
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer3d(Mesh* const mesh) const
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//頂点バッファ
	HRESULT hr = device_->CreateVertexBuffer(
		sizeof(Vertex3d) * mesh->vertex_number,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&mesh->vertex_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : CreateBuffer3d ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//インデックスバッファの作成
	hr = device_->CreateIndexBuffer(
		sizeof(WORD) * mesh->index_number,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->index_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : Createindex_buffer ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  バーテックスとインデックスバッファの生成
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer3dSkin(Mesh* const mesh) const
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//頂点バッファ
	HRESULT hr = device_->CreateVertexBuffer(
		sizeof(Vertex3dSkin) * mesh->vertex_number,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&mesh->vertex_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : CreateBuffer3dSkin ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//インデックスバッファの作成
	hr = device_->CreateIndexBuffer(
		sizeof(WORD) * mesh->index_number,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->index_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : CreateBuffer3dSkin ERROR!!", L"エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  ドロータイプからポリゴンを構成する頂点数を取得する
//--------------------------------------------------------------------------------
int MeshManager::GetVertexNumberPerPolygon(const DrawType& type)
{
	switch (type)
	{
	case DrawType::kPointList:
		return 1;
	case DrawType::kLineList:
		return 2;
	case DrawType::kLineStrip:
		return 2;
	case DrawType::kTriangleList:
		return 3;
	case DrawType::kTriangleStrip:
		return 3;
	case DrawType::kTriangleFan:
		return 3;
	default:
		throw::runtime_error("error type");
		return -1;
	}
}