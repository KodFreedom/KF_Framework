//--------------------------------------------------------------------------------
//
//　textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
LPCSTR CTextureManager::m_apTexPath[TEX_MAX] = {
	NULL,										//先頭のテクスチャはNull
	"data/TEXTURE/sample.png",					//Test Texture
	"data/TEXTURE/polygon.jpg",					//Test Texture
	"data/TEXTURE/road_stone.jpg",				//Field Texture
	"data/TEXTURE/skybox000.jpg",				//Sky Texture
};

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CTextureManager::CTextureManager()
{
	
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CTextureManager::Init(void)
{
	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  全てのテクスチャを読み込む
//--------------------------------------------------------------------------------
void CTextureManager::LoadAll(void)
{
	Load(TEX_NONE, TEX_MAX);
}

//モード分けで読み込み方式
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
//  全てのテクスチャを破棄する
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
//  テクスチャを取得する
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(const TEX_NAME &texName)
{
	if (texName <= TEX_NONE || texName >= TEX_MAX) { return NULL; }

	return m_vectorTexture[texName];
}

//--------------------------------------------------------------------------------
//  テクスチャの読み込み
//--------------------------------------------------------------------------------
void CTextureManager::Load(const TEX_NAME &texBegin, const TEX_NAME &texEnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	LPSTR str = NULL;
	HRESULT hr;

	if (!m_vectorTexture.empty())
	{
		wsprintf(str, "m_vectorTextureが空じゃない!");
		MessageBox(NULL, str, "エラー", MB_OK | MB_ICONWARNING);
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
				wsprintf(str, "%d番のテクスチャの読み込みが失敗！！！", nCnt);
				MessageBox(NULL, str, "エラー", MB_OK | MB_ICONWARNING);
			}
		}

		m_vectorTexture.push_back(pTex);
	}
}