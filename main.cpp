//--------------------------------------------------------------------------------
//
//�@�L�[�t���[����ՃI�u�W�F�N�g
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
#include "main.h"
#include "main_system.h"
#include "input.h"

#if defined(_DEBUG) || defined(EDITOR)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "ImGui\imgui_impl_dx9.h"
#endif
#endif

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CLASS_NAME			L"KF_Framework"
#define WINDOW_NAME			L"�t���[�����[�N"

//--------------------------------------------------------------------------------
//	extern�֐�
//--------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(EDITOR)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#endif

//--------------------------------------------------------------------------------
//  �֐���`
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void CloseApp(HWND hwnd);

//--------------------------------------------------------------------------------
//	�֐����FWinMain
//  �֐������F���C���֐�
//	�����F	instance
//			previous_instance
//			cmd_line
//			cmd_show
//	�߂�l�Fint
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show)
{
	UNREFERENCED_PARAMETER(previous_instance);
	UNREFERENCED_PARAMETER(cmd_line);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEX�̃������T�C�Y���w��
	wcex.style = CS_CLASSDC;							//�\������E�C���h�E�̃X�^�C����ݒ�
	wcex.lpfnWndProc = WndProc;							//�֐��|�C���^�A�E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
	wcex.cbClsExtra = 0;								//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.cbWndExtra = 0;								//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.hInstance = instance;							//WinMain�̃p�����[�^�̃C���X�^���X�n���h��
	wcex.hIcon = NULL;									//����icon�o���A�g�p����A�C�R�����w��iWindows�������Ă���j
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//�}�E�X�J�[�\�����w��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
	wcex.lpszMenuName = NULL;							//Window�ɂ��郁�j���[��ݒ�i����Əd���j
	wcex.lpszClassName = CLASS_NAME;					//�E�C���h�E�N���X�̖��O
	wcex.hIconSm = NULL;								//�g�����ꂽ�����i�~�jicon�j�A�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	RegisterClassEx(&wcex);								//�E�C���h�E�N���X�̓o�^

	HWND hwnd;
	MSG message;
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT real_rect;

	//window size
	AdjustWindowRect(&rect, style, false);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//window pos
	GetWindowRect(GetDesktopWindow(), &real_rect);
	int real_width = real_rect.right - real_rect.left;
	int real_height = real_rect.bottom - real_rect.top;

	//��ʈ�o�h�~
	int window_x = width > real_width ? 0 : (real_width - width) / 2;
	int window_y = height > real_height ? 0 : (real_height - height) / 2;

	//�E�C���h�E���쐬
	hwnd = CreateWindowEx(
		0,				//�g���E�C���h�E�X�^�C��
		CLASS_NAME,		//�N���X�̖��O
		WINDOW_NAME,	//�E�C���h�E�̖��O
		style,			//**important**window type,�E�C���h�E�̃X�^�C��
		window_x,		//�E�C���h�E������WX
		window_y,		//�E�C���h�E������WY
		width,			//���i�E�C���h�E�S�́j
		height,			//�����i�E�C���h�E�S�́j
		NULL,			//�e�E�B���h�E�̃n���h��
		NULL,			//���j���[�n���h���܂��͎q�E�C���h�EID
		instance,		//�C���X�^���X�n���h��
		NULL);			//�E�C���h�E�쐬�f�[�^

	//�t���X�N���[��
#ifdef _DEBUG
	bool is_window_mode = true;
#else
	bool is_window_mode = true;
	UINT id = MessageBox(hWnd, "�t���X�N���[�����[�h�ŋN�����܂����H", "�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
	if (id == IDYES) { is_window_mode = false; }
#endif // _DEBUG

	//MainSystem����
	if (!MainSystem::Create(instance, hwnd, is_window_mode))
	{
		UnregisterClass(CLASS_NAME, wcex.hInstance);
		return -1;
	}

	//�E�C���h�E�̕\��
	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd);

	//���ԃJ�E���^
	LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER execLastTime;
	LARGE_INTEGER FPSLastTime;
	memset(&frequency, 0x00, sizeof frequency);
	memset(&currentTime, 0x00, sizeof currentTime);
	memset(&execLastTime, 0x00, sizeof execLastTime);
	memset(&FPSLastTime, 0x00, sizeof FPSLastTime);
	QueryPerformanceCounter(&execLastTime);
	FPSLastTime = execLastTime;

	//���b�Z�[�W���[�v
	for (;;)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//window����
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&message);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&message);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&currentTime);
			float fTime = (float)(currentTime.QuadPart - execLastTime.QuadPart) * 1000.0f / (float)frequency.QuadPart;
			
			if (fTime >= TIMER_INTERVAL)
			{
				execLastTime = currentTime;
				MainSystem::Instance()->Update();
				MainSystem::Instance()->LateUpdate();
				MainSystem::Instance()->Render();
			}
		}
	}

	// �I������
	MainSystem::Release();

	//�E�C���h�E�N���X�̓o�^������
	//�������F���N���X��
	//�������F�A�v���P�[�V�����C���X�^��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)message.wParam;
}

//--------------------------------------------------------------------------------
//	�֐����FWndProc
//  �֐������F�E�C���h�E�v���V�[�W���֐�
//	�����F	hwnd�F�E�C���h�E�̃n���h��
//			message�F���b�Z�[�W�̎��ʎq
//			wparam�F���b�Z�[�W�̍ŏ��̃p�����[�^
//			lparam�F���b�Z�[�W�̓�Ԗڂ̃p�����[�^
//	�߂�l�FLRESULT
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
#if defined(_DEBUG) || defined(EDITOR)
	ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam);
#endif // _DEBUG

	switch (message) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wparam) == VK_ESCAPE) { CloseApp(hwnd); }
		break;
	case WM_CLOSE:
		CloseApp(hwnd);
		return 0;
	case WM_DESTROY:		//�ۑ����邩�`�F�b�N�̎d�g�݂�����
		PostQuitMessage(0);	//WM_QUIT�Ƃ������b�Z�[�W���Ă�
		break;
	case WM_ACTIVATEAPP:
	{
		auto input = MainSystem::Instance()->GetInput();
		if (input)
		{
			if (GetActiveWindow())
			{
				input->Acquire();
			}
			else
			{
				input->Unacquire();
			}
		}
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

//--------------------------------------------------------------------------------
//	�֐����FCloseApp
//  �֐������F�A�v�������m�F�֐�
//	�����F	hwnd�F�E�C���h�E�̃n���h��
//	�߂�l�FLRESULT
//--------------------------------------------------------------------------------
void CloseApp(HWND hwnd)
{
	//�I���m�F
	UINT id = MessageBox(hwnd, L"�I�����܂����H", L"�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//��������
	if (id == IDYES) {
		//WM_DESTROY���b�Z�[�W�𑗐M
		DestroyWindow(hwnd);
	}
}