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
//  クラス宣言
//--------------------------------------------------------------------------------
class CKFUtility
{
public:
	static int		GetStrToken(FILE* pFp, const string& strToken, string& strBuf);
	static int		GetStrCount(FILE* pFp, const string& strToken, const string& strComp);
	static string	GetStrTag(const string& strName);
private:
	CKFUtility() {}
	~CKFUtility() {}
};