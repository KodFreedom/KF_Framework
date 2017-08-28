//--------------------------------------------------------------------------------
//
//　2DUIObject.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "UIObject.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class C2DUIObject : public CUIObject
{
public:
	C2DUIObject(const unsigned short& usOrder = 0);
	~C2DUIObject() {}

	virtual bool Init(void) override { return true; }
	virtual void Uninit(void) override;
	virtual void Draw(void) override;

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct SPRITE
	{
		unsigned short			usNumPolygon;
		string					strTexName;
#ifdef USING_DIRECTX
		LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;
#endif
	};

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<SPRITE> m_listSprite;
};