//--------------------------------------------------------------------------------
//	debug�\���Ǘ�����
//�@debugManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "debugManager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif //USING_DIRECTX

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CDebugManager::CDebugManager()
	: m_usCntScroll(0)
#ifdef USING_DIRECTX
	, m_pFont(nullptr)
#endif //USING_DIRECTX
{
	m_strDebugInfo.clear();
	m_listStrDebugScroll.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CDebugManager::Update(void)
{
	if (!m_listStrDebugScroll.empty())
	{
		++m_usCntScroll;
		if (m_usCntScroll >= sc_usScrollTime)
		{//��Ԗڂ�string���폜����
			m_usCntScroll = 0;
			m_listStrDebugScroll.erase(m_listStrDebugScroll.begin());
		}
		
		for (auto& str : m_listStrDebugScroll)
		{//Debug�\���ɒǉ�����
			m_strDebugInfo += str;
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CDebugManager::Draw(void)
{
#ifdef USING_DIRECTX
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_strDebugInfo.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
#endif

	//�O�t���[���̕�������폜
	m_strDebugInfo.clear();
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayAlways
//  �֐������FDebug�\���ɏ�ɕ\���������������ǉ�����
//	�����F	strInfo�F�\��������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CDebugManager::DisplayAlways(const string& strInfo)
{
	m_strDebugInfo.append(strInfo);
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayAlways
//  �֐������FDebug�\���ɏ�ɕ\��������������ǉ�����
//	�����F	cInfo�F�\������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CDebugManager::DisplayAlways(const char& cInfo)
{
	m_strDebugInfo.push_back(cInfo);
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayScroll
//  �֐������FDebug�X�N���[���\���ɕ\���������������ǉ�����
//			  ��莞�Ԍo����������܂�
//	�����F	strInfo�F�\��������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CDebugManager::DisplayScroll(const string& strInfo)
{
	m_listStrDebugScroll.push_back(strInfo);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
void CDebugManager::init(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif //USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CDebugManager::uninit(void)
{
#ifdef USING_DIRECTX
	// �f�o�b�O���\���p�t�H���g�̔j��
	SAFE_RELEASE(m_pFont);
#endif //USING_DIRECTX

	m_strDebugInfo.clear();
}

#endif//_DEBUG