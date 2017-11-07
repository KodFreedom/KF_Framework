//--------------------------------------------------------------------------------
//	�֗��֐�
//�@KF_Utility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_Utility.h"

//--------------------------------------------------------------------------------
//	�֐����FGetStringUntilToken
//  �֐������F�����܂ŕ���������
//	�����F	filePointer�F�t�@�C���|�C���^
//			token�F����
//			buffer�F������ۑ��p�Ƃ���
//	�߂�l�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
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
//	�֐����FGetStringUntilToken
//  �֐������F�����܂ŕ���������
//	�����F	file�F�t�@�C���̕�����
//			token�F����
//			buffer�F������ۑ��p�Ƃ���
//	�߂�l�F�t�@�C�����I�������-1��Ԃ��A��������Ȃ��Ȃ當������Ԃ�
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
//	�֐����FGetStringCount
//  �֐������F��������J�E���g����
//	�����F	filePointer�F�t�@�C���|�C���^
//			token�F����
//			compareString�F
//	�߂�l�F�J�E���g��
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

	//File�̓��ɖ߂�
	fseek(filePointer, 0, SEEK_SET);

	return count;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStringCount
//  �֐������F��������J�E���g����
//	�����F	file�F�t�@�C���̕�����
//			token�F����
//			compareString�F
//	�߂�l�F�J�E���g��
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
//	�֐����FAnalyzeFilePath
//  �֐������F�p�X���疼�O�ƃ^�C�v����͂���
//	�����F	filePath�F�p�X
//	�߂�l�FFileInfo
//--------------------------------------------------------------------------------
Utility::FileInfo Utility::AnalyzeFilePath(const string& filePath)
{
	FileInfo info;
	auto copy = filePath;
	reverse(copy.begin(), copy.end());

	//�t�@�C���^�̎擾
	if (GetStringUntilToken(copy, ".", info.Type) > 0)
	{
		reverse(info.Type.begin(), info.Type.end());
	}

	//�t�@�C�����̎擾
	GetStringUntilToken(copy, "\\/", info.Name);
	reverse(info.Name.begin(), info.Name.end());
	return info;
}