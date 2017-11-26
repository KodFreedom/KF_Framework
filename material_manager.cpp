//--------------------------------------------------------------------------------
//　material_manager.cpp
//  manage the materials_' save, load
//	マテリアル管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "material_manager.h"
#include "main_system.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Material MaterialManager::kDefaultMaterial = Material();

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  与えられた名前のマテリアルを使う
//--------------------------------------------------------------------------------
void MaterialManager::Use(const String& material_name)
{
	auto iterator = materials_.find(material_name);
	if (iterator != materials_.end())
	{// すでに存在してる
		++iterator->second.user_number;
		return;
	}
	MaterialInfo info;
	info.pointer = LoadFromFile(material_name);
	if (!info.pointer)
	{// 読込できないの場合真っ赤で保存する
		info.pointer = new Material(Color::kRed, Color::kRed, Color::kRed, Color::kRed);
	}
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	texture_manager->Use(info.pointer->diffuse_texture);
	texture_manager->Use(info.pointer->specular_texture);
	texture_manager->Use(info.pointer->normal_texture);
	materials_.emplace(material_name, info);
}

//--------------------------------------------------------------------------------
//  与えられた名前のマテリアルを使う
//--------------------------------------------------------------------------------
void MaterialManager::Use(const String& material_name, Material* material)
{
	auto iterator = materials_.find(material_name);
	if (iterator != materials_.end())
	{// すでに存在してる
		++iterator->second.user_number;
		SAFE_DELETE(material);
		return;
	}

	MaterialInfo info;
	info.pointer = material;
	if (!info.pointer)
	{// materialがnullの場合真っ赤で保存する
		info.pointer = new Material(Color::kRed, Color::kRed, Color::kRed, Color::kRed);
	}
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	texture_manager->Use(info.pointer->diffuse_texture);
	texture_manager->Use(info.pointer->specular_texture);
	texture_manager->Use(info.pointer->normal_texture);
	materials_.emplace(material_name, info);
}

//--------------------------------------------------------------------------------
//  与えられた名前のマテリアルを使わない
//--------------------------------------------------------------------------------
void MaterialManager::Disuse(const String& material_name)
{
	auto iterator = materials_.find(material_name);
	if (materials_.end() == iterator) return;
	if (--iterator->second.user_number <= 0)
	{// 誰も使ってないので破棄する
		auto texture_manager = MainSystem::Instance()->GetTextureManager();
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture);
		texture_manager->Disuse(iterator->second.pointer->specular_texture);
		texture_manager->Disuse(iterator->second.pointer->normal_texture);
		delete iterator->second.pointer;
		materials_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MaterialManager::Uninit(void)
{
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	for (auto iterator = materials_.begin(); iterator != materials_.end();)
	{
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture);
		texture_manager->Disuse(iterator->second.pointer->specular_texture);
		texture_manager->Disuse(iterator->second.pointer->normal_texture);
		delete iterator->second.pointer;
		iterator = materials_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  ファイルからマテリアルの読み込み
//--------------------------------------------------------------------------------
Material* MaterialManager::LoadFromFile(const String& material_name)
{
	String path = L"data/material/" + material_name + L".material";
	ifstream file(path);
	if (!file.is_open())
	{
		assert("failed to open file!!");
		return nullptr;
	}
	BinaryInputArchive archive(file);
	auto result = new Material;
	int buffer_size;
	string buffer;
	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->diffuse_texture = String(buffer.begin(), buffer.end());
	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->specular_texture = String(buffer.begin(), buffer.end());
	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->normal_texture = String(buffer.begin(), buffer.end());
	archive.loadBinary(&result->ambient, sizeof(result->ambient));
	archive.loadBinary(&result->diffuse, sizeof(result->diffuse));
	archive.loadBinary(&result->specular, sizeof(result->specular));
	archive.loadBinary(&result->emissive, sizeof(result->emissive));
	archive.loadBinary(&result->power, sizeof(result->power));
	file.close();
	return result;
}