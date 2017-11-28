//--------------------------------------------------------------------------------
//　background_ui.cpp
//	background uiオブジェクト
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "background_ui.h"

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
BackgroundUI* BackgroundUI::Create(const String& texture)
{
	auto ui = new BackgroundUI;
	ui->Init(texture);
	return ui;
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool BackgroundUI::Init(const String& texture)
{
	//UIObject2D::Init();
	//Sprite sprite;
	//sprite.texture_name = texture;
	//TextureManager::Instance()->Use(sprite.texture);
	//sprite.polygonNumber = 2;
	//
	////#ifdef USING_DIRECTX
	////	auto& screenSize = Vector2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	////	UtilityDX::MakeVertex(sprite.pVtxBuffer, screenSize * 0.5f, screenSize);
	////#endif
	//sprites.push_back(sprite);
	return true;
}