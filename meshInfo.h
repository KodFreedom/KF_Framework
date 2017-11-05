//--------------------------------------------------------------------------------
//	メッシュ
//　meshInfo.h
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
enum DrawType
{
	PointList = 1,
	LineList = 2,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#else
enum DrawType
{
	PointList = 0,
	LineList = 1,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class MeshInfo
{
public:
	MeshInfo()
		: CurrentType(TriangleStrip)
		, VertexNumber(0)
		, IndexNumber(0)
		, PolygonNumber(0) {}
	~MeshInfo()
	{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);
#endif
	}

	DrawType CurrentType;
	int		 VertexNumber;
	int		 IndexNumber;
	int		 PolygonNumber;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DINDEXBUFFER9	IndexBuffer;
#endif
};