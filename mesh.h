//--------------------------------------------------------------------------------
//	メッシュ
//　mesh.h
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
enum DRAW_TYPE
{
	DT_POINTLIST = 1,
	DT_LINELIST = 2,
	DT_LINESTRIP = 3,
	DT_TRIANGLELIST = 4,
	DT_TRIANGLESTRIP = 5,
	DT_TRIANGLEFAN = 6,
};
#else
enum DRAW_TYPE
{
	DT_POINTLIST = 0,
	DT_LINELIST = 1,
	DT_LINESTRIP = 3,
	DT_TRIANGLELIST = 4,
	DT_TRIANGLESTRIP = 5,
	DT_TRIANGLEFAN = 6,
};
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CMesh
{
public:
	CMesh()
		: m_drawType(DT_TRIANGLESTRIP)
		, m_nNumVtx(0)
		, m_nNumIdx(0)
		, m_nNumPolygon(0) {}
	~CMesh() 
	{
		SAFE_RELEASE(m_pVtxBuffer);
		SAFE_RELEASE(m_pIdxBuffer);
	}

	DRAW_TYPE		m_drawType;
	int				m_nNumVtx;
	int				m_nNumIdx;
	int				m_nNumPolygon;

#ifdef USING_DIRECTX
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// インデックスへのポインタ
#endif
};