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
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FGetStrToken
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
int CKFUtility::GetStrToken(FILE* pFp, const string& strToken, string& strBuf)
{
	char c;
	strBuf.clear();
	while ((c = (char)fgetc(pFp)) != EOF)
	{
		for (int nCnt = 0; nCnt < (int)strToken.length(); nCnt++)
		{
			if (c == strToken.at(nCnt))
			{
				return strBuf.length();
			}
		}
		strBuf += c;
	}
	return -1;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStrToken
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
int CKFUtility::GetStrToken(string& str, const string& strToken, string& strBuf)
{
	strBuf.clear();
	for (auto itr = str.begin(); itr != str.end();)
	{
		auto c = *itr;
		itr = str.erase(itr);
		for (int nCnt = 0; nCnt < (int)strToken.length(); nCnt++)
		{
			if (c == strToken.at(nCnt))
			{
				return strBuf.length();
			}
		}
		strBuf += c;
	}

	return -1;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStrCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
int CKFUtility::GetStrCount(FILE* pFp, const string& strToken, const string& strComp)
{
	int nCnt = 0;
	string strBuf;
	while (GetStrToken(pFp, strToken, strBuf) >= 0)
	{
		if (strBuf.compare(strComp) == 0)
		{
			nCnt++;
		}
	}

	//File�̓��ɖ߂�
	fseek(pFp, 0, SEEK_SET);

	return nCnt;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStrCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
int CKFUtility::GetStrCount(string& str, const string& strToken, const string& strComp)
{
	int nCnt = 0;
	string strBuf;
	while (GetStrToken(str, strToken, strBuf) >= 0)
	{
		if (strBuf.compare(strComp) == 0)
		{
			nCnt++;
		}
	}
	return nCnt;
}

//--------------------------------------------------------------------------------
//	�֐����FGetFileName
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
string CKFUtility::GetFileName(const string& strFilePath)
{
	auto strCpy = strFilePath;

	//�t�]
	reverse(strCpy.begin(), strCpy.end());

	//�t�@�C���^�̎擾
	string strType;
	GetStrToken(strCpy, ".", strType);

	//�t�@�C�����̎擾
	string strName;
	GetStrToken(strCpy, "\\/", strName);
	reverse(strName.begin(), strName.end());
	return strName;
}

//--------------------------------------------------------------------------------
//	�֐����FGetStrCount
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CKFUtility::AnalyzeFilePath(const string& strTexPath, string& strName, string& strType)
{
	auto strCpy = strTexPath;

	//�t�]
	reverse(strCpy.begin(), strCpy.end());

	//�t�@�C���^�̎擾
	if (GetStrToken(strCpy, ".", strType) > 0)
	{
		reverse(strType.begin(), strType.end());
	}

	//�t�@�C�����̎擾
	GetStrToken(strCpy, "\\/", strName);
	reverse(strName.begin(), strName.end());
}