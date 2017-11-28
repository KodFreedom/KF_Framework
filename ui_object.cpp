//--------------------------------------------------------------------------------
//�@ui_object.h
//	ui�I�u�W�F�N�g
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "ui_object.h"
#include "main_system.h"
#include "ui_system.h"

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
UIObject::UIObject(const int order)
	: order_(order)
{
	MainSystem::Instance()->GetUISystem()->Register(this);
}

//--------------------------------------------------------------------------------
//  �����[�X
//--------------------------------------------------------------------------------
void UIObject::Release(void)
{
	MainSystem::Instance()->GetUISystem()->Deregister(this);
	Uninit();
	delete this;
}