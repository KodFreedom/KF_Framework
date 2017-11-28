//--------------------------------------------------------------------------------
//　ui_object_2d.h
//	2duiオブジェクト
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "ui_object.h"
#include "main.h"

//--------------------------------------------------------------------------------
//  2duiオブジェクトクラス
//--------------------------------------------------------------------------------
class UIObject2D : public UIObject
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	UIObject2D(const int order = 0) : UIObject(order) {}
	~UIObject2D() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual bool Init(void) override { return true; }

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual void Uninit(void) override;

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual void Render(void) override;

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct Sprite
	{
		String texture_name;
	};

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<Sprite> sprites_;
};