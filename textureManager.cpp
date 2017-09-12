//--------------------------------------------------------------------------------
//
//　textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  全てのテクスチャを破棄する
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
//  テクスチャの読み込み
//--------------------------------------------------------------------------------
void CTextureManager::UseTexture(const string& strName)
{
	//何もないの場合処理終了
	if (strName.empty()) { return; }

	//すでに読み込んだら処理終了
	auto itr = m_umTexture.find(strName);
	if (itr != m_umTexture.end())
	{
		++itr->second.usNumUsers;
		return;
	}

	//テクスチャの読み込み
#ifdef USING_DIRECTX
	TEXTURE texture;
	texture.usNumUsers = 1;
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	string strPath = "data/TEXTURE/" + strName;
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, strPath.c_str(), &texture.pTexture);
	if (FAILED(hr))
	{
		string str = strPath + "が見つからない！！！";
		MessageBox(NULL, str.c_str(), "エラー", MB_OK | MB_ICONWARNING);
		texture.pTexture = nullptr;
	}
	m_umTexture.emplace(strName, texture);
#endif
}

//--------------------------------------------------------------------------------
//  テクスチャの破棄
//--------------------------------------------------------------------------------
void CTextureManager::DisuseTexture(const string& strName)
{
	if (strName.empty()) { return; }
	auto itr = m_umTexture.find(strName);
	--itr->second.usNumUsers;
	if (itr->second.usNumUsers == 0)
	{//誰も使ってないので破棄する
		SAFE_RELEASE(itr->second.pTexture);
		m_umTexture.erase(itr);
	}
}