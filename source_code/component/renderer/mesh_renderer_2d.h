//--------------------------------------------------------------------------------
//  2d���b�V���`��R���|�l���g
//�@mesh_renderer_2d.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mesh_renderer.h"
#include "../../kf_utility/kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer2d : public MeshRenderer
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    MeshRenderer2d(GameObject& owner
        , const RenderPriority& priority = kDefaultPriority
        , const ShaderType& shader_type = kDefaultShader)
        : MeshRenderer(owner, priority, shader_type, kMeshRenderer2d) {}
    ~MeshRenderer2d() {}

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  �`�揈��
    //--------------------------------------------------------------------------------
    void RenderBy(const RenderSystem& render_system) override;

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    const Vector2& GetUvScale(void) const { return uv_scale_; }
    const Vector2& GetUvOffset(void) const { return uv_offset_; }

    //--------------------------------------------------------------------------------
    //  �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetUvScale(const Vector2& scale) { uv_scale_ = scale; }
    void SetUvOffset(const Vector2& offset) { uv_offset_ = offset; }

private:
    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    Vector2 uv_offset_ = Vector2::kZero;
    Vector2 uv_scale_ = Vector2::kOne;
};