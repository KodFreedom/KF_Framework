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
//	�֐����FGetToken
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
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
//	�֐����FGetToken
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
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
//	�֐����FGetStringCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
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

	//File�̓��ɖ߂�
	fseek(filePointer, 0, SEEK_SET);

	return count;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStringCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
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
//	�֐����FGetFileName
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
string Utility::GetFileName(const string& filePath)
{
	auto copy = filePath;

	//�t�]
	reverse(copy.begin(), copy.end());

	//�t�@�C���^�̎擾
	string strType;
	GetToken(copy, ".", strType);

	//�t�@�C�����̎擾
	string strName;
	GetToken(copy, "\\/", strName);
	reverse(strName.begin(), strName.end());
	return strName;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStringCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Utility::AnalyzeFilePath(const string& strTexPath, string& strName, string& strType)
{
	auto copy = strTexPath;

	//�t�]
	reverse(copy.begin(), copy.end());

	//�t�@�C���^�̎擾
	if (GetToken(copy, ".", strType) > 0)
	{
		reverse(strType.begin(), strType.end());
	}

	//�t�@�C�����̎擾
	GetToken(copy, "\\/", strName);
	reverse(strName.begin(), strName.end());
}