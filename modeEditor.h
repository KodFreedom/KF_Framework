//--------------------------------------------------------------------------------
//
//　modeEditor.h
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
#pragma once
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "mode.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModeEditor : public CMode
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CModeEditor();
	~CModeEditor();

	void	Init(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
	void	EndMode(void) override {}

private:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	void	uninit(void) override;
};
#endif // _DEBUG
