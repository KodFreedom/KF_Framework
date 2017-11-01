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
//	関数名：GetToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetToken(FILE* filePointer, const string& token, string& buffer)
{
	char c;
	buffer.clear();
	while ((c = (char)fgetc(filePointer)) != EOF)
	{
		for (int count = 0; count < (int)token.length(); ++count)
		{
			if (c == token.at(count))
			{
				return buffer.length();
			}
		}
		buffer += c;
	}
	return -1;
}

//--------------------------------------------------------------------------------
//	関数名：GetToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetToken(string& str, const string& token, string& buffer)
{
	buffer.clear();
	for (auto itr = str.begin(); itr != str.end();)
	{
		auto c = *itr;
		itr = str.erase(itr);
		for (int count = 0; count < (int)token.length(); ++count)
		{
			if (c == token.at(count))
			{
				return buffer.length();
			}
		}
		buffer += c;
	}

	return -1;
}

//--------------------------------------------------------------------------------
//	関数名：GetStringCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetStringCount(FILE* filePointer, const string& token, const string& compareString)
{
	int count = 0;
	string buffer;
	while (GetToken(filePointer, token, buffer) >= 0)
	{
		if (buffer.compare(compareString) == 0)
		{
			++count;
		}
	}

	//Fileの頭に戻る
	fseek(filePointer, 0, SEEK_SET);

	return count;
}

//--------------------------------------------------------------------------------
//	関数名：GetStringCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetStringCount(string& str, const string& token, const string& compareString)
{
	int count = 0;
	string buffer;
	while (GetToken(str, token, buffer) >= 0)
	{
		if (buffer.compare(compareString) == 0)
		{
			++count;
		}
	}
	return count;
}

//--------------------------------------------------------------------------------
//	関数名：GetFileName
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
string Utility::GetFileName(const string& filePath)
{
	auto copy = filePath;

	//逆転
	reverse(copy.begin(), copy.end());

	//ファイル型の取得
	string strType;
	GetToken(copy, ".", strType);

	//ファイル名の取得
	string strName;
	GetToken(copy, "\\/", strName);
	reverse(strName.begin(), strName.end());
	return strName;
}

//--------------------------------------------------------------------------------
//	関数名：GetStringCount
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void Utility::AnalyzeFilePath(const string& strTexPath, string& strName, string& strType)
{
	auto copy = strTexPath;

	//逆転
	reverse(copy.begin(), copy.end());

	//ファイル型の取得
	if (GetToken(copy, ".", strType) > 0)
	{
		reverse(strType.begin(), strType.end());
	}

	//ファイル名の取得
	GetToken(copy, "\\/", strName);
	reverse(strName.begin(), strName.end());
}