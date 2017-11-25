//--------------------------------------------------------------------------------
//　kf_utility.h
//  utility methods
//	便利関数
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  ディファイン
//--------------------------------------------------------------------------------
#define SAFE_RELEASE(p)		if(p){ p->Release(); p = nullptr; }
#define SAFE_UNINIT(p)		if(p){ p->uninit(); delete p; p = nullptr; }
#define SAFE_DELETE(p)		if(p){ delete p; p = nullptr; }

namespace KodFreedom
{
	namespace Utility
	{
		struct FileInfo
		{
			string name;
			string type;
		};

		//--------------------------------------------------------------------------------
		//  符号まで文字列を取る
		//  file_pointer：ファイルポインタ
		//  token：符号
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		int GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  符号まで文字列を取る
		//  file：ファイルのストリング
		//  token：符号
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		int GetStringUntilToken(string& file, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  比較する文字列まで文字列を取る
		//  file_pointer：ファイルポインタ
		//  compare：比較する文字列
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		int GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer);

		//--------------------------------------------------------------------------------
		//  文字列をカウントする
		//  file_pointer：ファイルポインタ
		//  token：符号
		//  compare：比較する文字列
		//  return：カウント数
		//--------------------------------------------------------------------------------
		int GetStringCount(FILE* file_pointer, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  文字列をカウントする
		//  file：ファイルの文字列
		//  token：符号
		//  compare：比較する文字列
		//  return：カウント数
		//--------------------------------------------------------------------------------
		int GetStringCount(string& file, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  パスから名前とタイプを解析する
		//  path：パス
		//  return：FileInfo
		//--------------------------------------------------------------------------------
		FileInfo AnalyzeFilePath(const string& path);
	} // namespace Utility
} // namespace KodFreedom