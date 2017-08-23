//--------------------------------------------------------------------------------
//	3Dメッシュコンポネント
//　3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "meshManager.h"
#include "mesh.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void C3DMeshComponent::Uninit(void)
{
	if (!m_strMeshName.empty())
	{
		CMain::GetManager()->GetMeshManager()->DisuseMesh(m_strMeshName);
		m_strMeshName.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DMeshComponent::SetMeshName(const string& strMeshName)
{
	m_strMeshName = strMeshName;
	CMain::GetManager()->GetMeshManager()->UseMesh(m_strMeshName);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DMeshComponent::SetMeshName(const string& strFileName, string& strTexName)
{
	m_strMeshName = strFileName;
	CMain::GetManager()->GetMeshManager()->UseMesh(m_strMeshName, strTexName);
}