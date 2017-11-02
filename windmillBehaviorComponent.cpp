//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@windmillBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "windmillBehaviorComponent.h"
#include "gameObject.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CWindmillBehaviorComponent::Init(void)
{
	m_pFan = findFan(m_pGameObj);

	if(!m_pFan)
	{
		assert("No Fan!!");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CWindmillBehaviorComponent::Update(void)
{
	m_pFan->GetTransformComponent()->RotByRoll(RotSpeed);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
GameObject* CWindmillBehaviorComponent::findFan(GameObject* pParent)
{
	auto& listChild = pParent->GetTransformComponent()->GetChildren();
	for (auto pTrans : listChild)
	{
		auto pObj = pTrans->GetGameObject();
		if (pObj->GetName() == "Fan") { return pObj; }
		else
		{
			auto pFan = findFan(pObj);
			if (pFan) { return pFan; }
		}
	}

	return nullptr;
}