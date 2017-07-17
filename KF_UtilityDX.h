//--------------------------------------------------------------------------------
//	DX用便利関数
//　KF_UtilityDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
//struct SPRITE2D
//{
//	unsigned short			usNumPolygon;
//	string					strTexName;
//	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;
//};

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
//class CSprite2D
//{
//public:
//	CSprite2D()
//		: m_usNumPolygon(0), m_strTexName("polygon.jpg"), m_pVtxBuffer(nullptr) {}
//	~CSprite2D()
//	{
//		m_strTexName.clear();
//	}
//
//	unsigned short			m_usNumPolygon;
//	string					m_strTexName;
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;
//};

class CKFUtilityDX
{
public:
	CKFUtilityDX() {}
	~CKFUtilityDX() {}

	static bool MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const CKFVec2& vPosCenter, const CKFVec2& vSize,
		const CKFVec2& vUVBegin = CKFVec2(0.0f), const CKFVec2& vUVSize = CKFVec2(1.0f),
		const float& fRot = 0.0f, const CKFColor& cColor = CKFColor(1.0f));
	static bool MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const CKFVec2& vPosLeftTop, const CKFVec2& vSize,
		const CKFColor& cColor = CKFColor(1.0f),
		const CKFVec2& vUVBegin = CKFVec2(0.0f), const CKFVec2& vUVSize = CKFVec2(1.0f));
	static void UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const CKFVec2& vPosLeftTop, const CKFVec2& vSize,
		const float& fRate, const CKFColor& cColor = CKFColor(1.0f),
		const CKFVec2& vUVBegin = CKFVec2(0.0f), const CKFVec2& vUVSize = CKFVec2(1.0f));
	static void UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vUVBegin, const CKFVec2& vUVSize);
	//static void DrawSprite(const SPRITE2D& sprite);
};
#endif