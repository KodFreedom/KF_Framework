//--------------------------------------------------------------------------------
//  renderer_directX9.h
//　レンダーシステム(directx9)
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#include "main_system.h"
#include "mesh_manager.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "ImGui\imgui_impl_dx9.h"
#endif

//--------------------------------------------------------------------------------
//  library / ライブラリ
//--------------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理(2D)
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Render2dMesh(const String& mesh_name) const
{
    auto mesh = MainSystem::Instance()->GetMeshManager()->GetMesh(mesh_name);
    device_->SetVertexDeclaration(vertex_declaration_2d_);
    device_->SetStreamSource(0, mesh->vertex_buffer, 0, sizeof(Vertex2d));
    device_->SetIndices(mesh->index_buffer);
    device_->DrawIndexedPrimitive(static_cast<_D3DPRIMITIVETYPE>(mesh->draw_type),
        0, 0, mesh->vertex_number, 0, mesh->polygon_number);
}

//--------------------------------------------------------------------------------
//  描画処理(3D)
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Render3dMesh(const String& mesh_name) const
{
    auto mesh = MainSystem::Instance()->GetMeshManager()->GetMesh(mesh_name);
    device_->SetVertexDeclaration(vertex_declaration_3d_);
    device_->SetStreamSource(0, mesh->vertex_buffer, 0, sizeof(Vertex3d));
    device_->SetIndices(mesh->index_buffer);
    device_->DrawIndexedPrimitive(static_cast<_D3DPRIMITIVETYPE>(mesh->draw_type),
        0, 0, mesh->vertex_number, 0, mesh->polygon_number);
}

//--------------------------------------------------------------------------------
//  描画処理(3dSkin)
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Render3dSkin(const String& skin_name) const
{
    auto mesh = MainSystem::Instance()->GetMeshManager()->GetMesh(skin_name);
    device_->SetVertexDeclaration(vertex_declaration_3d_skin_);
    device_->SetStreamSource(0, mesh->vertex_buffer, 0, sizeof(Vertex3dSkin));
    device_->SetIndices(mesh->index_buffer);
    device_->DrawIndexedPrimitive(static_cast<_D3DPRIMITIVETYPE>(mesh->draw_type),
        0, 0, mesh->vertex_number, 0, mesh->polygon_number);
}

//--------------------------------------------------------------------------------
// カリングモードの設定
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const CullMode& value)
{
    device_->SetRenderState(D3DRS_CULLMODE, static_cast<_D3DCULL>(value + 1));
}

//--------------------------------------------------------------------------------
//  フィルモードの設定
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const FillMode& value)
{
    device_->SetRenderState(D3DRS_FILLMODE, static_cast<_D3DFILLMODE>(value + 1));
}

//--------------------------------------------------------------------------------
//  αモードの設定
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const AlphaMode& value)
{
    device_->SetRenderState(D3DRS_ALPHATESTENABLE, value == kAlphaTest ? TRUE : FALSE);
}

