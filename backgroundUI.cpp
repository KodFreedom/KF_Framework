//--------------------------------------------------------------------------------
//
//　BackgroundUI.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-28
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "textureManager.h"
#include "backgroundUI.h"

#ifdef USING_DIRECTX
#include "KF_UtilityDX.h"
#endif

//--------------------------------------------------------------------------------
//  Public
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool BackgroundUI::Init(const string& texture)
{
	UIObject2D::Init();
	SPRITE sprite;
	sprite.texture = texture;
	TextureManager::Instance()->Use(sprite.texture);
	sprite.polygonNumber = 2;

//#ifdef USING_DIRECTX
//	auto& screenSize = Vector2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
//	UtilityDX::MakeVertex(sprite.pVtxBuffer, screenSize * 0.5f, screenSize);
//#endif
	sprites.push_back(sprite);
	return true;
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
BackgroundUI* BackgroundUI::Create(const string& texture)
{
	auto ui = new BackgroundUI;
	ui->Init(texture);
	return ui;
}