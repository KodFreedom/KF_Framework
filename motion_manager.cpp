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
//  �^����ꂽ���O�̃��[�V�������g��
//--------------------------------------------------------------------------------
void MotionManager::Use(const String& motion_name)
{
    if (motion_name.empty()) return;

    //���łɓǂݍ��񂾂珈���I��
    auto iterator = motions.find(motion_name);
    if (iterator != motions.end())
    {
        ++iterator->second.user_number;
        return;
    }

    //���[�V�����̓ǂݍ���
    MotionInfo info;
    info.pointer = LoadFromFile(motion_name);
    motions.emplace(motion_name, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃��[�V�������g��Ȃ�
//--------------------------------------------------------------------------------
void MotionManager::Disuse(const String& motion_name)
{
    if (motion_name.empty()) return;
    auto iterator = motions.find(motion_name);
    if (iterator == motions.end()) return;
    --iterator->second.user_number;
    if (iterator->second.user_number == 0)
    {// �N���g���ĂȂ��̂Ŕj������
        SAFE_DELETE(iterator->second.pointer);
        motions.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//    
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �N���A����
//--------------------------------------------------------------------------------
void MotionManager::Clear(void)
{
    for (auto iterator = motions.begin(); iterator != motions.end();)
    {
        SAFE_DELETE(iterator->second.pointer);
        iterator = motions.erase(iterator);
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