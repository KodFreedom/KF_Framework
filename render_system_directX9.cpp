//--------------------------------------------------------------------------------
//  renderer_directX9.h
//�@�����_�[�V�X�e��(directx9)
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#include "meshManager.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "ImGui\imgui_impl_dx9.h"
#endif

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`�揈��(3D)
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Render(const string& mesh_name)
{
	auto mesh = MeshManager::Instance()->GetMesh(mesh_name);
	device_->SetVertexDeclaration(vertex_declaration_3d_);
	device_->SetStreamSource(0, mesh->VertexBuffer, 0, sizeof(Vertex3d));
	device_->SetIndices(mesh->IndexBuffer);
	device_->DrawIndexedPrimitive(static_cast<_D3DPRIMITIVETYPE>(mesh->CurrentType),
		0, 0, mesh->VertexNumber, 0, mesh->PolygonNumber);
}

//--------------------------------------------------------------------------------
// �J�����O���[�h�̐ݒ�
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const CullMode& value)
{
	device_->SetRenderState(D3DRS_CULLMODE, static_cast<_D3DCULL>(value + 1));
}

//--------------------------------------------------------------------------------
//  �t�B�����[�h�̐ݒ�
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const FillMode& value)
{
	device_->SetRenderState(D3DRS_FILLMODE, static_cast<_D3DFILLMODE>(value + 1));
}

//--------------------------------------------------------------------------------
//  �����[�h�̐ݒ�
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::SetRenderState(const AlphaMode& value)
{
	device_->SetRenderState(D3DRS_ALPHATESTENABLE, value == kAlphaTest ? TRUE : FALSE);
}

//--------------------------------------------------------------------------------
//  �t�H�O���[�h�̐ݒ�
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
//  ������
//--------------------------------------------------------------------------------
bool RenderSystemDirectX9::Init(HWND hwnd, BOOL is_window_mode)
{
	if (!CreateDevice(hwnd, is_window_mode)) return false;
	InitVertexDeclaration();
	InitRenderSate();
	InitSamplerState();
	InitFog();
#if defined(_DEBUG) || defined(EDITOR)
	ImGui_ImplDX9_Init(hwnd, device_);
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::Uninit(void)
{
#if defined(_DEBUG) || defined(EDITOR)
	ImGui_ImplDX9_Shutdown();
#endif
	SAFE_RELEASE(vertex_declaration_2d_);
	SAFE_RELEASE(vertex_declaration_3d_);
	SAFE_RELEASE(device_);
	SAFE_RELEASE(instance_);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
bool RenderSystemDirectX9::CreateDevice(HWND hwnd, BOOL is_window_mode)
{
	D3DPRESENT_PARAMETERS present_parameters;
	D3DDISPLAYMODE display_mode;

	// Direct3D�I�u�W�F�N�g�̍쐬
	instance_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == instance_)
	{
		MessageBox(NULL, L"Direct3D�I�u�W�F�N�g�̍쐬�Ɏ��s�I", L"RenderSystemDirectX9", MB_ICONWARNING);
		return false;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(instance_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
	{
		MessageBox(NULL, L"���݂̃f�B�X�v���C���[�h���擾�Ɏ��s�I", L"RenderSystemDirectX9", MB_ICONWARNING);
		Release();
		return false;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&present_parameters, sizeof(present_parameters));					// ���[�N���[���N���A
	present_parameters.BackBufferCount				= 1;							// �o�b�N�o�b�t�@�̐�
	present_parameters.BackBufferWidth				= SCREEN_WIDTH;					// �Q�[����ʃT�C�Y(��)
	present_parameters.BackBufferHeight				= SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	present_parameters.BackBufferFormat				= display_mode.Format;			// �J���[���[�h�̎w��
	present_parameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	present_parameters.EnableAutoDepthStencil		= TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	present_parameters.AutoDepthStencilFormat		= D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	present_parameters.Windowed						= is_window_mode;				// �E�B���h�E���[�h
	present_parameters.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	present_parameters.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	//present_parameters.MultiSampleType				= D3DMULTISAMPLE_8_SAMPLES;		// �R�����A���`�G�C�W���O�̎g�p

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&present_parameters, &device_)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&present_parameters, &device_)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(instance_->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hwnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&present_parameters, &device_)))
			{
				// �������s
				MessageBox(NULL, L"�f�o�C�X�̐����Ɏ��s�I", L"RenderSystemDirectX9", MB_ICONWARNING);
				Release();
				return false;
			}
		}
	}

	// �R����
	//if (FAILED(instance_->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	D3DFMT_R8G8B8,
	//	FALSE,
	//	D3DMULTISAMPLE_2_SAMPLES,
	//	NULL)))
	//{
	//	//
	//}

	return true;
}

//--------------------------------------------------------------------------------
//  �o�[�e�b�N�X�f�N�����[�V�����̏�����
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitVertexDeclaration(void)
{
	// 2d
	D3DVERTEXELEMENT9 elements_2d[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	device_->CreateVertexDeclaration(elements_2d, &vertex_declaration_2d_);

	// 3d
	D3DVERTEXELEMENT9 elements_3d[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	device_->CreateVertexDeclaration(elements_3d, &vertex_declaration_3d_);
}

//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g�̏�����
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitRenderSate(void)
{
	device_->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // ���u�����h���s��
	device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // ���\�[�X�J���[�̎w��
	device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // ���f�X�e�B�l�[�V�����J���[�̎w��
	device_->SetRenderState(D3DRS_ALPHAREF, static_cast<DWORD>(0x00000001)); // ���e�X�g�Ɋւ���ݒ�
	device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL); // ���e�X�g�Ɋւ���ݒ�
	SetRenderState(CullMode::kCcw);
	SetRenderState(FillMode::kSolid);
	SetRenderState(FogMode::kFogOff);
	SetRenderState(AlphaMode::kAlphaNone);
}

//--------------------------------------------------------------------------------
//  �T���v���[�X�e�[�g�̏�����
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitSamplerState(void)
{
	device_->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	device_->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�
}

//--------------------------------------------------------------------------------
//  Fog�̏�����
//--------------------------------------------------------------------------------
void RenderSystemDirectX9::InitFog(void)
{
	// �t�H�O��L���ɂ���
	device_->SetRenderState(D3DRS_FOGENABLE, TRUE);
	
	// �t�H�O�J���[�ݒ�
	device_->SetRenderState(D3DRS_FOGCOLOR, fog_color_);
	
	// �o�[�e�b�N�X�t�H�O(���`����)���g�p
	device_->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	
	// �t�H�O�͈͐ݒ�
	device_->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&fog_start_z_)));
	device_->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&fog_end_z_)));
	
	// �s�N�Z���t�H�O(�w���̂Q )���g�p
	device_->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP2);
	
	// �t�H�O���x�ݒ�
	device_->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&fog_density_)));
	
	// �͈̓x�[�X�̃t�H�O���g�p
	device_->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}
#endif