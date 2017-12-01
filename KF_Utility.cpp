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
int utility::GetStringUntilToken(String& file, const String& token, String& buffer)
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
int utility::GetStringCount(String& file, const String& token, const String& compare)
{
	int count = 0;
	String buffer;
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
utility::FileInfo utility::AnalyzeFilePath(const String& path)
{
	FileInfo info;
	auto copy = path;
	reverse(copy.begin(), copy.end());

	//ファイル型の取得
	if (GetStringUntilToken(copy, L".", info.type) > 0)
	{
		reverse(info.type.begin(), info.type.end());
	}

	//ファイル名の取得
	GetStringUntilToken(copy, L"\\/", info.name);
	reverse(info.name.begin(), info.name.end());
	return info;
}

//--------------------------------------------------------------------------------
//  フォルダからファイル名を取得する
//--------------------------------------------------------------------------------
vector<String> utility::GetFilesFromFolder(const String& path, const String& extension)
{
	HANDLE handle;
	WIN32_FIND_DATA data;
	vector<String> file_names;

	//拡張子の設定
	String search_name = path + L"\\*." + extension;

	handle = FindFirstFile(search_name.c_str(), &data);

	if (handle == INVALID_HANDLE_VALUE) 
	{
		throw runtime_error("file not found");
	}

	do 
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}
		else 
		{
			file_names.push_back(data.cFileName);
		}
	} while (FindNextFile(handle, &data));
	FindClose(handle);
	file_names.shrink_to_fit();
	return file_names;
}

//--------------------------------------------------------------------------------
//  プロジェクトのパスを取得する
//--------------------------------------------------------------------------------
String utility::GetProjectPath(void)
{
	char path[MAX_PATH + 1];
	if (0 != GetModuleFileNameA(NULL, path, MAX_PATH))
	{// 実行ファイルの完全パスを取得
		char drive[MAX_PATH + 1]
			, directory[MAX_PATH + 1]
			, filename[MAX_PATH + 1]
			, extension[MAX_PATH + 1];
		//パス名を構成要素に分解します
		_splitpath_s(path, drive, directory, filename, extension);
		string projectpath = drive;
		projectpath += directory;
		return String(projectpath.begin(), projectpath.end());
	}
	throw runtime_error("error to get path");
	return String();
}

//--------------------------------------------------------------------------------
//	関数名：FindChildBy
//  関数説明：子供から名前対応のオブジェクトを探し出す
//	引数：	name：探したいオブジェクトの名前
//			parent：親のオブジェクト
//	戻り値：FileInfo
//--------------------------------------------------------------------------------
//GameObject* utility::FindChildBy(const String& name, GameObject* const parent)
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