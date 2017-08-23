//--------------------------------------------------------------------------------
//	3D���b�V���R���|�l���g
//�@3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "3DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "meshManager.h"
#include "mesh.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
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
//  �X�V����
//--------------------------------------------------------------------------------
void C3DMeshComponent::SetMeshName(const string& strMeshName)
{
	m_strMeshName = strMeshName;
	CMain::GetManager()->GetMeshManager()->UseMesh(m_strMeshName);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DMeshComponent::SetMeshName(const string& strFileName, string& strTexName)
{
	m_strMeshName = strFileName;
	CMain::GetManager()->GetMeshManager()->UseMesh(m_strMeshName, strTexName);
}