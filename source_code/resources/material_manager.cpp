//--------------------------------------------------------------------------------
//�@material_manager.cpp
//  manage the materials_' save, load
//  �}�e���A���Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include <cereal/archives/binary.hpp>
using namespace cereal;
#include "material_manager.h"
#include "texture_manager.h"
#include "main_system.h"
#include "resources.h"
#ifdef _DEBUG
#include "main_system.h"
#include "debug_observer.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ��������
//  return : MaterialManager*
//--------------------------------------------------------------------------------
MaterialManager* MaterialManager::Create(void)
{
    auto instance = MY_NEW MaterialManager();
    instance->Init();
    return instance;
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃}�e���A�����g��
//--------------------------------------------------------------------------------
void MaterialManager::Use(const String& material_name, const Color& diffuse
    , const String& color_texture, const Color& ambient, const Color& specular
    , const Color& emissive, const String& diffuse_texture
    , const String& diffuse_texture_mask, const String& specular_texture
    , const String& specular_texture_mask, const String& normal_texture
    , const String& detail_texture, const String& detail_mask
    , const String& tint_by_base_mask, const String& rim_mask
    , const String& translucency, const String& metalness_mask
    , const String& self_illum_mask, const String& fresnel_warp_color
    , const String& fresnel_warp_rim, const String& fresnel_warp_specular)
{;
    size_t key = hash<String>()(material_name);
    auto iterator = materials_.find(key);
    if (iterator != materials_.end())
    {// ���łɑ��݂��Ă�
        ++iterator->second.user_number;
        return;
    }

    MaterialInfo info;
    info.pointer = MY_NEW Material(color_texture, diffuse_texture, diffuse_texture_mask
        , specular_texture, specular_texture_mask, normal_texture, detail_texture
        , detail_mask, tint_by_base_mask, rim_mask, translucency
        , metalness_mask, self_illum_mask, fresnel_warp_color, fresnel_warp_rim, fresnel_warp_specular
        , ambient, diffuse, specular, emissive);

    UseTexture(*info.pointer);
    materials_.emplace(key, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃}�e���A���̎擾
//--------------------------------------------------------------------------------
Material* MaterialManager::Get(const String& material_name) const
{
    auto iterator = materials_.find(hash<String>()(material_name));
    if (materials_.end() == iterator)
    {// �Ȃ��̏ꍇ�f�t�H���g�̃}�e���A����Ԃ�
        return nullptr;
    }
    return iterator->second.pointer;
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

}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MaterialManager::Uninit(void)
{
    for (auto iterator = materials_.begin(); iterator != materials_.end();)
    {
        DisuseTexture(*iterator->second.pointer);
        MY_DELETE iterator->second.pointer;
        iterator = materials_.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//  �}�e���A���Ǎ�����
//--------------------------------------------------------------------------------
void MaterialManager::LoadResource(void)
{
    lock_guard<mutex> lock(mutex_);
    if (load_tasks_.empty()) return;
    String material_name = load_tasks_.front();
    load_tasks_.pop();
    size_t key = hash<String>()(material_name);

    auto iterator = materials_.find(key);
    if (iterator != materials_.end())
    {// ���łɑ��݂��Ă�
        ++iterator->second.user_number;
        return;
    }

    MaterialInfo info;
    info.pointer = LoadFromFile(material_name);
    if (!info.pointer)
    {// �Ǎ��ł��Ȃ��̏ꍇ�^���Ԃŕۑ�����
#ifdef _DEBUG
        MainSystem::Instance().GetDebugObserver().Display(L"MaterialManager::LoadResource : " + material_name + L"��������Ȃ��I�I�I");
#endif // _DEBUG

        info.pointer = MY_NEW Material(L"polygon.jpg"
            , String(), String(), String(), String(), String()
            , String(), String(), String(), String(), String()
            , String(), String(), String(), String(), String()
            , Color::kRed, Color::kRed, Color::kRed, Color::kRed);
    }
    UseTexture(*info.pointer);
    materials_.emplace(key, info);
}

//--------------------------------------------------------------------------------
//  �}�e���A�������[�X����
//--------------------------------------------------------------------------------
void MaterialManager::ReleaseResource(void)
{
    lock_guard<mutex> lock(mutex_);
    if (release_tasks_.empty()) return;
    size_t key = release_tasks_.front();
    release_tasks_.pop();
    auto iterator = materials_.find(key);
    if (materials_.end() == iterator) return;

    if (--iterator->second.user_number <= 0)
    {// �N���g���ĂȂ��̂Ŕj������
        auto pointer = iterator->second.pointer;
        materials_.erase(iterator);
        DisuseTexture(*pointer);
        MY_DELETE pointer;
    }
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̎g�p
//--------------------------------------------------------------------------------
void MaterialManager::UseTexture(const Material& material)
{
    auto& texture_manager = MainSystem::Instance().GetResources().GetTextureManager();
    texture_manager.Use(material.color_texture_);
    texture_manager.Use(material.diffuse_texture_);
    texture_manager.Use(material.diffuse_texture_mask_);
    texture_manager.Use(material.specular_texture_);
    texture_manager.Use(material.specular_texture_mask_);
    texture_manager.Use(material.normal_texture_);
    texture_manager.Use(material.detail_texture_);
    texture_manager.Use(material.detail_mask_);
    texture_manager.Use(material.tint_by_base_mask_);
    texture_manager.Use(material.rim_mask_);
    texture_manager.Use(material.translucency_);
    texture_manager.Use(material.metalness_mask_);
    texture_manager.Use(material.self_illum_mask_);
    texture_manager.Use(material.fresnel_warp_color_);
    texture_manager.Use(material.fresnel_warp_rim_);
    texture_manager.Use(material.fresnel_warp_specular_);
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̔j��
//--------------------------------------------------------------------------------
void MaterialManager::DisuseTexture(const Material& material)
{
    auto& texture_manager = MainSystem::Instance().GetResources().GetTextureManager();
    texture_manager.Disuse(material.color_texture_);
    texture_manager.Disuse(material.diffuse_texture_);
    texture_manager.Disuse(material.diffuse_texture_mask_);
    texture_manager.Disuse(material.specular_texture_);
    texture_manager.Disuse(material.specular_texture_mask_);
    texture_manager.Disuse(material.normal_texture_);
    texture_manager.Disuse(material.detail_texture_);
    texture_manager.Disuse(material.detail_mask_);
    texture_manager.Disuse(material.tint_by_base_mask_);
    texture_manager.Disuse(material.rim_mask_);
    texture_manager.Disuse(material.translucency_);
    texture_manager.Disuse(material.metalness_mask_);
    texture_manager.Disuse(material.self_illum_mask_);
    texture_manager.Disuse(material.fresnel_warp_color_);
    texture_manager.Disuse(material.fresnel_warp_rim_);
    texture_manager.Disuse(material.fresnel_warp_specular_);
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