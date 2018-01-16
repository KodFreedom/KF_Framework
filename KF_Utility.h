//--------------------------------------------------------------------------------
//　kf_utility.h
//  utility methods
//	便利関数
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

#define SAFE_RELEASE(p)		if(p){ p->Release(); p = nullptr; }
#define SAFE_UNINIT(p)		if(p){ p->Uninit(); MY_DELETE p; p = nullptr; }
#define SAFE_DELETE(p)		if(p){ MY_DELETE p; p = nullptr; }

namespace kodfreedom
{
	struct FileInfo
	{
		String name;
		String type;
	};

	class Utility
	{
	public:
		//--------------------------------------------------------------------------------
		//  符号まで文字列を取る
		//  file_pointer：ファイルポインタ
		//  token：符号
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		static int GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  符号まで文字列を取る
		//  file：ファイルのストリング
		//  token：符号
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		static int GetStringUntilToken(String& file, const String& token, String& buffer);

		//--------------------------------------------------------------------------------
		//  比較する文字列まで文字列を取る
		//  file_pointer：ファイルポインタ
		//  compare：比較する文字列
		//  buffer：文字列保存用ところ
		//  return：ファイルが終わったら-1を返す、そうじゃないなら文字数を返す
		//--------------------------------------------------------------------------------
		static int GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer);

		//--------------------------------------------------------------------------------
		//  文字列をカウントする
		//  file_pointer：ファイルポインタ
		//  token：符号
		//  compare：比較する文字列
		//  return：カウント数
		//--------------------------------------------------------------------------------
		static int GetStringCount(FILE* file_pointer, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  文字列をカウントする
		//  file：ファイルの文字列
		//  token：符号
		//  compare：比較する文字列
		//  return：カウント数
		//--------------------------------------------------------------------------------
		static int GetStringCount(String& file, const String& token, const String& compare);

		//--------------------------------------------------------------------------------
		//  パスから名前とタイプを解析する
		//  path：パス
		//  return：FileInfo
		//--------------------------------------------------------------------------------
		static FileInfo AnalyzeFilePath(const String& path);
		
		//--------------------------------------------------------------------------------
		//  フォルダからファイル名を取得する
		//  path：絶対パス
		//  extension : 拡張子
		//  return : vector<String>
		//--------------------------------------------------------------------------------
		static vector<String> GetFilesFromFolder(const String& path, const String& extension);

		//--------------------------------------------------------------------------------
		//  プロジェクトのパスを取得する
		//  return : String
		//--------------------------------------------------------------------------------
		static String GetProjectPath(void);

        //--------------------------------------------------------------------------------
        //  テキストをu8に変更
        //  return : string
        //--------------------------------------------------------------------------------
        static string ToUtf8(const String& text);
	};
} // namespace KodFreedom