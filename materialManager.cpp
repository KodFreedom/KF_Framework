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
//	�֐����FUse
//  �֐������F�}�e���A���̒ǉ�
//	�����F	materialName�F�}�e���A���̖��O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void MaterialManager::Use(const string& materialName, Material* material)
{
	assert(material);
	auto iterator = materials.find(materialName);
	if (iterator != materials.end())
	{// ���łɑ��݂��Ă�
		++iterator->second.UserNumber;
		delete material;
		return;
	}
	MaterialInfo newMaterial;
	newMaterial.Pointer = material;
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
		delete iterator->second.Pointer;
		materials.erase(iterator);
	}
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

	//file open
	fopen_s(&filePointer, filePath.c_str(), "rb");

	if (!filePointer)
	{
		assert("failed to open file!!");
		return nullptr;
	}

	auto result = new Material;

	// Todo : read from file

	fclose(filePointer);
	return result;
}