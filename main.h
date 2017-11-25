//--------------------------------------------------------------------------------
//�@main.h
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "common_setting.h"

// KF�֐�
#include "kf_math.h"
#include "kf_utility.h"
using namespace KodFreedom;

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH		(1280)								//�E�C���h�E��
#define SCREEN_HEIGHT		(720)								//�E�C���h�E����
#define FRAME_PER_SECOND	(60)								
#define TIMER_INTERVAL		(1000.0f / FRAME_PER_SECOND)		//�X�V�Ԋu
#define DELTA_TIME			(TIMER_INTERVAL * 0.001f)			//�X�V�Ԋu(�b�P��)


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
	//	�֐����FWinMain
	//  �֐������F���C���֐�
	//	�����F	instance
	//			previous_instance
	//			cmd_line
	//			cmd_show
	//	�߂�l�Fint
	//--------------------------------------------------------------------------------
	static int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show);
private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	Main() {}
	Main(const Main& value) {}
	~Main() {}
	Main& operator=(const Main& value) {}

	//--------------------------------------------------------------------------------
	//	�֐����FWndProc
	//  �֐������F�E�C���h�E�v���V�[�W���֐�
	//	�����F	hwnd�F�E�C���h�E�̃n���h��
	//			message�F���b�Z�[�W�̎��ʎq
	//			wparam�F���b�Z�[�W�̍ŏ��̃p�����[�^
	//			lparam�F���b�Z�[�W�̓�Ԗڂ̃p�����[�^
	//	�߂�l�FLRESULT
	//--------------------------------------------------------------------------------
	static LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	
	//--------------------------------------------------------------------------------
	//	�֐����FCloseApp
	//  �֐������F�A�v�������m�F�֐�
	//	�����F	hwnd�F�E�C���h�E�̃n���h��
	//	�߂�l�FLRESULT
	//--------------------------------------------------------------------------------
	static void CloseApp(HWND hwnd);
};
