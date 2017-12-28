//--------------------------------------------------------------------------------
//�@material_manager.cpp
//  manage the materials_' save, load
//	�}�e���A���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "material_manager.h"
#include "main_system.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
Material MaterialManager::kDefaultMaterial = Material(L"polygon.jpg");

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃}�e���A�����g��
//--------------------------------------------------------------------------------
void MaterialManager::Use(const String& material_name)
{
	auto iterator = materials_.find(material_name);
	if (iterator != materials_.end())
	{// ���łɑ��݂��Ă�
		++iterator->second.user_number;
		return;
	}
	MaterialInfo info;
	info.pointer = LoadFromFile(material_name);
	if (!info.pointer)
	{// �Ǎ��ł��Ȃ��̏ꍇ�^���Ԃŕۑ�����
		info.pointer = MY_NEW Material(L"polygon.jpg"
			, String(), String(), String(), String(), String()
			, String(), String(), String(), String(), String()
			, String(), String(), String(), String(), String()
			, Color::kRed, Color::kRed, Color::kRed, Color::kRed);
	}
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	texture_manager->Use(info.pointer->color_texture_);
	texture_manager->Use(info.pointer->diffuse_texture_);
	texture_manager->Use(info.pointer->diffuse_texture_mask_);
	texture_manager->Use(info.pointer->specular_texture_);
	texture_manager->Use(info.pointer->specular_texture_mask_);
	texture_manager->Use(info.pointer->normal_texture_);
	texture_manager->Use(info.pointer->detail_texture_);
	texture_manager->Use(info.pointer->detail_mask_);
	texture_manager->Use(info.pointer->tint_by_base_mask_);
	texture_manager->Use(info.pointer->rim_mask_);
	texture_manager->Use(info.pointer->translucency_);
	texture_manager->Use(info.pointer->metalness_mask_);
	texture_manager->Use(info.pointer->self_illum_mask_);
	texture_manager->Use(info.pointer->fresnel_warp_color_);
	texture_manager->Use(info.pointer->fresnel_warp_rim_);
	texture_manager->Use(info.pointer->fresnel_warp_specular_);
	materials_.emplace(material_name, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃}�e���A�����g��
//--------------------------------------------------------------------------------
void MaterialManager::Use(const String& material_name, Material* material)
{
	auto iterator = materials_.find(material_name);
	if (iterator != materials_.end())
	{// ���łɑ��݂��Ă�
		++iterator->second.user_number;
		SAFE_DELETE(material);
		return;
	}

	MaterialInfo info;
	info.pointer = material;
	if (!info.pointer)
	{// material��null�̏ꍇ�^���Ԃŕۑ�����
		info.pointer = MY_NEW Material(L"polygon.jpg"
			, String(), String(), String(), String(), String()
			, String(), String(), String(), String(), String()
			, String(), String(), String(), String(), String()
			, Color::kRed, Color::kRed, Color::kRed, Color::kRed);
	}
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	texture_manager->Use(info.pointer->color_texture_);
	texture_manager->Use(info.pointer->diffuse_texture_);
	texture_manager->Use(info.pointer->diffuse_texture_mask_);
	texture_manager->Use(info.pointer->specular_texture_);
	texture_manager->Use(info.pointer->specular_texture_mask_);
	texture_manager->Use(info.pointer->normal_texture_);
	texture_manager->Use(info.pointer->detail_texture_);
	texture_manager->Use(info.pointer->detail_mask_);
	texture_manager->Use(info.pointer->tint_by_base_mask_);
	texture_manager->Use(info.pointer->rim_mask_);
	texture_manager->Use(info.pointer->translucency_);
	texture_manager->Use(info.pointer->metalness_mask_);
	texture_manager->Use(info.pointer->self_illum_mask_);
	texture_manager->Use(info.pointer->fresnel_warp_color_);
	texture_manager->Use(info.pointer->fresnel_warp_rim_);
	texture_manager->Use(info.pointer->fresnel_warp_specular_);
	materials_.emplace(material_name, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃}�e���A�����g��Ȃ�
//--------------------------------------------------------------------------------
void MaterialManager::Disuse(const String& material_name)
{
	auto iterator = materials_.find(material_name);
	if (materials_.end() == iterator) return;
	if (--iterator->second.user_number <= 0)
	{// �N���g���ĂȂ��̂Ŕj������
		auto texture_manager = MainSystem::Instance()->GetTextureManager();
		texture_manager->Disuse(iterator->second.pointer->color_texture_);
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture_);
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture_mask_);
		texture_manager->Disuse(iterator->second.pointer->specular_texture_);
		texture_manager->Disuse(iterator->second.pointer->specular_texture_mask_);
		texture_manager->Disuse(iterator->second.pointer->normal_texture_);
		texture_manager->Disuse(iterator->second.pointer->detail_texture_);
		texture_manager->Disuse(iterator->second.pointer->detail_mask_);
		texture_manager->Disuse(iterator->second.pointer->tint_by_base_mask_);
		texture_manager->Disuse(iterator->second.pointer->rim_mask_);
		texture_manager->Disuse(iterator->second.pointer->translucency_);
		texture_manager->Disuse(iterator->second.pointer->metalness_mask_);
		texture_manager->Disuse(iterator->second.pointer->self_illum_mask_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_color_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_rim_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_specular_);
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
//  ����������
//--------------------------------------------------------------------------------
void MaterialManager::Init(void)
{
	// default material
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	texture_manager->Use(kDefaultMaterial.color_texture_);
	texture_manager->Use(kDefaultMaterial.diffuse_texture_);
	texture_manager->Use(kDefaultMaterial.diffuse_texture_mask_);
	texture_manager->Use(kDefaultMaterial.specular_texture_);
	texture_manager->Use(kDefaultMaterial.specular_texture_mask_);
	texture_manager->Use(kDefaultMaterial.normal_texture_);
	texture_manager->Use(kDefaultMaterial.detail_texture_);
	texture_manager->Use(kDefaultMaterial.detail_mask_);
	texture_manager->Use(kDefaultMaterial.tint_by_base_mask_);
	texture_manager->Use(kDefaultMaterial.rim_mask_);
	texture_manager->Use(kDefaultMaterial.translucency_);
	texture_manager->Use(kDefaultMaterial.metalness_mask_);
	texture_manager->Use(kDefaultMaterial.self_illum_mask_);
	texture_manager->Use(kDefaultMaterial.fresnel_warp_color_);
	texture_manager->Use(kDefaultMaterial.fresnel_warp_rim_);
	texture_manager->Use(kDefaultMaterial.fresnel_warp_specular_);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MaterialManager::Uninit(void)
{
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	for (auto iterator = materials_.begin(); iterator != materials_.end();)
	{
		texture_manager->Disuse(iterator->second.pointer->color_texture_);
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture_);
		texture_manager->Disuse(iterator->second.pointer->diffuse_texture_mask_);
		texture_manager->Disuse(iterator->second.pointer->specular_texture_);
		texture_manager->Disuse(iterator->second.pointer->specular_texture_mask_);
		texture_manager->Disuse(iterator->second.pointer->normal_texture_);
		texture_manager->Disuse(iterator->second.pointer->detail_texture_);
		texture_manager->Disuse(iterator->second.pointer->detail_mask_);
		texture_manager->Disuse(iterator->second.pointer->tint_by_base_mask_);
		texture_manager->Disuse(iterator->second.pointer->rim_mask_);
		texture_manager->Disuse(iterator->second.pointer->translucency_);
		texture_manager->Disuse(iterator->second.pointer->metalness_mask_);
		texture_manager->Disuse(iterator->second.pointer->self_illum_mask_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_color_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_rim_);
		texture_manager->Disuse(iterator->second.pointer->fresnel_warp_specular_);
		delete iterator->second.pointer;
		iterator = materials_.erase(iterator);
	}

	// default material
	texture_manager->Disuse(kDefaultMaterial.color_texture_);
	texture_manager->Disuse(kDefaultMaterial.diffuse_texture_);
	texture_manager->Disuse(kDefaultMaterial.diffuse_texture_mask_);
	texture_manager->Disuse(kDefaultMaterial.specular_texture_);
	texture_manager->Disuse(kDefaultMaterial.specular_texture_mask_);
	texture_manager->Disuse(kDefaultMaterial.normal_texture_);
	texture_manager->Disuse(kDefaultMaterial.detail_texture_);
	texture_manager->Disuse(kDefaultMaterial.detail_mask_);
	texture_manager->Disuse(kDefaultMaterial.tint_by_base_mask_);
	texture_manager->Disuse(kDefaultMaterial.rim_mask_);
	texture_manager->Disuse(kDefaultMaterial.translucency_);
	texture_manager->Disuse(kDefaultMaterial.metalness_mask_);
	texture_manager->Disuse(kDefaultMaterial.self_illum_mask_);
	texture_manager->Disuse(kDefaultMaterial.fresnel_warp_color_);
	texture_manager->Disuse(kDefaultMaterial.fresnel_warp_rim_);
	texture_manager->Disuse(kDefaultMaterial.fresnel_warp_specular_);
}

//--------------------------------------------------------------------------------
//  �t�@�C������}�e���A���̓ǂݍ���
//--------------------------------------------------------------------------------
Material* MaterialManager::LoadFromFile(const String& material_name)
{
	String path = L"data/material/" + material_name + L".material";
	ifstream file(path, ios::binary);
	if (!file.is_open())
	{
		assert(file.is_open());
		return nullptr;
	}
	BinaryInputArchive archive(file);
	auto result = MY_NEW Material;
	int buffer_size;
	string buffer;

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->color_texture_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->diffuse_texture_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->diffuse_texture_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->specular_texture_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->specular_texture_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->normal_texture_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->detail_texture_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->detail_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->tint_by_base_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->rim_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->translucency_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->metalness_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->self_illum_mask_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->fresnel_warp_color_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->fresnel_warp_rim_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&buffer_size, sizeof(buffer_size));
	buffer.resize(buffer_size);
	archive.loadBinary(&buffer[0], buffer_size);
	result->fresnel_warp_specular_ = String(buffer.begin(), buffer.end());

	archive.loadBinary(&result->ambient_, sizeof(result->ambient_));
	archive.loadBinary(&result->diffuse_, sizeof(result->diffuse_));
	archive.loadBinary(&result->specular_, sizeof(result->specular_));
	archive.loadBinary(&result->emissive_, sizeof(result->emissive_));
	archive.loadBinary(&result->power_, sizeof(result->power_));
	file.close();
	return result;
}