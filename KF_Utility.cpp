//--------------------------------------------------------------------------------
//　kf_utility.cpp
//  utility methods
//	便利関数
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  符号まで文字列を取る
//--------------------------------------------------------------------------------
int utility::GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer)
{
	char c;
	buffer.clear();
	while ((c = (char)fgetc(file_pointer)) != EOF)
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
//  符号まで文字列を取る
//--------------------------------------------------------------------------------
int utility::GetStringUntilToken(string& file, const string& token, string& buffer)
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
//  比較する文字列まで文字列を取る
//--------------------------------------------------------------------------------
int utility::GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer)
{
	static string nullBuffer;
	if (nullBuffer.empty()) nullBuffer.resize(256);
	do
	{
		buffer = nullBuffer;
		fgets(&buffer[0], (int)buffer.capacity(), file_pointer);
		if (buffer.find(compare) != string::npos)
		{
			return buffer.length();
		}
	} while (buffer.find("END_SCRIPT") == string::npos);
	return -1;
}

//--------------------------------------------------------------------------------
//  文字列をカウントする
//--------------------------------------------------------------------------------
int utility::GetStringCount(FILE* file_pointer, const string& token, const string& compare)
{
	int count = 0;
	string buffer;
	while (GetStringUntilToken(file_pointer, token, buffer) >= 0)
	{
		if (buffer.compare(compare) == 0)
		{
			++count;
		}
	}

	//Fileの頭に戻る
	fseek(file_pointer, 0, SEEK_SET);

	return count;
}

//--------------------------------------------------------------------------------
//  文字列をカウントする
//--------------------------------------------------------------------------------
int utility::GetStringCount(string& file, const string& token, const string& compare)
{
	int count = 0;
	string buffer;
	while (GetStringUntilToken(file, token, buffer) >= 0)
	{
		if (buffer.compare(compare) == 0)
		{
			++count;
		}
	}
	return count;
}

//--------------------------------------------------------------------------------
//  パスから名前とタイプを解析する
//--------------------------------------------------------------------------------
utility::FileInfo utility::AnalyzeFilePath(const string& path)
{
	FileInfo info;
	auto copy = path;
	reverse(copy.begin(), copy.end());

	//ファイル型の取得
	if (GetStringUntilToken(copy, ".", info.type) > 0)
	{
		reverse(info.type.begin(), info.type.end());
	}

	//ファイル名の取得
	GetStringUntilToken(copy, "\\/", info.name);
	reverse(info.name.begin(), info.name.end());
	return info;
}

//--------------------------------------------------------------------------------
//	関数名：FindChildBy
//  関数説明：子供から名前対応のオブジェクトを探し出す
//	引数：	name：探したいオブジェクトの名前
//			parent：親のオブジェクト
//	戻り値：FileInfo
//--------------------------------------------------------------------------------
//GameObject* utility::FindChildBy(const string& name, GameObject* const parent)
//{
//	assert(!name.empty());
//	auto& children = parent->GetTransform()->GetChildren();
//	for (auto child : children)
//	{
//		auto childObject = child->GetGameObject();
//		if (childObject->GetName() == name) { return childObject; }
//		else
//		{
//			auto result = FindChildBy(name, childObject);
//			if (result) return result;
//		}
//	}
//	return nullptr;
//}