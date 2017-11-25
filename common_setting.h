//--------------------------------------------------------------------------------
//　common_setting.h
//  common setting and include common heads
//	通用の設定とインクルードファイル
//  通用设定与头文件
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  define / ディファイン / 宏定义
//--------------------------------------------------------------------------------

// エディタ
#define EDITOR

// RenderSystem
#define USING_DIRECTX
#ifdef USING_DIRECTX
#define DIRECTX_VERSION 9
#endif

//--------------------------------------------------------------------------------
//  include / インクルード / 读取头文件
//--------------------------------------------------------------------------------

// System
#include <math.h>
#include <time.h>

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

// DirectX
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound

//--------------------------------------------------------------------------------
//  library / ライブラリ / 数据连接库
//--------------------------------------------------------------------------------

// DirectX
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

// システム時刻取得に必要
//#pragma comment(lib,"winmm.lib")	

//--------------------------------------------------------------------------------
//  typedef / タイプディファイン / 型声明
//--------------------------------------------------------------------------------

typedef unsigned short us;
typedef unsigned long  ul;

#ifdef UNICODE
typedef std::string String;
typedef char Char;
#else
typedef std::string String;
typedef char Char;
#endif