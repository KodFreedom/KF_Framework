//--------------------------------------------------------------------------------
//
//　main.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>

// STL
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <sstream>
#include <assert.h>
using namespace std;

// OpenMP
#include <omp.h>

// KF関数
#include "KF_Math.h"
#include "KF_Utility.h"
using namespace KF;

// DirectX系
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound

//--------------------------------------------------------------------------------
//  ライブラリ読み込み
//--------------------------------------------------------------------------------
// DirectX系
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

// システム時刻取得に必要
#pragma comment(lib,"winmm.lib")	

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH		(1280)								//ウインドウ幅
#define SCREEN_HEIGHT		(720)								//ウインドウ高さ
#define FRAME_PER_SECOND	(60)								
#define TIMER_INTERVAL		(1000.0f / FRAME_PER_SECOND)		//更新間隔
#define DELTA_TIME			(TIMER_INTERVAL * 0.001f)			//更新間隔(秒単位)
#define CLASS_NAME			"KF_Framework"						//クラスの名前
#define WINDOW_NAME			"フレームワーク"					//ウインドウの名前

#ifdef USING_DIRECTX
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//頂点フォーマット
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//3D頂点フォーマット
#endif											

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct VERTEX_2D
{// 2D頂点情報構造体　頂点フォーマットと構造を合わせること
	Vector3			Position;
	float			Rhw;
	unsigned long	Color;
	Vector2			UV;
};

struct VERTEX_3D
{// 3D頂点情報構造体　頂点フォーマットと構造を合わせること
	Vector3			Position;
	Vector3			Normal;
	unsigned long	Color;
	Vector2			UV;
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Main
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static int				WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int cmdShow);
private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Main() {}
	~Main() {}

	static LRESULT CALLBACK	wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void				closeApp(HWND hWnd);
};
