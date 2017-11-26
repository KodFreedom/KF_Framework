//--------------------------------------------------------------------------------
//�@mesh_manager.cpp
//	���b�V���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "mesh_manager.h"
#include "kf_utility.h"
#include "rendererManager.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��b�V�����g��
//--------------------------------------------------------------------------------
void MeshManager::Use(const String& mesh_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() != iterator)
	{// ���łɑ��݂��Ă�
		++iterator->second.user_number;
		return;
	}

	// ���b�V���̍쐬
	MeshInfo info;
	if (mesh_name.find(L".mesh") != String::npos) info = LoadFromMesh(mesh_name);
	//else if (mesh_name.find(L".x") != String::npos) info = LoadFromXFile(mesh_name);
	else if (mesh_name._Equal(L"cube")) info = CreateCube();
	else if (mesh_name._Equal(L"sphere")) info = CreateSphere();
	else if (mesh_name._Equal(L"skyBox")) info = CreateSkyBox();
	else
	{
		assert("wrong type!!");
		return;
	}
	meshes_.emplace(mesh_name, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��b�V�����g��
//--------------------------------------------------------------------------------
void MeshManager::Use(const String& mesh_name, const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes)
{
	auto iterator = meshes_.find(mesh_name);
	if (iterator != meshes_.end())
	{// ���łɑ��݂��Ă�
		++iterator->second.user_number;
		return;
	}

	// ���b�V���̍쐬
	MeshInfo newMesh = CreateMesh(type, vertexes, indexes);
	meshes_.emplace(mesh_name, newMesh);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��b�V�����g��Ȃ�
//--------------------------------------------------------------------------------
void MeshManager::Disuse(const String& mesh_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;
	if (--iterator->second.user_number <= 0)
	{// �N���g���ĂȂ��̂Ŕj������
		SAFE_DELETE(iterator->second.pointer);
		meshes_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��b�V����^����ꂽ���_�ƃC���f�b�N�X�ōX�V����
//--------------------------------------------------------------------------------
void MeshManager::Update(const String& mesh_name, const vector<Vertex3d>& vertexes, const list<int>& indexes)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	Vertex3d* vertexpointer;
	iterator->second.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	for (auto index : indexes)
	{
		vertexpointer[index] = vertexes[index];
	}
	iterator->second.pointer->vertex_buffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��b�V�����t�@�C���ɏ����o��
//--------------------------------------------------------------------------------
void MeshManager::SaveMeshToFile(const String& mesh_name, const String& file_name)
{
	auto iterator = meshes_.find(mesh_name);
	if (meshes_.end() == iterator) return;

	auto pointer = iterator->second.pointer;

	String& path = L"data/mesh/" + file_name + L".mesh";
	ofstream file(path);
	if (!file.is_open()) return;
	BinaryOutputArchive archive(file);

	archive.saveBinary(&pointer->draw_type, sizeof(pointer->draw_type));
	archive.saveBinary(&pointer->vertex_number, sizeof(pointer->vertex_number));
	archive.saveBinary(&pointer->index_number, sizeof(pointer->index_number));
	archive.saveBinary(&pointer->polygon_number, sizeof(pointer->polygon_number));

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//���_�f�[�^
	Vertex3d* vertexpointer;
	pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	archive.saveBinary(vertexpointer, sizeof(Vertex3d) * pointer->vertex_number);
	pointer->vertex_buffer->Unlock();
	
	//�C���f�b�N�X
	WORD* indexpointer;
	pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);
	archive.saveBinary(indexpointer, sizeof(WORD) * pointer->index_number);
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
//  �I������
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
//  mesh�t�@�C������f�[�^��ǂݍ���
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadFromMesh(const String& mesh_name)
{
	MeshInfo info;
	String& path = L"data/mesh/" + mesh_name;
	ifstream file(path);
	if (!file.is_open())
	{
		assert("failed to open file!!");
		return info;
	}
	BinaryInputArchive archive(file);

	info.pointer = new Mesh;
	info.pointer->type = k3dMesh;
	archive.loadBinary(&info.pointer->draw_type, sizeof(info.pointer->draw_type));
	archive.loadBinary(&info.pointer->vertex_number, sizeof(info.pointer->vertex_number));
	archive.loadBinary(&info.pointer->index_number, sizeof(info.pointer->index_number));
	archive.loadBinary(&info.pointer->polygon_number, sizeof(info.pointer->polygon_number));

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer(info.pointer))
	{
		assert("failed to create buffer!!");
		file.close();
		return info;
	}

	//���_�f�[�^
	Vertex3d* vertexpointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	archive.loadBinary(vertexpointer, sizeof(Vertex3d) * info.pointer->vertex_number);
	info.pointer->vertex_buffer->Unlock();

	//�C���f�b�N�X
	WORD* indexpointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);
	archive.loadBinary(indexpointer, sizeof(WORD) * info.pointer->index_number);
	info.pointer->index_buffer->Unlock();
#endif
	file.close();
	return info;
}

//--------------------------------------------------------------------------------
//  x�t�@�C������f�[�^��ǂݍ���
//--------------------------------------------------------------------------------
/*
MeshManager::MeshInfo MeshManager::LoadFromXFile(const String& mesh_name)
{
MeshInfo info;

//file open
fopen_s(&filepointer, filePath.c_str(), "r");

if (!filepointer)
{
assert("failed to open file!!");
return info;
}

info.pointer = new Mesh;
info.pointer->CurrentType = DrawType::TriangleList;
vector<Vector3>	vertexes;
vector<Vector3>	normals;
vector<Vector2>	uvs;
vector<Color>	colors;
vector<int>		vertexIndexes;
vector<int>		normalIndexes;
vector<int>		colorIndexes;

string buffer;
while (Utility::GetStringUntilToken(filepointer, "\n", buffer) >= 0)
{
if (buffer.compare("Mesh {") == 0)
{
//���_���̓Ǎ�
Utility::GetStringUntilToken(filepointer, ";", buffer);
stringstream ss;
int vertex_number;
ss << buffer;
ss >> vertex_number;

//���_�f�[�^�̓Ǎ�
vertexes.resize(vertex_number);
for (int count = 0; count < vertex_number; ++count)
{
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> vertexes[count].X;
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> vertexes[count].Y;
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> vertexes[count].Z;
Utility::GetStringUntilToken(filepointer, "\n", buffer);
}

//�|���S�����̓Ǎ�
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> info.pointer->polygon_number;

//�C���f�b�N�X�̓Ǎ�
info.pointer->vertex_number =
info.pointer->index_number = info.pointer->polygon_number * 3;
vertexIndexes.resize(info.pointer->index_number);
for (int count = 0; count < info.pointer->polygon_number; ++count)
{
//3;���΂�
Utility::GetStringUntilToken(filepointer, ";", buffer);
Utility::GetStringUntilToken(filepointer, ",", buffer);
ss.clear();
ss << buffer;
ss >> vertexIndexes[count * 3];
Utility::GetStringUntilToken(filepointer, ",", buffer);
ss.clear();
ss << buffer;
ss >> vertexIndexes[count * 3 + 1];
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> vertexIndexes[count * 3 + 2];
Utility::GetStringUntilToken(filepointer, "\n", buffer);
}
}

if (buffer.compare(" MeshMaterialList {") == 0)
{
//Material��
Utility::GetStringUntilToken(filepointer, ";", buffer);
stringstream ss;
int colorNumber;
ss << buffer;
ss >> colorNumber;
colors.resize(colorNumber);

//�C���f�b�N�X��
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
int polygon_number;
ss << buffer;
ss >> polygon_number;
colorIndexes.resize(polygon_number * 3);

//�C���f�b�N�X�̓ǂݍ���
for (int count = 0; count < polygon_number; ++count)
{
Utility::GetStringUntilToken(filepointer, ",;", buffer);
ss.clear();
ss << buffer;
ss >> colorIndexes[count * 3];
colorIndexes[count * 3 + 1] =
colorIndexes[count * 3 + 2] =
colorIndexes[count * 3];
}

//�}�e���A���̓ǂݍ���
for (int count = 0; count < colorNumber; ++count)
{
while (Utility::GetStringUntilToken(filepointer, "\n", buffer) >= 0)
{
if (buffer.compare("  Material {") == 0)
{
//R
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> colors[count].R;

//G
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> colors[count].G;

//B
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> colors[count].B;

//A
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> colors[count].A;

break;
}
}
}
}

if (buffer.compare(" Meshnormals {") == 0)
{
//�@�����̓Ǎ�
Utility::GetStringUntilToken(filepointer, ";", buffer);
stringstream ss;
int normalNumber;
ss << buffer;
ss >> normalNumber;

//�@���f�[�^�̓Ǎ�
normals.resize(normalNumber);
for (int count = 0; count < normalNumber; ++count)
{
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> normals[count].X;
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> normals[count].Y;
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> normals[count].Z;
Utility::GetStringUntilToken(filepointer, "\n", buffer);
}

//�@���C���f�b�N�X����Ǎ�(���_���Ɠ����̂���)
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
int normalindex_number;
ss << buffer;
ss >> normalindex_number;
if (normalindex_number != info.pointer->polygon_number)
{//�G���[
MessageBox(NULL,"MeshManager:�@�����ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
}

//�@���C���f�b�N�X�̓Ǎ�
normalIndexes.resize(normalindex_number * 3);
for (int count = 0; count < info.pointer->polygon_number; ++count)
{
//3;���΂�
Utility::GetStringUntilToken(filepointer, ";", buffer);
Utility::GetStringUntilToken(filepointer, ",", buffer);
ss.clear();
ss << buffer;
ss >> normalIndexes[count * 3];
Utility::GetStringUntilToken(filepointer, ",", buffer);
ss.clear();
ss << buffer;
ss >> normalIndexes[count * 3 + 1];
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> normalIndexes[count * 3 + 2];
Utility::GetStringUntilToken(filepointer, "\n", buffer);
}
}

if (buffer.compare(" MeshTextureCoords {") == 0)
{
//uv���̓Ǎ�
Utility::GetStringUntilToken(filepointer, ";", buffer);
stringstream ss;
int uvNumber;
ss << buffer;
ss >> uvNumber;
if (uvNumber != vertexes.size())
{//�G���[
MessageBox(NULL, "MeshManager:uv���ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
}

//uv�f�[�^�̓Ǎ�
uvs.resize(uvNumber);
for (int count = 0; count < uvNumber; ++count)
{
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> uvs[count].X;
Utility::GetStringUntilToken(filepointer, ";", buffer);
ss.clear();
ss << buffer;
ss >> uvs[count].Y;
Utility::GetStringUntilToken(filepointer, "\n", buffer);
}
}
}

fclose(filepointer);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
if (!createBuffer(info.pointer))
{
assert("failed to create buffer!!");
return info;
}

//���z�A�h���X���擾���邽�߂̃|�C���^
Vertex3d *vertexpointer;

//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);

for (int count = 0; count < info.pointer->index_number; ++count)
{
vertexpointer[count].position = vertexes[vertexIndexes[count]];
vertexpointer[count].normal = normals[normalIndexes[count]];
vertexpointer[count].uv = uvs[vertexIndexes[count]];
vertexpointer[count].Color = colors[colorIndexes[count]];
}

//���z�A�h���X���
info.pointer->vertex_buffer->Unlock();

//�C���f�b�N�X
WORD *indexpointer;
info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);

for (int count = 0; count < info.pointer->index_number; ++count)
{
indexpointer[count] = count;
}

info.pointer->index_buffer->Unlock();
#endif

return info;
}
*/

//--------------------------------------------------------------------------------
//  Cube�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateCube(void)
{
	MeshInfo info;
	info.pointer = new Mesh;
	info.pointer->vertex_number = 6 * 4;
	info.pointer->index_number = 6 * 4 + 5 * 2;
	info.pointer->polygon_number = 6 * 2 + 5 * 4;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (!CreateBuffer(info.pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

	Vertex3d* vertexpointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	auto& half_size = Vector3::kOne * 0.5f;
	int count_vertex = 0;

	// ���
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			-half_size.z_);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kBack;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			half_size.y_,
			half_size.z_ - (count / 2) * half_size.z_ * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kUp;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-half_size.x_,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			half_size.z_ - (count % 2) * half_size.z_ * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kLeft;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-half_size.x_ + (count % 2) * half_size.x_ * 2.0f,
			-half_size.y_,
			-half_size.z_ + (count / 2) * half_size.z_ * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kDown;
		++count_vertex;
	}

	// �E
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			half_size.x_,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			-half_size.z_ + (count % 2) * half_size.z_ * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kRight;
		++count_vertex;
	}

	// ����
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			half_size.x_ - (count % 2) * half_size.x_ * 2.0f,
			half_size.y_ - (count / 2) * half_size.y_ * 2.0f,
			half_size.z_);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 1.0f, (count / 2) * 1.0f);
		vertexpointer[count_vertex].normal = Vector3::kForward;
		++count_vertex;
	}
	info.pointer->vertex_buffer->Unlock();

	// �C���f�b�N�X
	WORD *indexpointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);

	for (int count = 0; count < info.pointer->index_number; ++count)
	{
		if (count % 6 < 4)
		{
			indexpointer[count] = (WORD)((count / 6) * 4 + (count % 6) % 4);
		}
		else
		{// �k��
			indexpointer[count] = (WORD)((count / 6) * 4 + (count % 2) + 3);
		}
	}
	info.pointer->index_buffer->Unlock();

	/*
	// Model�t�@�C���̕ۑ�
	string fileName = "data/MODEL/cube.model";
	FILE *filepointer;

	// file open
	fopen_s(&filepointer, fileName.c_str(), "wb");

	// Node��
	string nodeName = "cube";
	int stringSize = (int)nodeName.size();
	fwrite(&stringSize, sizeof(int), 1, filepointer);
	fwrite(&nodeName[0], sizeof(char), stringSize, filepointer);

	// Offset
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filepointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filepointer);
	fwrite(&Vector3::One, sizeof(Vector3), 1, filepointer);

	// Collider
	int colliderNumber = 1;
	fwrite(&colliderNumber, sizeof(int), 1, filepointer);

	int type = 1;
	fwrite(&type, sizeof(int), 1, filepointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filepointer);
	fwrite(&Vector3::Zero, sizeof(Vector3), 1, filepointer);
	fwrite(&Vector3::One, sizeof(Vector3), 1, filepointer);

	// Texture
	int textureNumber = 1;
	fwrite(&textureNumber, sizeof(int), 1, filepointer);
	string textureName = "nomal_cube.jpg";
	stringSize = textureName.size();
	fwrite(&stringSize, sizeof(int), 1, filepointer);
	fwrite(&textureName[0], sizeof(char), stringSize, filepointer);

	// Mesh
	int meshNumber = 1;
	fwrite(&meshNumber, sizeof(int), 1, filepointer);
	string mesh_name = nodeName + ".mesh";
	stringSize = (int)mesh_name.size();
	fwrite(&stringSize, sizeof(int), 1, filepointer);
	fwrite(&mesh_name[0], sizeof(char), stringSize, filepointer);

	fclose(filepointer);

	// Mesh
	fileName = "data/MESH/cube.mesh";

	// file open
	fopen_s(&filepointer, fileName.c_str(), "wb");

	// DrawType
	int drawType = (int)info.pointer->CurrentType;
	fwrite(&drawType, sizeof(int), 1, filepointer);

	// NumVtx
	fwrite(&info.pointer->vertex_number, sizeof(int), 1, filepointer);

	// NumIdx
	fwrite(&info.pointer->index_number, sizeof(int), 1, filepointer);

	// NumPolygon
	fwrite(&info.pointer->polygon_number, sizeof(int), 1, filepointer);

	// Vtx&Idx
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	fwrite(vertexpointer, sizeof(Vertex3d), info.pointer->vertex_number, filepointer);
	info.pointer->vertex_buffer->Unlock();
	info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);
	fwrite(indexpointer, sizeof(WORD), info.pointer->index_number, filepointer);
	info.pointer->index_buffer->Unlock();

	// Texture
	textureName = "nomal_cube.jpg";
	stringSize = textureName.size();
	fwrite(&stringSize, sizeof(int), 1, filepointer);
	fwrite(&textureName[0], sizeof(char), stringSize, filepointer);

	//Lighting
	auto lighting = Lighting_On;
	fwrite(&lighting, sizeof(lighting), 1, filepointer);

	//CullMode
	auto cullMode = Cull_CCW;
	fwrite(&cullMode, sizeof(cullMode), 1, filepointer);

	//Synthesis
	auto synthesis = S_Multiplication;
	fwrite(&synthesis, sizeof(synthesis), 1, filepointer);

	//FillMode
	auto fillMode = Fill_Solid;
	fwrite(&fillMode, sizeof(fillMode), 1, filepointer);

	//Alpha
	auto alpha = A_None;
	fwrite(&alpha, sizeof(alpha), 1, filepointer);

	//Fog
	auto fog = Fog_On;
	fwrite(&fog, sizeof(fog), 1, filepointer);

	fclose(filepointer);
	*/
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  Sphere�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSphere(void)
{
	MeshInfo info;
	assert("������!!");
	return info;
}

