//--------------------------------------------------------------------------------
//�@kf_utility.h
//  utility methods
//	�֗��֐�
//	Author : �����^(KodFreedom)
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
		//  �����܂ŕ���������
		//  file_pointer�F�t�@�C���|�C���^
		//  token�F����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		static int GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  �����܂ŕ���������
		//  file�F�t�@�C���̃X�g�����O
		//  token�F����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		static int GetStringUntilToken(String& file, const String& token, String& buffer);

		//--------------------------------------------------------------------------------
		//  ��r���镶����܂ŕ���������
		//  file_pointer�F�t�@�C���|�C���^
		//  compare�F��r���镶����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		static int GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer);

		//--------------------------------------------------------------------------------
		//  ��������J�E���g����
		//  file_pointer�F�t�@�C���|�C���^
		//  token�F����
		//  compare�F��r���镶����
		//  return�F�J�E���g��
		//--------------------------------------------------------------------------------
		static int GetStringCount(FILE* file_pointer, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  ��������J�E���g����
		//  file�F�t�@�C���̕�����
		//  token�F����
		//  compare�F��r���镶����
		//  return�F�J�E���g��
		//--------------------------------------------------------------------------------
		static int GetStringCount(String& file, const String& token, const String& compare);

		//--------------------------------------------------------------------------------
		//  �p�X���疼�O�ƃ^�C�v����͂���
		//  path�F�p�X
		//  return�FFileInfo
		//--------------------------------------------------------------------------------
		static FileInfo AnalyzeFilePath(const String& path);
		
		//--------------------------------------------------------------------------------
		//  �t�H���_����t�@�C�������擾����
		//  path�F��΃p�X
		//  extension : �g���q
		//  return : vector<String>
		//--------------------------------------------------------------------------------
		static vector<String> GetFilesFromFolder(const String& path, const String& extension);

		//--------------------------------------------------------------------------------
		//  �v���W�F�N�g�̃p�X���擾����
		//  return : String
		//--------------------------------------------------------------------------------
		static String GetProjectPath(void);

        //--------------------------------------------------------------------------------
        //  �e�L�X�g��u8�ɕύX
        //  return : string
        //--------------------------------------------------------------------------------
        static string ToUtf8(const String& text);
	};
} // namespace KodFreedom