//--------------------------------------------------------------------------------
//�@motion_manager.cpp
//  manage the motiondatas' save, load
//  ���[�V�����Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "motion_manager.h"
#include "motion_data.h"
#include "kf_utility.h"
#include <cereal/archives/binary.hpp>
using namespace cereal;

//--------------------------------------------------------------------------------
//    
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
MotionManager* MotionManager::Create(void)
{
    auto instance = MY_NEW MotionManager;
    instance->Init();
    return instance;
}

//--------------------------------------------------------------------------------
//  ���[�V�����f�[�^���擾
//--------------------------------------------------------------------------------
MotionData* MotionManager::Get(const String& motion_name)
{
    auto iterator = motions_.find(hash<String>()(motion_name));
    if (motions_.end() == iterator) return nullptr;
    return iterator->second.pointer;
}

//--------------------------------------------------------------------------------
//    
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MotionManager::Uninit(void)
{
    for (auto iterator = motions_.begin(); iterator != motions_.end();)
    {
        SAFE_DELETE(iterator->second.pointer);
        iterator = motions_.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//  �}�e���A���Ǎ�����
//--------------------------------------------------------------------------------
void MotionManager::LoadResource(void)
{
    lock_guard<mutex> lock(mutex_);
    if (load_tasks_.empty()) return;
    String motion_name = load_tasks_.front();
    load_tasks_.pop();
    size_t key = hash<String>()(motion_name);

    //���łɓǂݍ��񂾂珈���I��
    auto iterator = motions_.find(key);
    if (iterator != motions_.end())
    {
        ++iterator->second.user_number;
        return;
    }

    //���[�V�����̓ǂݍ���
    MotionInfo info;
    info.pointer = LoadFromFile(motion_name);
    motions_.emplace(key, info);
}

//--------------------------------------------------------------------------------
//  �}�e���A�������[�X����
//--------------------------------------------------------------------------------
void MotionManager::ReleaseResource(void)
{
    lock_guard<mutex> lock(mutex_);
    if (release_tasks_.empty()) return;
    size_t key = release_tasks_.front();
    release_tasks_.pop();
    auto iterator = motions_.find(key);
    if (iterator == motions_.end()) return;
    --iterator->second.user_number;
    if (iterator->second.user_number == 0)
    {// �N���g���ĂȂ��̂Ŕj������
        auto pointer = iterator->second.pointer;
        motions_.erase(iterator);
        SAFE_DELETE(pointer);
    }
}

//--------------------------------------------------------------------------------
//  �t�@�C������ǂݍ���
//--------------------------------------------------------------------------------
MotionData* MotionManager::LoadFromFile(const String& motion_name)
{
    auto& path = L"data/motion/" + motion_name + L".motion";
    ifstream file(path, ios::binary);
    if (!file.is_open()) return nullptr;

    auto data = MY_NEW MotionData;
    BinaryInputArchive archive(file);

    archive.loadBinary(&data->is_loop_, sizeof(data->is_loop_));

    int frame_number = 0;
    archive.loadBinary(&frame_number, sizeof(frame_number));

    data->frames_.resize(frame_number);
    for (auto& frame : data->frames_)
    {
        int bone_number = 0;
        archive.loadBinary(&bone_number, sizeof(bone_number));
        frame.bone_transforms_.resize(bone_number);
        for (auto& bone_transform : frame.bone_transforms_)
        {
            archive.loadBinary(&bone_transform.translation_, sizeof(bone_transform.translation_));
            archive.loadBinary(&bone_transform.rotation_, sizeof(bone_transform.rotation_));
            archive.loadBinary(&bone_transform.scale_, sizeof(bone_transform.scale_));
        }
    }

    file.close();
    return data;
}