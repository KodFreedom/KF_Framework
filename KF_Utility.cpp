//--------------------------------------------------------------------------------
//�@kf_utility.cpp
//  utility methods
//	�֗��֐�
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  �����܂ŕ���������
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
//  �����܂ŕ���������
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
//  ��r���镶����܂ŕ���������
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
//  ��������J�E���g����
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

	//File�̓��ɖ߂�
	fseek(file_pointer, 0, SEEK_SET);

	return count;
}

//--------------------------------------------------------------------------------
//  ��������J�E���g����
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
//  �p�X���疼�O�ƃ^�C�v����͂���
//--------------------------------------------------------------------------------
utility::FileInfo utility::AnalyzeFilePath(const string& path)
{
	FileInfo info;
	auto copy = path;
	reverse(copy.begin(), copy.end());

	//�t�@�C���^�̎擾
	if (GetStringUntilToken(copy, ".", info.type) > 0)
	{
		reverse(info.type.begin(), info.type.end());
	}

	//�t�@�C�����̎擾
	GetStringUntilToken(copy, "\\/", info.name);
	reverse(info.name.begin(), info.name.end());
	return info;
}

//--------------------------------------------------------------------------------
//	�֐����FFindChildBy
//  �֐������F�q�����疼�O�Ή��̃I�u�W�F�N�g��T���o��
//	�����F	name�F�T�������I�u�W�F�N�g�̖��O
//			parent�F�e�̃I�u�W�F�N�g
//	�߂�l�FFileInfo
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