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
	auto pMesh = m_pGameObj->GetMeshComponent();
	auto pMeshManager = CMain::GetManager()->GetMeshManager();
	auto renderInfo = pMeshManager->GetRenderInfo(pMesh->GetMeshName());
	
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
		CMain::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CRenderComponent::Update(void)
{
	CMain::GetManager()->GetRenderManager()->Register(this, m_renderPriority, m_renderState);
}

//--------------------------------------------------------------------------------
//  テクスチャ設定
//--------------------------------------------------------------------------------
void CRenderComponent::SetTexName(const string& strTexName)
{
	if (!m_strTexName.empty())
	{
		CMain::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}

	m_strTexName = strTexName;
	CMain::GetManager()->GetTextureManager()->UseTexture(m_strTexName);
}