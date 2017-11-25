//--------------------------------------------------------------------------------
//
//�@main.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "common_setting.h"

// KF�֐�
#include "KF_Math.h"
#include "KF_Utility.h"
using namespace KodFreedom;

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH		(1280)								//�E�C���h�E��
#define SCREEN_HEIGHT		(720)								//�E�C���h�E����
#define FRAME_PER_SECOND	(60)								
#define TIMER_INTERVAL		(1000.0f / FRAME_PER_SECOND)		//�X�V�Ԋu
#define DELTA_TIME			(TIMER_INTERVAL * 0.001f)			//�X�V�Ԋu(�b�P��)
#define CLASS_NAME			"KF_Framework"						//�N���X�̖��O
#define WINDOW_NAME			"�t���[�����[�N"					//�E�C���h�E�̖��O

#ifdef USING_DIRECTX
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//���_�t�H�[�}�b�g
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//3D���_�t�H�[�}�b�g
#endif											

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
struct VERTEX_2D
{// 2D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��
	Vector3			Position;
	float			Rhw;
	unsigned long	Color;
	Vector2			UV;
};

struct VERTEX_3D
{// 3D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��
	Vector3			Position;
	Vector3			Normal;
	unsigned long	Color;
	Vector2			UV;
};

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class Main
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static int				WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int cmdShow);
private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	Main() {}
	~Main() {}

	static LRESULT CALLBACK	wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void				closeApp(HWND hWnd);
};
