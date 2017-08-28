//--------------------------------------------------------------------------------
//
//　modeDemo.h
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
#pragma once

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
class CModeDemo : public CMode
{
public:
	CModeDemo();
	~CModeDemo();

	void	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
	void	EndMode(void) override { m_bEndMode = true; }
	void	EndMode(const bool& bGameOver)
	{
		if (m_bEndMode) { return; }
		m_bEndMode = true;
		m_bGameOver = bGameOver;
	}
private:
	bool	m_bEndMode;
	bool	m_bGameOver;
};