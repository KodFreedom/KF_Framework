//--------------------------------------------------------------------------------
//  renderer_directX9.h
//�@�����_�[�V�X�e��(directx9)
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "../common_setting.h"
#include "../kf_utility/kf_utility.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#include "render_system.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class RenderSystemDirectX9 : public RenderSystem
{
public:
    //--------------------------------------------------------------------------------
    //  return the instance if create successfully
    //  ��������
    //  return : RenderSystemDirectX9*
    //--------------------------------------------------------------------------------
    static RenderSystemDirectX9* Create(HWND hwnd, BOOL is_window_mode)
    {
        auto instance = MY_NEW RenderSystemDirectX9;
        if (!instance->Init(hwnd, is_window_mode))
        {
            SAFE_RELEASE(instance);
        }
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  �f�B�o�C�X��Ԃ�
    //  return : LPDIRECT3DDEVICE9
    //--------------------------------------------------------------------------------
    const auto GetDevice(void) const { return device_; }

    //--------------------------------------------------------------------------------
    //  �`��J�n
    //--------------------------------------------------------------------------------
    bool BeginRender(void) override
    {
        device_->SetRenderTarget(0, back_buffer_surface_);
        device_->SetDepthStencilSurface(depth_stencil_surface_);
        device_->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), background_color_, 1.0f, 0);
        return device_->BeginScene() >= 0;
    }

    //--------------------------------------------------------------------------------
    //  �`��I��
    //--------------------------------------------------------------------------------
    void EndRender(void) override
    {
        device_->EndScene();
    }

    //--------------------------------------------------------------------------------
    //  swap backbuffer / �o�b�N�o�b�t�@�̐؂�ւ�
    //--------------------------------------------------------------------------------
    void Present(void) override
    {
        device_->Present(NULL, NULL, NULL, NULL);
    }

    //--------------------------------------------------------------------------------
    //  �`�揈��(2D)
    //  mesh_name�F���b�V�����O
    //--------------------------------------------------------------------------------
    void Render2dMesh(const String& mesh_name) const override;

    //--------------------------------------------------------------------------------
    //  �`�揈��(3D)
    //  mesh_name�F���b�V�����O
    //--------------------------------------------------------------------------------
    void Render3dMesh(const String& mesh_name) const override;

    //--------------------------------------------------------------------------------
    //  �`�揈��(3dSkin)
    //  skin_name�F���b�V�����O
    //--------------------------------------------------------------------------------
    void Render3dSkin(const String& skin_name) const override;

    //--------------------------------------------------------------------------------
    //  �J�����O���[�h�̐ݒ�
    //--------------------------------------------------------------------------------
    void SetRenderState(const CullMode& value) override;

    //--------------------------------------------------------------------------------
    //  �t�B�����[�h�̐ݒ�
    //--------------------------------------------------------------------------------
    void SetRenderState(const FillMode& value) override;

    //--------------------------------------------------------------------------------
    //  �t�H�O���[�h�̐ݒ�
    //--------------------------------------------------------------------------------
    void SetRenderState(const FogMode& value) override;

    //--------------------------------------------------------------------------------
    //  �����[�h�̐ݒ�
    //--------------------------------------------------------------------------------
    void SetRenderState(const AlphaMode& value) override;

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    RenderSystemDirectX9() : RenderSystem()
        , instance_(nullptr), device_(nullptr)
        , vertex_declaration_2d_(nullptr), vertex_declaration_3d_(nullptr)
        , vertex_declaration_3d_skin_(nullptr), back_buffer_surface_(nullptr)
        , depth_stencil_surface_(nullptr) {}
    RenderSystemDirectX9(const RenderSystemDirectX9& value) {}
    RenderSystemDirectX9& operator=(const RenderSystemDirectX9& value) {}
    ~RenderSystemDirectX9() {}

    //--------------------------------------------------------------------------------
    //  ������
    //--------------------------------------------------------------------------------
    bool Init(HWND hwnd, BOOL is_window_mode);

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  �f�B�o�C�X�̐���
    //--------------------------------------------------------------------------------
    bool CreateDevice(HWND hwnd, BOOL is_window_mode);
    
    //--------------------------------------------------------------------------------
    //  �o�[�e�b�N�X�f�N�����[�V�����̏�����
    //--------------------------------------------------------------------------------
    void InitVertexDeclaration(void);

    //--------------------------------------------------------------------------------
    //  �����_�[�X�e�[�g�̏�����
    //--------------------------------------------------------------------------------
    void InitRenderSate(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    LPDIRECT3D9    instance_; // Direct3D�I�u�W�F�N�g
    LPDIRECT3DDEVICE9 device_; // Device�f�B�o�C�X
    LPDIRECT3DSURFACE9 back_buffer_surface_; // �o�b�N�o�b�t�@�̃T�[�t�F�X
    LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_2d_; // 2d�o�[�e�b�N�X�f�N�����[�V����
    LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_3d_; // 3d�o�[�e�b�N�X�f�N�����[�V����
    LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_3d_skin_; // 3d�X�L���o�[�e�b�N�X�f�N�����[�V����
    LPDIRECT3DSURFACE9 depth_stencil_surface_;
};
#endif