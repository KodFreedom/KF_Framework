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
	result.Ambient = ambient;
	result.Diffuse = diffuse;
	result.Emissive = emissive;
	result.Specular = specular;
	result.Power = power;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//	operator==
//--------------------------------------------------------------------------------
bool Material::operator==(const Material& material) const
{
	return this->ambient == material.ambient
		&& this->diffuse == material.diffuse
		&& this->emissive == material.emissive
		&& this->specular == material.specular
		&& this->power == material.power;
}

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
//  SaveMaterial
//--------------------------------------------------------------------------------
const unsigned short MaterialManager::SaveMaterial(const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power)
{
	auto& newMaterial = Material(ambient, diffuse, specular, emissive, power);

	//���܂ł̃}�e���A���Ɣ�r����
	for (auto& material : materials)
	{
		if (material.second == newMaterial)
		{//���łɂ���Ȃ�ID��Ԃ�
			return material.first;
		}
	}

	//Map�ɒǉ�����
	auto id = (unsigned short)materials.size();
	materials.emplace(id, newMaterial);
	return id;
}

//--------------------------------------------------------------------------------
//  GetMaterial
//--------------------------------------------------------------------------------
const Material& MaterialManager::GetMaterial(const unsigned short& usID)
{
	return materials.at(usID);
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
	SaveMaterial(Color::White, Color::White, Color::Black, Color::Black, 1.0f);
}
