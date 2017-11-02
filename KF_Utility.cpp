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
//	関数名：GetStringUntilToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetStringUntilToken(FILE* filePointer, const string& token, string& buffer)
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
//	関数名：GetStringUntilToken
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
int Utility::GetStringUntilToken(string& file, const string& token, string& buffer)
{
	buffer.clear();
	for (auto itr = file.begin(); itr != file.end();)
	{
		auto c = *itr;
		itr = file.erase(itr);
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
	while (GetStringUntilToken(filePointer, token, buffer) >= 0)
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
int Utility::GetStringCount(string& file, const string& token, const string& compareString)
{
	int count = 0;
	string buffer;
	while (GetStringUntilToken(file, token, buffer) >= 0)
	{
		if (buffer.compare(compareString) == 0)
		{
			++count;
		}
	}
	return count;
}

//--------------------------------------------------------------------------------
//	関数名：AnalyzeFilePath
//  関数説明：パスから名前とタイプを解析する
//	引数：	filePath：パス
//	戻り値：FileInfo
//--------------------------------------------------------------------------------
Utility::FileInfo Utility::AnalyzeFilePath(const string& filePath)
{
	FileInfo info;
	auto copy = filePath;
	reverse(copy.begin(), copy.end());

	//ファイル型の取得
	if (GetStringUntilToken(copy, ".", info.Type) > 0)
	{
		reverse(info.Type.begin(), info.Type.end());
	}

	//ファイル名の取得
	GetStringUntilToken(copy, "\\/", info.Name);
	reverse(info.Name.begin(), info.Name.end());
	return info;
}