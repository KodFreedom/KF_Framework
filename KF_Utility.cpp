//--------------------------------------------------------------------------------
//	便利関数
//　KF_Utility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_Utility.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：GetStrToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int CKFUtility::GetStrToken(FILE* pFp, const string& strToken, string& strBuf)
{
	char c;
	strBuf.clear();
	while ((c = (char)fgetc(pFp)) != EOF)
	{
		for (int nCnt = 0; nCnt < (int)strToken.length(); nCnt++)
		{
			if (c == strToken.at(nCnt))
			{
				return strBuf.length();
			}
		}
		strBuf += c;
	}
	return -1;
}

//--------------------------------------------------------------------------------
//	関数名：GetStrToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int CKFUtility::GetStrToken(string& str, const string& strToken, string& strBuf)
{
	strBuf.clear();
	for (auto itr = str.begin(); itr != str.end();)
	{
		auto c = *itr;
		itr = str.erase(itr);
		for (int nCnt = 0; nCnt < (int)strToken.length(); nCnt++)
		{
			if (c == strToken.at(nCnt))
			{
				return strBuf.length();
			}
		}
		strBuf += c;
	}

	return -1;
}

//--------------------------------------------------------------------------------
//	関数名：GetStrCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int CKFUtility::GetStrCount(FILE* pFp, const string& strToken, const string& strComp)
{
	int nCnt = 0;
	string strBuf;
	while (GetStrToken(pFp, strToken, strBuf) >= 0)
	{
		if (strBuf.compare(strComp) == 0)
		{
			nCnt++;
		}
	}

	//Fileの頭に戻る
	fseek(pFp, 0, SEEK_SET);

	return nCnt;
}

//--------------------------------------------------------------------------------
//	関数名：GetStrCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int CKFUtility::GetStrCount(string& str, const string& strToken, const string& strComp)
{
	int nCnt = 0;
	string strBuf;
	while (GetStrToken(str, strToken, strBuf) >= 0)
	{
		if (strBuf.compare(strComp) == 0)
		{
			nCnt++;
		}
	}
	return nCnt;
}

//--------------------------------------------------------------------------------
//	関数名：GetFileName
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
string CKFUtility::GetFileName(const string& strFilePath)
{
	auto strCpy = strFilePath;

	//逆転
	reverse(strCpy.begin(), strCpy.end());

	//ファイル型の取得
	string strType;
	GetStrToken(strCpy, ".", strType);

	//ファイル名の取得
	string strName;
	GetStrToken(strCpy, "\\/", strName);
	reverse(strName.begin(), strName.end());
	return strName;
}

//--------------------------------------------------------------------------------
//	関数名：GetStrCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CKFUtility::AnalyzeFilePath(const string& strTexPath, string& strName, string& strType)
{
	auto strCpy = strTexPath;

	//逆転
	reverse(strCpy.begin(), strCpy.end());

	//ファイル型の取得
	if (GetStrToken(strCpy, ".", strType) > 0)
	{
		reverse(strType.begin(), strType.end());
	}

	//ファイル名の取得
	GetStrToken(strCpy, "\\/", strName);
	reverse(strName.begin(), strName.end());
}