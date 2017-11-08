//--------------------------------------------------------------------------------
//	���b�V���}�l�[�W��
//�@meshManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "meshManager.h"
#include "meshInfo.h"
#include "KF_Utility.h"
#include "renderManager.h"
#include "renderer.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FUse
//  �֐������F���b�V���̒ǉ�
//	�����F	meshName�F���b�V���̖��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MeshManager::Use(const string& meshName)
{
	auto itr = meshes.find(meshName);
	if (itr != meshes.end()) 
	{// ���łɑ��݂��Ă�
		++itr->second.UserNumber;
		return;
	}

	// ���b�V���̍쐬
	MeshStruct meshStruct;
	if (meshName.find(".mesh") != string::npos) meshStruct = loadFromMesh(meshName);
	else if (meshName.find(".x") != string::npos) meshStruct = loadFromXFile(meshName);
	else if (meshName._Equal("cube")) meshStruct = createCube();
	else if (meshName._Equal("sphere")) meshStruct = createSphere();
	else if (meshName._Equal("skyBox")) meshStruct = createSkyBox();
	else
	{
		assert("wrong type!!");
		return;
	}
	meshes.emplace(meshName, meshStruct);
}

//--------------------------------------------------------------------------------
//	�֐����FDisuse
//  �֐������F���b�V���̔j��
//	�����F	meshName�F���b�V���̖��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MeshManager::Disuse(const string& meshName)
{
	auto itr = meshes.find(meshName);
	--itr->second.UserNumber;
	if (itr->second.UserNumber == 0)
	{// �N���g���ĂȂ��̂Ŕj������
		delete itr->second.CurrentMesh;
		meshes.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FCreateEditorField
//  �֐������F�G�f�B�^�p�t�B�[���h�̍쐬
//	�����F	blockXNumber�FX�����̃u���b�N��
//			blockZNumber�FZ�����̃u���b�N��
//			blockSize�F�u���b�N�̃T�C�Y
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MeshManager::CreateEditorField(const int blockXNumber, const int blockZNumber, const Vector2& blockSize)
{
	auto itr = meshes.find("field");
	if (itr != meshes.end())
	{ //���łɑ��݂��Ă�
		return;
	}

	auto mesh = new MeshInfo;
	mesh->VertexNumber = (blockXNumber + 1) * (blockZNumber + 1);
	mesh->IndexNumber = ((blockXNumber + 1) * 2 + 2) * blockZNumber - 1;
	mesh->PolygonNumber = (blockXNumber + 2) * 2 * blockZNumber - 4;
	if (!createBuffer(mesh)) return;

#ifdef USING_DIRECTX
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	mesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
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

	//���z�A�h���X���
	mesh->VertexBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	mesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int countZ = 0; countZ < blockZNumber; ++countZ)
	{
		for (int countX = 0; countX < (blockXNumber + 1) * 2 + 2; ++countX)
		{
			if (countX < (blockXNumber + 1) * 2)
			{
				pIdx[countZ * ((blockXNumber + 1) * 2 + 2) + countX] = countX / 2 + (countX + 1) % 2 * (blockXNumber + 1) + countZ * (blockXNumber + 1);
			}
			else if (countZ * ((blockXNumber + 1) * 2 + 2) + countX < (((blockXNumber + 1) * 2 + 2) * blockZNumber - 1))//�k�ރ|���S��
			{
				pIdx[countZ * ((blockXNumber + 1) * 2 + 2) + countX] = (countX - 1) / 2 % (blockXNumber + 1) + countX % 2 * 2 * (blockXNumber + 1) + countZ * (blockXNumber + 1);
			}
		}
	}

	mesh->IndexBuffer->Unlock();
#endif

	//���b�V���̍쐬
	MeshStruct info;
	info.CurrentMesh = mesh;

	//�ۑ�
	meshes.emplace("field", info);
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateEditorField
//  �֐������F�G�f�B�^�p�t�B�[���h�f�[�^�̍X�V
//	�����F	vertexes�F���_�f�[�^
//			choosenIndexes�F�I�����ꂽ���_�̃C���f�b�N�X
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MeshManager::UpdateEditorField(const vector<Vector3>& vertexes, const list<int>& choosenIndexes)
{
#ifdef USING_DIRECTX

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	auto mesh = GetMeshBy("field");
	mesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
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

		//�@���v�Z
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

	//���z�A�h���X���
	mesh->VertexBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//	�֐����FSaveEditorFieldAs
//  �֐������F�G�f�B�^�p�t�B�[���h�f�[�^�̕ۑ�
//	�����F	fileName�F�t�@�C����
//	�߂�l�F�Ȃ�
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
	//���_�f�[�^
	VERTEX_3D* pVtx;
	mesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//�F�𔒂ɖ߂�
	unsigned long white = Color::White;
	for (int count = 0; count < mesh->VertexNumber; ++count)
	{
		pVtx[count].Color = white;
	}

	//�ۑ�
	fwrite(pVtx, sizeof(VERTEX_3D), mesh->VertexNumber, filePointer);
	mesh->VertexBuffer->Unlock();
	
	//�C���f�b�N�X
	WORD* pIdx;
	mesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), mesh->IndexNumber, filePointer);
	mesh->IndexBuffer->Unlock();
	
	//Texture
	int nTexSize = 0;
	fwrite(&nTexSize, sizeof(int), 1, filePointer);

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
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MeshManager::uninit(void)
{
	for (auto itr = meshes.begin(); itr != meshes.end();)
	{
		SAFE_RELEASE(itr->second.CurrentMesh->VertexBuffer);
		SAFE_RELEASE(itr->second.CurrentMesh->IndexBuffer);
		itr = meshes.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FloadFromMesh
//  �֐������F���b�V���t�@�C������f�[�^�̓Ǎ�
//	�����F	fileName�F�t�@�C���̖��O 
//	�߂�l�FMESH
//--------------------------------------------------------------------------------
MeshManager::MeshStruct MeshManager::loadFromMesh(const string& fileName)
{
	string filePath = "data/MESH/" + fileName;
	MeshStruct info;
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "rb");

	if (!filePointer) 
	{
		assert("failed to open file!!");
		return info;
	}
	
	info.CurrentMesh = new MeshInfo;

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

	//���_�f�[�^
	VERTEX_3D* pVtx;
	info.CurrentMesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fread_s(pVtx, sizeof(VERTEX_3D) * info.CurrentMesh->VertexNumber, sizeof(VERTEX_3D), info.CurrentMesh->VertexNumber, filePointer);
	info.CurrentMesh->VertexBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	info.CurrentMesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fread_s(pIdx, sizeof(WORD) * info.CurrentMesh->IndexNumber, sizeof(WORD), info.CurrentMesh->IndexNumber, filePointer);
	info.CurrentMesh->IndexBuffer->Unlock();
#endif

	//Texture
	int stringSize;
	fread_s(&stringSize, sizeof(int), sizeof(int), 1, filePointer);
	info.CurrentRenderInfo.TextureName.resize(stringSize);
	fread_s(&info.CurrentRenderInfo.TextureName[0], stringSize, sizeof(char), stringSize, filePointer);

	//Render Priority
	//fread_s(&info.CurrentRenderInfo.CurrentPriority, sizeof(RenderPriority), sizeof(RenderPriority), 1, filePointer);

	//Render State
	//fread_s(&info.CurrentRenderInfo.CurrentState, sizeof(RenderState), sizeof(RenderState), 1, filePointer);

	//Lighting
	fread_s(&info.CurrentRenderInfo.CurrentLighting, sizeof(Lighting), sizeof(Lighting), 1, filePointer);

	//CullMode
	fread_s(&info.CurrentRenderInfo.CurrentCullMode, sizeof(CullMode), sizeof(CullMode), 1, filePointer);

	//Synthesis
	fread_s(&info.CurrentRenderInfo.CurrentSynthesis, sizeof(Synthesis), sizeof(Synthesis), 1, filePointer);

	//FillMode
	fread_s(&info.CurrentRenderInfo.CurrentFillMode, sizeof(FillMode), sizeof(FillMode), 1, filePointer);

	//Alpha
	fread_s(&info.CurrentRenderInfo.CurrentAlpha, sizeof(Alpha), sizeof(Alpha), 1, filePointer);

	//Fog
	fread_s(&info.CurrentRenderInfo.CurrentFog, sizeof(Fog), sizeof(Fog), 1, filePointer);

	fclose(filePointer);

	return info;
}

//--------------------------------------------------------------------------------
//	�֐����FloadFromXFile
//  �֐������FX�t�@�C������f�[�^�̓Ǎ�
//	�����F	filePath�F�t�@�C���̃p�X
//	�߂�l�FMESH
//--------------------------------------------------------------------------------
MeshManager::MeshStruct MeshManager::loadFromXFile(const string& filePath)
{
	MeshStruct info;
	FILE* filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "r");

	if (!filePointer)
	{
		assert("failed to open file!!");
		return info;
	}

	info.CurrentMesh = new MeshInfo;
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
			//���_���̓Ǎ�
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int vertexNumber;
			ss << buffer;
			ss >> vertexNumber;

			//���_�f�[�^�̓Ǎ�
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

			//�|���S�����̓Ǎ�
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			ss << buffer;
			ss >> info.CurrentMesh->PolygonNumber;

			//�C���f�b�N�X�̓Ǎ�
			info.CurrentMesh->VertexNumber =
				info.CurrentMesh->IndexNumber = info.CurrentMesh->PolygonNumber * 3;
			vertexIndexes.resize(info.CurrentMesh->IndexNumber);
			for (int count = 0; count < info.CurrentMesh->PolygonNumber; ++count)
			{
				//3;���΂�
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
			//Material��
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int colorNumber;
			ss << buffer;
			ss >> colorNumber;
			colors.resize(colorNumber);

			//�C���f�b�N�X��
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			int polygonNumber;
			ss << buffer;
			ss >> polygonNumber;
			colorIndexes.resize(polygonNumber * 3);

			//�C���f�b�N�X�̓ǂݍ���
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

			//�}�e���A���̓ǂݍ���
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
			//�@�����̓Ǎ�
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int normalNumber;
			ss << buffer;
			ss >> normalNumber;

			//�@���f�[�^�̓Ǎ�
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

			//�@���C���f�b�N�X����Ǎ�(���_���Ɠ����̂���)
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			ss.clear();
			int normalIndexNumber;
			ss << buffer;
			ss >> normalIndexNumber;
			if (normalIndexNumber != info.CurrentMesh->PolygonNumber)
			{//�G���[
				MessageBox(NULL,"MeshManager:�@�����ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
			}

			//�@���C���f�b�N�X�̓Ǎ�
			normalIndexes.resize(normalIndexNumber * 3);
			for (int count = 0; count < info.CurrentMesh->PolygonNumber; ++count)
			{
				//3;���΂�
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
			//UV���̓Ǎ�
			Utility::GetStringUntilToken(filePointer, ";", buffer);
			stringstream ss;
			int uvNumber;
			ss << buffer;
			ss >> uvNumber;
			if (uvNumber != vertexes.size())
			{//�G���[
				MessageBox(NULL, "MeshManager:UV���ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
			}

			//UV�f�[�^�̓Ǎ�
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

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	info.CurrentMesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		pVtx[count].Position = vertexes[vertexIndexes[count]];
		pVtx[count].Normal = normals[normalIndexes[count]];
		pVtx[count].UV = uvs[vertexIndexes[count]];
		pVtx[count].Color = colors[colorIndexes[count]];
	}

	//���z�A�h���X���
	info.CurrentMesh->VertexBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	info.CurrentMesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		pIdx[count] = count;
	}

	info.CurrentMesh->IndexBuffer->Unlock();
#endif

	return info;
}

//--------------------------------------------------------------------------------
//  Cube�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshStruct MeshManager::createCube(void)
{
	MeshStruct info;
	info.CurrentMesh = new MeshInfo;
	info.CurrentMesh->VertexNumber = 6 * 4;
	info.CurrentMesh->IndexNumber = 6 * 4 + 5 * 2;
	info.CurrentMesh->PolygonNumber = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	// ���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	info.CurrentMesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto& halfSize = Vector3::One * 0.5f;
	unsigned long white = Color::White;
	int countVertex = 0;

	// ���
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

	// ��
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

	// ��
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

	// ��
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

	// �E
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

	// ����
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

	// ���z�A�h���X���
	info.CurrentMesh->VertexBuffer->Unlock();

	// �C���f�b�N�X
	WORD *pIdx;
	info.CurrentMesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < info.CurrentMesh->IndexNumber; ++count)
	{
		if (count % 6 < 4)
		{
			pIdx[count] = (WORD)((count / 6) * 4 + (count % 6) % 4);
		}
		else
		{// �k��
			pIdx[count] = (WORD)((count / 6) * 4 + (count % 2) + 3);
		}
	}

	info.CurrentMesh->IndexBuffer->Unlock();

	// Model�t�@�C���̕ۑ�
	string fileName = "data/MODEL/cube.model";
	FILE *filePointer;

	// file open
	fopen_s(&filePointer, fileName.c_str(), "wb");

	// Node��
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
	info.CurrentMesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fwrite(pVtx, sizeof(VERTEX_3D), info.CurrentMesh->VertexNumber, filePointer);
	info.CurrentMesh->VertexBuffer->Unlock();
	info.CurrentMesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), info.CurrentMesh->IndexNumber, filePointer);
	info.CurrentMesh->IndexBuffer->Unlock();

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
//  Sphere�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshStruct MeshManager::createSphere(void)
{
	MeshStruct info;
	assert("������!!");
	return info;
}

//--------------------------------------------------------------------------------
//  SkyBox�̍쐬
//--------------------------------------------------------------------------------
MeshManager::MeshStruct MeshManager::createSkyBox(void)
{
	MeshStruct info;
	info.CurrentMesh = new MeshInfo;
	info.CurrentMesh->VertexNumber = 6 * 4;
	info.CurrentMesh->IndexNumber = 6 * 4 + 5 * 2;
	info.CurrentMesh->PolygonNumber = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(info.CurrentMesh))
	{
		assert("failed to create buffer!!");
		return info;
	}

	// ���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	info.CurrentMesh->VertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto camera = CameraManager::Instance()->GetMainCamera();
	float length = camera ? (float)camera->GetFar() * 0.5f : 500.0f;
	unsigned long white = Color::White;
	int countVertex = 0;
	float uvTweens = 1.0f / 1024.0f;	//���Ԃ𖳂������߂�UV��1px�k�߂�

	// ���
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

	// ��
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

	// ��
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

	// ��
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

	// �E
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

	// ����
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

	//���z�A�h���X���
	info.CurrentMesh->VertexBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	info.CurrentMesh->IndexBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int count = 0; count < 6 * 4 + 5 * 2; ++count)
	{
		if (count % 6 < 4)
		{
			pIdx[count] = (count / 6) * 4 + (count % 6) % 4;
		}
		else
		{//�k��
			pIdx[count] = (count / 6) * 4 + (count % 2) + 3;
		}
	}

	info.CurrentMesh->IndexBuffer->Unlock();
#endif
	
	//Render State
	info.CurrentRenderInfo.CurrentLighting = Lighting_Off;
	info.CurrentRenderInfo.CurrentCullMode = Cull_CCW;
	info.CurrentRenderInfo.CurrentSynthesis = S_Multiplication;
	info.CurrentRenderInfo.CurrentFillMode = Fill_Solid;
	info.CurrentRenderInfo.CurrentAlpha = A_None;
	info.CurrentRenderInfo.CurrentFog = Fog_Off;

	return info;
}

//--------------------------------------------------------------------------------
//  �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------
bool MeshManager::createBuffer(MeshInfo* mesh)
{
#ifdef USING_DIRECTX
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * mesh->VertexNumber,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_3D,							//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,						//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&mesh->VertexBuffer,					//���_�o�b�t�@�̃|�C���^
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * mesh->IndexNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mesh->IndexBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "MeshManager : CreateIndexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}