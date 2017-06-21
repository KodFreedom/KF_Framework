//--------------------------------------------------------------------------------
//
//　main.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include <Windows.h>
#include "KF_Math.h"

#ifdef USING_DIRECTX9
#include <d3dx9.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound
#include "KF_UtilityDX.h"
#endif

//--------------------------------------------------------------------------------
//  ライブラリ読み込み
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX9
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#endif

#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH		(800)							//ウインドウ幅
#define SCREEN_HEIGHT		(600)							//ウインドウ高さ
#define FRAME_PER_SECOND	(60)
#define TIMER_INTERVAL		(1000.0f / FRAME_PER_SECOND)	//更新間隔
#define DELTA_TIME			(TIMER_INTERVAL * 0.001f)		//更新間隔(秒単位)
#define CLASS_NAME			"KF_Framework"					//クラスの名前
#define WINDOW_NAME			"フレームワーク"

#ifdef USING_DIRECTX9
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//頂点フォーマット
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//3D頂点フォーマット
#endif											//ウインドウの名前

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CManager;

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct
{
	CKFVec3			vPos;	//xyz
	float			fRhw;	//rhw
	unsigned long	ulColor;//色
	CKFVec2			vUV;	//テクスチャ
}VERTEX_2D;//2D頂点情報構造体　頂点フォーマットと構造を合わせること

typedef struct//頂点情報構造体　上の頂点フォーマットと構造を合わせること
{
	CKFVec3			vPos;	//xyz
	CKFVec3			vNormal;//法線
	unsigned long	ulColor;//色
	CKFVec2			vUV;	//uv
}VERTEX_3D;//3D頂点情報構造体　頂点フォーマットと構造を合わせること

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
CManager *GetManager(void);

#ifdef _DEBUG
int GetCountFPS(void);
#endif

#endif
