//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
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
//  キャスト(D3DMATERIAL9)
//	DXの環境のため(マテリアル)オーバーロードする
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

	//今までのマテリアルと比較する
	for (auto& material : materials)
	{
		if (material.second == newMaterial)
		{//すでにあるならIDを返す
			return material.first;
		}
	}

	//Mapに追加する
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
//	コンストラクタ
//--------------------------------------------------------------------------------
MaterialManager::MaterialManager()
{
	materials.clear();
	SaveMaterial(Color::White, Color::White, Color::Black, Color::Black, 1.0f);
}
