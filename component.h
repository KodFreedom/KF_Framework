//--------------------------------------------------------------------------------
//　component.h
//  コンポネント基底クラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObject;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	Component(GameObject& owner) : owner_(owner) {}
	~Component() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual bool Init(void) = 0;

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	virtual void Uninit(void) = 0;

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	virtual void Release(void)
	{
		Uninit();
		MY_DELETE this;
	}

	//--------------------------------------------------------------------------------
	//  受信処理
	//--------------------------------------------------------------------------------
	//virtual void	ReceiveMsg(const MESSAGE &msg) {}

	//--------------------------------------------------------------------------------
	//  スリープ処理
	//--------------------------------------------------------------------------------
	virtual void Sleep(void) {}

	//--------------------------------------------------------------------------------
	//  目覚める処理
	//--------------------------------------------------------------------------------
	virtual void Awake(void) {}

	//--------------------------------------------------------------------------------
	//  ゲームオブジェクト取得処理
	//--------------------------------------------------------------------------------
	GameObject& GetGameObject(void) const { return owner_; }

protected:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	GameObject& owner_;
};