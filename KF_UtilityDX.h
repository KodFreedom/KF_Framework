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

class UtilityDX
{
public:
	UtilityDX() {}
	~UtilityDX() {}

	static bool MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const Vector2& PositionCenter, const Vector2& vSize,
		const Vector2& UVBegin = Vector2(0.0f), const Vector2& UVSize = Vector2(1.0f),
		const float& fRot = 0.0f, const Color& cColor = Color(1.0f));
	static bool MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const Vector2& PositionLeftTop, const Vector2& vSize,
		const Color& cColor = Color(1.0f),
		const Vector2& UVBegin = Vector2(0.0f), const Vector2& UVSize = Vector2(1.0f));
	static void UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		const Vector2& PositionLeftTop, const Vector2& vSize,
		const float& fRate, const Color& cColor = Color(1.0f),
		const Vector2& UVBegin = Vector2(0.0f), const Vector2& UVSize = Vector2(1.0f));
	static void UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& UVBegin, const Vector2& UVSize);
	//static void DrawSprite(const SPRITE2D& sprite);
};
#endif