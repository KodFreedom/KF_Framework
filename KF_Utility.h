//--------------------------------------------------------------------------------
//�@kf_utility.h
//  utility methods
//	�֗��֐�
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �f�B�t�@�C��
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
		//  �����܂ŕ���������
		//  file_pointer�F�t�@�C���|�C���^
		//  token�F����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		int GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  �����܂ŕ���������
		//  file�F�t�@�C���̃X�g�����O
		//  token�F����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		int GetStringUntilToken(string& file, const string& token, string& buffer);

		//--------------------------------------------------------------------------------
		//  ��r���镶����܂ŕ���������
		//  file_pointer�F�t�@�C���|�C���^
		//  compare�F��r���镶����
		//  buffer�F������ۑ��p�Ƃ���
		//  return�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
		//--------------------------------------------------------------------------------
		int GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer);

		//--------------------------------------------------------------------------------
		//  ��������J�E���g����
		//  file_pointer�F�t�@�C���|�C���^
		//  token�F����
		//  compare�F��r���镶����
		//  return�F�J�E���g��
		//--------------------------------------------------------------------------------
		int GetStringCount(FILE* file_pointer, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  ��������J�E���g����
		//  file�F�t�@�C���̕�����
		//  token�F����
		//  compare�F��r���镶����
		//  return�F�J�E���g��
		//--------------------------------------------------------------------------------
		int GetStringCount(string& file, const string& token, const string& compare);

		//--------------------------------------------------------------------------------
		//  �p�X���疼�O�ƃ^�C�v����͂���
		//  path�F�p�X
		//  return�FFileInfo
		//--------------------------------------------------------------------------------
		FileInfo AnalyzeFilePath(const string& path);
	} // namespace Utility
} // namespace KodFreedom