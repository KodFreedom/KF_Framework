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