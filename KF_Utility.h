//--------------------------------------------------------------------------------
//	�֗��֐�
//�@KF_Utility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �f�B�t�@�C��
//--------------------------------------------------------------------------------
#define SAFE_RELEASE(p)		if(p){ p->Release(); p = nullptr; }
#define SAFE_UNINIT(p)		if(p){ p->uninit(); delete p; p = nullptr; }
#define SAFE_DELETE(p)		if(p){ delete p; p = nullptr; }

//--------------------------------------------------------------------------------
//  Typedef
//--------------------------------------------------------------------------------
typedef unsigned short us;
typedef unsigned long  ul;

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObject;

namespace KF
{
	namespace Utility
	{
		//--------------------------------------------------------------------------------
		//  �\���̒�`
		//--------------------------------------------------------------------------------
		struct FileInfo
		{
			string Name;
			string Type;
		};

		//--------------------------------------------------------------------------------
		//  �֐��錾
		//--------------------------------------------------------------------------------
		int			GetStringUntilToken(FILE* filePointer, const string& token, string& buffer);
		int			GetStringUntilToken(string& file, const string& token, string& buffer);
		int			GetStringUntilString(FILE* filePointer, const string& compare, string& buffer);
		int			GetStringCount(FILE* filePointer, const string& token, const string& compareString);
		int			GetStringCount(string& file, const string& token, const string& compareString);
		FileInfo	AnalyzeFilePath(const string& filePath);
		GameObject* FindChildBy(const string& name, GameObject* const parent);
	}
}