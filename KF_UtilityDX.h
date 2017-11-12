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
//	us			polygonNumber;
//	string					texture;
//	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;
//};

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
//class CSprite2D
//{
//public:
//	CSprite2D()
//		: m_polygonNumber(0), m_texture("polygon.jpg"), vertexBuffer(nullptr) {}
//	~CSprite2D()
//	{
//		m_texture.clear();
//	}
//
//	us			m_polygonNumber;
//	string					m_texture;
//	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
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