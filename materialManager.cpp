//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//�@materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "materialManager.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
MaterialManager* MaterialManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//	Material
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �L���X�g(D3DMATERIAL9)
//	DX�̊��̂���(�}�e���A��)�I�[�o�[���[�h����
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Material::operator D3DMATERIAL9() const
{
	D3DMATERIAL9 result;
	result.Ambient = Ambient;
	result.Diffuse = Diffuse;
	result.Emissive = Emissive;
	result.Specular = Specular;
	result.Power = Power;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//
//	MaterialManager
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FUse
//  �֐������F�}�e���A���̒ǉ�
//	�����F	materialName�F�}�e���A���̖̂��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MaterialManager::Use(const string& materialName)
{
	auto iterator = materials.find(materialName);
	if (iterator != materials.end())
	{// ���łɑ��݂��Ă�
		++iterator->second.UserNumber;
		return;
	}
	MaterialInfo newMaterial;
	newMaterial.Pointer = loadFrom(materialName);
	if (!newMaterial.Pointer) return;
	materials.emplace(materialName, newMaterial);
}

//--------------------------------------------------------------------------------
//	�֐����FDisuse
//  �֐������F�}�e���A���̔j��
//	�����F	materialName�F�}�e���A���̖��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MaterialManager::Disuse(const string& materialName)
{
	auto iterator = materials.find(materialName);
	if (materials.end() == iterator) return;
	if (--iterator->second.UserNumber <= 0)
	{// �N���g���ĂȂ��̂Ŕj������
		TextureManager::Instance()->Disuse(iterator->second.Pointer->MainTexture);
		delete iterator->second.Pointer;
		materials.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FCreateMaterial
//  �֐������F�}�e���A���̍쐬
//	�����F	materialName�F�}�e���A���̖��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MaterialManager::CreateMaterialFile(const string& materialName, const string& mainTextureName,
	const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, const float& power)
{
	string filePath = "data/MATERIAL/" + materialName + ".material";
	FILE *filePointer = nullptr;
	fopen_s(&filePointer, filePath.c_str(), "wb");
	if (!filePointer)
	{
		assert("failed to open file!!");
		return;
	}
	fwrite(&ambient, sizeof(Color), 1, filePointer);
	fwrite(&diffuse, sizeof(Color), 1, filePointer);
	fwrite(&specular, sizeof(Color), 1, filePointer);
	fwrite(&emissive, sizeof(Color), 1, filePointer);
	fwrite(&power, sizeof(float), 1, filePointer);
	int number = mainTextureName.size();
	fwrite(&number, sizeof(int), 1, filePointer);
	fwrite(&mainTextureName[0], sizeof(char), number, filePointer);
	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�R���X�g���N�^
//--------------------------------------------------------------------------------
MaterialManager::MaterialManager()
{
	materials.clear();
}

//--------------------------------------------------------------------------------
//	�֐����FloadFrom
//  �֐������F�}�e���A���̓ǂݍ���
//	�����F	materialName�F�}�e���A���̖��O
//	�߂�l�FMaterial*
//--------------------------------------------------------------------------------
Material* MaterialManager::loadFrom(const string& materialName)
{
	string filePath = "data/MATERIAL/" + materialName + ".material";
	FILE *filePointer = nullptr;
	fopen_s(&filePointer, filePath.c_str(), "rb");
	if (!filePointer)
	{
		assert("failed to open file!!");
		return nullptr;
	}
	auto result = new Material;
	fread_s(&result->Ambient, sizeof(Color), sizeof(Color), 1, filePointer);
	fread_s(&result->Diffuse, sizeof(Color), sizeof(Color), 1, filePointer);
	fread_s(&result->Specular, sizeof(Color), sizeof(Color), 1, filePointer);
	fread_s(&result->Emissive, sizeof(Color), sizeof(Color), 1, filePointer);
	fread_s(&result->Power, sizeof(float), sizeof(float), 1, filePointer);
	int number = 0;
	fread_s(&number, sizeof(int), sizeof(int), 1, filePointer);
	result->MainTexture.resize(number);
	fread_s(&result->MainTexture[0], number, sizeof(char), number, filePointer);
	fclose(filePointer);
	return result;
}