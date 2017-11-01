//--------------------------------------------------------------------------------
//
//�@rendererDirectX9.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "rendererDirectX9.h"

#ifdef _DEBUG
#include "manager.h"
#include "inputManager.h"
#include "inputDX.h"
#endif

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FBeginRender
//  �֐������F�`��J�n
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool RendererDirectX9::BeginRender(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	lpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), backgroundColor, 1.0f, 0);
	return (bool)lpD3DDevice->BeginScene();
}

//--------------------------------------------------------------------------------
//	�֐����FEndRender
//  �֐������F�`��I��
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void RendererDirectX9::EndRender(void)
{
	lpD3DDevice->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	lpD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------------------------------------
//	�֐����FSetWireFrameFlag
//  �֐������F�����_�[���[�h�ݒ�
//	�����F	isEnable�FWireFrame�t���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void RendererDirectX9::SetWireFrameFlag(const bool& isEnable)
{
	Renderer::SetWireFrameFlag(isEnable);
	lpD3DDevice->SetRenderState(D3DRS_FILLMODE, isEnable ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool RendererDirectX9::init(HWND hWnd, BOOL isWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	lpDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == lpDirect3D9) return false;

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(lpDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) return false;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ���[�N���[���N���A
	d3dpp.BackBuffer++count				= 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// �J���[���[�h�̎w��
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil		= TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed						= isWindowMode;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	
	// �R����
	//d3dpp.MultiSampleType				= D3DMULTISAMPLE_8_SAMPLES;		// �A���`�G�C�W���O�̎g�p

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &lpD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &lpD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &lpD3DDevice)))
			{
				// �������s
				return false;
			}
		}
	}

	// �R����
	//if (FAILED(lpDirect3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	D3DFMT_R8G8B8,
	//	FALSE,
	//	D3DMULTISAMPLE_2_SAMPLES,
	//	NULL)))
	//{
	//	//
	//}

	SetWireFrameFlag(false);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void RendererDirectX9::uninit(void)
{
	SAFE_RELEASE(lpD3DDevice);
	SAFE_RELEASE(lpDirect3D9);
}
#endif