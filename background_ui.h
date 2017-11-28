//--------------------------------------------------------------------------------
//�@background_ui.h
//	background ui�I�u�W�F�N�g
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "ui_object_2d.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class BackgroundUI : public UIObject2D
{
public:
	//--------------------------------------------------------------------------------
	//  ��������
	//--------------------------------------------------------------------------------
	static BackgroundUI* Create(const String& texture);

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	BackgroundUI() : UIObject2D(100) {}
	BackgroundUI(const BackgroundUI& value) {}
	BackgroundUI& operator=(const BackgroundUI& value) {}
	~BackgroundUI() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	bool Init(const String& texture);
};