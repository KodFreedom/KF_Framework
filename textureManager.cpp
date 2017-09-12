//--------------------------------------------------------------------------------
//
//�@textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �S�Ẵe�N�X�`����j������
//--------------------------------------------------------------------------------
void CTextureManager::UnloadAll(void)
{
	for (auto itr = m_umTexture.begin(); itr != m_umTexture.end();)
	{
		SAFE_RELEASE(itr->second.pTexture);
		itr = m_umTexture.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̓ǂݍ���
//--------------------------------------------------------------------------------
void CTextureManager::UseTexture(const string& strName)
{
	//�����Ȃ��̏ꍇ�����I��
	if (strName.empty()) { return; }

	//���łɓǂݍ��񂾂珈���I��
	auto itr = m_umTexture.find(strName);
	if (itr != m_umTexture.end())
	{
		++itr->second.usNumUsers;
		return;
	}

	//�e�N�X�`���̓ǂݍ���
#ifdef USING_DIRECTX
	TEXTURE texture;
	texture.usNumUsers = 1;
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	string strPath = "data/TEXTURE/" + strName;
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, strPath.c_str(), &texture.pTexture);
	if (FAILED(hr))
	{
		string str = strPath + "��������Ȃ��I�I�I";
		MessageBox(NULL, str.c_str(), "�G���[", MB_OK | MB_ICONWARNING);
		texture.pTexture = nullptr;
	}
	m_umTexture.emplace(strName, texture);
#endif
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̔j��
//--------------------------------------------------------------------------------
void CTextureManager::DisuseTexture(const string& strName)
{
	if (strName.empty()) { return; }
	auto itr = m_umTexture.find(strName);
	--itr->second.usNumUsers;
	if (itr->second.usNumUsers == 0)
	{//�N���g���ĂȂ��̂Ŕj������
		SAFE_RELEASE(itr->second.pTexture);
		m_umTexture.erase(itr);
	}
}