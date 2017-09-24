//--------------------------------------------------------------------------------
//
//�@�L�[�t���[����ՃI�u�W�F�N�g
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"

#ifdef _DEBUG
#include "debugManager.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx9.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ��錾
//--------------------------------------------------------------------------------
CManager*	CMain::m_pManager = nullptr;

//--------------------------------------------------------------------------------
//	extern�֐�
//--------------------------------------------------------------------------------
#ifdef _DEBUG
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

//--------------------------------------------------------------------------------
//	���C���֐�
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	return CMain::Main(hInstance, hPrevInstance, IpCmdLine, nCmdShow);
}

//--------------------------------------------------------------------------------
//	�N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FWinMain
//  �֐������F���C���֐�
//	�����F	hInstance
//			hPrevInstance
//			IpCmdLine
//			nCmdShow
//	�߂�l�Fint
//--------------------------------------------------------------------------------
int CMain::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(IpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEX�̃������T�C�Y���w��
	wcex.style = CS_CLASSDC;							//�\������E�C���h�E�̃X�^�C����ݒ�
	wcex.lpfnWndProc = wndProc;							//�֐��|�C���^�A�E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
	wcex.cbClsExtra = 0;								//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.cbWndExtra = 0;								//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.hInstance = hInstance;							//WinMain�̃p�����[�^�̃C���X�^���X�n���h��
	wcex.hIcon = NULL;									//����icon�o���A�g�p����A�C�R�����w��iWindows�������Ă���j
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//�}�E�X�J�[�\�����w��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
	wcex.lpszMenuName = NULL;							//Window�ɂ��郁�j���[��ݒ�i����Əd���j
	wcex.lpszClassName = CLASS_NAME;					//�E�C���h�E�N���X�̖��O
	wcex.hIconSm = NULL;								//�g�����ꂽ�����i�~�jicon�j�A�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	RegisterClassEx(&wcex);								//�E�C���h�E�N���X�̓o�^

	HWND hWnd;
	MSG msg;
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT cr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT dr;

	//window size
	AdjustWindowRect(&cr, style, false);
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//window pos
	GetWindowRect(GetDesktopWindow(), &dr);
	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	//��ʈ�o�h�~
	int wx = ww > dw ? 0 : (dw - ww) / 2;
	int wy = wh > dh ? 0 : (dh - wh) / 2;

	//�E�C���h�E���쐬
	hWnd = CreateWindowEx(
		0,				//�g���E�C���h�E�X�^�C��
		CLASS_NAME,		//�N���X�̖��O
		WINDOW_NAME,	//�E�C���h�E�̖��O
		style,			//**important**window type,�E�C���h�E�̃X�^�C��
		wx,				//�E�C���h�E������WX
		wy,				//�E�C���h�E������WY
		ww,				//���i�E�C���h�E�S�́j
		wh,				//�����i�E�C���h�E�S�́j
		NULL,			//�e�E�B���h�E�̃n���h��
		NULL,			//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,		//�C���X�^���X�n���h��
		NULL);			//�E�C���h�E�쐬�f�[�^

	//�t���X�N���[��
#ifdef _DEBUG
	bool bWindow = true;
#else
	bool bWindow = true;
	UINT nID = MessageBox(hWnd, "�t���X�N���[�����[�h�ŋN�����܂����H", "�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
	if (nID == IDYES) { bWindow = false; }
#endif // _DEBUG

	//Manager����
	m_pManager = new CManager;
	if (!m_pManager->Init(hInstance, hWnd, bWindow))
	{
		return -1;
	};

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���ԃJ�E���^
	LARGE_INTEGER nFrequency;
	LARGE_INTEGER nCurrentTime;
	LARGE_INTEGER nExecLastTime;
	LARGE_INTEGER nFPSLastTime;
	memset(&nFrequency, 0x00, sizeof nFrequency);
	memset(&nCurrentTime, 0x00, sizeof nCurrentTime);
	memset(&nExecLastTime, 0x00, sizeof nExecLastTime);
	memset(&nFPSLastTime, 0x00, sizeof nFPSLastTime);
	QueryPerformanceCounter(&nExecLastTime);
	nFPSLastTime = nExecLastTime;

	//���b�Z�[�W���[�v
	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//window����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			QueryPerformanceFrequency(&nFrequency);
			QueryPerformanceCounter(&nCurrentTime);
			float fTime = (float)(nCurrentTime.QuadPart - nExecLastTime.QuadPart) * 1000.0f / (float)nFrequency.QuadPart;
			
			if (fTime >= TIMER_INTERVAL)
			{
				nExecLastTime = nCurrentTime;

				// �X�V����
				m_pManager->Update();
				m_pManager->LateUpdate();

				// �`�揈��
				m_pManager->Draw();
			}
		}
	}

	// �I������
	if (m_pManager)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}

	//�E�C���h�E�N���X�̓o�^������
	//�������F���N���X��
	//�������F�A�v���P�[�V�����C���X�^��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------
//	�֐����FwndProc
//  �֐������F�E�C���h�E�v���V�[�W���֐�
//	�����F	hWnd�F�E�C���h�E�̃n���h��
//			uMsg�F���b�Z�[�W�̎��ʎq
//			wParam�F���b�Z�[�W�̍ŏ��̃p�����[�^
//			lParam�F���b�Z�[�W�̓�Ԗڂ̃p�����[�^
//	�߂�l�FLRESULT
//--------------------------------------------------------------------------------
LRESULT CALLBACK CMain::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam);
#endif // _DEBUG

	switch (uMsg) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wParam) == VK_ESCAPE) { closeApp(hWnd); }
		break;
	case WM_CLOSE:
		closeApp(hWnd);
		return 0;
	case WM_DESTROY:		//�ۑ����邩�`�F�b�N�̎d�g�݂�����
		PostQuitMessage(0);	//WM_QUIT�Ƃ������b�Z�[�W���Ă�
		break;
	case WM_ACTIVATEAPP:
		if (m_pManager)
		{
			BOOL bActive = (BOOL)GetActiveWindow();
			auto pInput = m_pManager->GetInputManager();

			if (bActive)
			{
				pInput->Acquire();
			}
			else
			{
				pInput->Unacquire();
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------------------------
//	�֐����FcloseApp
//  �֐������F�A�v�������m�F�֐�
//	�����F	hWnd�F�E�C���h�E�̃n���h��
//	�߂�l�FLRESULT
//--------------------------------------------------------------------------------
void CMain::closeApp(HWND hWnd)
{
	UINT nID = 0;//���b�Z�[�Wbox�߂�l

	//�I���m�F
	nID = MessageBox(hWnd, "�I�����܂����H", "�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//��������
	if (nID == IDYES) {
		//WM_DESTROY���b�Z�[�W�𑗐M
		DestroyWindow(hWnd);
	}
}