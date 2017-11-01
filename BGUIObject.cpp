//--------------------------------------------------------------------------------
//
//�@BGUIObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-28
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "BGUIObject.h"

#ifdef USING_DIRECTX
#include "KF_UtilityDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CBGUIObject::CBGUIObject()
	: C2DUIObject(100)
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool CBGUIObject::Init(const string& strTexName)
{
	C2DUIObject::Init();
	SPRITE sprite;
	sprite.strTexName = strTexName;
	auto pTexManager = Main::GetManager()->GetTextureManager();
	pTexManager->UseTexture(sprite.strTexName);
	sprite.usNumPolygon = 2;

#ifdef USING_DIRECTX
	Vector2 vScreenSize = Vector2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	UtilityDX::MakeVertex(sprite.pVtxBuffer, vScreenSize * 0.5f, vScreenSize);
#endif
	m_listSprite.push_back(sprite);
	return true;
}

//--------------------------------------------------------------------------------
//  �쐬
//--------------------------------------------------------------------------------
CBGUIObject* CBGUIObject::Create(const string& strTexName)
{
	auto pUI = new CBGUIObject;
	pUI->Init(strTexName);
	return pUI;
}