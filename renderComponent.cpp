//--------------------------------------------------------------------------------
//	描画コンポネント
//　renderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "renderComponent.h"
#include "manager.h"
#include "gameObject.h"
#include "meshComponent.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//	クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CRenderComponent::Init(void)
{
	auto mesh = m_pGameObj->GetMeshComponent();
	auto meshManager = Main::GetManager()->GetMeshManager();
	auto renderInfo = meshManager->GetRenderInfo(mesh->GetMeshName());
	
	if (!renderInfo.strTex.empty()) { SetTexName(renderInfo.strTex); }
	m_renderPriority = renderInfo.renderPriority;
	m_renderState = renderInfo.renderState;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CRenderComponent::Uninit(void)
{
	if (!m_strTexName.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CRenderComponent::Update(void)
{
	Main::GetManager()->GetRenderManager()->Register(this, m_renderPriority, m_renderState);
}

//--------------------------------------------------------------------------------
//  テクスチャ設定
//--------------------------------------------------------------------------------
void CRenderComponent::SetTexName(const string& strTexName)
{
	if (!m_strTexName.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}

	m_strTexName = strTexName;
	Main::GetManager()->GetTextureManager()->UseTexture(m_strTexName);
}