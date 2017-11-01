//--------------------------------------------------------------------------------
//	便利関数
//　KF_Utility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  ディファイン
//--------------------------------------------------------------------------------
#define SAFE_RELEASE(p)		if(p){ p->Release(); p = nullptr; }
#define SAFE_UNINIT(p)		if(p){ p->uninit(); delete p; p = nullptr; }
#define SAFE_DELETE(p)		if(p){ delete p; p = nullptr; }

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
namespace KF
{
	namespace Utility
	{
		int		GetStrToken(FILE* pFp, const string& strToken, string& strBuf);
		int		GetStrToken(string& str, const string& strToken, string& strBuf);
		int		GetStrCount(FILE* pFp, const string& strToken, const string& strComp);
		int		GetStrCount(string& str, const string& strToken, const string& strComp);
		string	GetFileName(const string& strFilePath);
		void	AnalyzeFilePath(const string& strTexPath, string& strName, string& strType);
	}
}