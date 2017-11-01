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
		int		GetToken(FILE* filePointer, const string& token, string& buffer);
		int		GetToken(string& str, const string& token, string& buffer);
		int		GetStringCount(FILE* filePointer, const string& token, const string& compareString);
		int		GetStringCount(string& str, const string& token, const string& compareString);
		string	GetFileName(const string& filePath);
		void	AnalyzeFilePath(const string& strTexPath, string& strName, string& strType);
	}
}