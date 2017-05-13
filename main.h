//--------------------------------------------------------------------------------
//
//�@main.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <Windows.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound

#include "KF_Math.h"
#include "KF_UtilityDX.h"

//--------------------------------------------------------------------------------
//  ���C�u�����ǂݍ���
//--------------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH	(800)															//�E�C���h�E��
#define SCREEN_HEIGHT	(600)															//�E�C���h�E����
#define TIMER_INTERVAL	(1000/60)														//�X�V�Ԋu
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//���_�t�H�[�}�b�g
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//3D���_�t�H�[�}�b�g
#define CLASS_NAME		"KF_Framework"													//�N���X�̖��O
#define WINDOW_NAME		"�t���[�����[�N"												//�E�C���h�E�̖��O

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CManager;

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct
{
	CKFVec3			vPos;	//xyz
	float			fRhw;	//rhw
	unsigned long	ulColor;//�F
	CKFVec2			vUV;	//�e�N�X�`��
}VERTEX_2D;//2D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��

typedef struct//���_���\���́@��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��
{
	CKFVec3			vPos;	//xyz
	CKFVec3			vNormal;//�@��
	unsigned long	ulColor;//�F
	CKFVec2			vUV;	//uv
}VERTEX_3D;//3D���_���\���́@���_�t�H�[�}�b�g�ƍ\�������킹�邱��

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
CManager *GetManager(void);

#ifdef _DEBUG
int GetCountFPS(void);
#endif

#endif
