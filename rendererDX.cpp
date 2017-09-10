//--------------------------------------------------------------------------------
//
//�@rendererDX.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

#ifdef _DEBUG
#include "manager.h"
#include "inputManager.h"
#include "inputDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CRendererDX::CRendererDX()
	: m_pD3D(nullptr)
	, m_pD3DDevice(nullptr)
{
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CRendererDX::~CRendererDX()
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool CRendererDX::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return false;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ���[�N���[���N���A
	d3dpp.BackBufferCount				= 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// �J���[���[�h�̎w��
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil		= TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed						= bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	//�R����
	//d3dpp.MultiSampleType				= D3DMULTISAMPLE_8_SAMPLES;		// �A���`�G�C�W���O�̎g�p

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return false;
			}
		}
	}

	//�R����
	//if (FAILED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	D3DFMT_R8G8B8,
	//	FALSE,
	//	D3DMULTISAMPLE_2_SAMPLES,
	//	NULL)))
	//{
	//	//
	//}

	// �����_�[�X�e�[�g�̐ݒ�
	SetRenderMode(RM_NORMAL);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	
	//�R����
	//m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);			// �A���`�G�C�W���O

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �ŏ��̃A���t�@����(�����l��D3DTA_TEXTURE�A�e�N�X�`�����Ȃ��ꍇ��D3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����(�����l��D3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// �Q�Ԗڂ̃A���t�@����(�����l��D3DTA_CURRENT)

	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CRendererDX::Release(void)
{
	// �f�o�C�X�̔j��
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(m_pD3D);

	delete this;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CRendererDX::Update(void)
{
#ifdef _DEBUG
	CKeyboardDX *pKeyboard = CMain::GetManager()->GetInputManager()->GetKeyboard();
	
	if (pKeyboard->GetKeyPress(DIK_F8))
	{
		SetRenderMode(RM_WIREFRAME);
	}
	else
	{
		SetRenderMode(RM_NORMAL);
	}

#endif
}

//--------------------------------------------------------------------------------
//  �`��J�n
//--------------------------------------------------------------------------------
bool CRendererDX::BeginDraw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 0, 128, 255), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (FAILED(m_pD3DDevice->BeginScene())) { return false; }

	return true;
}

//--------------------------------------------------------------------------------
//  �`��I��
//--------------------------------------------------------------------------------
void CRendererDX::EndDraw(void)
{
	// Direct3D�ɂ��`��̏I��
	m_pD3DDevice->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------------------------------------
// �����_�[���[�h�ݒ�
//--------------------------------------------------------------------------------
void CRendererDX::SetRenderMode(const RENDER_MODE &rm)
{
	switch (rm)
	{
	case RM_NORMAL:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		//�w�ʂ������ŃJ�����O����
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//�h��Ԃ�
		break;
	case RM_WIREFRAME:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		//�w�ʂ̃J�����O�͂��Ȃ�
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);//���C���[�t���[��
		break;
	default:
		break;
	}
}

#endif