//--------------------------------------------------------------------------------
//  フォグモードの設定
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const FogMode& value)
{
    device_->SetRenderState(D3DRS_FOGENABLE, static_cast<DWORD>(value));
    device_->SetRenderState(D3DRS_RANGEFOGENABLE, static_cast<DWORD>(value));
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool RenderSystemDirectX9::Init(HWND hwnd, BOOL is_window_mode)
{
    if (!CreateDevice(hwnd, is_window_mode)) return false;
    InitVertexDeclaration();
    InitRenderSate();
#if defined(_DEBUG) || defined(EDITOR)
    ImGui_ImplDX9_Init(hwnd, device_);
#endif
    device_->GetRenderTarget(0, &back_buffer_surface_);
    device_->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT
        , D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &depth_stencil_surface_, NULL);
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Uninit(void)
{
#if defined(_DEBUG) || defined(EDITOR)
    ImGui_ImplDX9_Shutdown();
#endif
    SAFE_RELEASE(vertex_declaration_2d_);
    SAFE_RELEASE(vertex_declaration_3d_);
    SAFE_RELEASE(vertex_declaration_3d_skin_);
    SAFE_RELEASE(device_);
    SAFE_RELEASE(instance_);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
bool RenderSystemDirectX9::CreateDevice(HWND hwnd, BOOL is_window_mode)
{
    D3DPRESENT_PARAMETERS present_parameters;
    D3DDISPLAYMODE display_mode;

    // Direct3Dオブジェクトの作成
    instance_ = Direct3DCreate9(D3D_SDK_VERSION);
    if (nullptr == instance_)
    {
        MessageBox(NULL, L"Direct3Dオブジェクトの作成に失敗！", L"RenderSystemDirectX9", MB_ICONWARNING);
        return false;
    }

    // 現在のディスプレイモードを取得
    if (FAILED(instance_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
    {
        MessageBox(NULL, L"現在のディスプレイモードを取得に失敗！", L"RenderSystemDirectX9", MB_ICONWARNING);
        Release();
        return false;
    }

    // デバイスのプレゼンテーションパラメータの設定
    ZeroMemory(&present_parameters, sizeof(present_parameters));                 // ワークをゼロクリア
    present_parameters.BackBufferCount            = 1;                           // バックバッファの数
    present_parameters.BackBufferWidth            = SCREEN_WIDTH;                // ゲーム画面サイズ(幅)
    present_parameters.BackBufferHeight           = SCREEN_HEIGHT;               // ゲーム画面サイズ(高さ)
    present_parameters.BackBufferFormat           = display_mode.Format;         // カラーモードの指定
    present_parameters.SwapEffect                 = D3DSWAPEFFECT_DISCARD;       // 映像信号に同期してフリップする
    present_parameters.EnableAutoDepthStencil     = TRUE;                        // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
    present_parameters.AutoDepthStencilFormat     = D3DFMT_D24S8;                // デプスバッファとして16bitを使う
    present_parameters.Windowed                   = is_window_mode;              // ウィンドウモード
    present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // リフレッシュレート
    present_parameters.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; // インターバル
    //present_parameters.MultiSampleType          = D3DMULTISAMPLE_8_SAMPLES;    // 抗劇歯アンチエイジングの使用

    // デバイスの生成
    // ディスプレイアダプタを表すためのデバイスを作成
    // 描画と頂点処理をハードウェアで行なう
    if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
        &present_parameters, &device_)))
    {
        // 上記の設定が失敗したら
        // 描画をハードウェアで行い、頂点処理はCPUで行なう
        if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
            &present_parameters, &device_)))
        {
            // 上記の設定が失敗したら
            // 描画と頂点処理をCPUで行なう
            if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF, hwnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                &present_parameters, &device_)))
            {
                // 生成失敗
                MessageBox(NULL, L"デバイスの生成に失敗！", L"RenderSystemDirectX9", MB_ICONWARNING);
                Release();
                return false;
            }
        }
    }

    // 抗劇歯
    //if (FAILED(instance_->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
    //    D3DDEVTYPE_HAL,
    //    D3DFMT_R8G8B8,
    //    FALSE,
    //    D3DMULTISAMPLE_2_SAMPLES,
    //    NULL)))
    //{
    //    //
    //}

    return true;
}

//--------------------------------------------------------------------------------
//  バーテックスデクラレーションの初期化
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitVertexDeclaration(void)
{
    // 2d
    D3DVERTEXELEMENT9 elements_2d[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        { 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        D3DDECL_END()
    };
    device_->CreateVertexDeclaration(elements_2d, &vertex_declaration_2d_);

    // 3d
    D3DVERTEXELEMENT9 elements_3d[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        { 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        D3DDECL_END()
    };
    device_->CreateVertexDeclaration(elements_3d, &vertex_declaration_3d_);

    // 3dSkin
    D3DVERTEXELEMENT9 elements_3d_skin[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
        { 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
        { 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 56, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
        { 0, 60, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1 },
        { 0, 64, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 2 },
        { 0, 68, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 3 },
        { 0, 72, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 4 },
        { 0, 76, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
        { 0, 84, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1 },
        { 0, 92, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 2 },
        { 0, 100, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 3 },
        { 0, 108, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 4 },
        D3DDECL_END()
    };
    device_->CreateVertexDeclaration(elements_3d_skin, &vertex_declaration_3d_skin_);
}

//--------------------------------------------------------------------------------
//  レンダーステートの初期化
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitRenderSate(void)
{
    device_->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // αブレンドを行う
    device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // αソースカラーの指定
    device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // αデスティネーションカラーの指定
    device_->SetRenderState(D3DRS_ALPHAREF, static_cast<DWORD>(0x00000001)); // αテストに関する設定
    device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL); // αテストに関する設定
    SetRenderState(CullMode::kCcw);
    SetRenderState(FillMode::kSolid);
    SetRenderState(FogMode::kFogOff);
    SetRenderState(AlphaMode::kAlphaNone);
}
#endif