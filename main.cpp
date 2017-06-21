//--------------------------------------------------------------------------------
//
//�@�L�[�t���[����ՃI�u�W�F�N�g
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				CloseApp(HWND hWnd);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
CManager*	g_pManager = NULL;	//Game Manager

#ifdef _DEBUG
int			g_nCntFPS = 0;		//FPS�J�E���^
#endif

//--------------------------------------------------------------------------------
//  main�֐�
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(IpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEX�̃������T�C�Y���w��
	wcex.style = CS_CLASSDC;							//�\������E�C���h�E�̃X�^�C����ݒ�
	wcex.lpfnWndProc = WndProc;							//�֐��|�C���^�A�E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
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

	//Manager����
	g_pManager = new CManager;

	if (g_pManager->Init(hInstance, hWnd, true) == KF_FAILED)
	{
		return -1;
	};

	//FPS
	DWORD dwFrameCount;
	DWORD dwCurrentTime;//������
	DWORD dwExecLastTime;//���s�I��������
	DWORD dwFPSLastTime;

	//����\��ݒ�
	timeBeginPeriod(1);

	//�e�J�E���^�[������
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();//�V�X�e���������~���b�P�ʂŎ擾

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= TIMER_INTERVAL)
			{
				dwExecLastTime = dwCurrentTime;//������������

#ifdef _DEBUG
				if ((dwCurrentTime - dwFPSLastTime) >= 500)//0.5�b���ƂɎ��s
				{
					g_nCntFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;
					dwFrameCount = 0;
				}
#endif//_DEBUG

				// �X�V����
				g_pManager->Update();
				g_pManager->LateUpdate();

				// �`�揈��
				g_pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// �I������
	if (g_pManager != NULL)
	{
		g_pManager->Uninit();
		delete g_pManager;
		g_pManager = NULL;
	}

	//�E�C���h�E�N���X�̓o�^������
	//�������F���N���X��
	//�������F�A�v���P�[�V�����C���X�^��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------
//  �E�C���h�E�v���V�[�W���֐�
//  (CALLBACK : Win32API�֐����Ăяo�����̋K��)
//  hWnd ; �E�C���h�E�̃n���h��
//  uMsg : ���b�Z�[�W�̎��ʎq
//  wPara : ���b�Z�[�W�̍ŏ��̃p�����[�^
//  iParam : ���b�Z�[�W�̓�Ԗڂ̃p�����[�^
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wParam) == VK_ESCAPE) { CloseApp(hWnd); }
		break;
	case WM_CLOSE:
		CloseApp(hWnd);
		return 0;
	case WM_DESTROY:		//�ۑ����邩�`�F�b�N�̎d�g�݂�����
		PostQuitMessage(0);	//WM_QUIT�Ƃ������b�Z�[�W���Ă�
		break;
	case WM_ACTIVATEAPP:
		if (g_pManager)
		{
			BOOL bActive = (BOOL)GetActiveWindow();
			CInputManager* pInput = g_pManager->GetInputManager();

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
//  close�m�F�֐�
//--------------------------------------------------------------------------------
void CloseApp(HWND hWnd)
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

//--------------------------------------------------------------------------------
//  Game Manager�擾
//--------------------------------------------------------------------------------
CManager *GetManager(void)
{
	return g_pManager;
}

//--------------------------------------------------------------------------------
//  FPS�J�E���^�[�擾
//--------------------------------------------------------------------------------
#ifdef _DEBUG
int GetCountFPS(void)
{
	return g_nCntFPS;
}
#endif