//--------------------------------------------------------------------------------
//  SkyBox�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSkyBox(void)
{
	MeshInfo info;
	info.pointer = new Mesh;
	info.pointer->vertex_number = 6 * 4;
	info.pointer->index_number = 6 * 4 + 5 * 2;
	info.pointer->polygon_number = 6 * 2 + 5 * 4;

	if (!CreateBuffer(info.pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	Vertex3d *vertexpointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	auto camera = CameraManager::Instance()->GetMainCamera();
	float length = camera ? (float)camera->GetFar() * 0.5f : 500.0f;
	int count_vertex = 0;
	float uv_tweens = 1.0f / 1024.0f; //���Ԃ𖳂������߂�uv��1px�k�߂�

	// ���
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			-length);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kForward;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			length,
			length - (count / 2) * length * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kDown;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-length,
			length - (count / 2) * length * 2.0f,
			-length + (count % 2) * length * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.5f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kRight;
		++count_vertex;
	}

	// ��
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			length - (count % 2) * length * 2.0f,
			-length,
			-length + (count / 2) * length * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 2.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kUp;
		++count_vertex;
	}

	// �E
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			length,
			length - (count / 2) * length * 2.0f,
			length - (count % 2) * length * 2.0f);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kLeft;
		++count_vertex;
	}

	// ����
	for (int count = 0; count < 4; ++count)
	{
		vertexpointer[count_vertex].position = Vector3(
			-length + (count % 2) * length * 2.0f,
			length - (count / 2) * length * 2.0f,
			length);
		vertexpointer[count_vertex].uv = Vector2((count % 2) * 0.25f + 0.75f + uv_tweens - (count % 2) * uv_tweens * 2.0f,
			(count / 2) * 1.0f / 3.0f + 1.0f / 3.0f + uv_tweens - (count / 2) * uv_tweens * 2.0f);
		vertexpointer[count_vertex].normal = Vector3::kBack;
		++count_vertex;
	}
	info.pointer->vertex_buffer->Unlock();

	//�C���f�b�N�X
	WORD *indexpointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);
	for (int count = 0; count < 6 * 4 + 5 * 2; ++count)
	{
		if (count % 6 < 4)
		{
			indexpointer[count] = (count / 6) * 4 + (count % 6) % 4;
		}
		else
		{//�k��
			indexpointer[count] = (count / 6) * 4 + (count % 2) + 3;
		}
	}
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���_�ƃC���f�b�N�X�Ń��b�V���𐶐�����
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateMesh(const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes)
{
	MeshInfo info;
	info.pointer = new Mesh;
	info.pointer->vertex_number = (int)vertexes.size();
	info.pointer->index_number = (int)indexes.size();
	info.pointer->polygon_number = info.pointer->index_number / GetVertexNumberPerPolygon(type);

	if (!CreateBuffer(info.pointer))
	{
		assert("failed to create buffer!!");
		return info;
	}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//Vertex
	Vertex3d* vertexpointer;
	info.pointer->vertex_buffer->Lock(0, 0, (void**)&vertexpointer, 0);
	memcpy_s(vertexpointer, info.pointer->vertex_number, &vertexes.front(), vertexes.size());
	info.pointer->vertex_buffer->Unlock();

	//�C���f�b�N�X
	WORD* indexpointer;
	info.pointer->index_buffer->Lock(0, 0, (void**)&indexpointer, 0);
	memcpy_s(indexpointer, info.pointer->index_number, &indexes.front(), indexes.size());
	info.pointer->index_buffer->Unlock();
#endif
	return info;
}

//--------------------------------------------------------------------------------
//  �o�[�e�b�N�X�ƃC���f�b�N�X�o�b�t�@�̐���
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer(Mesh* mesh)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//���_�o�b�t�@
	HRESULT hr = device_->CreateVertexBuffer(
		sizeof(Vertex3d) * mesh->vertex_number,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&mesh->vertex_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : CreateBuffer ERROR!!", L"�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = device_->CreateIndexBuffer(
		sizeof(WORD) * mesh->index_number,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->index_buffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"MeshManager : Createindex_buffer ERROR!!", L"�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  �h���[�^�C�v����|���S�����\�����钸�_�����擾����
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
		return 0;
	}
}