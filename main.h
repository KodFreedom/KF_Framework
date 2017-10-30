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
#include <Windows.h>
#include <stdio.h>

//STL
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <sstream>
#include <assert.h>
using namespace std;

//OpenMP
#include <omp.h>

//KF�֐�
#include "KF_Math.h"
#include "KF_Utility.h"

//DirectX�n
#ifdef USING_DIRECTX
#include <d3dx9.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound
#endif

//--------------------------------------------------------------------------------
//  ���C�u�����ǂݍ���
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#endif

#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v

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
//  �O���錾
//--------------------------------------------------------------------------------
class CManager;

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct
{
	Vector3			vPos;	//xyz
	float			fRhw;	//rhw
	unsigned long	ulColor;//�F
	Vector2			vUV;	//�e�N�X�`��
}VERTEX_2D;//2D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��

typedef struct//���_���\���́@��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��
{
	Vector3			vPos;	//xyz
	Vector3			vNormal;//�@��
	unsigned long	ulColor;//�F
	Vector2			vUV;	//uv
}VERTEX_3D;//3D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CMain
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static int				Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow);

	//Get�֐�
	static CManager*		GetManager(void) { return m_pManager; }
private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CMain() {}
	~CMain() {}

	static LRESULT CALLBACK	wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void				closeApp(HWND hWnd);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	static CManager*		m_pManager;
};
