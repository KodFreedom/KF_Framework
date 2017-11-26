//--------------------------------------------------------------------------------
//　common_setting.h
//  common setting and include common heads
//	通用の設定とインクルードファイル
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  define / ディファイン
//--------------------------------------------------------------------------------

// エディタ
#define EDITOR

// Render Api
#define USING_DIRECTX
#ifdef USING_DIRECTX
#define DIRECTX_VERSION 9
#endif

// 今後newをオーバーロードため
#define MY_NEW new

// 今後deleteをオーバーロードため
#define MY_DELETE delete

//--------------------------------------------------------------------------------
//  include / インクルード
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
#include <fstream>
#include <assert.h>
using namespace std;

//Cereal
#include <cereal/archives/JSON.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
using namespace cereal;

// Openmp
#include <omp.h>

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound

//--------------------------------------------------------------------------------
//  library / ライブラリ
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

// システム時刻取得に必要
//#pragma comment(lib,"winmm.lib")	

//--------------------------------------------------------------------------------
//  typedef / タイプディファイン
//--------------------------------------------------------------------------------
typedef unsigned short us;
typedef unsigned long  ul;

#ifdef UNICODE
typedef std::wstring String;
typedef wchar_t Char;
#else
typedef std::string String;
typedef char Char;
#endif