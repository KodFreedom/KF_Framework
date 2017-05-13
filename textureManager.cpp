//--------------------------------------------------------------------------------
//
//�@textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
LPCSTR CTextureManager::m_apTexPath[TEX_MAX] = {
	NULL,										//�擪�̃e�N�X�`����Null
	"data/TEXTURE/sample.png",					//Test Texture
	"data/TEXTURE/polygon.jpg",					//Test Texture
	"data/TEXTURE/road_stone.jpg",				//Field Texture
	"data/TEXTURE/skybox000.jpg",				//Sky Texture
};

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CTextureManager::CTextureManager()
{
	
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CTextureManager::Init(void)
{
	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  �S�Ẵe�N�X�`����ǂݍ���
//--------------------------------------------------------------------------------
void CTextureManager::LoadAll(void)
{
	Load(TEX_NONE, TEX_MAX);
}

//���[�h�����œǂݍ��ݕ���
//void CTextureManager::LoadAll(const CManager::MODE &mode)
//{
//	switch (mode)
//	{
//	case CManager::MODE_DEMO:
//		Load(&m_apDemoTexPath[0], DEMO_TEX_MAX);
//		break;
//	default:
//		break;
//	}
//}

//--------------------------------------------------------------------------------
//  �S�Ẵe�N�X�`����j������
//--------------------------------------------------------------------------------
void CTextureManager::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < (int)m_vectorTexture.size(); nCnt++)
	{
		if (m_vectorTexture[nCnt] != NULL)
		{
			m_vectorTexture[nCnt]->Release();
			m_vectorTexture[nCnt] = NULL;
		}
	}

	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  �e�N�X�`�����擾����
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(const TEX_NAME &texName)
{
	if (texName <= TEX_NONE || texName >= TEX_MAX) { return NULL; }

	return m_vectorTexture[texName];
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̓ǂݍ���
//--------------------------------------------------------------------------------
void CTextureManager::Load(const TEX_NAME &texBegin, const TEX_NAME &texEnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	LPSTR str = NULL;
	HRESULT hr;

	if (!m_vectorTexture.empty())
	{
		wsprintf(str, "m_vectorTexture���󂶂�Ȃ�!");
		MessageBox(NULL, str, "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}

	for (int nCnt = texBegin; nCnt < texEnd; nCnt++)
	{
		LPDIRECT3DTEXTURE9 pTex = NULL;

		if(m_apTexPath[nCnt] != NULL)
		{
			hr = D3DXCreateTextureFromFile(pDevice, m_apTexPath[nCnt], &pTex);

			if (FAILED(hr))
			{
				wsprintf(str, "%d�Ԃ̃e�N�X�`���̓ǂݍ��݂����s�I�I�I", nCnt);
				MessageBox(NULL, str, "�G���[", MB_OK | MB_ICONWARNING);
			}
		}

		m_vectorTexture.push_back(pTex);
	}
}