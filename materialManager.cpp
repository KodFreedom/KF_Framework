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
#include "textureManager.h"

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
		TextureManager::Instance()->Disuse(iterator->second.Pointer->MainTexture);
		delete iterator->second.Pointer;
		materials.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	関数名：CreateMaterial
//  関数説明：マテリアルの作成
//	引数：	materialName：マテリアルの名前
//	戻り値：なし
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