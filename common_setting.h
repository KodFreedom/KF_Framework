//--------------------------------------------------------------------------------
//　common_setting.h
//  common setting and include common heads
//  通用の設定とインクルードファイル
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  define / ディファイン
//--------------------------------------------------------------------------------
// エディタモード
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

#define SCREEN_WIDTH    (1280) //ウインドウ幅
#define SCREEN_HEIGHT   (720)  //ウインドウ高さ
#define SCREEN_RATE     (SCREEN_WIDTH / 1920.0f)

//--------------------------------------------------------------------------------
//  include / インクルード
//--------------------------------------------------------------------------------
// STL
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <thread>
#include <queue>
using namespace std;

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