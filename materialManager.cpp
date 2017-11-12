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
//	関数名：Use
//  関数説明：マテリアルの追加
//	引数：	materialName：マテリアルのの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MaterialManager::Use(const string& materialName)
{
	auto iterator = materials.find(materialName);
	if (iterator != materials.end())
	{// すでに存在してる
		++iterator->second.UserNumber;
		return;
	}
	MaterialInfo newMaterial;
	newMaterial.Pointer = loadFrom(materialName);
	if (!newMaterial.Pointer) return;
	materials.emplace(materialName, newMaterial);
}

//--------------------------------------------------------------------------------
//	関数名：Use
//  関数説明：マテリアルの追加
//	引数：	materialName：マテリアルの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MaterialManager::Use(const string& materialName, Material* material)
{
	assert(material);
	auto iterator = materials.find(materialName);
	if (iterator != materials.end())
	{// すでに存在してる
		++iterator->second.UserNumber;
		delete material;
		return;
	}
	MaterialInfo newMaterial;
	newMaterial.Pointer = material;
	materials.emplace(materialName, newMaterial);
}

//--------------------------------------------------------------------------------
//	関数名：Disuse
//  関数説明：マテリアルの破棄
//	引数：	materialName：マテリアルの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void MaterialManager::Disuse(const string& materialName)
{
	auto iterator = materials.find(materialName);
	if (materials.end() == iterator) return;
	if (--iterator->second.UserNumber <= 0)
	{// 誰も使ってないので破棄する
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
//	コンストラクタ
//--------------------------------------------------------------------------------
MaterialManager::MaterialManager()
{
	materials.clear();
}

//--------------------------------------------------------------------------------
//	関数名：loadFrom
//  関数説明：マテリアルの読み込み
//	引数：	materialName：マテリアルの名前
//	戻り値：Material*
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